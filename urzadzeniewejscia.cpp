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
		delete[] this->nawigatory;
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
	this->nawigatory = new int[this->iloscNawigatorow];
	this->dzojstiki = new short int[this->iloscDzojstikow];
	
	for(int i = 0; i < this->iloscPrzyciskow; i++)
		this->przyciskiPolozenie[i] = this->przyciskiWcisniete[i] = false;
	
	return true;
}

void UrzadzenieWejscia::odswiez(){
	if(this->urzadzenie == 0)
		return;
	
	SDL_JoystickUpdate();
	
	for(int i = 0; i < this->iloscPrzyciskow; i++){
		if(!SDL_JoystickGetButton(this->urzadzenie, i))
			this->przyciskiPolozenie[i] = this->przyciskiWcisniete[i] = false;
		else if(!this->przyciskiPolozenie[i])
			this->przyciskiPolozenie[i] = true;
		else if(!this->przyciskiWcisniete[i])
			this->przyciskiWcisniete[i] = true;
	}
	
	for(int i = 0; i < this->iloscNawigatorow; i++)
		this->nawigatory[i] = SDL_JoystickGetHat(this->urzadzenie, i);
	
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

int UrzadzenieWejscia::statusNawigator(int numerNawigatora) const{
	if(this->urzadzenie != 0 && numerNawigatora < this->iloscNawigatorow)
		return this->nawigatory[numerNawigatora];
	return 0;
}

double UrzadzenieWejscia::statusDzojstik(int numerDzojstika) const{
	if(this->urzadzenie != 0 && numerDzojstika < this->iloscDzojstikow)
		return double(this->dzojstiki[numerDzojstika]) / 0x8000;
	return 0.0;
}
