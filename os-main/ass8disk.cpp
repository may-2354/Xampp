#include <bits/stdc++.h>
using namespace std;

int fifo(const vector<int>& req, int head, int seekTime) {
    int total = 0, curr = head;
    cout << "\nFIFO Order:\nHead at: " << curr;
    for (int r : req) {
        cout << " -> " << r;
        total += abs(r - curr);
        curr = r;
    }
    int totalTime = total * seekTime;
    cout << "\nTotal Seek Time (FIFO): " << totalTime << " ms\n";
    return totalTime;
}

int sstf(vector<int> req, int head, int seekTime) {
    int total = 0, curr = head;
    vector<bool> visited(req.size(), false);
    cout << "\nSSTF Order:\nHead at: " << curr;

    for (size_t i = 0; i < req.size(); ++i) {
        int minDist = INT_MAX, index = -1;
        for (size_t j = 0; j < req.size(); ++j) {
            if (!visited[j] && abs(req[j] - curr) < minDist) {
                minDist = abs(req[j] - curr);
                index = j;
            }
        }
        visited[index] = true;
        cout << " -> " << req[index];
        total += abs(req[index] - curr);
        curr = req[index];
    }

    int totalTime = total * seekTime;
    cout << "\nTotal Seek Time (SSTF): " << totalTime << " ms\n";
    return totalTime;
}

int scan(vector<int> req, int head, int seekTime, int disk) {
    req.push_back(head);
    req.push_back(0);
    req.push_back(disk - 1);  // Add upper boundary
    sort(req.begin(), req.end());

    int total = 0, curr = head;
    int idx = find(req.begin(), req.end(), head) - req.begin();

    cout << "\nSCAN Order:\nHead at: " << curr;

    // Move right
    for (size_t i = idx + 1; i < req.size(); ++i) {
        cout << " -> " << req[i];
        total += abs(req[i] - curr);
        curr = req[i];
    }

    // Move left
    for (int i = idx - 1; i >= 0; --i) {
        cout << " -> " << req[i];
        total += abs(req[i] - curr);
        curr = req[i];
    }

    int totalTime = total * seekTime;
    cout << "\nTotal Seek Time (SCAN): " << totalTime << " ms\n";
    return totalTime;
}

int cscan(vector<int> req, int head, int seekTime, int disk) {
    req.push_back(head);
    req.push_back(0);
    req.push_back(disk - 1);
    sort(req.begin(), req.end());

    int total = 0, curr = head;
    int idx = find(req.begin(), req.end(), head) - req.begin();

    cout << "\nC-SCAN Order:\nHead at: " << curr;

    // Move right
    for (size_t i = idx + 1; i < req.size(); ++i) {
        cout << " -> " << req[i];
        total += abs(req[i] - curr);
        curr = req[i];
    }

    // Jump to 0 (simulate circular)
    if (curr != disk - 1) {
        total += abs(curr - (disk - 1));
        curr = 0;
        cout << " -> " << curr;
    }

    // Process from start to head
    for (size_t i = 0; i < idx; ++i) {
        cout << " -> " << req[i];
        total += abs(req[i] - curr);
        curr = req[i];
    }

    int totalTime = total * seekTime;
    cout << "\nTotal Seek Time (C-SCAN): " << totalTime << " ms\n";
    return totalTime;
}


int main() {
    int seekTime, n, disk, head;
    vector<int> req;

    try {
        cout << "Enter Seek Time per Track (in ms): ";
            cin >> seekTime;
        cout << "Enter Number of Requests: ";
            cin >> n;
        req.resize(n);
        cout << "Enter Request Sequence: ";
        for (int i = 0; i < n; ++i) {
                cin >> req[i];
            }

        cout << "Enter Total Disk Size: ";
            cin >> disk;
        cout << "Enter Initial Head Position: ";
        cin >> head;
        int choice;
        do {
            cout << "\n--- Disk Scheduling Menu ---\n";
            cout << "1. FIFO\n";
            cout << "2. SSTF\n";
            cout << "3. SCAN\n";
            cout << "4. C-SCAN\n";
            cout << "5. Run All & Compare\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            cin >> choice;
            

            switch (choice) {
                case 1:
                    fifo(req, head, seekTime);
                    break;
                case 2:
                    sstf(req, head, seekTime);
                    break;
                case 3:
                    scan(req, head, seekTime, disk);
                    break;
                case 4:
                    cscan(req, head, seekTime, disk);
                    break;
                case 5: {
                    map<string, int> times;
                    times["FIFO"] = fifo(req, head, seekTime);
                    times["SSTF"] = sstf(req, head, seekTime);
                    times["SCAN"] = scan(req, head, seekTime, disk);
                    times["C-SCAN"] = cscan(req, head, seekTime, disk);

                    auto best = times.begin();
                    for (auto it = times.begin(); it != times.end(); ++it) {
                        if (it->second < best->second) {
                            best = it;
                        }
                    }

                    cout << "\nMost Efficient Algorithm: " << best->first
                         << " with Total Seek Time: " << best->second << " ms\n";
                    break;
                }
                case 0:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }

        } while (choice != 0);
    }
    catch (const exception& e) {
        cerr << "\nError: " << e.what() << endl;
        return 1;
    }

    return 0;
}

// Seek Time per Track: 2 ms
// Request Sequence: [98, 183, 37, 122, 14, 124, 65, 67]
// Disk Size: 200
// Initial Head Position: 53