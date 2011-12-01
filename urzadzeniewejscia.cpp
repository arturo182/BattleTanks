#include "urzadzeniewejscia.h"

UrzadzenieWejscia::UrzadzenieWejscia():
	urzadzenie(0){
	SDL_Init(SDL_INIT_JOYSTICK);
}
	
UrzadzenieWejscia::~UrzadzenieWejscia(){
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
	
bool UrzadzenieWejscia::otworz(int numerUrzadzenia){
	this->urzadzenie = SDL_JoystickOpen(numerUrzadzenia);
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

void UrzadzenieWejscia::odswiez(){
	if(this->urzadzenie == 0)
		return;
	
	SDL_JoystickUpdate();
	
	bool polozenie;
	for(int i = 0; i < this->iloscPrzyciskow; i++){
		polozenie = SDL_JoystickGetButton(this->urzadzenie, i);
		this->przyciskiWcisniete[i] = this->przyciskiPolozenie[i] && polozenie;
		this->przyciskiPolozenie[i] = polozenie;
	}
	
	int status;
	for(int i = 0; i < this->iloscNawigatorow; i++){
		status = SDL_JoystickGetHat(this->urzadzenie, i);
		this->nawigatoryWcisniete[i] = this->nawigatoryPolozenie[i] & status;
		this->nawigatoryPolozenie[i] = status;
	}
	
	for(int i = 0; i < this->iloscDzojstikow; i++)
		this->dzojstiki[i] = SDL_JoystickGetAxis(this->urzadzenie, i);
}

bool UrzadzenieWejscia::statusPrzyciskPolozenie(int numerPrzycisku) const{
	if(this->urzadzenie != 0 && numerPrzycisku < this->iloscPrzyciskow)
		return this->przyciskiPolozenie[numerPrzycisku];
	return false;
}

bool UrzadzenieWejscia::statusPrzyciskWcisniecie(int numerPrzycisku) const{
	if(this->urzadzenie != 0 && numerPrzycisku < this->iloscPrzyciskow)
		return this->przyciskiPolozenie[numerPrzycisku] && !this->przyciskiWcisniete[numerPrzycisku];
	return false;
}

int UrzadzenieWejscia::statusNawigatorPolozenie(int numerNawigatora) const{
	if(this->urzadzenie != 0 && numerNawigatora < this->iloscNawigatorow)
		return this->nawigatoryPolozenie[numerNawigatora];
	return 0;
}

int UrzadzenieWejscia::statusNawigatorWcisniecie(int numerNawigatora) const{
	if(this->urzadzenie != 0 && numerNawigatora < this->iloscNawigatorow)
		return this->nawigatoryPolozenie[numerNawigatora] ^ this->nawigatoryWcisniete[numerNawigatora];
	return 0;
}

double UrzadzenieWejscia::statusDzojstik(int numerDzojstika) const{
	if(this->urzadzenie != 0 && numerDzojstika < this->iloscDzojstikow)
		return double(this->dzojstiki[numerDzojstika]) / 0x8000;
	return 0.0;
}
