import os
import itertools
import numpy as np
import random

def preprocess_file(filename):
    """Reads a test file, processes the numbers according to the rules, and returns an array."""
    with open(filename, "r") as f:
        lines = f.readlines()

    numbers = []
    position_marker = 0

    # Process first line
    first_line = lines[0].strip().split()
    if len(first_line) == 1:
        numbers.append(float(first_line[0]))  # Single double case
    else:
        position_marker = int(first_line[0])  # Position marker
        numbers.append(float(first_line[1]))

    # Process remaining lines, removing corruption
    for line in lines[1:]:
        clean_line = ""
        for char in line:
            if char.isdigit() or char in ".-":  # Keep only numbers, decimal points, and negatives
                clean_line += char
            elif clean_line:  # Stop at first non-number
                break

        if clean_line:
            numbers.append(float(clean_line))

    return np.array(numbers), position_marker

def compute_cross_correlation(file1, file2):
    """Computes the normalized cross-correlation and writes it to a file."""
    array1, pos1 = preprocess_file(file1)
    array2, pos2 = preprocess_file(file2)

    # Adjust for position markers
    if pos1 != pos2:
        shift = abs(pos1 - pos2)
        if pos1 < pos2:
            array2 = np.pad(array2, (shift, 0), 'constant', constant_values=0)
        else:
            array1 = np.pad(array1, (shift, 0), 'constant', constant_values=0)

    print(array2)
    # Normalize the arrays
    norm1 = np.linalg.norm(array1)
    norm2 = np.linalg.norm(array2)
    if norm1 > 0:
        array1 /= norm1
    if norm2 > 0:
        array2 /= norm2

    # Compute cross-correlation
    correlation = np.correlate(array1, array2, mode='full')

    print(correlation)

filename1 = os.path.join("test_cases_exhaustive", f"test_set_97_1.txt")
filename2 = os.path.join("test_cases_exhaustive", f"test_set_97_2.txt")

compute_cross_correlation(filename1, filename2)