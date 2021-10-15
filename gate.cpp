#include "gate.h"
#include <thread>

using namespace std;

int Gate::getId()
{
    return this->id;
}
void Gate::setId(int gateId)
{
    this->id = gateId;
}

void Gate::setFree(bool free)
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    this->isFree = free;
}

bool Gate::getFree()
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    return isFree;
}
Gate::Gate()
{
    gateVar.notify_all();
}