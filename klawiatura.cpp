#include "klawiatura.h"

#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

Klawiatura::Klawiatura():
	QObject(),
	nawigator(0),
	nawigatorPolozenie(0),
	nawigatorWcisniete(0)
{
	for(size_t i = 0; i <= sizeof(this->przyciskiPolozenie); i++) {
		this->przyciski[i] = false;
		this->przyciskiPolozenie[i] = false;
		this->przyciskiWcisniete[i] = false;
	}

	this->dzojstiki[0] = 0.0f;
	this->dzojstiki[1] = 0.0f;
}

Klawiatura::~Klawiatura()
{
	qApp->removeEventFilter(this);

}

bool Klawiatura::otworz()
{
	qApp->installEventFilter(this);

	return true;
}

void Klawiatura::odswiez()
{
	for(size_t i = 0; i <= sizeof(this->przyciski); i++) {
		bool polozenie = this->przyciski[i];
		this->przyciskiWcisniete[i] = polozenie && !this->przyciskiPolozenie[i];
		this->przyciskiPolozenie[i] = polozenie;
	}

	int status = this->nawigator;
	this->nawigatorWcisniete = status & ~this->nawigatorPolozenie;
	this->nawigatorPolozenie = status;
}

bool Klawiatura::statusPrzyciskPolozenie(Przycisk przycisk) const
{
	if(przycisk <= (int)sizeof(this->przyciskiPolozenie))
		return this->przyciskiPolozenie[przycisk];

	return false;
}

bool Klawiatura::statusPrzyciskWcisniecie(Przycisk przycisk) const
{
	if(przycisk <= (int)sizeof(this->przyciskiWcisniete))
		return this->przyciskiWcisniete[przycisk];

	return false;
}

int Klawiatura::statusNawigatorPolozenie() const
{
	return this->nawigatorPolozenie;
}

int Klawiatura::statusNawigatorWcisniecie() const
{
	return this->nawigatorWcisniete;
}

float Klawiatura::statusDzojstik(Dzojstik dzojstik) const
{
	if(dzojstik == LEWY_PIONOWY)
		return this->dzojstiki[0];
	else if(dzojstik == PRAWY_PIONOWY)
		return this->dzojstiki[1];

	return 0.0f;
}

bool Klawiatura::eventFilter(QObject *obj, QEvent *ev)
{
	if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease) {
		if(QKeyEvent *event = static_cast<QKeyEvent*>(ev)) {
			if(!event->isAutoRepeat()) {
				//strzałki to nawigatory
				if(event->key() == Qt::Key_Up) {
					if(ev->type() == QEvent::KeyPress)
						this->nawigator |= GORA;
					else if(ev->type() == QEvent::KeyRelease)
						this->nawigator &= ~GORA;
				} else if(event->key() == Qt::Key_Down) {
					if(ev->type() == QEvent::KeyPress)
						this->nawigator |= DOL;
					else if(ev->type() == QEvent::KeyRelease)
						this->nawigator &= ~DOL;
				} else if(event->key() == Qt::Key_Left) {
					if(ev->type() == QEvent::KeyPress)
						this->nawigator |= LEWO;
					else if(ev->type() == QEvent::KeyRelease)
						this->nawigator &= ~LEWO;
				} else if(event->key() == Qt::Key_Right) {
					if(ev->type() == QEvent::KeyPress)
						this->nawigator |= PRAWO;
					else if(ev->type() == QEvent::KeyRelease)
						this->nawigator &= ~PRAWO;

				//gąsiennice
				} else if(event->key() == Qt::Key_Q) {
					if(ev->type() == QEvent::KeyPress)
						if(this->dzojstiki[0] > -1.0)
							this->dzojstiki[0] -= 0.1;
				} else if(event->key() == Qt::Key_E) {
					if(ev->type() == QEvent::KeyPress)
						if(this->dzojstiki[1] > -1.0)
							this->dzojstiki[1] -= 0.1;
				} else if(event->key() == Qt::Key_A) {
					if(ev->type() == QEvent::KeyPress)
						if(this->dzojstiki[0] < 1.0)
							this->dzojstiki[0] += 0.1;
				} else if(event->key() == Qt::Key_D) {
					if(ev->type() == QEvent::KeyPress)
						if(this->dzojstiki[1] < 1.0)
							this->dzojstiki[1] += 0.1;
				} else {
					//inne klawisze
					int przycisk = keyNaPrzycisk(event->key());
					if(przycisk > -1) {
						if(ev->type() == QEvent::KeyPress)
							this->przyciski[przycisk] = true;
						else if(ev->type() == QEvent::KeyRelease)
							this->przyciski[przycisk] = false;
					}
				}
			}
		}
	}

	return QObject::eventFilter(obj, ev);
}

UrzadzenieWejscia::Przycisk Klawiatura::keyNaPrzycisk(int key)
{
	if(key == Qt::Key_Return)
		return WYBIERZ;
	else if(key == Qt::Key_Backspace)
		return COFNIJ;
	else if(key == Qt::Key_Delete)
		return USUN;
	else if(key == Qt::Key_Space)
		return WYBIERZ_ALT;
	else if(key == Qt::Key_W)
		return WYSTRZAL;
	else if(key == Qt::Key_Z)
		return WIEZA_LEWO;
	else if(key == Qt::Key_C)
		return WIEZA_PRAWO;
	else if(key == Qt::Key_1)
		return BRON_MINUS;
	else if(key == Qt::Key_3)
		return BRON_PLUS;

	return NIEZNANY;
}
