#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <limits>
using namespace std;

// Function to display the current state of memory blocks
void displayBlockState(int blockSize[], int tempBlockSize[], int number_of_blocks) {
    cout << "\nCurrent Memory Block States:" << endl;
    cout << left << setw(15) << "Block No." << setw(15) << "Total Size" << setw(15) << "Available Size" << "Used Space" << endl;
    cout << string(60, '-') << endl;
    for (int i = 0; i < number_of_blocks; i++) {
        cout << left << setw(15) << i + 1 << setw(15) << blockSize[i] << setw(15) << tempBlockSize[i] 
             << blockSize[i] - tempBlockSize[i] << endl;
    }
    cout << endl;
}

// Function to print allocation results for processes
void printAllocation(const string &method, int allocation[], int tempBlockSize[], int processSize[], int number_of_process) {
    cout << "\n" << method << " Allocation Results:\n";
    cout << left << setw(15) << "Process No." << setw(15) << "Process Size" << setw(15) << "Allocated Block" << "Fragmentation" << endl;
    cout << string(60, '-') << endl;
    for (int i = 0; i < number_of_process; i++) {
        if (allocation[i] != -1)
            cout << left << setw(15) << i + 1 << setw(15) << processSize[i] << setw(15) << allocation[i] + 1 << tempBlockSize[allocation[i]] << endl;
        else
            cout << left << setw(15) << i + 1 << setw(15) << processSize[i] << setw(15) << "Not Allocated" << "-" << endl;
    }
}

// Memory allocation methods (First Fit, Best Fit, Worst Fit, Next Fit)
// Implementation for First Fit
void firstFit(int blockSize[], int processSize[], int number_of_blocks, int number_of_process) {
    try {
        int allocation[number_of_process];
        int tempBlockSize[number_of_blocks];

        // Initialize allocation array with -1 (indicating unallocated)
        for (int i = 0; i < number_of_process; i++) {
            allocation[i] = -1;
        }

        // Copy block sizes to temporary array for tracking available space
        for (int i = 0; i < number_of_blocks; i++) {
            if (blockSize[i] <= 0)
                throw invalid_argument("Block sizes must be positive");
            tempBlockSize[i] = blockSize[i];
        }

        // Allocate memory for each process
        for (int i = 0; i < number_of_process; i++) {
            if (processSize[i] <= 0)
                throw invalid_argument("Process sizes must be positive");

            cout << "\nAllocating Process " << i + 1 << " (Size: " << processSize[i] << ")" << endl;
            for (int j = 0; j < number_of_blocks; j++) {
                if (tempBlockSize[j] >= processSize[i]) {
                    allocation[i] = j;  // Allocate process to this block
                    tempBlockSize[j] -= processSize[i];  // Update remaining space
                    break;  // Move to next process after allocation
                }
            }
            if (allocation[i] == -1) {
                cout << "Process " << i + 1 << " could not be allocated." << endl;
            }

            // Display block state after each allocation attempt
            displayBlockState(blockSize, tempBlockSize, number_of_blocks);
        }
        printAllocation("First Fit", allocation, tempBlockSize, processSize, number_of_process);
    }
    catch (const exception& e) {
        cerr << "Error in First Fit allocation: " << e.what() << endl;
    }
}

// Best Fit Allocation Method
void bestFit(int blockSize[], int processSize[], int number_of_blocks, int number_of_process) {
    try {
        int allocation[number_of_process];
        int tempBlockSize[number_of_blocks];

        for (int i = 0; i < number_of_process; i++) {
            allocation[i] = -1;
        }
        for (int i = 0; i < number_of_blocks; i++) {
            if (blockSize[i] <= 0)
                throw invalid_argument("Block sizes must be positive");
            tempBlockSize[i] = blockSize[i];
        }

        for (int i = 0; i < number_of_process; i++) {
            if (processSize[i] <= 0)
                throw invalid_argument("Process sizes must be positive");

            int bestIdx = -1;
            cout << "\nAllocating Process " << i + 1 << " (Size: " << processSize[i] << ")" << endl;
            for (int j = 0; j < number_of_blocks; j++) {
                if (tempBlockSize[j] >= processSize[i]) {
                    if (bestIdx == -1 || tempBlockSize[j] < tempBlockSize[bestIdx]) {
                        bestIdx = j;
                    }
                }
            }
            if (bestIdx != -1) {
                allocation[i] = bestIdx;
                tempBlockSize[bestIdx] -= processSize[i];
                cout << "Process " << i + 1 << " allocated to Block " << bestIdx + 1 << endl;
            } else {
                cout << "Process " << i + 1 << " could not be allocated." << endl;
            }

            displayBlockState(blockSize, tempBlockSize, number_of_blocks);
        }
        printAllocation("Best Fit", allocation, tempBlockSize, processSize, number_of_process);
    }
    catch (const exception& e) {
        cerr << "Error in Best Fit allocation: " << e.what() << endl;
    }
}

// Worst Fit Allocation Method
void worstFit(int blockSize[], int processSize[], int number_of_blocks, int number_of_process) {
    try {
        int allocation[number_of_process];
        int tempBlockSize[number_of_blocks];

        for (int i = 0; i < number_of_process; i++) {
            allocation[i] = -1;
        }
        for (int i = 0; i < number_of_blocks; i++) {
            if (blockSize[i] <= 0)
                throw invalid_argument("Block sizes must be positive");
            tempBlockSize[i] = blockSize[i];
        }

        for (int i = 0; i < number_of_process; i++) {
            if (processSize[i] <= 0)
                throw invalid_argument("Process sizes must be positive");

            int worstIdx = -1;
            cout << "\nAllocating Process " << i + 1 << " (Size: " << processSize[i] << ")" << endl;
            for (int j = 0; j < number_of_blocks; j++) {
                if (tempBlockSize[j] >= processSize[i]) {
                    if (worstIdx == -1 || tempBlockSize[j] > tempBlockSize[worstIdx]) {
                        worstIdx = j;
                    }
                }
            }
            if (worstIdx != -1) {
                allocation[i] = worstIdx;
                tempBlockSize[worstIdx] -= processSize[i];
                cout << "Process " << i + 1 << " allocated to Block " << worstIdx + 1 << endl;
            } else {
                cout << "Process " << i + 1 << " could not be allocated." << endl;
            }

            displayBlockState(blockSize, tempBlockSize, number_of_blocks);
        }
        printAllocation("Worst Fit", allocation, tempBlockSize, processSize, number_of_process);
    }
    catch (const exception& e) {
        cerr << "Error in Worst Fit allocation: " << e.what() << endl;
    }
}

// Next Fit Allocation Method
void nextFit(int blockSize[], int processSize[], int number_of_blocks, int number_of_process) {
    try {
        int allocation[number_of_process];
        int tempBlockSize[number_of_blocks];

        for (int i = 0; i < number_of_process; i++) {
            allocation[i] = -1;
        }
        for (int i = 0; i < number_of_blocks; i++) {
            if (blockSize[i] <= 0)
                throw invalid_argument("Block sizes must be positive");
            tempBlockSize[i] = blockSize[i];
        }

        int lastAllocated = 0;
        for (int i = 0; i < number_of_process; i++) {
            if (processSize[i] <= 0)
                throw invalid_argument("Process sizes must be positive");

            cout << "\nAllocating Process " << i + 1 << " (Size: " << processSize[i] << ")" << endl;
            int j = lastAllocated;
            bool allocated = false;
            do {
                if (tempBlockSize[j] >= processSize[i]) {
                    allocation[i] = j;
                    tempBlockSize[j] -= processSize[i];
                    lastAllocated = (j + 1) % number_of_blocks;
                    cout << "Process " << i + 1 << " allocated to Block " << j + 1 << endl;
                    allocated = true;
                    break;
                }
                j = (j + 1) % number_of_blocks;
            } while (j != lastAllocated);

            if (!allocated) {
                cout << "Process " << i + 1 << " could not be allocated." << endl;
            }

            displayBlockState(blockSize, tempBlockSize, number_of_blocks);
        }
        printAllocation("Next Fit", allocation, tempBlockSize, processSize, number_of_process);
    }
    catch (const exception& e) {
        cerr << "Error in Next Fit allocation: " << e.what() << endl;
    }
}

// Main function
int main() {
    try {
        int number_of_blocks, number_of_process, choice;

        cout << "Enter the number of blocks: ";
        cin >> number_of_blocks;

        if (number_of_blocks <= 0) throw invalid_argument("Number of blocks must be positive.");

        cout << "Enter the number of processes: ";
        cin >> number_of_process;

        if (number_of_process <= 0) {
            throw invalid_argument("Number of processes must be positive.");
        }
        
        int blockSize[number_of_blocks], processSize[number_of_process];

        cout << "Enter the block sizes: ";
        for (int i = 0; i < number_of_blocks; i++) {
            cin >> blockSize[i];
            if (blockSize[i] <= 0) throw invalid_argument("Block sizes must be positive.");
        }

        cout << "Enter the process sizes: ";
        for (int i = 0; i < number_of_process; i++) {
            cin >> processSize[i];
            if (processSize[i] <= 0) throw invalid_argument("Process sizes must be positive.");
        }

        do {
            cout << "\nChoose Allocation Method:\n";
            cout << "1. First Fit\n2. Best Fit\n3. Worst Fit\n4. Next Fit\n0. Exit\nEnter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: 
                    firstFit(blockSize, processSize, number_of_blocks, number_of_process); 
                    break;

                case 2: 
                    bestFit(blockSize, processSize, number_of_blocks, number_of_process); 
                    break;
                    
                case 3: 
                    worstFit(blockSize, processSize, number_of_blocks, number_of_process); 
                    break;
                    
                case 4: 
                    nextFit(blockSize, processSize, number_of_blocks, number_of_process); 
                    break;
                    
                case 0: 
                    cout << "Exiting program.\n"; 
                    break;
                    
                default: 
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 0);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "Program terminated due to error." << endl;
        return 1;
    }
    return 0;
}


// Enter the number of blocks: 5
// Enter the number of processes: 4
// Enter the block sizes: 100 500 200 300 600
// Enter the process sizes: 212 417 112 426