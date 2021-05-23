#pragma once

#include "core/Service.hpp"

#include <chrono>
#include <queue>
#include <inttypes.h>

// when each task is triggered, this callback type will be called, passing the userdata pointer & the current UNIX EPOCH time
typedef void (*TaskCallback)(void* userData, uint64_t currTime, uint64_t realDelta);

class TaskService : public Service {
private:
    struct Task {
        TaskCallback callback;
        void* userData;
        uint64_t event; // time event is scheduled at
        uint64_t delta;
        bool timer;

        // this will automagically sort our task queue so the first member will be the next task in EPOCH time, makes our tick() well optimized :)
        bool operator<(const Task& other) const {
            return event > other.event;
        }
    };

    std::priority_queue<Task> tasks;

public:
    TaskService(ObjectPtr r);

    void pushTask(TaskCallback callBk, void* userData, uint64_t delta, bool timer);

    void tick();
};