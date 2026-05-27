#include "ThreadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(int numThreads) {
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back([this, i] {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] {
                        return !tasks.empty() || stopping;
                    });

                    if (stopping && tasks.empty()) {
                        return;
                    }

                    task = tasks.front();
                    tasks.pop();
                }

                std::cout << "Thread [" << i << "] processes request\n";
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stopping = true;
    }

    cv.notify_all();
    for (auto& w : workers) {
        w.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.push(task);
    }

    cv.notify_all();
}