#include "pch.h"
#include "utils.hpp"


class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    void AddTask(std::function<int(int, std::vector<uint64_t>)> task);
    int GetSum();

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<int(int, std::vector<uint64_t>)>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stop;
    int m_sum;
};

ThreadPool::ThreadPool(size_t num_threads)
{
    m_stop = false;
    m_sum = 0;
    for (size_t i = 0; i < num_threads; i++) {
        m_threads.push_back(std::thread([this]() {
            while (true) {
                std::function<int(int, std::vector<uint64_t>)> task;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_cv.wait(lock, [this]() { return m_stop || !m_tasks.empty(); });
                    if (m_stop && m_tasks.empty()) {
                        return;
                    }
                    task = m_tasks.front();
                    m_tasks.pop();
                }

                m_sum += task(0, std::vector<uint64_t>());
            }
        }));
    }
}

void ThreadPool::AddTask(std::function<int(int, std::vector<uint64_t>)> task)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.push(task);
    m_cv.notify_one();
}

bool SplitInHalf(uint64_t number, uint64_t& out1, uint64_t& out2) {
    std::string num_str = std::to_string(number);
    if (num_str.size() % 2 != 0) {
        return false;
    }

    out1 = std::stoull(num_str.substr(0, num_str.size() / 2));
    out2 = std::stoull(num_str.substr(num_str.size() / 2));
    return true;
}

#define NUM_THREADS 24
int Blink(std::vector<uint64_t> stones, int part = 1)
{
    const int iterations = part == 1 ? 25 : 75;
    
    int thread_count = 1;
    std::vector<std::thread> threads;


    std::function <int(int, std::vector<uint64_t>)> do_task = [&](int curr_i, std::vector<uint64_t> local_stones) -> int
    {
        for (int i = curr_i; i < iterations; i++) {
            for (int idx = 0; idx < local_stones.size(); idx++) {
                
                // increment by 1 if 0
                if (local_stones[idx] == 0) {
                    local_stones[idx]++;
                    continue;
                }

                // split into two numbers if even digits
                uint64_t left = 0;
                uint64_t right = 0;
                if (SplitInHalf(local_stones[idx], left, right)) {
                    local_stones[idx] = left;
                    local_stones.insert(local_stones.begin() + idx + 1, right);
                    idx++;
                } else {
                    // multiply by 2024 if odd digits
                    local_stones[idx] *= 2024;
                }
            }

            if (thread_count < NUM_THREADS) {
                for (int j = 0; j < NUM_THREADS; j++) {
                    threads.push_back(std::thread(do_task, i, local_stones));
                }

                for (int j = 0; j < NUM_THREADS; j++) {
                    threads[j].join();
                }
            }
        }

        return local_stones.size();
    }




    for (int i = 0; i < iterations; i++) {
        for (int idx = 0; idx < stones.size(); idx++) {
            
            // increment by 1 if 0
            if (stones[idx] == 0) {
                stones[idx]++;
                continue;
            }

            // split into two numbers if even digits
            uint64_t left = 0;
            uint64_t right = 0;
            if (SplitInHalf(stones[idx], left, right)) {
                stones[idx] = left;
                stones.insert(stones.begin() + idx + 1, right);
                idx++;
            } else {
                // multiply by 2024 if odd digits
                stones[idx] *= 2024;
            }
        }
    }

    return stones.size();
}

void DoDay11()
{
    std::cout << "Day 9: " << std::endl;
    std::ifstream file("inputs/day11/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::vector<uint64_t> stones;
    while (std::getline(file, line)) {
        SplitIntoLargeNumbers(line, stones);
    }

    std::cout << "    Part 1: " << Blink(stones, 1) << std::endl;
    std::cout << "    Part 2: " << Blink(stones, 2) << std::endl;
}


