#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 511
#define MAX_CODE_LEN 256

// --- Huffman Tree Node Structure ---
typedef struct Node {
    unsigned char ch;
    int freq;
    struct Node *left, *right;
} Node;

// --- Bit Packer Structures ---
typedef struct {
    FILE *file;
    unsigned char buffer;
    int bits_count;
} BitWriter;

typedef struct {
    FILE *file;
    unsigned char buffer;
    int bits_count;
} BitReader;

// --- Bitwise Operations Helpers ---
void bit_writer_init(BitWriter *bw, FILE *f) {
    bw->file = f;
    bw->buffer = 0;
    bw->bits_count = 0;
}

void write_bit(BitWriter *bw, int bit) {
    bw->buffer <<= 1;
    if (bit) bw->buffer |= 1;
    bw->bits_count++;
    if (bw->bits_count == 8) {
        fputc(bw->buffer, bw->file);
        bw->buffer = 0;
        bw->bits_count = 0;
    }
}

void bit_writer_flush(BitWriter *bw) {
    if (bw->bits_count > 0) {
        bw->buffer <<= (8 - bw->bits_count);
        fputc(bw->buffer, bw->file);
    }
}

void bit_reader_init(BitReader *br, FILE *f) {
    br->file = f;
    br->buffer = 0;
    br->bits_count = 0;
}

int read_bit(BitReader *br) {
    if (br->bits_count == 0) {
        int c = fgetc(br->file);
        if (c == EOF) return -1;
        br->buffer = (unsigned char)c;
        br->bits_count = 8;
    }
    int bit = (br->buffer >> 7) & 1;
    br->buffer <<= 1;
    br->bits_count--;
    return bit;
}

// --- Tree Processing Logic ---
Node* create_node(unsigned char ch, int freq, Node *left, Node *right) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->ch = ch;
    n->freq = freq;
    n->left = left;
    n->right = right;
    return n;
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

Node* build_huffman_tree(int freq[]) {
    Node *nodes[256];
    int num_nodes = 0;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[num_nodes++] = create_node((unsigned char)i, freq[i], NULL, NULL);
        }
    }

    if (num_nodes == 0) return NULL;

    while (num_nodes > 1) {
        int min1 = 0, min2 = 1;
        if (nodes[min1]->freq > nodes[min2]->freq) {
            min1 = 1; min2 = 0;
        }

        for (int i = 2; i < num_nodes; i++) {
            if (nodes[i]->freq < nodes[min1]->freq) {
                min2 = min1;
                min1 = i;
            } else if (nodes[i]->freq < nodes[min2]->freq) {
                min2 = i;
            }
        }

        Node *left = nodes[min1];
        Node *right = nodes[min2];
        Node *parent = create_node(0, left->freq + right->freq, left, right);

        int next_num_nodes = 0;
        for (int i = 0; i < num_nodes; i++) {
            if (i != min1 && i != min2) {
                nodes[next_num_nodes++] = nodes[i];
            }
        }
        nodes[next_num_nodes++] = parent;
        num_nodes = next_num_nodes;
    }
    return nodes[0];
}

void generate_codes(Node *root, char *code, int depth, char codes[256][MAX_CODE_LEN]) {
    if (!root) return;
    if (!root->left && !root->right) {
        code[depth] = '\0';
        strcpy(codes[root->ch], code);
        return;
    }
    code[depth] = '0';
    generate_codes(root->left, code, depth + 1, codes);
    code[depth] = '1';
    generate_codes(root->right, code, depth + 1, codes);
}

// --- Dynamic Compression Phase ---
void compress_telemetry() {
    int freq[256] = {0};
    FILE *in = fopen("telemetry.txt", "rb");
    if (!in) {
        in = fopen("telemetry.txt", "wb");
        fprintf(in, "TMP:24C;HUM:62%%;BAT:88%%;MSG:SYSTEM_OK_ALL_NODES_OPERATIONAL_RELOAD\n");
        fclose(in);
        in = fopen("telemetry.txt", "rb");
    }

    fseek(in, 0, SEEK_END);
    long orig_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    int ch;
    int unique_chars = 0;
    while ((ch = fgetc(in)) != EOF) {
        if (freq[ch] == 0) unique_chars++;
        freq[ch]++;
    }

    Node *root = build_huffman_tree(freq);
    char codes[256][MAX_CODE_LEN] = {{0}};
    char code_buf[MAX_CODE_LEN];
    if (root) {
        generate_codes(root, code_buf, 0, codes);
    }

    FILE *out = fopen("telemetry.huff", "wb");
    
    // Write optimized header tokens
    fwrite(&orig_size, sizeof(long), 1, out);
    fwrite(&unique_chars, sizeof(int), 1, out);
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            unsigned char c = (unsigned char)i;
            fwrite(&c, sizeof(unsigned char), 1, out);
            fwrite(&freq[i], sizeof(int), 1, out);
        }
    }

    fseek(in, 0, SEEK_SET);
    BitWriter bw;
    bit_writer_init(&bw, out);

    while ((ch = fgetc(in)) != EOF) {
        char *c = codes[ch];
        for (int i = 0; c[i] != '\0'; i++) {
            write_bit(&bw, c[i] == '1');
        }
    }
    bit_writer_flush(&bw);

    long comp_size = ftell(out);
    fclose(in);
    fclose(out);
    if (root) free_tree(root);

    printf("\n--- Compression Pipeline Run Completed ---\n");
    printf("Original Size   : %ld bytes\n", orig_size);
    printf("Compressed Size : %ld bytes\n", comp_size);
    printf("Compression Ratio: %.2f%%\n", ((double)comp_size / orig_size) * 100.0);
}

// --- Dynamic Decompression Phase ---
void decompress_telemetry() {
    FILE *in = fopen("telemetry.huff", "rb");
    if (!in) return;

    long total_chars = 0;
    int unique_chars = 0;
    int freq[256] = {0};
    
    fread(&total_chars, sizeof(long), 1, in);
    fread(&unique_chars, sizeof(int), 1, in);

    for (int i = 0; i < unique_chars; i++) {
        unsigned char c;
        int f;
        fread(&c, sizeof(unsigned char), 1, in);
        fread(&f, sizeof(int), 1, in);
        freq[c] = f;
    }

    Node *root = build_huffman_tree(freq);
    FILE *out = fopen("telemetry_restored.txt", "wb");

    BitReader br;
    bit_reader_init(&br, in);

    long items_restored = 0;
    while (items_restored < total_chars && root) {
        Node *curr = root;
        while (curr->left || curr->right) {
            int bit = read_bit(&br);
            if (bit == -1) break;
            curr = bit ? curr->right : curr->left;
        }
        fputc(curr->ch, out);
        items_restored++;
    }

    fclose(in);
    fclose(out);
    if (root) free_tree(root);
    printf("\n--- Decompression Engine Reconstruction Completed ---\n");
}

// --- Verification Phase ---
void verify_payloads() {
    FILE *f1 = fopen("telemetry.txt", "rb");
    FILE *f2 = fopen("telemetry_restored.txt", "rb");

    if (!f1 || !f2) {
        printf("Verification Failed: System files are missing.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return;
    }

    int c1, c2;
    int match = 1;
    while (1) {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 != c2) {
            match = 0;
            break;
        }
        if (c1 == EOF) break;
    }

    fclose(f1);
    fclose(f2);

    if (match) {
        printf("\n[SUCCESS] Verification Confirmed: telemetry_restored.txt matches telemetry.txt exactly!\n");
    } else {
        printf("\n[FAILURE] Divergence detected between source and restored telemetry sets.\n");
    }
}

int main() {
    printf("Telemetry Compression/Decompression Tool Initializing...\n");
    compress_telemetry();
    decompress_telemetry();
    verify_payloads();
    return 0;
}
