#include <iostream>
#include <numeric>
#include <math.h>
using namespace std;
#define MAX_PROCESS 10

int num_of_proc = 3, count, remain, time_quant;
int exec_time[MAX_PROCESS], time_period[MAX_PROCESS], remain_time[MAX_PROCESS], deadline[MAX_PROCESS], rem_deadline[MAX_PROCESS];
int bursting_timee[MAX_PROCESS], wait_time[MAX_PROCESS], complete_time[MAX_PROCESS], arrive_time[MAX_PROCESS];

//collecting details of processes
void get_process_info(int selected_algo) {
	cout << "Enter total number of processes(maximum " << MAX_PROCESS << "): ";
	cin >> num_of_proc;
	if (num_of_proc < 1) {
		cout << "Do you really want to schedule " << num_of_proc << " processes? -_-" << endl;
		exit(0);
	}

	for (int i = 0; i < num_of_proc; i++) {
		cout << endl << "Process " << i + 1 << ":-" << endl;
		if (selected_algo > 0) {
			cout << "==> Execution time: ";
			cin >> exec_time[i];
			remain_time[i] = exec_time[i];
			if (selected_algo == 2) {
				cout << "==> Deadline: ";
				cin >> deadline[i];
			}
			else {
				cout << "==> Period: ";
				cin >> time_period[i];
			}
		}
    		if (selected_algo > 2)
    		{
    		    cout<<"Enter Again"<<endl;
    		}
	
	}
}

//get maximum of three numbers
int getmaxx(int a, int b, int c) {
	long max, lcom, count, flag = 0;
	if (a >= b && a >= c)
		return max = a;
	else if (b >= a && b >= c)
		return max = b;
	else if (c >= a && c >= b)
		return max = c;
}

//calculating the observation time for scheduling timeline
int get_obs_time(int selected_algo) {


	if (selected_algo == 1) {
		return getmaxx(time_period[0], time_period[1], time_period[2]);
	}

	else if (selected_algo == 2) {
		return getmaxx(deadline[0], deadline[1], deadline[2]);
	}
}

//print scheduling sequence
void printing_schedule(int process_list[], int cycles) {
	cout << endl << "Scheduling:-" << endl << endl;
	cout << "Time: ";
	for (int i = 0; i < cycles; i++) {
		if (i < 10)
			cout << "| 0" << i << " ";
		else
			cout << "| " << i << " ";
	}
	cout << "|" << endl;

	for (int i = 0; i < num_of_proc; i++) {
		cout << "P[" << i + 1 << "]: ";
		for (int j = 0; j < cycles; j++) {
			if (process_list[j] == i + 1)
				cout << "|####";
			else
				cout << "|    ";
		}
		cout << "|" << endl;
	}
}

void RM(int time) {     //RATE MONOTONIC
	float utilize = 0;
	for (int i = 0; i < num_of_proc; i++) {
		utilize += (1.0 * exec_time[i]) / time_period[i];
	}
	int n = num_of_proc;
	if (utilize > n * (pow(2, 1.0 / n) - 1)) {
		cout << endl << "Given problem is not schedulable under said scheduling algorithm." << endl;
		exit(0);
	}

	int p_list[time] = { 0 }, min = 999, next_proc = 0;
	for (int i = 0; i < time; i++) {
		min = 1000;
		for (int j = 0; j < num_of_proc; j++) {
			if (remain_time[j] > 0) {
				if (min > time_period[j]) {
					min = time_period[j];
					next_proc = j;
				}
			}
		}

		if (remain_time[next_proc] > 0) {
			p_list[i] = next_proc + 1; 	// +1 for catering 0 array index.
			remain_time[next_proc] -= 1;
		}

		for (int k = 0; k < num_of_proc; k++) {
			if ((i + 1) % time_period[k] == 0) {
				remain_time[k] = exec_time[k];
				next_proc = k;
			}
		}
	}
	printing_schedule(p_list, time);
}

void EDF(int time) {    //EARLIEST DEADLINE FIRST
	float utilize = 0;
	for (int i = 0; i < num_of_proc; i++) {
		utilize += (1.0 * exec_time[i]) / deadline[i];
	}
	int n = num_of_proc;
	if (utilize > 1) {
		cout << endl << "Given problem is not schedulable under said scheduling algorithm." << endl;
		exit(0);
	}

	int process[num_of_proc];
	int max_deadline, curr_proc = 0, min_deadline, process_list[time];;
	bool is_ready[num_of_proc];

	for (int i = 0; i < num_of_proc; i++) {
		is_ready[i] = true;
		process[i] = i + 1;
	}

	max_deadline = deadline[0];
	for (int i = 1; i < num_of_proc; i++) {
		if (deadline[i] > max_deadline)
			max_deadline = deadline[i];
	}

	for (int i = 0; i < num_of_proc; i++) {
		for (int j = i + 1; j < num_of_proc; j++) {
			if (deadline[j] < deadline[i]) {
				int temp = exec_time[j];
				exec_time[j] = exec_time[i];
				exec_time[i] = temp;
				temp = deadline[j];
				deadline[j] = deadline[i];
				deadline[i] = temp;
				temp = process[j];
				process[j] = process[i];
				process[i] = temp;
			}
		}
	}

	for (int i = 0; i < num_of_proc; i++) {
		remain_time[i] = exec_time[i];
		rem_deadline[i] = deadline[i];
	}

	for (int t = 0; t < time; t++) {
		if (curr_proc != -1) {
			--exec_time[curr_proc];
			process_list[t] = process[curr_proc];
		}
		else
			process_list[t] = 0;

		for (int i = 0; i < num_of_proc; i++) {
			--deadline[i];
			if ((exec_time[i] == 0) && is_ready[i]) {
				deadline[i] += rem_deadline[i];
				is_ready[i] = false;
			}
			if ((deadline[i] <= rem_deadline[i]) && (is_ready[i] == false)) {
				exec_time[i] = remain_time[i];
				is_ready[i] = true;
			}
		}

		min_deadline = max_deadline;
		curr_proc = -1;
		for (int i = 0; i < num_of_proc; i++) {
			if ((deadline[i] <= min_deadline) && (exec_time[i] > 0)) {
				curr_proc = i;
				min_deadline = deadline[i];
			}
		}
	}
	printing_schedule(process_list, time);
}

int main(int argc, char* argv[]) {
	int opt = 0;
	cout << "-----------------------------" << endl;
	cout << "CPU Scheduling Algorithms: " << endl;
	cout << "-----------------------------" << endl;
	cout << "1. Rate Monotonic Scheduling" << endl;
	cout << "2. Earliest Deadline First" << endl;
	cout << "-----------------------------" << endl;
	cout << "Select > "; cin >> opt;
	cout << "-----------------------------" << endl;

	get_process_info(opt);		//collecting processes detail
	int obs_time = get_obs_time(opt);

	//	if (option == 1)
	//	 	first_come_first_serve(observation_time);
	//	else if (option == 2)
	//		round_robin(observation_time);
	if (opt == 1)
		RM(obs_time);
	else if (opt == 2)
		EDF(obs_time);
	return 0;
}