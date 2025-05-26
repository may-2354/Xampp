#include <queue>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <climits>
using namespace std;

void calculatetat(int n, vector<int>& ct, vector<int>& at, vector<int>& tat) {
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
    }
}

void calculateWaitingTime(int n, vector<int>& tat, vector<int>& bt, vector<int>& wt) {
    for (int i = 0; i < n; i++) {
        wt[i] = tat[i] - bt[i];
    }
}

void printResults(int n, vector<int>& process, vector<int>& at, vector<int>& bt, 
    vector<int>& ct, vector<int>& tat, vector<int>& wt, 
    vector<int>& gantt) {
cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT" << endl;
for (int i = 0; i < n; i++) {
cout << process[i] << "\t" << at[i] << "\t" << bt[i] << "\t"
<< ct[i] << "\t" << tat[i] << "\t" << wt[i] << endl;
}

cout << "\nGantt Chart:\n";
cout << " ";
for (int i = 0; i < gantt.size(); i++) {
cout << "----";
}
cout << "\n|";

for (int i = 0; i < gantt.size(); i++) {
cout << " P" << gantt[i] << " |";
}

cout << "\n ";
for (int i = 0; i < gantt.size(); i++) {
cout << "----";
}

cout << "\n0";
int time = 0;
for (int i = 0; i < gantt.size(); i++) {
time += bt[gantt[i] - 1]; 
cout << "   " << time;
}
cout << endl;
}


void fcfs(int n, vector<int>& process, vector<int>& at, vector<int>& bt) {
    vector<int> ct(n), tat(n), wt(n);
    vector<int> gantt;
    int ttat = 0, twt = 0;

    ct[0] = at[0] + bt[0];
    gantt.push_back(process[0]);

    for (int i = 1; i < n; i++) {
        int startTime = max(ct[i - 1], at[i]);
        ct[i] = startTime + bt[i];
        gantt.push_back(process[i]);
    }

    calculatetat(n, ct, at, tat);
    calculateWaitingTime(n, tat, bt, wt);

    for (int i = 0; i < n; i++) {
        ttat += tat[i];
        twt += wt[i];
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}



void sjfNonPreemptive(int n, vector<int>& process, vector<int>& at, vector<int>& bt) {
    vector<int> ct(n), tat(n), wt(n);
    vector<int> gantt;
    int ttat = 0, twt = 0;
    vector<bool> visited(n, false);
    int currentTime = 0, completed = 0;

    while (completed < n) {
        int minBurst = INT_MAX, minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && at[i] <= currentTime && bt[i] < minBurst) {
                minBurst = bt[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            visited[minIndex] = true;
            currentTime += bt[minIndex];
            ct[minIndex] = currentTime;
            tat[minIndex] = ct[minIndex] - at[minIndex];
            wt[minIndex] = tat[minIndex] - bt[minIndex];

            gantt.push_back(process[minIndex]);
            ttat += tat[minIndex];
            twt += wt[minIndex];
            completed++;
        }
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}

void sjfPreemptive(int n, vector<int>& process, vector<int>& at, vector<int>& bt) {
    vector<int> remainingTime(n), ct(n), tat(n), wt(n);
    vector<int> gantt;
    int ttat = 0, twt = 0;
    int completed = 0, currentTime = 0;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) remainingTime[i] = bt[i];

    while (completed < n) {
        int minBurst = INT_MAX, minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (at[i] <= currentTime && remainingTime[i] > 0 && remainingTime[i] < minBurst) {
                minBurst = remainingTime[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            remainingTime[minIndex]--;
            currentTime++;

            gantt.push_back(process[minIndex]);

            if (remainingTime[minIndex] == 0) {
                completed++;
                ct[minIndex] = currentTime;
                tat[minIndex] = ct[minIndex] - at[minIndex];
                wt[minIndex] = tat[minIndex] - bt[minIndex];

                ttat += tat[minIndex];
                twt += wt[minIndex];
            }
        }
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}

void roundRobin(int n, vector<int>& process, vector<int>& at, vector<int>& bt, int timeQuantum) {
    vector<int> remainingTime(n), ct(n, -1), tat(n), wt(n);
    vector<int> gantt;
    int ttat = 0, twt = 0;
    queue<int> readyQueue;
    int time = 0, completed = 0;

    for (int i = 0; i < n; i++) {
        remainingTime[i] = bt[i];
    }

    int index = 0;
    while (completed < n) {
        while (index < n && at[index] <= time) {
            readyQueue.push(index);
            index++;
        }

        if (!readyQueue.empty()) {
            int idx = readyQueue.front();
            readyQueue.pop();
            gantt.push_back(process[idx]);

            int executionTime = min(timeQuantum, remainingTime[idx]);
            time += executionTime;
            remainingTime[idx] -= executionTime;

            while (index < n && at[index] <= time) {
                readyQueue.push(index);
                index++;
            }

            if (remainingTime[idx] == 0) {
                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];

                ttat += tat[idx];
                twt += wt[idx];
                completed++;
            } else {
                readyQueue.push(idx);
            }
        } else {
            time = at[index]; 
        }
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}

void priorityNonPreemptive(int n, vector<int>& process, vector<int>& at,
                           vector<int>& bt, vector<int>& priority) {
    vector<int> ct(n), tat(n), wt(n);
    vector<int> gantt;
    vector<bool> isDone(n, false);
    int ttat = 0, twt = 0;
    int currentTime = 0, completed = 0;

    while (completed < n) {
        int idx = -1;
        int minPriority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!isDone[i] && at[i] <= currentTime) {
                if (priority[i] < minPriority) {
                    minPriority = priority[i];
                    idx = i;
                } else if (priority[i] == minPriority) {
                    if (at[i] < at[idx]) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            currentTime = max(currentTime, at[idx]);
            currentTime += bt[idx];

            ct[idx] = currentTime;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            ttat += tat[idx];
            twt += wt[idx];
            gantt.push_back(process[idx]);
            isDone[idx] = true;
            completed++;
        } else {
            currentTime++;
        }
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}

void priorityPreemptive(int n, vector<int>& process, vector<int>& at, vector<int>& bt, vector<int>& priority) {
    vector<int> remainingTime(bt.begin(), bt.end());
    vector<int> ct(n, -1), tat(n), wt(n);
    vector<int> gantt;
    int ttat = 0, twt = 0;
    int completed = 0, currentTime = 0;
    vector<bool> isCompleted(n, false);

    while (completed < n) {
        int minPriority = INT_MAX, idx = -1;

        for (int i = 0; i < n; i++) {
            if (at[i] <= currentTime && remainingTime[i] > 0) {
                if (priority[i] < minPriority) {
                    minPriority = priority[i];
                    idx = i;
                } else if (priority[i] == minPriority) {
                    // Tie-breaking with earlier arrival
                    if (at[i] < at[idx]) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            gantt.push_back(process[idx]);
            remainingTime[idx]--;
            currentTime++;

            if (remainingTime[idx] == 0) {
                completed++;
                ct[idx] = currentTime;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
                ttat += tat[idx];
                twt += wt[idx];
                isCompleted[idx] = true;
            }
        } else {
            currentTime++;
        }
    }

    printResults(n, process, at, bt, ct, tat, wt, gantt);

    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << (float)ttat / n << endl;
    cout << "Average Waiting Time: " << (float)twt / n << endl;
}


int main() {
    int choice, n;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<int> process(n), at(n), bt(n), priority(n);

    cout << "Enter the processes: \n";
    for (int i = 0; i < n; i++) {
        cin >> process[i];
    }

    cout << "Enter the arrival time for the respective processes: \n";
    for (int i = 0; i < n; i++) {
        cin >> at[i];
    }

    cout << "Enter the burst time for the respective processes: \n";
    for (int i = 0; i < n; i++) {
        cin >> bt[i];
    }

    do {
        cout << "\nChoose a scheduling algorithm:";
        cout << "\n1. First Come First Serve (FCFS)";
        cout << "\n2. Non-Preemptive Shortest Job First (SJF)";
        cout << "\n3. Preemptive Shortest Job First (SJF)";
        cout << "\n4. Round Robin";
        cout << "\n5. Priority Non-Preemptive";
        cout << "\n6. Priority Preemptive";
        cout << "\n7. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                fcfs(n, process, at, bt);
                break;
            case 2:
                sjfNonPreemptive(n, process, at, bt);
                break;
            case 3:
                sjfPreemptive(n, process, at, bt);
                break;
            case 4:
                int timeQuantum;
                cout << "Enter time quantum: ";
                cin >> timeQuantum;
                roundRobin(n, process, at, bt, timeQuantum);
                break;
            case 5:
                cout << "Enter the priority for the respective processes: \n";
                for (int i = 0; i < n; i++) {
                    cin >> priority[i];
                }
                priorityNonPreemptive(n, process, at, bt, priority);
                break;
            case 6:
                cout << "Enter the priority for the respective processes: \n";
                for (int i = 0; i < n; i++) {
                    cin >> priority[i];
                }
                priorityPreemptive(n, process, at, bt, priority);
                break;
            case 7:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
// Enter the number of processes: 4
// Enter the processes: 
// 1 2 3 4
// Enter the arrival time for the respective processes: 
// 0 1 2 3
// Enter the burst time for the respective processes: 
// 8 4 2 1
