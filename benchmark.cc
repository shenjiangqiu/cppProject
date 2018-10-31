#include <thread>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <map>
#include <random>
#include <vector>
#include <string>
using namespace std;
map<thread::id, int> idmap;
int get_time_now() { return chrono::system_clock::now().time_since_epoch().count(); }

void run()
{
    vector<int> m_array;
    cout << "thread: " << idmap[this_thread::get_id()] << "is running" << endl;
    default_random_engine eg(get_time_now());
    uniform_int_distribution<int> dist(10, 1000000);
    cout << "start to generate number" << endl;
    for (int i = 0; i < 100000000; i++)
    {
        m_array.push_back(dist(eg));
    }

    cout << "end generate number" << endl;
    sort(m_array.begin(), m_array.end());
    cout << "thread: " << idmap[this_thread::get_id()] << " Done!" << endl;
    int last = -1;
    bool correctness = true;
    for (auto i : m_array)
    {
        if (i < last)
        {
            correctness = false;
        }
        last = i;
    }
    if (correctness)
    {
        cout << "correct!" << endl;
    }
    else
    {
        cout << "wrong!" << endl;
    }
}
#define tasks 12;
int main(int argc, char **argv)
{
    int numthreads = stoi(string(argv[1]));
    vector<thread> thread_pool;
    int remains = tasks;

    auto start_time = chrono::steady_clock::now();

    while (remains > 0)
    {
        if (remains<numthreads) {
            numthreads=remains;
        }
        remains-=numthreads;
        for (int i = 0; i < numthreads; i++)
        {
            thread_pool.emplace_back(run);
        }

        for (int i = 0; i < numthreads; i++)
        {
            thread_pool[i].join();
        }
        for(int i=0;i<numthreads;i++){
            thread_pool.pop_back();
        }
    }
    auto end_time = chrono::steady_clock::now();

    auto duration = end_time - start_time;
    cout << chrono::duration_cast<chrono::milliseconds>(duration).count() << endl;

    return 0;
}
