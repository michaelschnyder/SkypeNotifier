#include "CountDownTimer.h"

void CountdownTimer::start(long timeOutInMilliseconds) {
    isStarted = true;
    startTime = millis();    
    timeoutInMs = timeOutInMilliseconds;
}

void CountdownTimer::restart() {
    isStarted = true;
    startTime = millis();    
}

void CountdownTimer::evaluate() {

    if (!isStarted) {
        return;
    }

    long currentMillis = millis();
    bool timeOut = false;
    if (currentMillis - startTime >= timeoutInMs) {
        timeOut = true;
    }

    if (timeOut) {
        isStarted = false;
        for (size_t i = 0; i < onCompletedHandlers.size(); i++)
        {
            onCompletedHandlers.get(i)();   
        }        
    }
}

void CountdownTimer::onCompleted(function<void()> handler) {
    onCompletedHandlers.add(handler);
}