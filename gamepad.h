#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "urzadzeniewejscia.h"

class Gamepad : public UrzadzenieWejscia
{
	private:
		SDL_Joystick* urzadzenie;
		int iloscPrzyciskow;
		int iloscNawigatorow;
		int iloscDzojstikow;
		bool* przyciskiPolozenie;
		bool* przyciskiWcisniete;
		int* nawigatoryPolozenie;
		int* nawigatoryWcisniete;
		short int* dzojstiki;

	private:
		int przyciskNaNumer(Przycisk) const;

	public:
		Gamepad();
		~Gamepad();

		bool otworz();
		void odswiez();

		bool statusPrzyciskPolozenie(Przycisk) const;
		bool statusPrzyciskWcisniecie(Przycisk) const;
		int statusNawigatorPolozenie() const;
		int statusNawigatorWcisniecie() const;
		float statusDzojstik(Dzojstik) const;
};

#endif // GAMEPAD_H
