#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include<climits>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int responseTime;
    int turnaroundTime;
};

// Function to sort the processes by arrival time
bool sortByArrivalTime(Process p1, Process p2) {
    return p1.arrivalTime < p2.arrivalTime;
}

int main() {
    // Queue 1: Round Robin scheduling algorithm with quantum (q=8)
    int q1 = 8;
    // Queue 2: Round Robin scheduling algorithm with quantum (q=16)
    int q2 = 16;
    // Queue 3: First Come First Serve scheduling
    int q3 = INT_MAX;

    // CPU time percentages for each queue
    int q1Percent = 60;
    int q2Percent = 25;
    int q3Percent = 15;

    // Input the number of processes
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    // Vector to store the processes
    vector<Process*> processes;
    // Input the arrival times and burst times of the processes
    for (int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "Enter the arrival time of process " << i + 1 << ": ";
        cin >> p.arrivalTime;
        cout << "Enter the burst time of process " << i + 1 << ": ";
        cin >> p.burstTime;
        processes.push_back(p);
    }

    // Sort the processes by arrival time
    sort(processes.begin(), processes.end(), sortByArrivalTime);

    // Vector to store the executed order of the processes
    vector<Process*> executedProcesses;

    // Current time
    int currentTime = 0;
    int cpuTime = 0;

    // Queues to store the processes
    queue<Process*> q1Queue;
    queue<Process*> q2Queue;
    queue<Process*> q3Queue;

    // Adding the processes to their respective queues based on arrival time
    for (int i = 0; i < n; i++) {
        Process p = processes[i];
        if (p.arrivalTime <= currentTime) {
            if (cpuTime < (q1Percent * 0.01 * currentTime)) {
                q1Queue.push(p);
            } else if (cpuTime < (q1Percent + q2Percent) * 0.01 * currentTime) {
                q2Queue.push(p);
            } else {
                q3Queue.push(p);
            }
        } else {
            q3Queue.push(p);
        }
    }

    // Execute the processes in the queues
    while (!q1Queue.empty() || !q2Queue.empty() || !q3Queue.empty()) {
        // Execute the processes in queue 1
        while (!q1Queue.empty()) {
            Process p = q1Queue.front();
            q1Queue.pop();
            if (p.burstTime > q1) {
                p.burstTime -= q1;
                currentTime += q1;
                q1Queue.push(p);
                cpuTime += q1;
            } else {
                currentTime += p.burstTime;
                p.burstTime = 0;
                p.waitingTime = currentTime - p.arrivalTime - p.burstTime;
                p.responseTime = currentTime - p.arrivalTime;
                p.turnaroundTime = currentTime - p.arrivalTime;
                executedProcesses.push_back(p);
                cpuTime += p.burstTime;
            }
            // Adding new processes to the queue if their arrival time has arrived
            for (int i = 0; i < n; i++) {
                Process newP = processes[i];
                if (newP.arrivalTime <= currentTime && !newP.burstTime) {
                    if (cpuTime < (q1Percent * 0.01 * currentTime)) {
                        q1Queue.push(newP);
                    } else if (cpuTime < (q1Percent + q2Percent) * 0.01 * currentTime) {
                        q2Queue.push(newP);
                    } else {
                        q3Queue.push(newP);
                    }
                }
            }
        }

        // Execute the processes in queue 2
        while (!q2Queue.empty()) {
            Process p = q2Queue.front();
            q2Queue.pop();
            if (p.burstTime > q2) {
                p.burstTime -= q2;
                currentTime += q2;
                q2Queue.push(p);
                cpuTime += q2;
            } else {
                currentTime += p.burstTime;
                p.burstTime = 0;
                p.waitingTime = currentTime - p.arrivalTime - p.burstTime;
                p.responseTime = currentTime - p.arrivalTime;
                p.turnaroundTime = currentTime - p.arrivalTime;
                executedProcesses.push_back(p);
                cpuTime += p.burstTime;
            }
            // Adding new processes to the queue if their arrival time has arrived
            for (int i = 0; i < n; i++) {
                Process newP = processes[i];
                if (newP.arrivalTime <= currentTime && !newP.burstTime) {
                    if (cpuTime < (q1Percent * 0.01 * currentTime)) {
                        q1Queue.push(newP);
                    } else if (cpuTime < (q1Percent + q2Percent) * 0.01 * currentTime) {
                        q2Queue.push(newP);
                    } else {
                        q3Queue.push(newP);
                    }
                }
            }
        }

        // Execute the processes in queue 3
        while (!q3Queue.empty()) {
            Process p = q3Queue.front();
            q3Queue.pop();
            currentTime += p.burstTime;
            p.burstTime = 0;
            p.waitingTime = currentTime - p.arrivalTime - p.burstTime;
            p.responseTime = currentTime - p.arrivalTime;
            p.turnaroundTime = currentTime - p.arrivalTime;
            executedProcesses.push_back(p);
            cpuTime += p.burstTime;

        // Adding new processes to the queue if their arrival time has arrived
        for (int i = 0; i < n; i++) {
            Process newP = processes[i];
            if (newP.arrivalTime <= currentTime && !newP.burstTime) {
                if (cpuTime < (q1Percent * 0.01 * currentTime)) {
                    q1Queue.push(newP);
                } else if (cpuTime < (q1Percent + q2Percent) * 0.01 * currentTime) {
                    q2Queue.push(newP);
                } else {
                    q3Queue.push(newP);
                }
            }
        }
    }
}
cout << "Executed Order of the Processes:" << endl;
for (int i = 0; i < executedProcesses.size(); i++) {
    cout << "Process " << executedProcesses[i].id << endl;
}

// Calculate the average waiting time
int totalWaitingTime = 0;
for (int i = 0; i < executedProcesses.size(); i++) {
    totalWaitingTime += executedProcesses[i].waitingTime;
}
double avgWaitingTime = (double)totalWaitingTime / n;

// Calculate the average response time
int totalResponseTime = 0;
for (int i = 0; i < executedProcesses.size(); i++) {
    totalResponseTime += executedProcesses[i].responseTime;
}
double avgResponseTime = (double)totalResponseTime / n;

// Calculate the average turnaround time
int totalTurnaroundTime = 0;
for (int i = 0; i < executedProcesses.size(); i++) {
    totalTurnaroundTime += executedProcesses[i].turnaroundTime;
}
double avgTurnaroundTime = (double)totalTurnaroundTime / n;

// Display the calculated values
cout << "Average Waiting Time: " << avgWaitingTime << endl;
cout << "Average Response Time: " << avgResponseTime << endl;
cout << "Average Turnaround Time: " << avgTurnaroundTime << endl;

return 0;

}
