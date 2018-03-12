// WZR_serwer.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include "siec.h"
#include "kwaternion.h"
#include "obiekty.h"



enum typy_ramek { STAN_OBIEKTU, NEW_PLAYER, CLOSE_WINDOW };


struct Ramka                                    // g³ówna struktura s³u¿¹ca do przesy³ania informacji
{
	int typ;
	StanObiektu stan;
	float wartosc_oferty;
};

struct User {
	unsigned long addr; 
	int id; 
};

unicast_net *uni_reciv;
unicast_net *uni_send;

std::vector<User*> klienci;
Ramka ramka;
StanObiektu stan;

int main()
{
	uni_reciv = new unicast_net(10002);
	uni_send = new unicast_net(10001);

	while (1) {
		int rozmiar;
		unsigned long adres;
		rozmiar = uni_reciv->reciv((char*)&ramka, &adres, sizeof(Ramka));
		switch (ramka.typ) {
		case typy_ramek::STAN_OBIEKTU:
			printf("ramka STAN_OBIEKTU");
			break; 
		case typy_ramek::NEW_PLAYER: 
			printf("ramka NEW_PLAYER");
			break; 
		case typy_ramek::CLOSE_WINDOW:
			printf("ramka CLOSE_WINDOW");
			break;
		}

		
		
		bool found = false;
		for (int i = 0; i < klienci.size(); i++) {
			if (klienci[i]->addr == adres && klienci[i]->id == ramka.stan.iID) {
				found = true;
				break;
			}
		}

		

		if (!found) {
			User* user = new User();
			user->addr = adres;
			user->id = ramka.stan.iID;
			klienci.push_back(user);
			printf("Connected %lu", adres);
		}

		for (int i = 0; i < klienci.size(); i++) {
			printf("To %lu (%d)\n", klienci[i], klienci.size());
			uni_send->send((char*)&ramka, klienci[i]->addr, sizeof(Ramka));
		}
	}

	return 0;
}


