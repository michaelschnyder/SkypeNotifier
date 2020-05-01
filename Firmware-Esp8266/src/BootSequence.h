#ifndef BootSequence_h
#define BootSequence_h

// #include <string>
#include <functional>
#include <String>
#include <Arduino.h>
#include "libs/QueueArray/QueueArray.h"
#include "libs/LinkedList2/LinkedList2.h"
#include <Log4Esp.h>

using namespace std;

// typedef function<void(String name)> BootStepHandlerFunction;
// typedef function<bool(String name)> BootStepStatusReturnFunction;

// typedef std::function<void(String *request)> ArRequestHandlerFunction2;;

// #define ArRequestHandlerFunction2 std::function<void(String)>

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

class BootSequence {

public:
    // void addStep(String, BootStepHandlerFunction, BootStepStatusReturnFunction);
    // void addStep(String, function<void(String)> onRequest);
    void addStep(String, function<void()> start, function<bool()> completed);
    void addStep(String, function<void()> start);

    void work();

    void onBeforeTaskStart(function<void(String)> eventHandler);
    void onCompleted(function<void()> eventHandler);

private:
    log4Esp::Logger logger = log4Esp::Logger("BootSequence");
    
    QueueArray <Task *> taskQueue;
    Task *currentTask;

    bool hasTask;
    bool hasStarted;

    LinkedList2<function<void(String)>> onBeforeTaskStartHandlers;
    LinkedList2<function<void()>> onCompletedHandlers;
    
};
#endif