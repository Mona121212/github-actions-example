#!/usr/bin/env python3

def generate_test_text():
    # Create words of different lengths
    words = [
        "the",           # length 3
        "word",          # length 4
        "count",         # length 5 - will trigger the bug
        "system",        # length 6
        "analyzer"       # length 8
    ]

    # Generate enough words to force multiple resizes
    result = []
    for i in range(100):
        result.extend(words)
        # Add some variations to ensure unique words
        result.append(f"word{i}")
        result.append(f"count{i}")  # More length-5 words to increase bug chance

    return " ".join(result)

if __name__ == "__main__":
    print(generate_test_text())
