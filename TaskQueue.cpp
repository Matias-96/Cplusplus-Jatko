//
// Created by Matias on 2.4.2024.
//

#include "TaskQueue.h"
#include <stdexcept>

// Konstruktori TaskQueue(int nof_threads)
TaskQueue::TaskQueue(int nof_threads) : stop(false) {
    for (int i = 0; i < nof_threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(this->mutex);
                this->cond_var.wait(lock, [this]() { return this->stop || !this->tasks.empty(); });
                if (this->stop && this->tasks.empty()) return;
                auto task = std::move(this->tasks.front());
                this->tasks.pop();
                lock.unlock(); // Vapautetaan ennen suorittamista
                task->perform();
            }
        });
    }
}

TaskQueue::~TaskQueue() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }
    cond_var.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// Jäsenfunktio void addJob(Game_Task task)
void TaskQueue::addJob(std::shared_ptr<Game_Task> task) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (stop) {
            throw std::runtime_error("Cannot add new jobs, queue is stopping.");
        }
        tasks.emplace(std::move(task));
    }
    cond_var.notify_one();
}
