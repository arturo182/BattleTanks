#ifndef ANIMACJA_H
#define ANIMACJA_H

#include "obiekt.h"

class SpecyfikacjaAnimacji;

class Animacja: public Obiekt{
	public:
		const SpecyfikacjaAnimacji* specyfikacja;
		bool status;
		int czasMilisekundy;
		int aktualnaKlatka;
		
		Animacja(SpecyfikacjaAnimacji*, QPointF);
		void odswiez(int);
		inline void zakoncz();
		inline bool sprawdzStatus() const;
		void rysuj(QPainter&, QPoint) const;
};

inline void Animacja::zakoncz(){
	this->status = false;
}

inline bool Animacja::sprawdzStatus() const{
	return this->status;
}

#endif // ANIMACJA_H
