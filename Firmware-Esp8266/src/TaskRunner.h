#ifndef TaskRunner_h
#define TaskRunner_h

#include <functional>
#include <String>
#include <Arduino.h>
#include "libs/QueueArray/QueueArray.h"
#include "libs/LinkedList2/LinkedList2.h"
#include <Log4Esp.h>

using namespace std;

class Task {

public:
    function<void()> startFn;
    function<bool()> completedFn;

    Task(const char * name, function<void()> start, function<bool()> completed) {
        
        strcpy(_name, name);
        startFn = start;
        completedFn = completed;
    }

    char* getName() {
        return _name;
    }

private:
    char _name[64];
};

class TaskRunner {

public:
    void addTask(String, function<void()> start, function<bool()> completed);
    void addTask(String, function<void()> start);

    void setTaskTimeoutInMs(long);
    void setTaskRetryCount(long);

    void work();
    void cancel();

    void onBeforeTaskStart(function<void(String)> eventHandler);
    void onCompleted(function<void()> eventHandler);
    void onTaskExpired(function<void(String)> eventHandler);

private:
    log4Esp::Logger logger = log4Esp::Logger("TaskRunner");
    
    QueueArray <Task *> taskQueue;
    Task *currentTask;

    bool hasTask;
    bool hasStarted;
    long currentTaskStartTime;
    long currentTaskRetryCount;

    long defaultTimeout;
    long defaultRetryCount;

    LinkedList2<function<void(String)>> onBeforeTaskStartHandlers;
    LinkedList2<function<void()>> onCompletedHandlers;
    LinkedList2<function<void(String)>> onTaskExpiredHandlers;
    
};
#endif