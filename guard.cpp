#include "guard.h"

using namespace std;

int Guard::getId()
{
    return this->id;
}
void Guard::setId(int guardId)
{
    this->id = guardId;
}

void Guard::setFree(bool free)
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    this->isFree = free;
}

bool Guard::getFree()
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    return isFree;
}
Guard::Guard()
{
    guardVar.notify_all();
}