#include "BootSequence.h"

void BootSequence::addStep(String name, function<void()> starter, function<bool()> completed) {

    Task *t = new Task(name.c_str(), starter, completed);

    tasks[headIdx] = t;
    taskQueue.enqueue(t);

    logger.verbose("Added task '%s' to queue", tasks[headIdx]->getName());
}

void BootSequence::addStep(String name, function<void()> starter) {
    BootSequence::addStep(name, starter, [](){ return true; });
}

void BootSequence::onBeforeTaskStart(function<void(String)> eventHandler) {

}
void BootSequence::onCompleted(function<void()> eventHandler) {

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
        
        currentTask->startFn();
        hasStarted = true;
        return;
    }

    if (currentTask->completedFn()) {
        logger.verbose("Task '%s' has completed.", currentTask->getName());
        hasTask = false;
        hasStarted = false;

        currentIdx++;
    }
}