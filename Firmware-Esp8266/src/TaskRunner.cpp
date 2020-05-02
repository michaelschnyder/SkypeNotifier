#include "TaskRunner.h"

void TaskRunner::addTask(String name, function<void()> starter, function<bool()> completed) {
    Task *t = new Task(name.c_str(), starter, completed);
    taskQueue.enqueue(t);
    logger.verbose("Added task '%s' to queue", name.c_str());
}

void TaskRunner::addTask(String name, function<void()> starter) {
    addTask(name, starter, [](){ return true; });
}

void TaskRunner::setTaskTimeoutInMs(long timeoutInMs) {
    defaultTimeout = timeoutInMs;
}
void TaskRunner::setTaskRetryCount(long retryCount) {
    defaultRetryCount = retryCount;
}

void TaskRunner::onBeforeTaskStart(function<void(String)> eventHandler) {
    onBeforeTaskStartHandlers.add(eventHandler);
}
void TaskRunner::onCompleted(function<void()> eventHandler) {
    onCompletedHandlers.add(eventHandler);
}

void TaskRunner::onTaskExpired(function<void(String)> eventHandler) {
    onTaskExpiredHandlers.add(eventHandler);
}

void TaskRunner::cancel() {
    while(!taskQueue.isEmpty()) {
        taskQueue.dequeue();
    }
}

void TaskRunner::work() {
    
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
        
        for (size_t i = 0; i < onBeforeTaskStartHandlers.size(); i++)
        {
            TaskRunner::onBeforeTaskStartHandlers.get(i)(String(currentTask->getName()));
        }

        currentTask->startFn();
        hasStarted = true;
        currentTaskStartTime = millis();
        currentTaskRetryCount = 0;
        return;
    }
    
    long currentMillis = millis();
    long startTimeIncludingRetry = currentTaskStartTime + (currentTaskRetryCount * defaultTimeout);
    
    if (currentMillis - startTimeIncludingRetry >= defaultTimeout) { 
        logger.warning("Task '%s' has not completed after %ds.", currentTask->getName(), defaultTimeout / 1000);

        currentTaskRetryCount++;

        if (currentTaskRetryCount >= defaultRetryCount) {
            logger.warning("Task '%s' has expired.", currentTask->getName());
            
            for (size_t i = 0; i < onBeforeTaskStartHandlers.size(); i++)
            {
                TaskRunner::onTaskExpiredHandlers.get(i)(String(currentTask->getName()));
            }

            hasTask = false;
            hasStarted = false;
            return;
        }
    }

    if (currentTask->completedFn()) {
        logger.verbose("Task '%s' has completed.", currentTask->getName());
        hasTask = false;
        hasStarted = false;
    }

    if (taskQueue.isEmpty()) {
        for (size_t i = 0; i < TaskRunner::onCompletedHandlers.size(); i++)
        {
            TaskRunner::onCompletedHandlers.get(i)();
        }
    }
}