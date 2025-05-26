#include <iostream>
#include <vector>
using namespace std;

bool isSafe(vector<vector<int>> &alloc, vector<vector<int>> &max, vector<int> &avail, int p, int r) {
    vector<int> work = avail; 
    vector<bool> finish(p, false);
    vector<int> safeSeq;

    int count = 0;
    while (count < p) {
        bool found = false;
        
        for (int i = 0; i < p; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < r; j++) {
                    if (max[i][j] - alloc[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                
                if (canAllocate) {
                    // Allocate resources to process `i`
                    for (int j = 0; j < r; j++) {
                        work[j] += alloc[i][j];
                    }
                    safeSeq.push_back(i);
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        
        if (!found) {
            cout << "\nThe system is in an **unsafe** state!" << endl;
            return false;
        }
    }

    cout << "\nThe system is in a **safe** state." << endl;
    cout << "Safe Sequence: ";
    for (int i : safeSeq) {
        cout << "P" << i << " ";
    }
    cout << endl;
    return true;
}

void requestResources(vector<vector<int>> &alloc, vector<vector<int>> &max, vector<int> &avail, int p, int r) {
    int process;
    cout << "\nEnter the process number (0 to " << p - 1 << ") making the request: ";
    cin >> process;

    vector<int> request(r);
    cout << "Enter the requested resources: ";
    for (int i = 0; i < r; i++) {
        cin >> request[i];
    }

    // Check if request <= need
    for (int i = 0; i < r; i++) {
        if (request[i] > max[process][i] - alloc[process][i]) {
            cout << "\nError: Process has exceeded its maximum claim." << endl;
            return;
        }
    }

    // Check if request <= available
    for (int i = 0; i < r; i++) {
        if (request[i] > avail[i]) {
            cout << "\nResources not available." << endl;
            return;
        }
    }

    // Pretend to allocate resources temporarily
    for (int i = 0; i < r; i++) {
        avail[i] -= request[i];
        alloc[process][i] += request[i];
    }

    // Check if the system is still in a safe state
    if (isSafe(alloc, max, avail, p, r)) {
        cout << "\nRequest granted successfully." << endl;
    } else {
        cout << "\nRequest denied to avoid unsafe state." << endl;
        // Rollback
        for (int i = 0; i < r; i++) {
            avail[i] += request[i];
            alloc[process][i] -= request[i];
        }
    }
}

int main() {
    int p, r;

    cout << "Enter the number of processes: ";
    cin >> p;
    
    cout << "Enter the number of resources: ";
    cin >> r;

    vector<vector<int>> max(p, vector<int>(r));
    vector<vector<int>> alloc(p, vector<int>(r));
    vector<int> avail(r);
    
    cout << "\nEnter the maximum resource matrix:\n";
    for (int i = 0; i < p; i++) {
        cout << "Process P" << i << ": ";
        for (int j = 0; j < r; j++) {
            cin >> max[i][j];
        }
    }

    cout << "\nEnter the allocation matrix:\n";
    for (int i = 0; i < p; i++) {
        cout << "Process P" << i << ": ";
        for (int j = 0; j < r; j++) {
            cin >> alloc[i][j];
        }
    }

    cout << "\nEnter the available resources: ";
    for (int i = 0; i < r; i++) {
        cin >> avail[i];
    }

    // Initial safety check
    isSafe(alloc, max, avail, p, r);

    // Handling resource requests
    char choice;
    do {
        requestResources(alloc, max, avail, p, r);
        cout << "\nDo you want to make another request? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

// Enter the number of processes: 5
// Enter the number of resources: 3

// Enter the maximum resource matrix:
// Process P0: 7 5 3
// Process P1: 3 2 2
// Process P2: 9 0 2
// Process P3: 2 2 2
// Process P4: 4 3 3

// Enter the allocation matrix:
// Process P0: 0 1 0
// Process P1: 2 0 0
// Process P2: 3 0 2
// Process P3: 2 1 1
// Process P4: 0 0 2

// Enter the available resources: 3 3 2

// The system is in a **safe** state.
// Safe Sequence: P1 P3 P4 P0 P2
