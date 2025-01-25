// ParallelArraySum.cpp: Creates 2 arrays of 1000 integers, and sum them to form a third array.
//                       The process is done both in serial mode and in parallel mode to compare.

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <limits>
#include <random>
#include <iomanip>
#include <omp.h>

// Method declaration
std::vector<float> createArray(int elements);
std::vector<float> serialExecution(const std::vector<float>& first, const std::vector<float>& second);
std::vector<float> parallelExecution(const std::vector<float>& first, const std::vector<float>& second);

// Program execution
int main() {
   
    // Set locale for thousands separator
    std::cout.imbue(std::locale("en_US.UTF-8"));

    // Stores the number of elements that each array has.
    int elements;
    std::cout << "This program will sum two arrays.\n\n";

    // Ask user for the number of elements in the array
    while (true) {
        std::cout << "Please type the number of elements for each array: \n";
        std::cin >> elements;

        if (std::cin.fail() || elements < 1 || elements > 1000000 || (elements % 10 != 0)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number between 1 and 1,000,000 that is a multiple of 10!\n\n";
        }
        else {
            break;
        }
    }

    // Generate two random arrays
    std::vector<float> first = createArray(elements);
    std::vector<float> second = createArray(elements);

    std::cout << "\nExecution results:\n\n";

    // Serial execution
    std::vector<float> serial = serialExecution(first, second);

    // Parallel execution
    std::vector<float> parallel = parallelExecution(first, second);

    // Print the first values for each array:
    for (int i = 0; i < 25; i++) {
        std::cout << "First value: " << std::fixed << std::setprecision(2) << first[i];
        std::cout << " | Second value: " << std::fixed << std::setprecision(2) << second[i];
        std::cout << " | Serial result: " << std::fixed << std::setprecision(2) << serial[i];
        std::cout << " | Parallel result: " << std::fixed << std::setprecision(2) << parallel[i];
        std::cout << std::endl;
    }

    // Normal end
    return 0;
}

// Method to generate an array with a given number of random integers
std::vector<float> createArray(int elements) {

    // Variable to hold the result
    std::vector<float> result(elements);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> dis(0.0, 1000.0);

    // Fill the array
    for (float& position : result) {
        position = dis(generator);
    }

    // Return the array
    return result;
}

// Method to sum two arrays in a serial fashion
std::vector<float> serialExecution(const std::vector<float>& first, const std::vector<float>& second) {

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Control variables
    int size = first.size();
    int i;

    // Variable to hold the result
    std::vector<float> result(size);

    // Iterate the array and perform the sum on each position
    for (i = 0; i < size; i++) {
        result[i] = first[i] + second[i];
    }

    // End timer
    auto end = std::chrono::high_resolution_clock::now();

    // Compute the duration
    std::chrono::duration<double, std::nano> executionTime = end - start;

    // Print the duration
    std::cout << "Serial execution duration: " << static_cast<long>(executionTime.count()) << " nanoseconds.\n";

    // Return the new array
    return result;
}

// Method to sum two arrays in a serial fashion
std::vector<float> parallelExecution(const std::vector<float>& first, const std::vector<float>& second) {

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Control variables
    int size = first.size();
    int chunks = size / 10;
    int i;

    // Variable to hold the result
    std::vector<float> result(size);
    
    // Iterate the array and perform the sum on each position
    #pragma omp parallel for shared(first, second, result, chunks) private(i) schedule(static, chunks)
    for (i = 0; i < size; i++) {
        result[i] = first[i] + second[i];
    }

    // End timer
    auto end = std::chrono::high_resolution_clock::now();

    // Compute the duration
    std::chrono::duration<double, std::nano> executionTime = end - start;

    // Print the duration
    std::cout << "Parallel execution duration: " << static_cast<long>(executionTime.count()) << " nanoseconds.\n\n";

    // Return the new array
    return result;
}