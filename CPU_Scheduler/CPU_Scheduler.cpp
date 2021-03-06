#pragma once
#include "pch.h"
#include "PCB.h"
#include "CPU_Scheduler.h"
#include <array>
#include <vector>
#include <queue>
#include <iostream>

//czyszczenie po agining()
void Scheduler::cleaning() {
	//czyszczenie starego uporzadkowania kolejek
	for (int i = 1; i < 16; i++) queue_of_priority[i].clear();
	//dodawanie procesow od nowa
	for (PCB* pcb : temp_vec) add(pcb);
	//czyszczenie kontenera zapasowego
	temp_vec.clear();
}

//wywlaszczanie
void Scheduler::preemptive() {
	//zmiana stanu wywlaszczanego procesu z wykonywany na gotowy
	RUNNING->setState(State::Ready); 
	//dodanie tego procesu z powrotem do kolejki
	add(RUNNING);
	std::cout << "W kolejce pojawil sie proces wazniejszy od obecnie wykonywanego, nastapilo wywlaszczenie" << std::endl << std::endl;
	//wybor kolejnego
	select();
}
//dodawanie procesu do kolejki
void Scheduler::add(PCB* added) {
	//dla procesu dummy
	if (added->getStaticPrio() == 0) {
		//sprawdzenie poprawnosci stanu
		if (added->getState() != State::Ready) {
			std::cout << "Nie mozna dodac tego procesu do kolejki - proces nie jest w stanie gotowy/n";
		}
		else {
			//dodanie procesu na koniec danej kolejki
			queue_of_priority[added->getStaticPrio()].push_back(added);
			std::cout << "Do kolejki dodano proces: ";
			added->displayProcess();
			DUMMY = added;
			select();
		}
	}
	//dla reszty procesow
	else {
		//sprawdzenie poprawnosci stanu
		if (added->getState() != Ready) {
			std::cout << "Nie mozna dodac tego procesu do kolejki - proces nie jest w stanie gotowy/n";
		}
		else {
			//dodanie procesu na koniec danej kolejki
			queue_of_priority[added->getDynamicPrio()].push_back(added);
			std::cout << "Do kolejki dodano proces: ";
			added->displayProcess();
			//sprawdzanie czy obecnie jakis proces jest wykonywany
			if (RUNNING != nullptr) {
				//jezeli tak to sprawdzenie czy nalezy wywlaszczyc
				if (added->getDynamicPrio() > RUNNING->getDynamicPrio()) {
					preemptive();
				}
			}
			//jezeli nie to wybrany zostanie jedyny proces w kolejce
			else select();
		}
	}
}
//usuwanie z kolejki
void Scheduler::remove(PCB* removed) {
	//przeszukiwanie kolejki
	std::vector<PCB*>::iterator it = queue_of_priority[removed->getDynamicPrio()].begin();
	int temp = 0;
	bool found = false;
	while (it != queue_of_priority[removed->getDynamicPrio()].end()) {
		if (*it == removed) {
			queue_of_priority[removed->getDynamicPrio()].erase(it);
			found = true;
			break;
		}
		else {
			it++;
			temp++;
		}
	}
	//gdy sie powiodlo wyswietlanie danych
	if (found) {
		std::cout << "Usunieto proces z kolejki: ";
		removed->displayProcess();
	}
}
//usuwanie procesu RUNNING
void Scheduler::remove_running() {
	RUNNING = nullptr;
	select();
}
//wybor nastepnego procesu do wykonania
void Scheduler::select() {
	//szukanie pierwszej niepustej kolejki
	for (int i = 15; i >= 0; i--) {
		if (!queue_of_priority[i].empty()) {
			//przypisanie pierwszego procesu z kolejki zgodnie z zasada FCFS
			RUNNING = queue_of_priority[i][0];
			//odznaczenie w tablicy pomocniczej ze proces byl wykonywany
			exe[RUNNING->getPID()-1] = 1;
			RUNNING->setState(State::Running);
			std::cout << "Procesor przydzielono: ";
			RUNNING->displayProcess();
			remove(RUNNING);
			break;
		}
	}
}
//postarzanie
void Scheduler::aging() {
	std::cout << "Wykonano trzy rozkazy wiec procesy czekajace w kolejce zostaly postarzone/odmlodzone" << std::endl;
	std::vector<PCB*>::iterator it;
	//przegladanie wszystkich kolejek
	for (int i = 1; i < 16; i++) {
		for (it = queue_of_priority[i].begin(); it != queue_of_priority[i].end(); it++) {
			//zapis edytowanych PCB w kontenerze zapasowym
			temp_vec.push_back((*it));
			int temp = (*it)->getPID() - 1;
			//odmladzanie procesow wykonywanych
			if (exe[temp]) {
				//zmniejszenie priorytetu dynamicznego
				if (((*it)->getDynamicPrio()) > ((*it)->getStaticPrio())) {
					(*it)->setDynamicPrio((*it)->getDynamicPrio() - 1);
				}
				//proces odmlodzony, od teraz bedzie ponownie postarzany
				else exe[temp] = 0;
			}
			//postarzanie procesow niewykonywanych
			else {
				//sprawdzanie czy nie przekroczono priorytetu
				if (((*it)->getDynamicPrio()) < 15) {
					(*it)->setDynamicPrio((*it)->getDynamicPrio() + 1);
				}
			}
		}
	}
	cleaning();
}
//wykonywanie procesu
void Scheduler::execute() {
	//odliczanie do postarzania (co 3 rozkazy)
	counter++;
	if (counter == 3) {
		counter = 0;
		aging();
	}
	//przekazanie procesu RUNNING do interpretera
	//.......
}
//wyszukiwanie procesu po nazwie
PCB* Scheduler::findproc(std::string name) {
	//przegladanie kontenera
	for (int i = 0; i <= 15; i++) {
		std::queue<PCB*> temp;
		if (!queue_of_priority[i].empty()) {
			//przegladanie niepustych kolejek
			for (int j = 0; j < queue_of_priority[i].size(); j++) {
				if (queue_of_priority[i][j]->getName() == name) return queue_of_priority[i][j];
			}
		}
	}
	std::cout << "W kolejce nie ma podanego procesu" << std::endl << std::endl;
	return nullptr;
}
//wyswietlanie kolejki wraz z obecnie wykonywanym procesem
void Scheduler::displayQueue() {
	std::cout << "RUNNING:" << std::endl;
	if (RUNNING != nullptr) RUNNING->displayProcess();
	else std::cout << "Obecnie zaden proces nie jest wykonywany" << std::endl << std::endl;
	std::cout << "KOLEJKA PROCESOW DO WYKONANIA:" << std::endl << std::endl;
	//proces 0
	if (!queue_of_priority[0].empty())  queue_of_priority[0].front()->displayProcess();
	for (int i = 1; i < 16; i++) {
		//poszczegolne kolejki priorytetow 1 - 15
		if (!queue_of_priority[i].empty()) {
			for (int j = 0; j < queue_of_priority[i].size(); j++) Scheduler::queue_of_priority[i][j]->displayProcess();
	    }
	}
}