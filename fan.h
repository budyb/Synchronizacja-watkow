#include "gate.h"
#include "guard.h"
#include "checkout.h"
#include <atomic>
#include <mutex>
#ifndef FAN_H
#define FAN_H

class Fan
{
private:
    std::atomic<bool> *running;
    bool ticket;
    int counter;
    int id;
    Gate *gate;
    Guard *guard;
    Checkout *checkout;
    std::chrono::milliseconds timesa;
    std::chrono::milliseconds cheerTime;
    std::chrono::milliseconds buyTime;
    std::chrono::milliseconds securityTime;
    std::chrono::milliseconds goingTime;
    std::string state;
    std::mutex *stateMutex;
    void buyTicket();
    void cheer();
    void goThrough(bool in);
    void takeSecurity();
    void goOut();

public:
    Fan(int x, std::atomic<bool> *run, std::mutex *statemutex, Gate *gt, Guard *gr, Checkout *check, bool ticket, std::chrono::milliseconds chtime);
    void live();
    int getCounter();
    void incCounter();
    int getId();
    std::string getState();
    void setState(bool append, std::string state);
};

#endif