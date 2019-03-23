#pragma once
#include "PCB.h"
#include <array>
#include <vector>
#include <queue>
//#include <memory>
#include <iostream>

class Scheduler {
private:
	int counter = 0;
	//czyszczenie po agining()
	void cleaning();
	//kontener do chwilowego trzymania PCB przy agining()
	std::vector<PCB*> temp_vec;
	//std::array<std::vector<int>, 15> executed;
	//tablica kolejek procesow gotowych do wykonania
	std::array<std::vector<PCB*>, 16> queue_of_priority;
	//wywlaszczanie
	void preemptive();
	//wybor nastepnego procesu do wykonania
	void select();
	//postarzanie
	void aging();

public:
	//tablica pomocnicza przy postarzaniu 0 - postarzanie, 1 - odmladzanie
	std::vector<int> exe;
	//proces dummy
	PCB* DUMMY = nullptr;
	//aktualnie wykonywany proces
	PCB* RUNNING = nullptr;
	//dodawanie procesu do kolejki wywolywane przez zarzadzanie procesami
	void add(PCB* added);
	//usuwanie z kolejki wywolywane przez zarzadzanie procesami
	void remove(PCB* removed);
	//usuwanie aktualnie wykonywanego procesu
	void remove_running();
	//wykonywanie procesu wywolywanie przez interpreter gdy kolejny rozkaz do wykonania pochodzi z tego samego procesu
	void execute();
	//wyszukiwanie procesu w kolejce po nazwie, uzywane przy usuwaniu wskazanego procesu
	PCB* findproc(std::string name);
	//wyswietla kolejke procesow
	void displayQueue();
};