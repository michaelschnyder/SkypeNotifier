#ifndef H_CountDownTimer
#define H_CountDownTimer

#include <functional>
#include <Arduino.h>
#include "libs/LinkedList2/LinkedList2.h"

using namespace std;

class CountdownTimer {

public:
    void start(long timeoutInMs);
    void restart();
    void evaluate();

    void onCompleted(function<void()> handler);
private:
    long timeoutInMs;
    long startTime;
    LinkedList2<function<void()>> onCompletedHandlers;

};

#endif // H_CountDownTimer
