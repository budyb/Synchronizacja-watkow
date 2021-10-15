#include <iostream>
#include <unistd.h>
#include <ncursesw/ncurses.h>
#include <string>
#include <ctime>
#include <chrono>
#include <cmath>
#include <thread>
#include <memory>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <random>

#include "fan.h"
#include "gate.h"
#include "checkout.h"

using namespace std;

atomic<bool> running(true);

void startFan(Fan *fan)
{
    fan->live();
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "pl_PL.UTF-8");
    int noFans = atoi(argv[1]);
    int sleepTime = atoi(argv[2]);
    int noGates;
    int noGuards;
    if (argc > 3)
        noGates = atoi(argv[3]);
    else
        noGates = round(noFans / 4);

    if (noGates > 1)
        noGuards = noGates / 2;
    else
        noGuards = 1;

    srand(time(0));
    sleepTime *= 1000;
    chrono::milliseconds stime(sleepTime);
    stime = stime + (chrono::milliseconds)(rand() % 41 - 20) * sleepTime / 100;

    shared_ptr<Gate[]> gateArray(new Gate[noGates]);
    shared_ptr<Guard[]> guardArray(new Guard[noGuards]);
    shared_ptr<mutex[]> mutexArray(new mutex[noFans]);
    vector<Fan> fanArray;
    vector<thread> threadArray;
    Checkout check;
    check.setId(1);

    for (int i = 0; i < noGates; i++)
    {
        gateArray[i].setId(i);
    }
    for (int i = 0; i < noGuards; i++)
    {
        guardArray[i].setId(i);
    }
    bool ticket;
    srand(time(NULL));
    for (int i = 0; i < noFans; i++)
    {
        ticket = rand() % 2;
        fanArray.emplace_back(Fan(i, &running, &mutexArray[i], &gateArray[i % noGates], &guardArray[i % noGuards], &check, ticket, stime));
    }
    for (int i = 0; i < noFans; i++)
    {
        threadArray.emplace_back(thread(&startFan, &fanArray[i]));
    }

    initscr();
    nodelay(stdscr, TRUE);
    int width, height = 0;
    string tekst;
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    attron(A_STANDOUT);
    while (getch() != 'q')
    {
        getmaxyx(stdscr, height, width);
        clear();
        for (int i = 0; i < noFans; i++)
        {
            tekst = "Fan: " + to_string(fanArray[i].getId()) + " przeszedł przez bramkę " + to_string(fanArray[i].getCounter()) + " krotnie \n";
            attron(COLOR_PAIR(2));
            if (i % 2 == 0)
                attron(COLOR_PAIR(1));
            printw(" [%s] ", fanArray[i].getState().data());
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(3));
            if (i % 2 == 0)
                attron(COLOR_PAIR(4));
            if (width >= 100)
                mvprintw(i, width - tekst.size(), tekst.data());
            else
                printw(tekst.data());
            attroff(COLOR_PAIR(3));
        }
        refresh();
        usleep(100000);
    }
    attroff(A_STANDOUT);

    running.store(false);
    attron(A_BOLD);
    mvprintw(noFans + 2, width / 2 - 12, "Kończenie pracy programu\n");
    attroff(A_BOLD);
    refresh();
    for (int i = 0; i < noFans; i++)
    {
        threadArray[i].join();
    }

    clear();
    for (int i = 0; i < noFans; i++)
    {
        if (i % 2 == 0)
            attron(A_STANDOUT);
        tekst = "Fan: " + to_string(fanArray[i].getId()) + " przeszedł przez bramkę: " + to_string(fanArray[i].getCounter()) + " krotnie\n";
        mvprintw(i, width / 2 - 16, tekst.data());
        attroff(A_STANDOUT);
    }
    refresh();
    usleep(10000000);
    endwin();
    return 0;
}