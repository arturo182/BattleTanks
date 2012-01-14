#ifndef MENU_H
#define MENU_H

#include "silnik.h"

#include <QTextOption>
#include <QPainter>
#include <QPoint>
#include <QMap>

namespace Phonon
{
  class MediaObject;
}

class BazaDanych;
class Plansza;
class Ekran;

class Menu: public QObject{
  Q_OBJECT

  public:
	enum Tryb{
	  WYBOR_PROFILU,
	  TWORZENIE_PROFILU,
	  MENU_GLOWNE,
	  WYBOR_TRYBU,
	  WYBOR_PLANSZY,
	  REKORDY,
	  USTAWIENIA_POMOC,
	  USTAWIENIA,
	  AUTORZY,
	  POMOC
	};

  private:
	Ekran* ekran;
	BazaDanych* bazaDanych;
	Plansza* plansza;
	Tryb tryb;
	int pozycja;
	int zaznaczenie;
	int idGracza;
	int trybGry;
	QString nowyProfil;
	QStringList alfabet;
	QStringList profile;
	QList<QStringList> plansze;
	QMap<QString, QPixmap> miniPlansze;
	QList<QStringList> rekordy;
	bool shift;
	QPixmap tloPixmapa;
	QPixmap logoPixmapa;
	QPixmap trybyPixmapa[4];
	Phonon::MediaObject *muzyka;
	int glosnosc;
	QString rozdzielczosc;
	QString jakosc;
	QString sterowanie;

  public:
	Menu(Ekran*, BazaDanych*, Plansza*);

	Silnik::Tryb odswiez(int, Silnik::Akcja);
	void rysuj() const;

	void odtwarzajMuzyke();

  private slots:
	void zapetlMuzyke();

  private:
	void wczytajProfile();
	void wczytajRekordy();
	void wczytajMuzyke();
	void wczytajGrafiki();
	void wczytajMiniPlansze();
	void wczytajPlansze();
	void wczytajUstawienia();
	void zapiszUstawienia();
};

#endif // MENU_H
