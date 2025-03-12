import os
import itertools
import numpy as np
import random

def set_seed(seed):
    random.seed(seed)
    np.random.seed(seed)

output_dir = "test_cases_exhaustive"
os.makedirs(output_dir, exist_ok=True)

# Feature variations
first_line_formats = ["one_double", "int_and_double"]
file_length_variations = ["more1", "more2", "equal"]
int_comparisons = ["larger", "smaller", "equal"]
corruption_types = ["none", "letters", "extra_text"]
empty_lines_options = [True, False]

test_cases = itertools.product(
    first_line_formats, file_length_variations, int_comparisons, corruption_types, empty_lines_options
)

def generate_number_string(corruption):
    """Generate a double number with optional corruption."""
    num = f"{random.uniform(-1000, 1000):.6f}"
    if corruption == "letters":
        num += random.choice(["A", "B", "XYZ"])
    elif corruption == "extra_text":
        num += " extra_data"
    return num

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

def compute_cross_correlation(file1, file2, output_filename):
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

    # Normalize the arrays
    norm1 = np.linalg.norm(array1)
    norm2 = np.linalg.norm(array2)
    if norm1 > 0:
        array1 /= norm1
    if norm2 > 0:
        array2 /= norm2

    # Compute cross-correlation
    correlation = np.correlate(array1, array2, mode='full')

    # Trim trailing zeroes
    correlation = np.trim_zeros(correlation, trim='fb')

    # Save to file
    with open(output_filename, "w") as f:
        f.writelines(f"{val:.6f}\n" for val in correlation)


set_seed(42)
for i, (first_format, length_var, int_comp, corruption, empty_lines) in enumerate(test_cases):
    filename1 = os.path.join(output_dir, f"test_set_{i}_1.txt")
    filename2 = os.path.join(output_dir, f"test_set_{i}_2.txt")
    output_filename = os.path.join(output_dir, f"test_set_{i}_corr.txt")

    # First line setup
    if first_format == "one_double":
        first_line1 = f"{random.uniform(-1000, 1000):.6f}\n"
        first_line2 = f"{random.uniform(-1000, 1000):.6f}\n"
    else:
        int1 = random.randint(-10, 10)
        int2 = random.randint(-10, 10)
        double1 = random.uniform(-1000, 1000)
        double2 = random.uniform(-1000, 1000)

        if int_comp == "larger":
            int1, int2 = max(int1, int2), min(int1, int2)
        elif int_comp == "smaller":
            int1, int2 = min(int1, int2), max(int1, int2)
        else:
            int2 = int1

        first_line1 = f"{int1} {double1:.6f}\n"
        first_line2 = f"{int2} {double2:.6f}\n"

    base_length = 1000
    if length_var == "more1":
        num_lines1, num_lines2 = base_length + 5, base_length
    elif length_var == "more2":
        num_lines1, num_lines2 = base_length, base_length + 5
    else:
        num_lines1 = num_lines2 = base_length

    lines1, lines2 = [], []

    for _ in range(num_lines1):
        if empty_lines and random.random() < 0.2:
            lines1.append("\n")
        else:
            lines1.append(generate_number_string(corruption) + "\n")

    for _ in range(num_lines2):
        if empty_lines and random.random() < 0.2:
            lines2.append("\n")
        else:
            lines2.append(generate_number_string(corruption) + "\n")

    with open(filename1, "w") as f1, open(filename2, "w") as f2:
        f1.write(first_line1)
        f1.writelines(lines1)
        f2.write(first_line2)
        f2.writelines(lines2)

    # Compute cross-correlation for the test pair
    compute_cross_correlation(filename1, filename2, output_filename)

print(f"Generated {i+1} test case pairs with corresponding cross-correlation outputs.")
