#include "pch.h"
#include "PCB.h"
#include "CPU_Scheduler.h"
#include <iostream>

int main(){
	Scheduler sch;
	//interface
	int option;
	int id = 1;
	std::cout << "ZARZADZANIE PROCESOREM METODA PRIORYTETOWA:" << std::endl << std::endl;
	//tworzenie procesu dummy
	//PCB* dummy = new PCB("dummy", 0, 0);
	//sch.add(dummy);
	do {
		std::cout << "0 - zakoncz prace modulu\n1 - dodaj nowy proces do modulu\n2 - usun proces z kolejki\n3 - wyswietl kolejke procesow\n"
			"4 - wykonaj kolejny rozkaz\n5 - usunac aktualnie wykonywany proces\n";
		std::cin >> option;
		switch (option) {
		case 1: {
			std::string name;
			std::cout << "Podaj dane procesu:\nnazwa\n";
			std::cin >> name;
			int prio;
			std::cout << "priorytet\n";
			std::cin >> prio;
			while ((prio > 15) || (prio < 1)) {
				std::cout << "podaj wartosc z zakresu <1, 15>" << std::endl;
				std::cin >> prio;
			}
			PCB* added = new PCB(name, prio, id);
			id++;
			sch.exe.push_back(0);
			sch.add(added);
			break;
		}
		case 2: {
			std::string name;
			std::cout << "Podaj nazwe procesu do usuniecia" << std::endl;
			std::cin >> name;
			PCB* removed = sch.findproc(name);
			if (removed!=nullptr) sch.remove(removed);
			break;
		}
		case 3: {
			sch.displayQueue();
			break;
		}
		case 4: {
			sch.execute();
			break;
		}
		case 5: {
			sch.remove_running();
			break;
		}
		}
	} while (option!=0);
	return 0;
}