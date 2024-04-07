//
// Created by Matias on 2.4.2024.
// Käytetään tehtävässä 7

#ifndef UNTITLED_TASKQUEUE_H
#define UNTITLED_TASKQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Game_Task.h"

class TaskQueue {
public:
    explicit TaskQueue(int nof_threads);
    ~TaskQueue();

    void addJob(std::shared_ptr<Game_Task> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::shared_ptr<Game_Task>> tasks;
    std::mutex mutex;
    std::condition_variable cond_var;
    bool stop;
};

#endif //UNTITLED_TASKQUEUE_H
