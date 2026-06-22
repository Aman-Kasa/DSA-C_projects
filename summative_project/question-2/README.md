# Input Validator — Question 2

A small command-line validator program that verifies input according to the procedures documented in `procedures.txt`.

## What it is
- `validator.c` implements routines to check input formatting and rules described in `procedures.txt`.
- A helper `audit.log` file records validation events when run.

## Files
- `validator.c` — C source
- `validator` — compiled binary (if present)
- `procedures.txt` — specification and test cases
- `audit.log` — runtime audit output (generated)

## Build & Run

```bash
gcc question-2/validator.c -o question-2/validator
./question-2/validator <input-file>
```

Check `procedures.txt` for sample inputs and expected behavior.

## Notes
- `validator` may read from stdin or a file depending on its prompts. Inspect source for exact command-line options.
