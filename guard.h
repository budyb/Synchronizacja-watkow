#include <mutex>
#include <condition_variable>

#ifndef GUARD_H
#define GUARD_H

class Guard
{
private:
    int id;
    bool isFree = true;

public:
    std::condition_variable guardVar;
    std::mutex guardMutex;
    std::mutex isFreeMutex;
    int getId();
    Guard();
    void setId(int id);
    bool getFree();
    void setFree(bool free);
};

#endif