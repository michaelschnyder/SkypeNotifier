#include "BootSequence.h"

void BootSequence::addStep(String name, function<void()> starter, function<bool()> completed) {

    Task *t = new Task(name.c_str(), starter, completed);

    taskQueue.enqueue(t);

    logger.verbose("Added task '%s' to queue", name.c_str());
}

void BootSequence::addStep(String name, function<void()> starter) {
    BootSequence::addStep(name, starter, [](){ return true; });
}

void BootSequence::onBeforeTaskStart(function<void(String)> eventHandler) {
    BootSequence::onBeforeTaskStartHandlers.add(eventHandler);
}
void BootSequence::onCompleted(function<void()> eventHandler) {
    BootSequence::onCompletedHandlers.add(eventHandler);
}

void BootSequence::work() {
    
    if (!hasTask) {
        if (taskQueue.isEmpty()) {
            return;
        }

        logger.verbose("Getting new task from queue...");
        currentTask = taskQueue.dequeue();

        logger.verbose("Task '%s' is ready to run.", currentTask->getName());

        hasTask = true;
        hasStarted = false;

        return;
    }

    if (!hasStarted) {
        logger.verbose("Task '%s' is starting.", currentTask->getName());
        
        for (size_t i = 0; i < BootSequence::onBeforeTaskStartHandlers.size(); i++)
        {
            BootSequence::onBeforeTaskStartHandlers.get(i)(String(currentTask->getName()));
        }

        currentTask->startFn();
        hasStarted = true;
        return;
    }

    if (currentTask->completedFn()) {
        logger.verbose("Task '%s' has completed.", currentTask->getName());
        hasTask = false;
        hasStarted = false;
    }

    if (taskQueue.isEmpty()) {
        for (size_t i = 0; i < BootSequence::onCompletedHandlers.size(); i++)
        {
            BootSequence::onCompletedHandlers.get(i)();
        }
    }
}