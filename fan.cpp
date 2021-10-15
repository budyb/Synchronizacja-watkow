#include "fan.h"
#include <iostream>
#include <thread>

using namespace std;

Fan::Fan(int x, atomic<bool> *run, std::mutex *statemutex, Gate *gt, Guard *guard, Checkout *checkout, bool ticket, chrono::milliseconds chtime)
{
    this->id = x;
    this->running = run;
    this->stateMutex = statemutex;
    this->cheerTime = chtime;
    this->buyTime = chtime;
    this->securityTime = chtime;
    this->goingTime = chtime;
    this->gate = gt;
    this->guard = guard;
    this->checkout = checkout;
    this->ticket = ticket;
    this->counter = 0;
}

void Fan::live()
{
    while (this->running->load())
    {
        if (!ticket)
            buyTicket();
        goThrough(true);
        cheer();
        goThrough(false);
        if (getCounter() % 10 == 0)
            ticket = false;
    }
}

void Fan::buyTicket()
{
    unique_lock<mutex> checkoutLock(checkout->checkoutMutex);
    checkout->checkoutVar.wait(checkoutLock, [&]()
                               { return checkout->getFree(); });
    checkout->setFree(false);
    setState(false, "Kupuje bilet");
    this_thread::sleep_for(this->buyTime);
    checkoutLock.unlock();
    checkout->setFree(true);
    setState(false, "Kupił bilet");
    checkout->checkoutVar.notify_all();
}

void Fan::cheer()
{
    setState(false, "Dopinguje");
    this_thread::sleep_for(this->cheerTime);
}

void Fan::goThrough(bool in)
{
    unique_lock<mutex> gateLock(gate->gateMutex);
    gate->gateVar.wait(gateLock, [&]()
                       { return gate->getFree(); });
    gate->setFree(false);
    setState(false, "Przechodzi przez bramkę: " + to_string(gate->getId()));
    this_thread::sleep_for(this->goingTime);
    if (in)
    {
        takeSecurity();
    }
    incCounter();
    setState(false, "Opuszcza bramkę: " + to_string(gate->getId()));
    gateLock.unlock();
    gate->setFree(true);
    setState(false, "Opuścił bramkę: " + to_string(gate->getId()));
    gate->gateVar.notify_all();
}

void Fan::takeSecurity()
{
    unique_lock<mutex> guardLock(guard->guardMutex);
    guard->guardVar.wait(guardLock, [&]()
                         { return guard->getFree(); });
    guard->setFree(false);
    setState(true, ", jest przeszukiwany " + to_string(guard->getId()));
    this_thread::sleep_for(this->securityTime);
    guardLock.unlock();
    guard->setFree(true);
    guard->guardVar.notify_all();
}

int Fan::getId()
{
    return this->id;
}

string Fan::getState()
{
    unique_lock<mutex> stateLock(*stateMutex);
    return this->state;
}

void Fan::setState(bool append, string state)
{
    unique_lock<mutex> stateLock(*stateMutex);
    if (append)
        this->state += state;
    else
        this->state = state;
}

int Fan::getCounter()
{
    unique_lock<mutex> stateLock(*stateMutex);
    return counter;
}

void Fan::incCounter()
{
    unique_lock<mutex> stateLock(*stateMutex);
    this->counter++;
}