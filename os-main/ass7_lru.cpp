#include <bits/stdc++.h>
using namespace std;

// First-Come, First-Served (FCFS) Page Replacement Algorithm
int fcfs(const vector<int>& seq, int frames) {
    try {
        vector<int> frameSpace(frames, -1); // Initialize all frames with -1 (empty)
        int framePointer = 0; // Points to the position where next page will be placed
        int faults = 0; // To count page faults

        cout << "The sequence are: " << endl;
        for (int i = 0; i < seq.size(); i++) {
            cout << seq[i] << " "; // Display the page sequence
        }
        cout << endl;

        // Process each page in the sequence
        for (int i = 0; i < seq.size(); i++) {
            int page = seq[i];
            bool pageFound = false;
            
            // Check if page is already in the frame
            for (int j = 0; j < frames; j++) {
                if (frameSpace[j] == page) {
                    pageFound = true;
                    break;
                }
            }

            cout << "Step " << i + 1 << ": Page " << page << " -> ";

            if (!pageFound) { // Page fault if page not found
                faults++; // Increment page fault
                
                // In FCFS, we replace the page at the current frame pointer position
                if (frameSpace[framePointer] != -1) {
                    cout << "Removing " << frameSpace[framePointer] << ", "; // Show which page is being removed
                }
                
                frameSpace[framePointer] = page; // Replace with the new page
                cout << "Inserted " << page;
                
                // Move the frame pointer in a circular manner
                framePointer = (framePointer + 1) % frames;
            } else {
                cout << "Page Hit!"; // Page hit if already in frame
            }

            // Display current frame status
            cout << " | Frames: [ ";
            for (int p : frameSpace) {
                if (p != -1) cout << p << " ";
                else cout << "- ";  // Show empty frame
            }
            if(!pageFound){
                cout << "] *\n";
            }else{
                cout << "]\n";
            }
        }

        cout << "\nTotal Page Faults: " << faults << endl;
        return faults; // Return the total page faults
    } catch (const exception& e) {
        cerr << "Error in FCFS algorithm: " << e.what() << endl;
        return -1;
    }
}

// Least Recently Used (LRU) Page Replacement Algorithm
int lru(const vector<int>& seq, int frames) {
    try {
        vector<int> frameSpace(frames, -1); // Initialize with empty frames
        vector<int> lastUsed(frames, -1); // Track when each frame was last used
        int faults = 0;

        cout << "The sequence are: " << endl;
        for (int i = 0; i < seq.size(); i++) {
            cout << seq[i] << " "; // Display the page sequence
        }
        cout << endl;

        for (int i = 0; i < seq.size(); i++) {
            int page = seq[i];
            bool pageFound = false;
            int frameIndex = -1;
            
            // Check if page exists and record its position
            for (int j = 0; j < frames; j++) {
                if (frameSpace[j] == page) {
                    pageFound = true;
                    frameIndex = j;
                    break;
                }
            }

            cout << "Step " << i + 1 << ": Page " << page << " -> ";

            if (!pageFound) { // Page fault
                faults++;
                
                // Find the least recently used frame
                int lruIndex = 0;
                for (int j = 1; j < frames; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex]) {
                        lruIndex = j;
                    }
                }
                
                // Replace the LRU page
                if (frameSpace[lruIndex] != -1) {
                    cout << "Removing " << frameSpace[lruIndex] << ", ";
                }
                
                frameSpace[lruIndex] = page; // Place new page in the LRU position
                lastUsed[lruIndex] = i; // Update usage time
                cout << "Inserted " << page;
            } else {
                // Update the usage time for the page that was hit
                lastUsed[frameIndex] = i;
                cout << "Page Hit!";
            }

            // Display current frame status
            cout << " | Frames: [ ";
            for (int p : frameSpace) {
                if (p != -1) cout << p << " ";
                else cout << "- ";
            }
            if(!pageFound){
                cout << "] *\n";
            }else{
                cout << "]\n";
            }
        }

        cout << "\nTotal Page Faults: " << faults << endl;
        return faults;
    } catch (const exception& e) {
        cerr << "Error in LRU algorithm: " << e.what() << endl;
        return -1;
    }
}

// Optimal Page Replacement Algorithm
int optimal(const vector<int>& seq, int frames) {
    try {
        vector<int> frameSpace(frames, -1); // Initialize with empty frames
        int faults = 0;

        cout << "The sequence are: " << endl;
        for (int i = 0; i < seq.size(); i++) {
            cout << seq[i] << " "; // Display the page sequence
        }
        cout << endl;

        for (int i = 0; i < seq.size(); i++) {
            int page = seq[i];
            bool pageFound = false;
            
            // Check if page is in the frame
            for (int j = 0; j < frames; j++) {
                if (frameSpace[j] == page) {
                    pageFound = true;
                    break;
                }
            }

            cout << "Step " << i + 1 << ": Page " << page << " -> ";

            if (!pageFound) { // Page fault
                faults++;
                
                // Check if there's an empty frame
                int emptyFrame = -1;
                for (int j = 0; j < frames; j++) {
                    if (frameSpace[j] == -1) {
                        emptyFrame = j;
                        break;
                    }
                }
                
                if (emptyFrame != -1) {
                    // If empty frame exists, use it
                    frameSpace[emptyFrame] = page;
                    cout << "Inserted " << page;
                } else {
                    // Find the optimal page to replace (page that won't be used for longest time)
                    vector<int> nextUse(frames, INT_MAX);
                    
                    // Calculate next use time for each frame
                    for (int j = 0; j < frames; j++) {
                        for (int k = i + 1; k < seq.size(); k++) {
                            if (seq[k] == frameSpace[j]) {
                                nextUse[j] = k;
                                break;
                            }
                        }
                    }
                    
                    // Find the frame with furthest next use
                    int optimalIndex = 0;
                    for (int j = 1; j < frames; j++) {
                        if (nextUse[j] > nextUse[optimalIndex]) {
                            optimalIndex = j;
                        }
                    }
                    
                    cout << "Removing " << frameSpace[optimalIndex] << ", ";
                    frameSpace[optimalIndex] = page; // Replace with new page
                    cout << "Inserted " << page;
                }
            } else {
                cout << "Page Hit!"; // Page hit
            }

            // Display current frame status
            cout << " | Frames: [ ";
            for (int p : frameSpace) {
                if (p != -1) cout << p << " ";
                else cout << "- ";
            }
            if(!pageFound){
                cout << "] *\n";
            }else{
                cout << "]\n";
            }
        }

        cout << "\nTotal Page Faults: " << faults << endl;
        return faults;
    } catch (const exception& e) {
        cerr << "Error in Optimal algorithm: " << e.what() << endl;
        return -1;
    }
}

// Function to compare FCFS, LRU, and Optimal algorithms
void compare_algorithms(const vector<int>& seq, int frames) {
    try {
        cout << "\nFCFS Algorithm\n";
        int fcfs_faults = fcfs(seq, frames);

        cout << "\nLRU Algorithm\n";
        int lru_faults = lru(seq, frames);

        cout << "\nOptimal Algorithm\n";
        int optimal_faults = optimal(seq, frames);

        cout << "\nComparison Analysis\n";
        cout << "Page sequence: ";
        for (int page : seq) {
            cout << page << " ";
        }
        cout << endl;

        cout << "Number of frames: " << frames << endl;
        cout << "\nPage Faults:" << endl;
        cout << "FCFS: " << fcfs_faults << " page faults" << endl;
        cout << "LRU: " << lru_faults << " page faults" << endl;
        cout << "Optimal: " << optimal_faults << " page faults" << endl;

        // Determine the best performing algorithm(s)
        int min_faults = min({fcfs_faults, lru_faults, optimal_faults});
        cout << "\nBest performing algorithm(s): ";
        if (fcfs_faults == min_faults) {
            cout << "FCFS ";
        }
        if (lru_faults == min_faults) {
            cout << "LRU ";
        }
        if (optimal_faults == min_faults) {
            cout << "Optimal ";
        }
        cout << "with " << min_faults << " page faults" << endl;

    } catch (const exception& e) {
        cerr << "Error in comparison: " << e.what() << endl;
    }
}

// Main function
int main() {
    try {
        int totalPages;
        cout << "Enter the total number of page sequence: " << endl;
        cin >> totalPages; // Input total number of pages

        vector<int> seq(totalPages);
        cout << "Enter the page sequence: " << endl;
        for (int i = 0; i < totalPages; i++) {
            cin >> seq[i]; // Input the actual page sequence
        }

        int frames;
        cout << "Enter the number of frames: ";
        cin >> frames;
        if (frames <= 0) {
            throw invalid_argument("Number of frames must be positive");
        }

        int choice;
        do {
            cout << "\n1. FCFS\n2. LRU\n3. Optimal\n4. Compare Algorithms\n0. Exit\nEnter the operation to perform: " << endl;
            cin >> choice;

            // Menu-driven interface for selecting the desired algorithm
            switch (choice) {
                case 0:
                    cout << "Exiting program..." << endl;
                    break;

                case 1:
                    cout << "\nFCFS Algorithm\n";
                    fcfs(seq, frames);
                    break;

                case 2:
                    cout << "\nLRU Algorithm\n";
                    lru(seq, frames);
                    break;

                case 3:
                    cout << "\nOptimal Algorithm\n";
                    optimal(seq, frames);
                    break;

                case 4:
                    compare_algorithms(seq, frames);
                    break;

                default:
                    cout << "Invalid choice. Please enter a number between 0 and 4." << endl;
                    break;
            }
        } while (choice != 0);

        return 0;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}

// Enter the total number of page sequence:
// 12
// Enter the page sequence:
// 7 0 1 2 0 3 0 4 2 3 0 3
// Enter the number of frames: 
// 3   fcs 9,lru 8, optimal 7