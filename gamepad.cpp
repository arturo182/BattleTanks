#include "gamepad.h"
#include <cmath>

Gamepad::Gamepad():
	urzadzenie(0){
	SDL_Init(SDL_INIT_JOYSTICK);
}

Gamepad::~Gamepad(){
	if(this->urzadzenie != 0){
		SDL_JoystickClose(this->urzadzenie);
		delete[] this->przyciskiPolozenie;
		delete[] this->przyciskiWcisniete;
		delete[] this->nawigatoryPolozenie;
		delete[] this->nawigatoryWcisniete;
		delete[] this->dzojstiki;
	}
	SDL_Quit();
}

int Gamepad::przyciskNaNumer(UrzadzenieWejscia::Przycisk przycisk) const
{
	if(przycisk == BRON_PLUS)
		return 2;
	else if(przycisk == BRON_PLUS2)
		return 4;
	else if(przycisk == BRON_MINUS)
		return 0;
	else if(przycisk == WIEZA_LEWO)
		return 6;
	else if(przycisk == WIEZA_PRAWO)
		return 7;
	else if(przycisk == WYSTRZAL)
		return 1;
	else if(przycisk == WYSTRZAL2)
		return 5;
	else if(przycisk == COFNIJ)
		return 3;
	else if(przycisk == USUN)
		return 0;
	else if(przycisk == WYBIERZ)
		return 1;
	else if(przycisk == WYBIERZ_ALT)
		return 2;
	else if(przycisk == PAUZA)
		return 9;

	return -1;
}

bool Gamepad::otworz(){
	this->urzadzenie = SDL_JoystickOpen(0);
	if(this->urzadzenie == 0)
		return false;

	this->iloscPrzyciskow = SDL_JoystickNumButtons(this->urzadzenie);
	this->iloscNawigatorow = SDL_JoystickNumHats(this->urzadzenie);
	this->iloscDzojstikow = SDL_JoystickNumAxes(this->urzadzenie);

	this->przyciskiPolozenie = new bool[this->iloscPrzyciskow];
	this->przyciskiWcisniete = new bool[this->iloscPrzyciskow];
	this->nawigatoryPolozenie = new int[this->iloscNawigatorow];
	this->nawigatoryWcisniete = new int[this->iloscNawigatorow];
	this->dzojstiki = new short int[this->iloscDzojstikow];

	for(int i = 0; i < this->iloscPrzyciskow; i++)
		this->przyciskiPolozenie[i] = this->przyciskiWcisniete[i] = false;

	for(int i = 0; i < this->iloscNawigatorow; i++)
		this->nawigatoryPolozenie[i] = this->nawigatoryWcisniete[i] = 0;

	return true;
}

void Gamepad::odswiez(){
	if(this->urzadzenie == 0)
		return;

	SDL_JoystickUpdate();

	bool polozenie;
	for(int i = 0; i < this->iloscPrzyciskow; i++){
		polozenie = SDL_JoystickGetButton(this->urzadzenie, i);
		this->przyciskiWcisniete[i] = polozenie && !this->przyciskiPolozenie[i];
		this->przyciskiPolozenie[i] = polozenie;
	}

	int status;
	for(int i = 0; i < this->iloscNawigatorow; i++){
		status = SDL_JoystickGetHat(this->urzadzenie, i);
		this->nawigatoryWcisniete[i] = status & ~this->nawigatoryPolozenie[i];
		this->nawigatoryPolozenie[i] = status;
	}

	for(int i = 0; i < this->iloscDzojstikow; i++)
		this->dzojstiki[i] = SDL_JoystickGetAxis(this->urzadzenie, i);
}

bool Gamepad::statusPrzyciskPolozenie(Przycisk przycisk) const{
	int numerPrzycisku = przyciskNaNumer(przycisk);

	if(this->urzadzenie != 0 && numerPrzycisku < this->iloscPrzyciskow)
		return this->przyciskiPolozenie[numerPrzycisku];
	return false;
}

bool Gamepad::statusPrzyciskWcisniecie(Przycisk przycisk) const{
	int numerPrzycisku = przyciskNaNumer(przycisk);

	if(this->urzadzenie != 0 && numerPrzycisku < this->iloscPrzyciskow)
		return this->przyciskiWcisniete[numerPrzycisku];
	return false;
}

int Gamepad::statusNawigatorPolozenie() const{
	if(this->urzadzenie != 0 && this->iloscNawigatorow > 0)
		return this->nawigatoryPolozenie[0];
	return 0;
}

int Gamepad::statusNawigatorWcisniecie() const{
	if(this->urzadzenie != 0 && this->iloscNawigatorow > 0)
		return this->nawigatoryWcisniete[0];
	return 0;
}

float Gamepad::statusDzojstik(Dzojstik numerDzojstika) const{
	if(this->urzadzenie != 0 && numerDzojstika < this->iloscDzojstikow){
		float status = float(this->dzojstiki[numerDzojstika]) / 0x8000;
		if(fabs(status) > DZOJSTIK_EPSILON)
			return status;
	}
	return 0.0F;
}
