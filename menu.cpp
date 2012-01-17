#include "menu.h"
#include "bazadanych.h"
#include "ladowanie.h"
#include "funkcje.h"
#include "plansza.h"
#include "widzety.h"
#include "dzwiek.h"
#include "ekran.h"

#include <Phonon/MediaObject>
#include <Phonon/VideoWidget>
#include <Phonon/AudioOutput>
#include <QApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QStyle>

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza, Ladowanie *ladowanie):
QObject(),
ekran(ekran),
bazaDanych(bazaDanych),
plansza(plansza),
ladowanie(ladowanie),
tryb(WYBOR_PROFILU),
pozycja(1){
	this->wczytajMuzyke();
	this->ladowanie->odswiez(0.17, "Wczytywanie grafiki menu");

	this->wczytajGrafiki();
	this->ladowanie->odswiez(0.19, "Wczytywanie profili");

	this->wczytajProfile();
	this->ladowanie->odswiez(0.20, "Generowanie miniatur map");

	this->wczytajMiniPlansze();

	for(int i = 0; i <= 9; i++)
		this->alfabet.append(QString::number(i));

	for(int i = 'a'; i <= 'z'; i++)
		this->alfabet.append(QChar(i));
}

Silnik::Tryb Menu::odswiez(int milisekundy, Silnik::Akcja akcja){
	//	DEBUG
	//	this->plansza->zaladuj("pustynia");
	//	return Silnik::ROZGRYWKA;

	int przezroczystosc = this->property("przezroczystosc").toInt();
	if(przezroczystosc < 100) {
		przezroczystosc += 5;
		this->setProperty("przezroczystosc", przezroczystosc);
	}

	if(this->tryb == WYBOR_TRYBU) {
		if(akcja == Silnik::WYBIERZ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->setProperty("trybGry", this->pozycja - 1);
			this->wczytajPlansze();
			this->pozycja = 1;
			this->ustawTryb(WYBOR_PLANSZY);
		} else if(akcja == Silnik::LEWO) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja = 1;
		} else if(akcja == Silnik::PRAWO) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja = 2;
		} else if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->ustawTryb(MENU_GLOWNE);
		}
	} else if(this->tryb == WYBOR_PLANSZY) {
		if(akcja == Silnik::WYBIERZ) {
			int id = this->plansze.at(this->pozycja-1).at(0).toInt();
			QString nazwa = this->plansze.at(this->pozycja-1).at(2);

			this->plansza->zaladuj(id, nazwa);
			this->ustawTryb(MENU_GLOWNE);
			this->pozycja = 1;
			this->muzyka->stop();
			return Silnik::ROZGRYWKA;
		} else if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->ustawTryb(WYBOR_TRYBU);
		} else if(akcja == Silnik::LEWO) {
			if(this->pozycja > 1)
				this->pozycja--;
		} else if(akcja == Silnik::PRAWO) {
			if(this->pozycja < this->plansze.count())
				this->pozycja++;
		}
	} else if(this->tryb == REKORDY) {
		if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->ustawTryb(MENU_GLOWNE);
		} else if(akcja == Silnik::GORA) {
			if(this->pozycja > 1)
				this->pozycja--;
		} else if(akcja == Silnik::DOL) {
			const int na_strone = 16;
			if(this->pozycja < (this->rekordy.count() / na_strone) + 1)
				this->pozycja++;
		}
	} else if(this->tryb == MENU_GLOWNE) {
		if(!this->muzyka->state() != Phonon::PlayingState)
			this->muzyka->play();

		if(this->bazaDanych->ustawienie("rozdzielczosc", "1280x720").toString() != this->rozdzielczosc) {
			this->wczytajUstawienia();
			this->wczytajGrafiki();
		}

		if(akcja == Silnik::WYBIERZ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");

			if(this->pozycja == 1) {
				this->pozycja = 1;
				this->ustawTryb(WYBOR_TRYBU);
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->ustawTryb(USTAWIENIA_POMOC);
			} else if(this->pozycja == 3) {
				this->wczytajRekordy();
				this->ustawTryb(REKORDY);
			} else if(pozycja == 4) {
				return Silnik::WYJSCIE;
			}
		} else if(akcja == Silnik::GORA) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == Silnik::DOL) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		}
	} else if(this->tryb == USTAWIENIA_POMOC) {
		if(akcja == Silnik::WYBIERZ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");

			if(this->pozycja == 1) {
				this->pozycja = 1;
				this->wczytajUstawienia();
				this->ustawTryb(USTAWIENIA);
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->ustawTryb(POMOC);
			} else if(this->pozycja == 3) {
				this->pozycja = 1;
				this->ustawTryb(AUTORZY);
			} else if(this->pozycja == 4) {
				this->pozycja = 1;
				this->ustawTryb(MENU_GLOWNE);
			}
		} else if(akcja == Silnik::GORA) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == Silnik::DOL) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->ustawTryb(MENU_GLOWNE);
		}
	} else if(this->tryb == USTAWIENIA) {
		if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->wczytajUstawienia();
			this->pozycja = 1;
			this->ustawTryb(USTAWIENIA_POMOC);
		} else if(akcja == Silnik::WYBIERZ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->zapiszUstawienia();
			this->pozycja = 1;
			this->ustawTryb(USTAWIENIA_POMOC);
		} else if(akcja == Silnik::GORA) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == Silnik::DOL) {
			Dzwiek::odtworz("dzwieki/menu_zmiana.mp3");
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

		//aktualizacja glosnosci
		if(this->muzyka->outputPaths().count()) {
			Phonon::Path sciezka = this->muzyka->outputPaths().first();
			Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
			wyjscie->setVolume(this->glosnosc / 10.0);
		}
	} else if(this->tryb == AUTORZY) {
		if(akcja == Silnik::COFNIJ) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->ustawTryb(USTAWIENIA_POMOC);
		}
	} else if(this->tryb == POMOC) {
		if(akcja == Silnik::WYBIERZ)
			this->pozycja++;

		if(this->pozycja == 3) {
			this->pozycja = 1;
			this->ustawTryb(USTAWIENIA_POMOC);
		}
	} else if(this->tryb == WYBOR_PROFILU) {
		if(akcja == Silnik::WYBIERZ && this->profile.count()){
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");

			this->setProperty("idGracza", this->bazaDanych->idProfilu(this->profile.at(this->pozycja-1)));
			this->pozycja = 1;
			this->ustawTryb(MENU_GLOWNE);
		} else if(akcja == Silnik::WYBIERZ_ALT) {
			Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
			this->setProperty("nowyProfil", "");
			this->ustawTryb(TWORZENIE_PROFILU);
			this->pozycja = 1;
		} else if(akcja == Silnik::USUN) {
			if(this->profile.count()) {
				int id = this->bazaDanych->idProfilu(this->profile.at(this->pozycja-1));

				this->bazaDanych->usunProfil(id);

				wczytajProfile();
			}
		} else if(akcja == Silnik::COFNIJ) {
			return Silnik::WYJSCIE;
		} else if(akcja == Silnik::GORA) {
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = this->profile.count();
		} else if(akcja == Silnik::DOL) {
			this->pozycja++;

			if(this->pozycja > this->profile.count())
				this->pozycja = 1;
		}
	} else if(this->tryb == TWORZENIE_PROFILU) {
		if(akcja == Silnik::PRAWO) {
			Dzwiek::odtworz("dzwieki/menu_litera.mp3");

			if(this->pozycja == 10)
				this->pozycja = 37;
			else if(this->pozycja == 20)
				this->pozycja = 38;
			else if(this->pozycja == 30)
				this->pozycja = 39;
			else if(this->pozycja == 36)
				this->pozycja = 40;
			else if(this->pozycja == 37)
				this->pozycja = 1;
			else if(this->pozycja == 38)
				this->pozycja = 11;
			else if(this->pozycja == 39)
				this->pozycja = 21;
			else if(this->pozycja == 40)
				this->pozycja = 31;
			else
				this->pozycja++;
		} else if(akcja == Silnik::LEWO) {
			Dzwiek::odtworz("dzwieki/menu_litera.mp3");

			if(this->pozycja == 1)
				this->pozycja = 37;
			else if(this->pozycja == 11)
				this->pozycja = 38;
			else if(this->pozycja == 21)
				this->pozycja = 39;
			else if(this->pozycja == 31)
				this->pozycja = 40;
			else if(this->pozycja == 37)
				this->pozycja = 10;
			else if(this->pozycja == 38)
				this->pozycja = 20;
			else if(this->pozycja == 39)
				this->pozycja = 30;
			else if(this->pozycja == 40)
				this->pozycja = 36;
			else
				this->pozycja--;
		} else if(akcja == Silnik::DOL) {
			Dzwiek::odtworz("dzwieki/menu_litera.mp3");

			if(this->pozycja == 27)
				this->pozycja = 7;
			else if(this->pozycja == 28)
				this->pozycja = 8;
			else if(this->pozycja == 29)
				this->pozycja = 9;
			else if(this->pozycja == 30)
				this->pozycja = 10;
			else if(this->pozycja >= 37 && this->pozycja <= 39)
				this->pozycja++;
			else if(this->pozycja == 40)
				this->pozycja = 37;
			else
				this->pozycja += 10;
		} else if(akcja == Silnik::GORA) {
			Dzwiek::odtworz("dzwieki/menu_litera.mp3");

			if(this->pozycja == 7)
				this->pozycja = 27;
			else if(this->pozycja == 8)
				this->pozycja = 28;
			else if(this->pozycja == 9)
				this->pozycja = 29;
			else if(this->pozycja == 10)
				this->pozycja = 30;
			else if(this->pozycja >= 38 && this->pozycja <= 40)
				this->pozycja--;
			else if(this->pozycja == 37)
				this->pozycja = 40;
			else
				this->pozycja -= 10;
		} else if(akcja == Silnik::WYBIERZ) {
			Dzwiek::odtworz("dzwieki/menu_karetka.mp3");

			if(this->pozycja <= this->alfabet.count()) {
				if(this->property("nowyProfil").toString().length() < 80) {
					QString litera = this->alfabet[this->pozycja-1];

					if(this->property("shift").toBool())
						litera = litera.toUpper();

					QString nowyProfil = this->property("nowyProfil").toString();
					nowyProfil.append(litera);
					this->setProperty("nowyProfil", nowyProfil);
				}
			} else if(this->pozycja == 37) {
				QString nowyProfil = this->property("nowyProfil").toString();
				nowyProfil.append(" ");
				this->setProperty("nowyProfil", nowyProfil);
			} else if(this->pozycja == 38)
				this->setProperty("shift", !this->property("shift").toBool());
			else if(this->pozycja == 39) {
				QString nowyProfil = this->property("nowyProfil").toString();
				nowyProfil.chop(1);
				this->setProperty("nowyProfil", nowyProfil);
			} else if(this->pozycja == 40) {
				if(this->property("nowyProfil").toString().length()) {
					this->bazaDanych->dodajProfil(this->property("nowyProfil").toString());
					wczytajProfile();
				}

				Dzwiek::odtworz("dzwieki/menu_wybor.mp3");
				this->pozycja = 1;
				this->ustawTryb(WYBOR_PROFILU);
			}
		}

		if(this->pozycja < 1)
			this->pozycja = 40;
		else if(this->pozycja > 40)
			this->pozycja = this->pozycja - 40;
	}

	return Silnik::MENU;
}

void Menu::rysuj() const{
	//	rysowanie na ekranie :P
	QPainter painter(&this->ekran->buforObrazu);

	int przezroczystosc = this->property("przezroczystosc").toInt();
	if(przezroczystosc < 100) {
		painter.setOpacity(przezroczystosc / 100.0);
	}

	painter.drawTiledPixmap(this->ekran->buforObrazu.rect(), this->tloPixmapa);
	painter.setPen(Qt::white);

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

	QFont czcionkaSrednia = painter.font();
	czcionkaSrednia.setFamily("Trebuchet MS");
	czcionkaSrednia.setPixelSize(wysokoscEkranu * 0.035);

	QString sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();

	if(this->tryb == TWORZENIE_PROFILU) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Tworzenie profilu", QTextOption(Qt::AlignHCenter));

		Widzety::cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Profil: " + this->property("nowyProfil").toString() + "_", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		int odstepX = szerokoscEkranu * 0.03;
		int odstepY = wysokoscEkranu * 0.06;
		int offsetY = wysokoscEkranu * 0.45;

		int wiersz = 0;
		int kolumna = 0;
		foreach(const QString &litera, this->alfabet) {
			if(this->pozycja - 1 == wiersz * 10 + kolumna) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
			Widzety::cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * (kolumna - 6)), offsetY + odstepY * wiersz, 100, 100), (this->property("shift").toBool())?litera.toUpper():litera);

			if(kolumna < 9) {
				kolumna++;
			} else {
				kolumna = 0;
				wiersz++;
			}
		}

		if(this->pozycja == 37) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY, 300, 100), "SPACJA");

		if(this->pozycja == 38) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY, 300, 100), "SHIFT");

		if(this->pozycja == 39) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 2, 300, 100), "USUŃ");

		if(this->pozycja == 40) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 3, 300, 100), "GOTOWE");
	} else if(this->tryb == WYBOR_PROFILU) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Wybierz profil", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		Widzety::listaWyboru(painter, QRectF(szerokoscEkranu * 0.3, wysokoscEkranu * 0.18, szerokoscEkranu * 0.4, wysokoscEkranu * 0.5), this->profile, this->pozycja);

		painter.setPen(Qt::white);

		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), QString("%1Stwórz nowy          Wyjdź").arg((this->profile.count())?"Wybierz          Usuń          ":""), QTextOption(Qt::AlignRight));

		if(sterowanie == "gamepad") {
			if((this->profile.count())) {
				Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wybierz          Usuń          Stwórz nowy          Wyjdź").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "2");
				Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Usuń          Stwórz nowy          Wyjdź").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "1");
			}

			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Stwórz nowy          Wyjdź").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "3");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wyjdź").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		} else {
			if((this->profile.count())) {
				Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect(" Wybierz          Usuń          Stwórz nowy          Wyjdź").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Enter");
				Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect(" Usuń          Stwórz nowy          Wyjdź").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Del");
			}

			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect(" Stwórz nowy          Wyjdź").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Spacja");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect(" Wyjdź").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
		}

	} else if(this->tryb == MENU_GLOWNE) {
		painter.drawPixmap(QPoint((szerokoscEkranu - this->logoPixmapa.width()) * 0.5, wysokoscEkranu * 0.05), this->logoPixmapa);

		painter.setFont(czcionkaTytulu);

		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.50, szerokoscEkranu, 100), "Graj", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.56, szerokoscEkranu, 100), "Ustawienia i pomoc", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.62, szerokoscEkranu, 100), "Rekordy", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.68, szerokoscEkranu, 100), "Wyjdź", QTextOption(Qt::AlignHCenter));
	} else if(this->tryb == REKORDY) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Rekordy", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		Widzety::tabelaRekordow(painter, QRectF(szerokoscEkranu * 0.2, wysokoscEkranu * 0.18, szerokoscEkranu * 0.6, wysokoscEkranu * 0.63), this->rekordy, this->pozycja);

		painter.setFont(czcionkaNormalna);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), "Zmień strony          Wróć", QTextOption(Qt::AlignRight));

		if(sterowanie == "gamepad") {
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("    Zmień strony          Wróć").width() - 2 * 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "↑");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Zmień strony          Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "↓");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		} else {
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("    Zmień strony          Wróć").width() - 2 * 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "↑");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Zmień strony          Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "↓");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
		}
	} else if(this->tryb == WYBOR_TRYBU) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Tryb gry:", QTextOption(Qt::AlignHCenter));

		painter.drawPixmap(QPoint(szerokoscEkranu * 0.5 - this->trybyPixmapa[0].width(), wysokoscEkranu * 0.15), this->trybyPixmapa[(this->pozycja == 1)?1:0]);
		painter.drawPixmap(QPoint(szerokoscEkranu * 0.5, wysokoscEkranu * 0.15), this->trybyPixmapa[(this->pozycja == 2)?3:2]);

		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.5 - this->trybyPixmapa[0].width(), wysokoscEkranu * 0.15 + this->trybyPixmapa[1].height(), this->trybyPixmapa[1].width(), 100), "Demolka", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.5, wysokoscEkranu * 0.15 + this->trybyPixmapa[1].height(), this->trybyPixmapa[1].width(), 100), "Labirynt", QTextOption(Qt::AlignHCenter));

		painter.setPen(Qt::white);
		painter.setFont(czcionkaNormalna);

		QString opis;
		if(this->pozycja == 1)
			opis = "Zniszcz wszystkich wrogów w jak najszybszym czasie.";
		else if(this->pozycja == 2)
			opis = "Dojedź do celu zanim zostaniesz zniszczony.";

		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.25 + this->trybyPixmapa[1].height(), szerokoscEkranu, 100), opis, QTextOption(Qt::AlignHCenter));

		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), "Dalej          Wróć", QTextOption(Qt::AlignRight));

		if(sterowanie == "gamepad") {
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Dalej          Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "2");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		} else {
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Dalej          Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Enter");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
		}
	} else if(this->tryb == WYBOR_PLANSZY) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Plansza:", QTextOption(Qt::AlignHCenter));

		QPixmap pixmapy[5];
		if(this->pozycja > 2) {
			pixmapy[0] = this->miniPlansze.value(this->plansze.at(this->pozycja-3).at(2));
			pixmapy[0] = pixmapy[0].scaled(
				Obiekt::skala * pixmapy[0].size() * 0.8,
				Qt::IgnoreAspectRatio,
				(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
			);
		}

		if(this->pozycja > 1) {
			pixmapy[1] = this->miniPlansze.value(this->plansze.at(this->pozycja-2).at(2));
			pixmapy[1] = pixmapy[1].scaled(
				Obiekt::skala * pixmapy[1].size() * 0.9,
				Qt::IgnoreAspectRatio,
				(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
			);
		}

		pixmapy[2] = this->miniPlansze.value(this->plansze.at(this->pozycja-1).at(2));
		pixmapy[2] = pixmapy[2].scaled(
			Obiekt::skala * pixmapy[2].size(),
			Qt::IgnoreAspectRatio,
			(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
		);

		if(this->pozycja < this->plansze.count()) {
			pixmapy[3] = this->miniPlansze.value(this->plansze.at(this->pozycja).at(2));
			pixmapy[3] = pixmapy[3].scaled(
				Obiekt::skala * pixmapy[3].size() * 0.9,
				Qt::IgnoreAspectRatio,
				(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
			);
		}

		if(this->pozycja+1 < this->plansze.count()) {
			pixmapy[4] = this->miniPlansze.value(this->plansze.at(this->pozycja+1).at(2));
			pixmapy[4] = pixmapy[4].scaled(
				Obiekt::skala * pixmapy[4].size() * 0.8,
				Qt::IgnoreAspectRatio,
				(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
			);
		}

		if(!pixmapy[0].isNull()) {
			Widzety::lustrzanyObrazek(painter, QRectF(szerokoscEkranu * 0.5 - pixmapy[0].width() * 2.85, wysokoscEkranu * 0.35 - pixmapy[0].height() * 0.5, pixmapy[0].width(), pixmapy[0].height()), pixmapy[0]);
		}

		if(!pixmapy[1].isNull()) {
			Widzety::lustrzanyObrazek(painter, QRectF(szerokoscEkranu * 0.5 - pixmapy[1].width() * 1.6, wysokoscEkranu * 0.35 - pixmapy[1].height() * 0.5, pixmapy[1].width(), pixmapy[1].height()), pixmapy[1]);
		}

		if(!pixmapy[2].isNull()) {
			Widzety::lustrzanyObrazek(painter, QRectF(szerokoscEkranu * 0.5 - pixmapy[2].width() * 0.5, wysokoscEkranu * 0.35 - pixmapy[2].height() * 0.5, pixmapy[2].width(), pixmapy[2].height()), pixmapy[2]);
		}

		if(!pixmapy[3].isNull()) {
			Widzety::lustrzanyObrazek(painter, QRectF(szerokoscEkranu * 0.5 + pixmapy[3].width() * 0.6, wysokoscEkranu * 0.35 - pixmapy[3].height() * 0.5, pixmapy[3].width(), pixmapy[3].height()), pixmapy[3]);
		}

		if(!pixmapy[4].isNull()) {
			Widzety::lustrzanyObrazek(painter, QRectF(szerokoscEkranu * 0.5 + pixmapy[4].width() * 1.85, wysokoscEkranu * 0.35 - pixmapy[4].height() * 0.5, pixmapy[4].width(), pixmapy[4].height()), pixmapy[4]);
		}

		const int promien = szerokoscEkranu * 0.006;
		const int szerokosc = this->plansze.count() * promien * 3;

		painter.setPen(Qt::NoPen);
		for(int i = 0; i < this->plansze.count(); i++) {
			if(i == this->pozycja-1) {
				painter.setBrush(kolorZaznaczenia);
			} else {
				painter.setBrush(Qt::white);
			}

			painter.drawEllipse(QPointF(szerokoscEkranu * 0.5 - szerokosc * 0.5 + i * (promien * 3), wysokoscEkranu * 0.85), promien, promien);
		}

		painter.setPen(Qt::white);
		painter.setFont(czcionkaNormalna);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), "Graj          Wróć", QTextOption(Qt::AlignRight));

		if(sterowanie == "gamepad") {
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Graj          Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "2");
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		} else {
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Graj          Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Enter");
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
		}
	} else if(this->tryb == USTAWIENIA_POMOC) {
		painter.drawPixmap(QPoint(szerokoscEkranu * 0.5 - this->logoPixmapa.width() * 0.5, wysokoscEkranu * 0.05), this->logoPixmapa);
		painter.setFont(czcionkaTytulu);

		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.50, szerokoscEkranu, 100), "Ustawienia", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.56, szerokoscEkranu, 100), "Pomoc", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.62, szerokoscEkranu, 100), "Autorzy", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.68, szerokoscEkranu, 100), "Wróć", QTextOption(Qt::AlignHCenter));
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
	} else if(this->tryb == AUTORZY) {
		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, obszarTytulu, "Autorzy", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.25, szerokoscEkranu, 100), "Programowanie:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.32, szerokoscEkranu, 100), "Paweł Jarzyński", QTextOption(Qt::AlignHCenter));
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.36, szerokoscEkranu, 100), "Artur Pacholec", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaTytulu);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.45, szerokoscEkranu, 100), "Grafika:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.52, szerokoscEkranu, 100), "Michał Wolski", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaTytulu);
		//Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.61, szerokoscEkranu, 100), "Dodatkowe zadania:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		//Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.68, szerokoscEkranu, 100), "Ewelina Kunikowska", QTextOption(Qt::AlignHCenter));

		Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.90, szerokoscEkranu * 0.9, 100), "Wróć", QTextOption(Qt::AlignRight));

		if(sterowanie == "gamepad")
			Widzety::przyciskGamepada(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 35 * Obiekt::skala, wysokoscEkranu * 0.902, 35 * Obiekt::skala, 35 * Obiekt::skala), "4");
		 else
			Widzety::przyciskKlawiatury(painter, QRectF(szerokoscEkranu * 0.9 - painter.fontMetrics().boundingRect("  Wróć").width() - 70 * Obiekt::skala, wysokoscEkranu * 0.902, 70 * Obiekt::skala, 35 * Obiekt::skala), "Backspace");
	} else if(this->tryb == POMOC) {
		if(this->pozycja == 1) {
			painter.drawPixmap(0, 0, szerokoscEkranu, wysokoscEkranu, QPixmap(QString("pomoc/%1.png").arg((sterowanie == "gamepad")?"pad":"klawiatura")));
		} else if(this->pozycja == 2) {
			painter.drawPixmap(0, 0, szerokoscEkranu, wysokoscEkranu, QPixmap("pomoc/interfejs.png"));
		}
	}

	painter.end();
	this->ekran->update();
}

int Menu::idGracza() const
{
	return this->property("idGracza").toInt();
}

void Menu::zapetlMuzyke()
{
	this->muzyka->enqueue(QString("dzwieki/muzyka/menu.mp3"));
}

void Menu::ustawTryb(Menu::Tryb tryb)
{
	this->tryb = tryb;
	this->setProperty("przezroczystosc", 0);
}

void Menu::wczytajProfile()
{
	this->profile = this->bazaDanych->profile();
	this->pozycja = 1;
}

void Menu::wczytajRekordy()
{
	this->rekordy = this->bazaDanych->rekordy();
	this->pozycja = 1;
}

void Menu::wczytajMuzyke()
{
	this->muzyka = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("dzwieki/muzyka/menu.mp3"));
	this->muzyka->pause();
	this->muzyka->setTransitionTime(-2000);

	Phonon::Path sciezka = this->muzyka->outputPaths().first();
	Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
	wyjscie->setVolume(this->bazaDanych->ustawienie("glosnosc", 5).toInt() / 10.0);

	connect(this->muzyka, SIGNAL(aboutToFinish()), SLOT(zapetlMuzyke()));
}

void Menu::wczytajGrafiki()
{
	QPixmap tlo("grafika/tlo_menu.png");
	this->tloPixmapa = tlo.scaled(
		Obiekt::skala * tlo.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);

	QPixmap logo("grafika/logo.png");
	this->logoPixmapa = logo.scaled(
		Obiekt::skala * logo.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);

	QPixmap demolka("grafika/demolka.png");
	this->trybyPixmapa[0] = demolka.scaled(
		Obiekt::skala * demolka.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);

	QPixmap demolka_zaz("grafika/demolka_zaznaczony.png");
	this->trybyPixmapa[1] = demolka_zaz.scaled(
		Obiekt::skala * demolka_zaz.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);

	QPixmap labirynt("grafika/labirynt.png");
	this->trybyPixmapa[2] = labirynt.scaled(
		Obiekt::skala * labirynt.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);

	QPixmap labirynt_zaz("grafika/labirynt_zaznaczony.png");
	this->trybyPixmapa[3] = labirynt_zaz.scaled(
		Obiekt::skala * labirynt_zaz.size(),
		Qt::IgnoreAspectRatio,
		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
	);
}

void Menu::wczytajMiniPlansze()
{
	QList<QStringList> plansze = this->bazaDanych->plansze();
	float krok = 0.5 / plansze.count();
	float postep = 0.20;
	foreach(const QStringList &plansza, plansze) {
		QString nazwa = plansza.at(2);

		this->miniPlansze.insert(nazwa, QPixmap("plansze/" + nazwa + ".png").scaled(QSize(420, 420), Qt::IgnoreAspectRatio, (this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation));

		postep += krok;
		this->ladowanie->odswiez(postep);
	}
}

void Menu::wczytajPlansze()
{
	this->plansze = this->bazaDanych->plansze(this->property("trybGry").toInt());
}

void Menu::wczytajUstawienia()
{
	this->glosnosc = this->bazaDanych->ustawienie("glosnosc", 5).toInt();
	this->jakosc = this->bazaDanych->ustawienie("jakosc", "niska").toString();
	this->rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "1280x720").toString();
	this->sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();
}

void Menu::zapiszUstawienia()
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
	this->wczytajGrafiki();

	Dzwiek::glosnosc = this->glosnosc;
	this->ekran->ustawJakosc(this->jakosc);
}
