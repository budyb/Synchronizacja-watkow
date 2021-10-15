#include <mutex>
#include <condition_variable>

#ifndef CHECKOUT_H
#define CHECKOUT_H

class Checkout
{
private:
    int id;
    bool isFree = true;

public:
    std::condition_variable checkoutVar;
    std::mutex checkoutMutex;
    std::mutex isFreeMutex;
    int getId();
    Checkout();
    void setId(int id);
    bool getFree();
    void setFree(bool free);
};

#endif