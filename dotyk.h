#ifndef DOTYK_H
#define DOTYK_H

#include <QAccelerometer>
#include <QTouchEvent>

QTM_USE_NAMESPACE

#include "urzadzeniewejscia.h"

class Dotyk : public QObject, public UrzadzenieWejscia
{
	Q_OBJECT

	private:
		bool przyciski[12];
		bool przyciskiPolozenie[12];
		bool przyciskiWcisniete[12];
		int nawigatorPolozenie;
		int nawigatorWcisniete;
		float dzojstiki[2];
		int nawig;
		QAccelerometer *sensor;

	public:
		Dotyk();

		bool otworz();
		void odswiez();
		void poke(QTouchEvent *event);
		void miziu(QGestureEvent *event);

		bool statusPrzyciskPolozenie(Przycisk) const;
		bool statusPrzyciskWcisniecie(Przycisk) const;
		int statusNawigatorPolozenie() const;
		int statusNawigatorWcisniecie() const;
		float statusDzojstik(Dzojstik) const;
};

#endif // DOTYK_H
