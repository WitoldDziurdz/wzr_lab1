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

unicast_net *uni_receive;
unicast_net *uni_send;

std::vector<unsigned long> klienci;
Ramka ramka;
StanObiektu stan;

int main()
{
	uni_receive = new unicast_net(10001);
	uni_send = new unicast_net(10002);

	while (1) {
		int rozmiar;
		unsigned long adres;
		rozmiar = uni_receive->reciv((char*)&ramka, &adres, sizeof(Ramka));
		switch (ramka.typ) {
		case typy_ramek::STAN_OBIEKTU:
			printf("stan");

		}
		
		bool found = false;
		for (int i = 0; i < klienci.size(); i++) {
			if (klienci[i] == adres) {
				found = true;
				break;
			}
		}

		if (!found) {
			klienci.push_back(adres);
			printf("Connected %lu", adres);
		}

		for (int i = 0; i < klienci.size(); i++) {
			printf("To %lu (%d)\n", klienci[i], klienci.size());
			uni_send->send((char*)&ramka, klienci[i], sizeof(Ramka));
		}
	}

	return 0;
}


