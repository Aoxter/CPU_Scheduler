#pragma once
#include "pch.h"
#include "PCB.h"
#include <random>

int RNG() {
	std::default_random_engine gen;
	std::uniform_int_distribution<int> dist(0, 255);
	return dist(gen);
}

PCB::PCB(std::string processName, int prio, int PID): PID(PID), processName(processName), priorityStatic(prio), priorityDynamic(prio), state(State::Ready) {
	registers.AX = RNG();
	registers.BX = RNG();
	registers.CX = RNG();
	registers.DX = RNG();
}

State PCB::getState() {
	return state;
}

void PCB::displayProcess() {
	std::cout << "Process: " << processName << "\nPID: " << PID << "\nstate: ";
	switch (state) {
	case 0: std::cout << "New\n"; break;
	case 1: std::cout << "Ready\n"; break;
	case 2: std::cout << "Running\n"; break;
	case 3: std::cout << "Blocked\n"; break;
	case 4: std::cout << "Terminated\n"; break;
	}
	std::cout<<"priority static: " << priorityStatic <<	"\npriority dynamic: " << priorityDynamic << "\nregisters: AX = " << registers.AX <<
	"; BX = " << registers.BX << "; CX = " << registers.CX << "; DX = " <<	registers.DX << std::endl << std::endl;
}

void PCB::setState(State state) {
	this->state = state;
}

int PCB::getPID() {
	return PID;
}

int PCB::getStaticPrio() {
	return priorityStatic;
}

int PCB::getDynamicPrio() {
	return priorityDynamic;
}

void PCB::setDynamicPrio(int prio) {
	priorityDynamic = prio;
}

std::string PCB::getName() {
	return processName;
}