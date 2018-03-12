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


bool isExist(unsigned long adres) {
	for (int i = 0; i < klienci.size(); i++) {
		if (klienci[i]->addr == adres && klienci[i]->id == ramka.stan.iID) {
			return true; 
		}
	}
	return false; 
}

int getIndex(unsigned long adres) {
	for (int i = 0; i < klienci.size(); i++) {
		if (klienci[i]->addr == adres && klienci[i]->id == ramka.stan.iID) {
			return i;
		}
	}
	return -1;
}

void addUser(unsigned long adres) {
	User* user = new User();
	user->addr = adres;
	user->id = ramka.stan.iID;
	klienci.push_back(user);
	printf("Connected %lu", adres);
}

void sendAllUsers() {
	for (int i = 0; i < klienci.size(); i++) {
		printf("To %lu (%d)\n", klienci[i], klienci.size());
		uni_send->send((char*)&ramka, klienci[i]->addr, sizeof(Ramka));
	}
}

void removeUser(unsigned long adres) {
	int index = getIndex(adres); 
	if (index >= 0) {
		klienci.erase(klienci.begin()+index);
	}
}


int main()
{
	uni_reciv = new unicast_net(10002);
	uni_send = new unicast_net(10001);

	while (1) {
		int rozmiar;
		unsigned long adres;
		rozmiar = uni_reciv->reciv((char*)&ramka, &adres, sizeof(Ramka));
		switch (ramka.typ) {
		case typy_ramek::NEW_PLAYER:
			if (!isExist(adres)) {
				addUser(adres); 
			}
			printf("ramka NEW_PLAYER ");
			break;
		case typy_ramek::STAN_OBIEKTU:
			printf("ramka STAN_OBIEKTU ");
			if (isExist(!adres)) {
				addUser(adres);
			}
			sendAllUsers(); 
			break; 
		case typy_ramek::CLOSE_WINDOW:
			removeUser(adres); 
			printf("ramka CLOSE_WINDOW ");
			break;
		}
	}

	return 0;
}


