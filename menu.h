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

QString qSizeToString(const QSize &size);
QSize qStringToSize(const QString &str);

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
	int glosnosc;
	QString rozdzielczosc;
	QString jakosc;
	QString sterowanie;

  public:
	enum Akcja{
	  GORA = 0,
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
	void przyciskUstawien(QPainter &painter, const QRectF &r, const QString &text, bool wstecz = true, bool dalej = true) const;
	void wczytajProfile();
	void wczytajUstawienia();
	void zapiszUstawienia();
};

#endif // MENU_H
