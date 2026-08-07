# LBYARCH-MP2

## MP2 Image Conversion Benchmark
This project benchmarks the x86-64 assembly implementation of grayscale pixel conversion. The C code is used to collect input, allocate memory, run the check, and print the results, while the assembly performs the actual pixel conversion.

The conversion is:
output = input / 255.0

### Testing Information
The test program contains:
1. Correctness check on a small 10x10 sample.
2. Timing test for 10x10, 100x100, and 1000x1000 inputs.

Each size is run 30 times, and the program prints the average execution time in milliseconds.

### Correctness output
The terminal output shows that the assembly output matches the expected int / 255.0 result for the test pixels.
PASS: all 100 asm outputs match expected int/255.0
This confirms that the assembly is producing the correct floating-point values for the sample inputs.

### Timing output
The test prints one line per image size:
- 10x10
- 100x100
- 1000x1000
Each line reports the average time taken by the assembly function over 30 runs.

### Analysis of results
- the 10x10 case is extremely small, so the measured time is very close to the timing overhead itself
- the 100x100 case gives a more stable measurement because the workload is larger
- the 1000x1000 case is the most useful for performance analysis because it contains enough pixels to show the real cost of the conversion routine

This makes the larger image sizes better for evaluating the assembly implementation.

<img width="555" height="347" alt="image" src="https://github.com/user-attachments/assets/badd6821-e53c-4109-9639-12921156b3e8" />


