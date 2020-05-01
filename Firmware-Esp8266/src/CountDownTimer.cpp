#include "CountDownTimer.h"

void CountdownTimer::start(long timeOutInMilliseconds) {
    
    startTime = millis();    
    timeoutInMs = timeOutInMilliseconds;
}

void CountdownTimer::restart() {
    startTime = millis();    
}

void CountdownTimer::evaluate() {

    long currentMillis = millis();
    bool timeOut = false;
    if (currentMillis - startTime >= timeoutInMs) {
        timeOut = true;
    }

    if (timeOut) {
        for (size_t i = 0; i < onCompletedHandlers.size(); i++)
        {
            onCompletedHandlers.get(i)();   
        }        
    }
}

void CountdownTimer::onCompleted(function<void()> handler) {
    onCompletedHandlers.add(handler);
}