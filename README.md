# GitHub Actions with MorphVM Example

Template showing GitHub Actions integration with MorphVM for automated testing and remote debugging. Uses a C program with a deliberate memory error to demonstrate crash capture and debug capabilities.

## Setup

1. Fork this repository
2. Add your Morph Cloud API key as `MORPH_API_KEY` in repository secrets (Settings > Secrets and variables > Actions)

## Repository Structure

- `text_analyzer.c`: Word frequency counter with an intentional memory bug
- `test_input.py`: Test data generator
- `.github/workflows/build_and_test.yml`: GitHub Actions workflow
- `Makefile`: Build configuration

## How It Works

On push to main, GitHub Actions:
- Starts a MorphVM instance
- Builds and tests the program under GDB
- Creates a snapshot if crash occurs

To connect to a crash debug session:
```bash
morphcloud instance ssh --rm $(morphcloud instance start SNAPSHOT_ID) -- tmux attach
```

## Local Development

```bash
# Build and run
make
./text_analyzer "sample text"

# Run with test data
python3 test_input.py | ./text_analyzer

# Debug
make debug
```

## License

Apache License 2.0

trigger github actions test
trigger github actions test
