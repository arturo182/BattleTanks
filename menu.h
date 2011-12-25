#ifndef MENU_H
#define MENU_H

#include <QTextOption>
#include <QPainter>
#include <QPoint>
#include "silnik.h"

namespace Phonon
{
  class MediaObject;
}

class QTreeWidget;

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
	  WYBOR_MAPY,
	  REKORDY,
	  USTAWIENIA_POMOC,
	  USTAWIENIA,
	  AUTORZY,
	  POMOC
	};

  private:
	QTreeWidget *drzewko;
	Ekran* ekran;
	BazaDanych* bazaDanych;
	Plansza* plansza;
	Tryb tryb;
	int pozycja;
	int zaznaczenie;
	int idGracza;
	QString nowyProfil;
	QStringList alfabet;
	bool shift;
	QPixmap tloPixmapa;
	Phonon::MediaObject *muzyka;

  public:
	enum Akcja{
	  GORA,
	  DOL,
	  PRAWO,
	  LEWO,
	  WYBIERZ,
	  WYBIERZ_ALT,
	  USUN,
	  COFNIJ,
	  BRAK
	};

	Menu(Ekran*, BazaDanych*, Plansza*);
	Silnik::Tryb odswiez(int, Akcja);
	void rysuj() const;
	void ladujMuzyke();

  private slots:
	void zapetlMuzyke();

  private:
	void cieniowanyTekst(QPainter &painter, const QRectF &r, const QString &text, const QTextOption &o = QTextOption()) const;
	void wczytajProfile();
};

#endif // MENU_H
