# Huffman Utility — Question 5

A small Huffman coding utility for compressing and restoring telemetry data. Includes example telemetry files used for demonstration.

## Files
- `huffman_utility.c` — C source
- `huffman_utility` — compiled binary (if present)
- `telemetry.txt` — sample input
- `telemetry.huff` — compressed sample (binary)
- `telemetry_restored.txt` — restored output example

## Build & Run

```bash
gcc question-5/huffman_utility.c -o question-5/huffman_utility
# Compress
./question-5/huffman_utility -c question-5/telemetry.txt -o question-5/telemetry.huff
# Decompress
./question-5/huffman_utility -d question-5/telemetry.huff -o question-5/telemetry_restored.txt
```

Inspect `huffman_utility.c` for exact command-line flags and behavior.
