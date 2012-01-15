#ifndef GRAF_H
#define GRAF_H

#include <set>
#include "wierzcholek.h"
#include "porownajsciezki.h"

class Graf{
	private:
		int iloscWierzcholkow;
		Wierzcholek** wierzcholki;
		int* drzewoPoprzednikow;
		float* odleglosci;
		std::set<int, PorownajSciezki> kolejka;
		
	public:
		Graf();
		~Graf();
		void utworz(int);
		void ustawWierzcholek(int, const QPoint&);
		void dodajKrawedz(int, int);
		inline QPoint pozycjaWierzcholka(int) const;
		inline float odleglosc(int) const;
		int najblizszyWierzcholek(const QPoint&) const;
		void ustawKorzen(int);
		void czysc();
};

QPoint Graf::pozycjaWierzcholka(int v) const{
	return this->wierzcholki[v]->pozycja;
}

float Graf::odleglosc(int v) const{
	return this->odleglosci[v];
}

#endif // GRAF_H
