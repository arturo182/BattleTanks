#include "pauza.h"
#include "bazadanych.h"
#include "funkcje.h"
#include "widzety.h"
#include "plansza.h"
#include "dzwiek.h"
#include "ekran.h"

#include <Phonon/AudioOutput>
#include <QTextOption>
#include <QSqlQuery>
#include <QPainter>
#include <ctime>

Pauza::Pauza(Ekran *ekran, BazaDanych *bazaDanych, Plansza *plansza):
ekran(ekran),
bazaDanych(bazaDanych),
plansza(plansza),
pozycja(1),
tryb(MENU_GLOWNE)
{
	qsrand(time(NULL));

	this->muzyka = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("dzwieki/muzyka/gra_" + QString::number(qrand() % 3 + 1) + ".mp3"));
	this->muzyka->pause();
	this->muzyka->setTransitionTime(-2000);

	Phonon::Path sciezka = this->muzyka->outputPaths().first();
	Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
	wyjscie->setVolume(this->bazaDanych->ustawienie("glosnosc", 5).toInt() / 10.0);

	connect(this->muzyka, SIGNAL(aboutToFinish()), SLOT(zapetlMuzyke()));
	this->zapetlMuzyke();
}

Silnik::Tryb Pauza::odswiez(int milisekundy, Silnik::Akcja akcja){
	if(this->plansza->koniecGry()) {
		QPixmap tlo("grafika/tlo_menu.png");
		this->tlo = tlo.scaled(
			Obiekt::skala * tlo.size(),
			Qt::IgnoreAspectRatio,
			(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
		);

		this->tryb = KONIEC_ROZGRYWKI;
	}

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
				this->plansza->restartuj();
				this->muzyka->setCurrentSource(this->muzyka->queue().first());
				this->muzyka->play();
				return Silnik::ROZGRYWKA;
			} else if(this->pozycja == 3) {
				this->pozycja = 1;
				this->wczytajUstawienia();
				this->tryb = USTAWIENIA;
			} else if(this->pozycja == 4) {
				this->pozycja = 1;
				this->plansza->czysc();
				this->muzyka->setCurrentSource(this->muzyka->queue().first());
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
	} else if(this->tryb == KONIEC_ROZGRYWKI) {
		if(akcja == Silnik::WYBIERZ) {
			if(this->pozycja == 1) {
				this->tryb = MENU_GLOWNE;
				this->pozycja = 1;
				this->plansza->restartuj();
				this->muzyka->setCurrentSource(this->muzyka->queue().first());
				this->muzyka->play();
				return Silnik::ROZGRYWKA;
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->plansza->czysc();
				this->muzyka->setCurrentSource(this->muzyka->queue().first());
				return Silnik::MENU;
			}
		} else if(akcja == Silnik::GORA) {
			if(this->pozycja == 2)
				this->pozycja = 1;
		} else if(akcja == Silnik::DOL) {
			if(this->pozycja == 1)
				this->pozycja = 2;
		}
	}

	return Silnik::PAUZA;
}

void Pauza::rysuj() const{
	QPainter painter(&this->ekran->buforObrazu);

	if(this->tryb == KONIEC_ROZGRYWKI)
		painter.drawTiledPixmap(this->ekran->buforObrazu.rect(), this->tlo);
	else
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
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), "Akceptuj          Anuluj", QTextOption(Qt::AlignRight));
		if(sterowanie == "gamepad") {
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Akceptuj          Anuluj").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "2");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Anuluj").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		} else {
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Akceptuj          Anuluj").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Enter");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Anuluj").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
		}
	} else if(this->tryb == KONIEC_ROZGRYWKI) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, this->plansza->sprawdzWygrana() ? "ZWYCIĘSTWO" : "PORAŻKA", QTextOption(Qt::AlignHCenter));

		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.3, szerokoscEkranu, 500), "Twoj wynik: " + QString::number(this->plansza->sprawdzPunkty()), QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.6, szerokoscEkranu, 500), "Zagraj ponownie", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.65, szerokoscEkranu, 500), "Wróć do menu", QTextOption(Qt::AlignHCenter));
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

void Pauza::grajMuzyke()
{
	this->muzyka->play();
}

void Pauza::zapetlMuzyke()
{
	int muz[3] = { 1, 2, 3};
	for(int i = 0; i < 3; i++)
		qSwap(muz[i], muz[qrand() % 3]);

	for(int i = 0; i < 3; i++)
		this->muzyka->enqueue("dzwieki/muzyka/gra_" + QString::number(muz[i]) + ".mp3");
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

	Phonon::Path sciezka = this->muzyka->outputPaths().first();
	Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
	wyjscie->setVolume(this->glosnosc / 10.0);

	this->ekran->ustawRozdzielczosc(qStringToSize(this->rozdzielczosc));
	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(WYSOKOSC_WIDOKU);
	Tekstura::przeskalujWszystko(Obiekt::skala);

	Dzwiek::glosnosc = this->glosnosc;
	this->ekran->ustawJakosc(this->jakosc);
}
