#ifndef KLAWIATURA_H
#define KLAWIATURA_H

#include "urzadzeniewejscia.h"

#include <QObject>

class Klawiatura : public QObject, public UrzadzenieWejscia
{
	Q_OBJECT

	private:
		bool przyciski[11];
		bool przyciskiPolozenie[11];
		bool przyciskiWcisniete[11];
		int nawigator;
		int nawigatorPolozenie;
		int nawigatorWcisniete;
		float dzojstiki[2];

	private:
		Przycisk keyNaPrzycisk(int key);

	public:
		Klawiatura();
		~Klawiatura();

		bool otworz();
		void odswiez();

		bool statusPrzyciskPolozenie(Przycisk) const;
		bool statusPrzyciskWcisniecie(Przycisk) const;
		int statusNawigatorPolozenie() const;
		int statusNawigatorWcisniecie() const;
		float statusDzojstik(Dzojstik) const;

	protected:
		bool eventFilter(QObject *, QEvent *);
};

#endif // KLAWIATURA_H
