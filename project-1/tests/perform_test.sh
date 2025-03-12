#!/bin/bash

# Define directories (modify as needed)
INPUT_DIR="test_cases_exhaustive"
OUTPUT_DIR="program_outputs"
EXPECTED_DIR="test_cases_correct_outputs"
EXECUTABLE="./project-1"  # Replace with your compiled C++ program

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Counters
total_tests=0
passed_tests=0

# Loop over test cases (1 to 108)
for n in {0..107}; do
    input1="$INPUT_DIR/test_set_${n}_1.txt"
    input2="$INPUT_DIR/test_set_${n}_2.txt"
    expected="$EXPECTED_DIR/test_set_${n}_corr.txt"
    output="$OUTPUT_DIR/test_set_${n}_out.txt"

    # Check if input and expected files exist
    if [[ ! -f "$input1" || ! -f "$input2" || ! -f "$expected" ]]; then
        echo "⚠️ Missing files for test_set_${n}, skipping..."
        continue
    fi

    ((total_tests++))

    # Run the C++ program with inputs and output file
    "$EXECUTABLE" "$input1" "$input2" "$output" >/dev/null 2>&1

    # Wait briefly to ensure the output file is created
    sleep 0.1

    # Check if output file was actually created
    if [[ ! -f "$output" ]]; then
        echo "❌ Test $n failed! Output file was not created."
        continue
    fi

    # Compare output with expected result
    if diff -q "$output" "$expected" >/dev/null; then
        echo "✅ Test $n passed."
        ((passed_tests++))
    else
        echo "❌ Test $n failed! See differences below:"
        diff "$output" "$expected"
    fi
done

# Calculate and display final report
if [[ $total_tests -gt 0 ]]; then
    percentage=$((100 * passed_tests / total_tests))
    echo "📊 Test Summary: $passed_tests/$total_tests passed ($percentage%)"
else
    echo "⚠️ No tests were executed."
fi

echo "Test suite completed."
