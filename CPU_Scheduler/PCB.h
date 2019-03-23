#ifndef PROCESSMANAGER_PCB_H
#define PROCESSMANAGER_PCB_H

#include <string>
#include <list>
#include <iostream>

enum State { New, Ready, Running, Blocked, Terminated };

struct Registers{
    int AX;
    int BX;
    int CX;
    int DX;
};

class PCB {

private:
    int PID;
    std::string processName;
    State state;
    std::list<PCB*>list;
    int PC;
    int priorityStatic;
	int priorityDynamic;
    //TODO rejestry
    Registers registers;

public:
    PCB(std::string processName, int prio, int PID);
    int getPID();
    State getState();
    void displayProcess();
    void setState(State state);
	int getStaticPrio();
	int getDynamicPrio();
	void setDynamicPrio(int prio);
	std::string getName();
};


#endif //PROCESSMANAGER_PCB_H
