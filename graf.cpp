#include <cmath>
#include <cfloat>
#include "graf.h"

Graf::Graf():
	iloscWierzcholkow(0),
	wierzcholki(0),
	drzewoPoprzednikow(0),
	odleglosci(0){}

Graf::~Graf(){
	this->czysc();
}

void Graf::utworz(int iloscWierzcholkow){
	this->czysc();

	this->iloscWierzcholkow = iloscWierzcholkow;
	this->wierzcholki = new Wierzcholek*[this->iloscWierzcholkow];
	for(int i = 0; i < this->iloscWierzcholkow; i++)
		this->wierzcholki[i] = 0;

	this->drzewoPoprzednikow = new int[this->iloscWierzcholkow];
	this->odleglosci = new float[this->iloscWierzcholkow];

	PorownajSciezki::ustawTabliceOdleglosci(this->odleglosci);
}

void Graf::ustawWierzcholek(int v, const QPoint& pozycja){
	this->wierzcholki[v] = new Wierzcholek(pozycja);
}

void Graf::dodajKrawedz(int v, int w){
	this->wierzcholki[v]->krawedzie.append(w);
	this->wierzcholki[w]->krawedzie.append(v);
}

int Graf::najblizszyWierzcholek(const QPointF& pozycja) const{
	float tmp, min;
	int v;

	min = pow(double(this->wierzcholki[0]->pozycja.x() - pozycja.x()), 2.0) + pow(double(this->wierzcholki[0]->pozycja.y() - pozycja.y()), 2.0);
	v = 0;
	for(int i = 1; i < this->iloscWierzcholkow; i++){
		tmp = pow(double(this->wierzcholki[i]->pozycja.x() - pozycja.x()), 2.0) + pow(double(this->wierzcholki[i]->pozycja.y() - pozycja.y()), 2.0);
		if(tmp < min){
			v = i;
			min = tmp;
		}
	}

	return v;
}

void Graf::ustawKorzen(int v){
	int v_tmp;
	float odleglosc_tmp;

	for(int i = 0; i < this->iloscWierzcholkow; i++)
		this->odleglosci[i] = FLT_MAX;
	this->odleglosci[v] = 0.0F;

	for(int i = 0; i < this->iloscWierzcholkow; i++)
		this->kolejka.insert(i);

	this->drzewoPoprzednikow[v] = v;
	while(!this->kolejka.empty()){
		v_tmp = *(this->kolejka.begin());
		this->kolejka.erase(this->kolejka.begin());

		for(QList<int>::iterator i = this->wierzcholki[v_tmp]->krawedzie.begin(); i != this->wierzcholki[v_tmp]->krawedzie.end(); i++){
			odleglosc_tmp = this->odleglosci[v_tmp] + sqrt(pow(double(this->wierzcholki[v_tmp]->pozycja.x() - this->wierzcholki[*i]->pozycja.x()), 2.0) + pow(double(this->wierzcholki[v_tmp]->pozycja.y() - this->wierzcholki[*i]->pozycja.y()), 2.0));
			if(odleglosc_tmp < this->odleglosci[*i]){
				this->kolejka.erase(this->kolejka.find(*i));
				this->odleglosci[*i] = odleglosc_tmp;
				this->kolejka.insert(*i);
				this->drzewoPoprzednikow[*i] = v_tmp;
			}
		}
	}
}

void Graf::czysc(){
	if(this->iloscWierzcholkow == 0)
		return;

	for(int i = 0; i < this->iloscWierzcholkow; i++)
		delete this->wierzcholki[i];
	delete[] this->wierzcholki;
	this->wierzcholki = 0;
	delete[] this->drzewoPoprzednikow;
	this->drzewoPoprzednikow = 0;
	delete[] this->odleglosci;
	this->odleglosci = 0;
	this->iloscWierzcholkow = 0;
}
