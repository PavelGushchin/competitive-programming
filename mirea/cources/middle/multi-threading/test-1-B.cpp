#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
using namespace std;
using namespace std::literals;

pair<long long, long long> numDisivors(long long x) {
	long long answer = 1;
    long long iterations = 0;
	for (long long p = 2; p * p <= x; p++)
	{
        iterations++;
		if (x % p == 0)
		{
			long long power = 1;
			x /= p;
			while (x % p == 0)
			{
                iterations++;
				power++;
				x /= p;
			}
			answer *= (power+1);
		}
	}
	if (x > 1)
		answer *= 2;
	return {answer, iterations};
}


int main()
{
    // создаём вектор input и заполняем его:
    const int nJobs = 100;
	vector<long long> input(nJobs);
	mt19937 gen;
	for (auto &number : input)
		number = uniform_int_distribution<long long>(1e17, 1e18)(gen);
	vector<long long> output(nJobs);
    // создаём потоки:
    const int nThreads = thread::hardware_concurrency(); // 16;
    cout << "nThreads = " << nThreads << endl;
    vector<thread> threads;
    mutex mtx;
    int nProcessed = 0;
    for (int id = 0; id < nThreads; id++)
    {
        threads.emplace_back(
            [threadId = id, &input, &output, &mtx, &nProcessed]()
            {
                {
                    lock_guard<mutex> lock(mtx);
                    cout << "Thread: " << threadId << " begin" << endl;
                }
                long long totalIters = 0;
                while (true)
                {
                    int i;
                    {
                        lock_guard<mutex> lock(mtx);
                        i = nProcessed++;
                    }
                    if (i >= (int)input.size())
                        break;
                    long long x = input[i];
                    auto [answ, iters] = numDisivors(x);
                    output[i] = answ;
                    totalIters += iters;
                }
                {
                    lock_guard<mutex> lock(mtx);
                    cout << "Thread: " << threadId << " end: " << totalIters << endl;
                }
            });
    }
    // ждать их выполнения:
    for (auto &t : threads)
        if (t.joinable())
            t.join();
    cout << "All threads have been finished!" << endl;
    return 0;
}
