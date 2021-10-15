#include <mutex>
#include <condition_variable>

#ifndef GATE_H
#define GATE_H

class Gate
{
private:
    int id;
    bool isFree = true;

public:
    std::condition_variable gateVar;
    std::mutex gateMutex;
    std::mutex isFreeMutex;
    int getId();
    Gate();
    void setId(int id);
    bool getFree();
    void setFree(bool free);
    std::string getPhilID();
};

#endif