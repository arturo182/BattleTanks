#ifndef PAUZA_H
#define PAUZA_H

#include "silnik.h"

//#include <Phonon/MediaObject>
#include <QPixmap>

class Pauza: public QObject
{
	Q_OBJECT

	public:
		enum Tryb{
			MENU_GLOWNE = 0,
			USTAWIENIA,
			KONIEC_ROZGRYWKI
		};

	private:
		Ekran *ekran;
		BazaDanych *bazaDanych;
		Plansza *plansza;
		QPixmap tlo;
		int pozycja;
		Tryb tryb;
		//Phonon::MediaObject *muzyka;
		int glosnosc;
		QString rozdzielczosc;
		QString jakosc;
		QString sterowanie;
		QPixmap zdjecie;
		class QCamera *m_camera;
		class QCameraImageCapture *m_capture;

	public:
		Pauza(Ekran*, BazaDanych*, Plansza*);

		Silnik::Tryb odswiez(int, Silnik::Akcja);
		void rysuj() const;

		void ustawTlo();
		void grajMuzyke();

	private slots:
		void zapetlMuzyke();
		void imageCaptured(int id, const QImage &preview);

	private:
		void wczytajUstawienia();
		void zapiszUstawienia();
};

#endif // PAUZA_H
