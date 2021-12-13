#include <iostream>
#include <thread>
#include <chrono> 
#include <string>
#include <vector>
#include <random>

using namespace std;

#define TIMESLICE 3
#define PROCESSES 5

struct  Process 
{
	Process(int _duration, int _id)
	{
		id = _id; 
		duration = _duration; 
	}

	int id; 
	int duration;
	int waiting_time = 0; 
};

vector<Process> waiting;

int main() 
{

	// in this example i assume that all jobs arrive at the ready q at the same time
	// the processes are single-threaded
	// no I/O
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10);
	
	for (int i = 0; i < PROCESSES; i++) 
	{
		waiting.push_back(Process(dist6(rng), i));
	}
	

	int waiting_time[PROCESSES]; 
	memset(waiting_time, 0, sizeof(int) * PROCESSES); 

	while (!waiting.empty())
	{
		Process p = waiting.front();
		waiting.erase(waiting.begin());

		auto start = chrono::steady_clock::now();
		std::this_thread::sleep_for(std::chrono::seconds(min(TIMESLICE, p.duration)));
		auto end = chrono::steady_clock::now();
		
		auto elapsed_time = chrono::duration_cast<chrono::seconds>(end - start).count();

		cout << "Elapsed time in seconds: " << elapsed_time	<< " sec\n";

		p.duration -= min(TIMESLICE, p.duration);

		for (auto& proc : waiting) 
		{
			proc.waiting_time += elapsed_time; 
		}

		if (p.duration == 0) 
		{
			waiting_time[p.id] = p.waiting_time;
			continue; 
		}

		waiting.push_back(p);
	}

	float average_waiting_time = 0.0f; 
	for (int i = 0; i < PROCESSES; ++i) 
	{
		cout << "waiting time for process " << i << ": " << waiting_time[i] << '\n';
		average_waiting_time += waiting_time[i]; 
	}

	cout << "average waiting time is " << average_waiting_time / float(PROCESSES);
}