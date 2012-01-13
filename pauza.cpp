#include "pauza.h"
#include "bazadanych.h"
#include "funkcje.h"
#include "widzety.h"
#include "plansza.h"
#include "dzwiek.h"
#include "ekran.h"

#include <QTextOption>
#include <QSqlQuery>
#include <QPainter>

Pauza::Pauza(Ekran *ekran, BazaDanych *bazaDanych, Plansza *plansza):
ekran(ekran),
bazaDanych(bazaDanych),
plansza(plansza),
pozycja(1),
tryb(MENU_GLOWNE)
{
}

Silnik::Tryb Pauza::odswiez(int milisekundy, Silnik::Akcja akcja){
	if(this->tryb == MENU_GLOWNE) {
		if(akcja == Silnik::START) {
			this->pozycja = 1;
			return Silnik::ROZGRYWKA;
		} else if(akcja == Silnik::WYBIERZ) {
			if(this->pozycja == 1) {
				this->pozycja = 1;
				return Silnik::ROZGRYWKA;
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
			} else if(this->pozycja == 3) {
				this->pozycja = 1;
				this->wczytajUstawienia();
				this->tryb = USTAWIENIA;
			} else if(this->pozycja == 4) {
				this->pozycja = 1;
				this->plansza->czysc();
				return Silnik::MENU;
			}
		} else if(akcja == Silnik::GORA) {
			if(this->pozycja > 1)
				this->pozycja--;
			else
				this->pozycja = 4;
		} else if(akcja == Silnik::DOL) {
			if(this->pozycja < 4)
				this->pozycja++;
			else
				this->pozycja = 1;
		}
	} else if(this->tryb == USTAWIENIA) {
		if(akcja == Silnik::COFNIJ) {
			this->wczytajUstawienia();
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		} else if(akcja == Silnik::WYBIERZ) {
			this->zapiszUstawienia();
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		} else if(akcja == Silnik::GORA) {
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == Silnik::DOL) {
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == Silnik::LEWO) {
			if(this->pozycja == 1) {
				if(this->glosnosc > 0)
					this->glosnosc -= 1;
			} else if(this->pozycja == 2) {
				if(this->rozdzielczosc == "1920x1080")
					this->rozdzielczosc = "1280x720";
				else if(this->rozdzielczosc == "1280x720")
					this->rozdzielczosc = "1024x768";
				else if(this->rozdzielczosc == "1024x768")
					this->rozdzielczosc = "854x480";
				else if(this->rozdzielczosc == "854x480")
					this->rozdzielczosc = "640x480";
			} else if(this->pozycja == 3) {
				if(this->jakosc == "wysoka")
					this->jakosc = "niska";
			} else if(this->pozycja == 4) {
				if(this->sterowanie == "klawiatura")
					this->sterowanie = "gamepad";
			}
		} else if(akcja == Silnik::PRAWO) {
			if(this->pozycja == 1) {
				if(this->glosnosc < 10)
					this->glosnosc += 1;
			} else if(this->pozycja == 2) {
				if(this->rozdzielczosc == "640x480")
					this->rozdzielczosc = "854x480";
				else if(this->rozdzielczosc == "854x480")
					this->rozdzielczosc = "1024x768";
				else if(this->rozdzielczosc == "1024x768")
					this->rozdzielczosc = "1280x720";
				else if(this->rozdzielczosc == "1280x720")
					this->rozdzielczosc = "1920x1080";
			} else if(this->pozycja == 3) {
				if(this->jakosc == "niska")
					this->jakosc = "wysoka";
			} else if(this->pozycja == 4) {
				if(this->sterowanie == "gamepad")
					this->sterowanie = "klawiatura";
			}
		}
	}

	return Silnik::PAUZA;
}

void Pauza::rysuj() const{
	QPainter painter(&this->ekran->buforObrazu);
	painter.drawPixmap(this->ekran->buforObrazu.rect(), this->tlo);

	const QColor kolorZaznaczenia("#278fbb");

	int szerokoscEkranu = this->ekran->buforObrazu.width();
	int wysokoscEkranu = this->ekran->buforObrazu.height();
	QRectF obszarTytulu = QRectF(0, wysokoscEkranu * 0.08, szerokoscEkranu, 100);

	QFont czcionkaTytulu = painter.font();
	czcionkaTytulu.setFamily("Trebuchet MS");
	czcionkaTytulu.setPixelSize(wysokoscEkranu * 0.04);

	QFont czcionkaNormalna = painter.font();
	czcionkaNormalna.setFamily("Trebuchet MS");
	czcionkaNormalna.setPixelSize(wysokoscEkranu * 0.03);

	QString sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();

	painter.setPen(Qt::white);
	if(this->tryb == MENU_GLOWNE) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "PAUZA", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);

		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.45, szerokoscEkranu, 500), "Kontynuuj", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.5, szerokoscEkranu, 500), "Zrestartuj", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.55, szerokoscEkranu, 500), "Ustawienia", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.6, szerokoscEkranu, 500), "Wróć do menu", QTextOption(Qt::AlignHCenter));
	} else if(this->tryb == USTAWIENIA) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Ustawienia:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		if(pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.4, 500, 100), "Głośność");

		Widzety::schodkiGlosnosci(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.4, szerokoscEkranu * 0.2, wysokoscEkranu * 0.06), this->glosnosc);

		if(pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5, 500, 100), "Rozdzielczość");
		Widzety::przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.5, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->rozdzielczosc, this->rozdzielczosc != "640x480", this->rozdzielczosc != "1920x1080");

		if(pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.6, 500, 100), "Jakość");
		Widzety::przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.6, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->jakosc, this->jakosc != "niska", this->jakosc != "wysoka");

		if(pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.7, 500, 100), "Sterowanie");
		Widzety::przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.7, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->sterowanie, this->sterowanie != "gamepad", this->sterowanie != "klawiatura");

		painter.setPen(Qt::white);
		Widzety::cieniowanyTekst(painter, QRectF(0, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu * 0.9, 100), QString("Akceptuj [%1]      Anuluj [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignRight));
	}

	painter.end();
	this->ekran->update();
}

void Pauza::ustawTlo()
{
	QPainter painter(&this->ekran->buforObrazu);
	painter.fillRect(this->ekran->buforObrazu.rect(), QColor(0, 0, 0, 128));
	painter.end();

	this->tlo = this->ekran->buforObrazu;
}

void Pauza::wczytajUstawienia()
{
	this->glosnosc = this->bazaDanych->ustawienie("glosnosc", 5).toInt();
	this->jakosc = this->bazaDanych->ustawienie("jakosc", "niska").toString();
	this->rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "1280x720").toString();
	this->sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();
}

void Pauza::zapiszUstawienia()
{
	//transakcja!
	QSqlQuery("BEGIN;", QSqlDatabase::database("dbUstawienia"));
	this->bazaDanych->zapiszUstawienie("glosnosc", this->glosnosc);
	this->bazaDanych->zapiszUstawienie("jakosc", this->jakosc);
	this->bazaDanych->zapiszUstawienie("rozdzielczosc", this->rozdzielczosc);
	this->bazaDanych->zapiszUstawienie("sterowanie", this->sterowanie);
	QSqlQuery("END;", QSqlDatabase::database("dbUstawienia"));

	this->ekran->ustawRozdzielczosc(qStringToSize(this->rozdzielczosc));
	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(WYSOKOSC_WIDOKU);
	Tekstura::przeskalujWszystko(Obiekt::skala);

	Dzwiek::glosnosc = this->glosnosc;
	this->ekran->ustawJakosc(this->jakosc);
}
