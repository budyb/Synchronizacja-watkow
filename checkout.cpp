#include "checkout.h"

using namespace std;

int Checkout::getId()
{
    return this->id;
}
void Checkout::setId(int checkoutId)
{
    this->id = checkoutId;
}

void Checkout::setFree(bool free)
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    this->isFree = free;
}

bool Checkout::getFree()
{
    unique_lock<mutex> isFreeLock(isFreeMutex);
    return isFree;
}
Checkout::Checkout()
{
    checkoutVar.notify_all();
}