
# DSA C Projects — Summative Collection

A polished collection of five C-language assignments exploring data structures, algorithms, and applied utilities. Each folder under `question-*` contains a standalone program, source code, and a short README describing usage and requirements.

## Highlights
- Clean, standalone C solutions focused on practical problem-solving.
- Small, easy-to-read codebases ideal for study and extension.
- Included sample data where relevant (see `question-5`).

## Prerequisites
- A POSIX-compatible shell (Linux/macOS) or WSL on Windows
- `gcc` (GNU Compiler Collection) or any C compiler that supports C99
- Make sure executables have execute permission if prebuilt binaries are present

## Project Structure

```
README.md
question-1/    -> incident_tracker (source + binary)
question-2/    -> validator (source + binary + procedures)
question-3/    -> airline_route (source + binary)
question-4/    -> campus_nav (source + binary)
question-5/    -> huffman_utility (source + binary + sample telemetry files)
```

Each `question-*` directory contains a small README with specific notes. Start there if you want per-problem instructions.

## Build & Run (quick guide)

You can build each program individually from the repository root. Examples below assume you're in the project root.

- Question 1 — Incident Tracker

```bash
gcc question-1/incident_tracker.c -o question-1/incident_tracker
./question-1/incident_tracker
```

- Question 2 — Validator

```bash
gcc question-2/validator.c -o question-2/validator
./question-2/validator
```

- Question 3 — Airline Route

```bash
gcc question-3/airline_route.c -o question-3/airline_route
./question-3/airline_route
```

- Question 4 — Campus Navigation

```bash
gcc question-4/campus_nav.c -o question-4/campus_nav
./question-4/campus_nav
```

- Question 5 — Huffman Utility (compression/restore)

```bash
gcc question-5/huffman_utility.c -o question-5/huffman_utility
# Example: compress or restore using included sample files
./question-5/huffman_utility <options> telemetry.txt
```

Notes
- If a directory already contains an executable (no `.c` compile needed), ensure it is executable: `chmod +x question-*/<binary>` and run it directly.
- Some programs expect input from files or interactive prompts; consult each subdirectory's README for exact usage.

## Testing & Examples
Check each `question-*/README.md` for test cases, sample input and expected output. For `question-5` the files `telemetry.txt` and `telemetry.huff` are included as examples.

## Contributing
Small, focused contributions are welcome. Open an issue or submit a pull request for bug fixes, clarity improvements, or small feature additions.

## License
This repository is provided for educational use. If you want a formal license added, say which one (e.g., MIT, BSD) and it will be included.

---
If you want, I can also:
- add a `Makefile` to build all programs with `make build`
- run each program and capture example outputs into the READMEs

Enjoy exploring these C projects — let me know if you'd like a specific README per question improved.
