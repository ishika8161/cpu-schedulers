#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 100
#define MAX_TIME 1000
// struct for a process
struct Process {
int pid;
int arrival_time;
int burst_time;
int priority;
int remaining_time;
int turnaround_time;
int response_time;
};
// struct for a queue of processes
struct Queue {
struct Process processes[MAX_PROCESSES];
int front;
int rear;
int size;
};
// struct for storing simulation results
struct Results {
float avg_waiting_time;

float avg_turnaround_time;
float avg_response_time;
};
// global variables
struct Process processes[MAX_PROCESSES];
int num_processes = 0;
struct Queue queues[4]; // one queue for each scheduling algorithm
// function prototypes
void enqueue_process(struct Queue *queue, struct Process process);
struct Process dequeue_process(struct Queue *queue);
void simulate_fcfs(struct Results *results);
void simulate_sjf(struct Results *results);
void simulate_priority(struct Results *results);
void simulate_round_robin(int time_quantum, struct Results *results);
int main() {
// read in the processes from input.txt
FILE *input_file = fopen("input.txt", "r");
if (input_file == NULL) {
printf("Error: input file not found\n");
return 1;
}
while (fscanf(input_file, "%d %d %d %d", &processes[num_processes].pid,
&processes[num_processes].arrival_time,

&processes[num_processes].burst_time, &processes[num_processes].priority) !=

EOF) {
processes[num_processes].remaining_time = processes[num_processes].burst_time;
num_processes++;
}
fclose(input_file);
// simulate each scheduling algorithm and print the results
struct Results results;
// First Come First Serve
printf("First Come First Serve:\n");
simulate_fcfs(&results);
printf("Average waiting time: %.2f\n", results.avg_waiting_time);

printf("Average turnaround time: %.2f\n", results.avg_turnaround_time);
printf("Average response time: %.2f\n\n", results.avg_response_time);
// Shortest Job First
printf("Shortest Job First:\n");
simulate_sjf(&results);
printf("Average waiting time: %.2f\n", results.avg_waiting_time);
printf("Average turnaround time: %.2f\n", results.avg_turnaround_time);
printf("Average response time: %.2f\n\n", results.avg_response_time);
// Priority Scheduling
printf("Priority Scheduling:\n");
simulate_priority(&results);
printf("Average waiting time: %.2f\n", results.avg_waiting_time);
printf("Average turnaround time: %.2f\n", results.avg_turnaround_time);
printf("Average response time: %.2f\n\n", results.avg_response_time);
// Round Robin
printf("Round Robin:\n");
int time_quantum;
printf("Enter time quantum: ");
scanf("%d", &time_quantum);
simulate_round_robin(time_quantum, &results);
printf("Average waiting time: %.2f\n", results.avg_waiting_time);
printf("Average turnaround time: %.2f\n", results.avg_turnaround_time);
printf("Average response time: %.2f\n", results.avg_response_time);
struct Results fcfs_results, sjf_results, priority_results, rr_results;
// First Come First Serve
simulate_fcfs(&fcfs_results);
// Shortest Job First
simulate_sjf(&sjf_results);
// Priority Scheduling
simulate_priority(&priority_results);
// Round Robin
printf("Enter time quantum: ");
scanf("%d", &time_quantum);
simulate_round_robin(time_quantum, &rr_results);

// Compare the results
printf("Comparison of Scheduling Algorithms:\n");
printf("-----------------------------------\n");
// Compare average turnaround time
printf("Average Turnaround Time:\n");
printf("FCFS: %.2f\n", fcfs_results.avg_turnaround_time);
printf("SJF: %.2f\n", sjf_results.avg_turnaround_time);
printf("Priority: %.2f\n", priority_results.avg_turnaround_time);
printf("Round Robin: %.2f\n", rr_results.avg_turnaround_time);
// Find the minimum average turnaround time
float min_turnaround_time = fcfs_results.avg_turnaround_time;
const char* best_algorithm = "First Come First Serve";
if (sjf_results.avg_turnaround_time < min_turnaround_time) {
min_turnaround_time = sjf_results.avg_turnaround_time;
best_algorithm = "Shortest Job First";
}
if (priority_results.avg_turnaround_time < min_turnaround_time) {
min_turnaround_time = priority_results.avg_turnaround_time;
best_algorithm = "Priority Scheduling";
}
if (rr_results.avg_turnaround_time < min_turnaround_time) {
min_turnaround_time = rr_results.avg_turnaround_time;
best_algorithm = "Round Robin";
}
printf("\nThe best scheduling algorithm is: %s\n", best_algorithm);
return 0;
}
// function to add a process to the back of a queue
void enqueue_process(struct Queue *queue, struct Process process) {
if (queue->size == MAX_PROCESSES) {
printf("Error: queue is full\n");
exit(1);
}
queue->processes[queue->rear] = process;
queue->rear = (queue->rear + 1) % MAX_PROCESSES;

queue->size++;
}
// function to remove a process from the front of a queue
struct Process dequeue_process(struct Queue *queue) {
if (queue->size == 0) {
printf("Error: queue is empty\n");
exit(1);
}
struct Process process = queue->processes[queue->front];
queue->front = (queue->front + 1) % MAX_PROCESSES;
queue->size--;
return process;
}
// function to simulate First Come First Serve scheduling
void simulate_fcfs(struct Results *results) {
int current_time = 0;
int total_waiting_time = 0;
int total_turnaround_time = 0;
int total_response_time = 0;
struct Queue queue = {.front = 0, .rear = 0, .size = 0};
for (int i = 0; i < num_processes; i++) {
enqueue_process(&queue, processes[i]);
}
while (queue.size > 0) {
struct Process process = dequeue_process(&queue);
// calculate waiting time, turnaround time, and response time
int waiting_time = current_time - process.arrival_time;
int turnaround_time = waiting_time + process.burst_time;
int response_time = waiting_time;
// update total waiting time, turnaround time, and response time
total_waiting_time += waiting_time;
total_turnaround_time += turnaround_time;
total_response_time += response_time;
// update current time
current_time += process.burst_time;

}
// calculate and store average waiting time, turnaround time, and response time
results->avg_waiting_time = (float) total_waiting_time / num_processes;
results->avg_turnaround_time = (float) total_turnaround_time / num_processes;
results->avg_response_time = (float) total_response_time / num_processes;
}
// function to simulate Shortest Job First scheduling
void simulate_sjf(struct Results *results) {
int current_time = 0;
int total_waiting_time = 0;
int total_turnaround_time = 0;
int total_response_time = 0;
struct Queue queue = {.front = 0, .rear = 0, .size = 0};
int num_completed_processes = 0;
while (num_completed_processes < num_processes) {
// add any processes that have arrived to the queue
for (int i = 0; i < num_processes; i++) {
if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
enqueue_process(&queue, processes[i]);
}
}
// if the queue is empty, increment the current time
if (queue.size == 0) {
current_time++;
continue;
}
// find the process with the shortest remaining time
int shortest_remaining_time = MAX_TIME + 1;
int shortest_process_index = -1;
for (int i = 0; i < queue.size; i++) {
int remaining_time = queue.processes[(queue.front + i) %
MAX_PROCESSES].remaining_time;
if (remaining_time < shortest_remaining_time) {
shortest_remaining_time = remaining_time;
shortest_process_index = (queue.front + i) % MAX_PROCESSES;
}

}
// remove the shortest job from the queue
struct Process shortest_process = queue.processes[shortest_process_index];
dequeue_process(&queue);
// calculate waiting time, turnaround time, and response time
int waiting_time = current_time - shortest_process.arrival_time;
int turnaround_time = waiting_time + shortest_process.burst_time;
int response_time = waiting_time;
// update total waiting time, turnaround time, and response time
total_waiting_time += waiting_time;
total_turnaround_time += turnaround_time;
total_response_time += response_time;
// update current time and remaining time for the executed process
current_time += shortest_process.burst_time;
shortest_process.remaining_time = 0;
// mark the process as completed
num_completed_processes++;
}
// calculate and store average waiting time, turnaround time, and response time
results->avg_waiting_time = (float)total_waiting_time / num_processes;
results->avg_turnaround_time = (float)total_turnaround_time / num_processes;
results->avg_response_time = (float)total_response_time / num_processes;
}
// function to simulate Priority Scheduling
void simulate_priority(struct Results *results) {
int current_time = 0;
int total_waiting_time = 0;
int total_turnaround_time = 0;
int total_response_time = 0;
struct Queue queue = {.front = 0, .rear = 0, .size = 0};
int num_completed_processes = 0;
while (num_completed_processes < num_processes) {
// add any processes that have arrived to the queue

for (int i = 0; i < num_processes; i++) {
if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
enqueue_process(&queue, processes[i]);
}
}
// if the queue is empty, increment the current time
if (queue.size == 0) {
current_time++;
continue;
}
// find the process with the highest priority
int highest_priority = MAX_PROCESSES + 1;
int highest_priority_index = -1;
for (int i = 0; i < queue.size; i++) {
int priority = queue.processes[(queue.front + i) % MAX_PROCESSES].priority;
if (priority < highest_priority) {
highest_priority = priority;
highest_priority_index = (queue.front + i) % MAX_PROCESSES;
}
}
// remove the highest priority process from the queue
struct Process highest_priority_process = queue.processes[highest_priority_index];
dequeue_process(&queue);
// calculate waiting time, turnaround time, and response time
int waiting_time = current_time - highest_priority_process.arrival_time;
int turnaround_time = waiting_time + highest_priority_process.burst_time;
int response_time = waiting_time;
// update total waiting time, turnaround time, and response time
total_waiting_time += waiting_time;
total_turnaround_time += turnaround_time;
total_response_time += response_time;
// update current time and remaining time for the executed process
current_time += highest_priority_process.burst_time;
highest_priority_process.remaining_time = 0;
// mark the process as completed

num_completed_processes++;
}
// calculate and store average waiting time, turnaround time, and response time
results->avg_waiting_time = (float)total_waiting_time / num_processes;
results->avg_turnaround_time = (float)total_turnaround_time / num_processes;
results->avg_response_time = (float)total_response_time / num_processes;
}
// function to simulate Round Robin scheduling
void simulate_round_robin(int time_quantum, struct Results *results) {
int current_time = 0;
int total_waiting_time = 0;
int total_turnaround_time = 0;
int total_response_time = 0;
struct Queue queue = {.front = 0, .rear = 0, .size = 0};
int num_completed_processes = 0;
while (num_completed_processes < num_processes) {
// add any processes that have arrived to the queue
for (int i = 0; i < num_processes; i++) {
if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
enqueue_process(&queue, processes[i]);
}
}
// if the queue is empty, increment the current time
if (queue.size == 0) {
current_time++;
continue;
}
// execute the process for the time quantum
struct Process current_process = dequeue_process(&queue);
// calculate response time if the process is being executed for the first time
if (current_process.response_time == -1) {
current_process.response_time = current_time - current_process.arrival_time;
}
// calculate the remaining time for the process after executing for the time quantum
if (current_process.remaining_time > time_quantum) {
current_process.remaining_time -= time_quantum;
current_time += time_quantum;

enqueue_process(&queue, current_process);
} else {
current_time += current_process.remaining_time;
current_process.remaining_time = 0;
// calculate waiting time and turnaround time
int waiting_time = current_time - current_process.arrival_time -
current_process.burst_time;
int turnaround_time = waiting_time + current_process.burst_time;
// update total waiting time, turnaround time, and response time
total_waiting_time += waiting_time;
total_turnaround_time += turnaround_time;
total_response_time += current_process.response_time;
// mark the process as completed
num_completed_processes++;
}
}
// calculate and store average waiting time, turnaround time, and response time
results->avg_waiting_time = (float)total_waiting_time / num_processes;
results->avg_turnaround_time = (float)total_turnaround_time / num_processes;
results->avg_response_time = (float)total_response_time / num_processes;
}