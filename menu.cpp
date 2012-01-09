#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"
#include "dzwiek.h"

#include <QStyleOptionQ3ListView>
#include <Phonon/MediaObject>
#include <Phonon/VideoWidget>
#include <Phonon/AudioOutput>
#include <QApplication>
#include <QTreeWidget>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QStyle>

QString qSizeToString(const QSize &size)
{
	return QString("%1x%2").arg(size.width()).arg(size.height());
}

QSize qStringToSize(const QString &str)
{
	int w = 0;
	int h = 0;

	QStringList lista = str.split("x");
	if(lista.size() == 2) {
		w = lista.at(0).toInt();
		h = lista.at(1).toInt();
	}

	return QSize(w, h);
}

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza):

ekran(ekran),
bazaDanych(bazaDanych),
plansza(plansza),
tryb(WYBOR_PROFILU),
pozycja(1),
shift(false){
	this->wczytajGrafiki();
	this->wczytajProfile();

	for(int i = 0; i <= 9; i++)
		this->alfabet.append(QString::number(i));

	for(int i = 'a'; i <= 'z'; i++)
		this->alfabet.append(QChar(i));
}

Silnik::Tryb Menu::odswiez(int milisekundy, Akcja akcja){
	if(this->tryb == WYBOR_TRYBU) {
		if(akcja == WYBIERZ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybierz.mp3");
			this->trybGry = this->pozycja;
			this->wczytajMapy();
			this->tryb = WYBOR_MAPY;
		} else if(akcja == LEWO) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja = 1;
		} else if(akcja == PRAWO) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja = 2;
		} else if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybierz.mp3");
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		}
	} else if(this->tryb == WYBOR_MAPY) {
		if(akcja == WYBIERZ) {
			this->muzyka->stop();
			this->plansza->zaladuj("planszaTestowa");
			this->tryb = MENU_GLOWNE;
			this->pozycja = 1;
			return Silnik::ROZGRYWKA;
		} else if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybierz.mp3");
			this->pozycja = 1;
			this->tryb = WYBOR_TRYBU;
		} else if(akcja == LEWO) {
			if(this->pozycja > 1)
				this->pozycja--;
		} else if(akcja == PRAWO) {
			if(this->pozycja < this->mapy.count())
				this->pozycja++;
		}
	} else if(this->tryb == REKORDY) {
		if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		} else if(akcja == GORA) {
			if(this->pozycja > 1)
				this->pozycja--;
		} else if(akcja == DOL) {
			const int na_strone = 16;
			if(this->pozycja < (this->rekordy.count() / na_strone) + 1)
				this->pozycja++;
		}
	} else if(this->tryb == MENU_GLOWNE) {
		if(akcja == WYBIERZ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");

			if(this->pozycja == 1) {
				this->pozycja = 1;
				this->tryb = WYBOR_TRYBU;
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->tryb = USTAWIENIA_POMOC;
			} else if(this->pozycja == 3) {
				this->wczytajRekordy();
				this->tryb = REKORDY;
			} else if(pozycja == 4) {
				return Silnik::WYJSCIE;
			}
		} else if(akcja == GORA) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		}
	} else if(this->tryb == USTAWIENIA_POMOC) {
		if(akcja == WYBIERZ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");

			if(this->pozycja == 1) {
				this->pozycja = 1;
				this->wczytajUstawienia();
				this->tryb = USTAWIENIA;
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->tryb = POMOC;
			} else if(this->pozycja == 3) {
				this->pozycja = 1;
				this->tryb = AUTORZY;
			} else if(this->pozycja == 4) {
				this->pozycja = 1;
				this->tryb = MENU_GLOWNE;
			}
		} else if(akcja == GORA) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		}
	} else if(this->tryb == USTAWIENIA) {
		if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->wczytajUstawienia();
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		} else if(akcja == WYBIERZ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->zapiszUstawienia();
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		} else if(akcja == GORA) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			Dzwiek::odtworz("dane/dzwieki/menu_zmiana.mp3");
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == LEWO) {
			if(this->pozycja == 1) {
				if(this->glosnosc > 0)
					this->glosnosc -= 1;
			} else if(this->pozycja == 2) {
				if(this->rozdzielczosc == "1920x1080")
					this->rozdzielczosc = "1024x768";
				else if(this->rozdzielczosc == "1024x768")
					this->rozdzielczosc = "800x600";
			} else if(this->pozycja == 3) {
				if(this->jakosc == "wysoka")
					this->jakosc = "niska";
			} else if(this->pozycja == 4) {
				if(this->sterowanie == "klawiatura")
					this->sterowanie = "gamepad";
			}
		} else if(akcja == PRAWO) {
			if(this->pozycja == 1) {
				if(this->glosnosc < 10)
					this->glosnosc += 1;
			} else if(this->pozycja == 2) {
				if(this->rozdzielczosc == "800x600")
					this->rozdzielczosc = "1024x768";
				else if(this->rozdzielczosc == "1024x768")
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
		Phonon::Path sciezka = this->muzyka->outputPaths().first();
		Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
		wyjscie->setVolume(this->glosnosc / 10.0);
	} else if(this->tryb == AUTORZY) {
		if(akcja == COFNIJ) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->tryb = USTAWIENIA_POMOC;
		}
	} else if(this->tryb == POMOC) {
		if(akcja == WYBIERZ)
			this->pozycja++;

		if(this->pozycja == 3) {
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		}
	} else if(this->tryb == WYBOR_PROFILU) {
		if(akcja == WYBIERZ && this->profile.count()){
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");

			this->idGracza = this->bazaDanych->idProfilu(this->profile.at(this->pozycja-1));
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
			this->muzyka->play();
		} else if(akcja == WYBIERZ_ALT) {
			Dzwiek::odtworz("dane/dzwieki/menu_wybor.mp3");
			this->nowyProfil = "";
			this->tryb = TWORZENIE_PROFILU;
			this->pozycja = 1;
		} else if(akcja == USUN) {
			if(this->profile.count()) {
				int id = this->bazaDanych->idProfilu(this->profile.at(this->pozycja-1));

				QSqlQuery query;
				query.prepare("DELETE FROM profile WHERE profil_id = :id;");
				query.bindValue(":id", id);
				query.exec();

				query.prepare("DELETE FROM rekordy WHERE profil_id = :id;");
				query.bindValue(":id", id);
				query.exec();

				wczytajProfile();
			}
		} else if(akcja == COFNIJ) {
			return Silnik::WYJSCIE;
		} else if(akcja == GORA) {
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = this->profile.count();
		} else if(akcja == DOL) {
			this->pozycja++;

			if(this->pozycja > this->profile.count())
				this->pozycja = 1;
		}
	} else if(this->tryb == TWORZENIE_PROFILU) {
		if(akcja == PRAWO) {
			Dzwiek::odtworz("dane/dzwieki/menu_litera.mp3");

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
		} else if(akcja == LEWO) {
			Dzwiek::odtworz("dane/dzwieki/menu_litera.mp3");

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
		} else if(akcja == DOL) {
			Dzwiek::odtworz("dane/dzwieki/menu_litera.mp3");

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
		} else if(akcja == GORA) {
			Dzwiek::odtworz("dane/dzwieki/menu_litera.mp3");

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
		} else if(akcja == WYBIERZ) {
			Dzwiek::odtworz("dane/dzwieki/menu_karetka.mp3");

			if(this->pozycja <= this->alfabet.count()) {
				if(this->nowyProfil.length() < 80) {
					QString litera = this->alfabet[this->pozycja-1];

					if(this->shift)
						litera = litera.toUpper();

					this->nowyProfil.append(litera);
				}
			} else if(this->pozycja == 37)
				this->nowyProfil.append(" ");
			else if(this->pozycja == 38)
				this->shift = !this->shift;
			else if(this->pozycja == 39)
				this->nowyProfil.chop(1);
			else if(this->pozycja == 40) {
				if(this->nowyProfil.trimmed().length()) {
					QSqlQuery query;
					query.prepare("INSERT INTO profile(nazwa) VALUES (:nazwa);");
					query.bindValue(":nazwa", this->nowyProfil);
					query.exec();

					wczytajProfile();
				}

				Dzwiek::odtworz("dane/dzwieki/menu_wybierz.mp3");
				this->tryb = WYBOR_PROFILU;
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
		this->cieniowanyTekst(painter, obszarTytulu, "Tworzenie profilu", QTextOption(Qt::AlignHCenter));

		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Profil: " + this->nowyProfil + "_", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		int odstepX = szerokoscEkranu * 0.03;
		int odstepY = wysokoscEkranu * 0.06;
		int offsetY = obszarTytulu.y() + (wysokoscEkranu * 0.4);

		int wiersz = 0;
		int kolumna = 0;
		foreach(const QString &litera, this->alfabet) {
			if(this->pozycja - 1 == wiersz * 10 + kolumna) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
			this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * (kolumna - 6)), offsetY + odstepY * wiersz, 100, 100), (this->shift)?litera.toUpper():litera);

			if(kolumna < 9) {
				kolumna++;
			} else {
				kolumna = 0;
				wiersz++;
			}
		}

		if(this->pozycja == 37) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY, 300, 100), "SPACJA");

		if(this->pozycja == 38) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY, 300, 100), "SHIFT");

		if(this->pozycja == 39) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 2, 300, 100), "USUŃ");

		if(this->pozycja == 40) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 3, 300, 100), "GOTOWE");
	} else if(this->tryb == WYBOR_PROFILU) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Wybierz profil", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		this->listaWyboru(painter, QRectF(szerokoscEkranu * 0.3, wysokoscEkranu * 0.18, szerokoscEkranu * 0.4, wysokoscEkranu * 0.5), this->profile, this->pozycja);

		painter.setPen(Qt::white);

		if(this->profile.count()) {
			this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.73, szerokoscEkranu, 100), QString("Wybierz [%1]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter"), QTextOption(Qt::AlignHCenter));
			this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.78, szerokoscEkranu, 100), QString("Usuń [%1]").arg((sterowanie == "gamepad")?"Przycisk 1":"Del"), QTextOption(Qt::AlignHCenter));
		}

		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.83, szerokoscEkranu, 100), QString("Stwórz nowy [%1]").arg((sterowanie == "gamepad")?"Przycisk 3":"Spacja"), QTextOption(Qt::AlignHCenter));
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.88, szerokoscEkranu, 100), QString("Wyjdź [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignHCenter));

	} else if(this->tryb == MENU_GLOWNE) {
		painter.drawPixmap(QPoint(szerokoscEkranu * 0.5 - this->logoPixmapa.width() * 0.5, wysokoscEkranu * 0.05), this->logoPixmapa);

		painter.setFont(czcionkaTytulu);

		if(pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.50, szerokoscEkranu, 100), "Graj", QTextOption(Qt::AlignHCenter));

		if(pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.56, szerokoscEkranu, 100), "Ustawienia i pomoc", QTextOption(Qt::AlignHCenter));

		if(pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.62, szerokoscEkranu, 100), "Rekordy", QTextOption(Qt::AlignHCenter));

		if(pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.68, szerokoscEkranu, 100), "Wyjdź", QTextOption(Qt::AlignHCenter));
	} else if(this->tryb == REKORDY) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Rekordy", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		this->tabelaRekordow(painter, QRectF(szerokoscEkranu * 0.2, wysokoscEkranu * 0.18, szerokoscEkranu * 0.6, wysokoscEkranu * 0.63), this->rekordy, this->pozycja);

		painter.setFont(czcionkaNormalna);
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.85, szerokoscEkranu * 0.9, 100), QString("Zmień strony [Góra/Dół]   Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignRight));
	} else if(this->tryb == WYBOR_TRYBU) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Tryb gry:", QTextOption(Qt::AlignHCenter));

		painter.drawPixmap(QPoint(szerokoscEkranu * 0.15, wysokoscEkranu * 0.15), this->trybyPixmapa[(this->pozycja == 1)?1:0]);
		painter.drawPixmap(QPoint(szerokoscEkranu * 0.55, wysokoscEkranu * 0.15), this->trybyPixmapa[(this->pozycja == 2)?3:2]);

		if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.15, wysokoscEkranu * 0.15 + this->trybyPixmapa[1].height(), this->trybyPixmapa[1].width(), 100), "Demolka", QTextOption(Qt::AlignHCenter));

		if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.55, wysokoscEkranu * 0.15 + this->trybyPixmapa[1].height(), this->trybyPixmapa[1].width(), 100), "Labirynt", QTextOption(Qt::AlignHCenter));

		painter.setPen(Qt::white);
		painter.setFont(czcionkaNormalna);

		QString opis;
		if(this->pozycja == 1)
			opis = "Zniszcz wszystkich wrogów w jak najszybszym czasie.";
		else if(this->pozycja == 2)
			opis = "Dojedź do celu zanim zostaniesz zniszczony.";

		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.25 + this->trybyPixmapa[1].height(), szerokoscEkranu, 100), opis, QTextOption(Qt::AlignHCenter));

		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu * 0.9, 100), QString("Dalej [%1]   Wróć [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignRight));
	} else if(this->tryb == WYBOR_MAPY) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Mapa:", QTextOption(Qt::AlignHCenter));

		QPixmap pixmapy[3];
		if(this->pozycja > 1) {
			pixmapy[0].load(QString("dane/plansze/%1_mini.png").arg(this->mapy.at(this->pozycja-2).at(2)));
			pixmapy[0] = pixmapy[0].scaled(
				Obiekt::skala * pixmapy[0].size(),
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation
			);
		}

		pixmapy[1] = QPixmap(QString("dane/plansze/%1_mini.png").arg(this->mapy.at(this->pozycja-1).at(2)));
		pixmapy[1] = pixmapy[1].scaled(
			Obiekt::skala * pixmapy[1].size(),
			Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation
		);

		if(this->pozycja < this->mapy.count()) {
			pixmapy[2].load(QString("dane/plansze/%1_mini.png").arg(this->mapy.at(this->pozycja).at(2)));
			pixmapy[2] = pixmapy[2].scaled(
				Obiekt::skala * pixmapy[2].size(),
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation
			);
		}

		if(!pixmapy[0].isNull()) {
			painter.drawPixmap(QPointF(szerokoscEkranu * 0.5 - pixmapy[0].width() * 1.6, wysokoscEkranu * 0.5 - pixmapy[0].height() * 0.5), pixmapy[0]);
		}

		if(!pixmapy[1].isNull()) {
			painter.drawPixmap(QPointF(szerokoscEkranu * 0.5 - pixmapy[1].width() * 0.5, wysokoscEkranu * 0.5 - pixmapy[1].height() * 0.5), pixmapy[1]);
		}

		if(!pixmapy[2].isNull()) {
			painter.drawPixmap(QPointF(szerokoscEkranu * 0.5 + pixmapy[2].width() * 0.6, wysokoscEkranu * 0.5 - pixmapy[2].height() * 0.5), pixmapy[2]);
		}

	} else if(this->tryb == USTAWIENIA_POMOC) {
		painter.drawPixmap(QPoint(szerokoscEkranu * 0.5 - this->logoPixmapa.width() * 0.5, wysokoscEkranu * 0.05), this->logoPixmapa);
		painter.setFont(czcionkaTytulu);

		if(pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.50, szerokoscEkranu, 100), "Ustawienia", QTextOption(Qt::AlignHCenter));

		if(pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.56, szerokoscEkranu, 100), "Pomoc", QTextOption(Qt::AlignHCenter));

		if(pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.62, szerokoscEkranu, 100), "Autorzy", QTextOption(Qt::AlignHCenter));

		if(pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.68, szerokoscEkranu, 100), "Wróć", QTextOption(Qt::AlignHCenter));
	} else if(this->tryb == USTAWIENIA) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Ustawienia:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		if(pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.4, 500, 100), "Głośność");

		//rysowanie glosnosci
		painter.setPen(Qt::black);

		float gruboscPaska = szerokoscEkranu * 0.01;
		float maxWysokosc = wysokoscEkranu * 0.06;
		float margines = gruboscPaska * 0.3;

		//czarne paski
		painter.fillRect(QRectF(szerokoscEkranu * 0.70, wysokoscEkranu * 0.4 + maxWysokosc * 0.9, gruboscPaska, maxWysokosc * 0.1), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.72, wysokoscEkranu * 0.4 + maxWysokosc * 0.8, gruboscPaska, maxWysokosc * 0.2), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.74, wysokoscEkranu * 0.4 + maxWysokosc * 0.7, gruboscPaska, maxWysokosc * 0.3), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.76, wysokoscEkranu * 0.4 + maxWysokosc * 0.6, gruboscPaska, maxWysokosc * 0.4), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.78, wysokoscEkranu * 0.4 + maxWysokosc * 0.5, gruboscPaska, maxWysokosc * 0.5), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.80, wysokoscEkranu * 0.4 + maxWysokosc * 0.4, gruboscPaska, maxWysokosc * 0.6), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.82, wysokoscEkranu * 0.4 + maxWysokosc * 0.3, gruboscPaska, maxWysokosc * 0.7), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.84, wysokoscEkranu * 0.4 + maxWysokosc * 0.2, gruboscPaska, maxWysokosc * 0.8), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.86, wysokoscEkranu * 0.4 + maxWysokosc * 0.1, gruboscPaska, maxWysokosc * 0.9), Qt::black);
		painter.fillRect(QRectF(szerokoscEkranu * 0.88, wysokoscEkranu * 0.4, gruboscPaska, maxWysokosc), Qt::black);

		//biale paski

		if(this->glosnosc >= 1)
			painter.fillRect(QRectF(szerokoscEkranu * 0.70 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.9 - margines, gruboscPaska, maxWysokosc * 0.1), Qt::white);

		if(this->glosnosc >= 2)
			painter.fillRect(QRectF(szerokoscEkranu * 0.72 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.8 - margines, gruboscPaska, maxWysokosc * 0.2), Qt::white);

		if(this->glosnosc >= 3)
			painter.fillRect(QRectF(szerokoscEkranu * 0.74 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.7 - margines, gruboscPaska, maxWysokosc * 0.3), Qt::white);

		if(this->glosnosc >= 4)
			painter.fillRect(QRectF(szerokoscEkranu * 0.76 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.6 - margines, gruboscPaska, maxWysokosc * 0.4), Qt::white);

		if(this->glosnosc >= 5)
			painter.fillRect(QRectF(szerokoscEkranu * 0.78 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.5 - margines, gruboscPaska, maxWysokosc * 0.5), Qt::white);

		if(this->glosnosc >= 6)
			painter.fillRect(QRectF(szerokoscEkranu * 0.80 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.4 - margines, gruboscPaska, maxWysokosc * 0.6), Qt::white);

		if(this->glosnosc >= 7)
			painter.fillRect(QRectF(szerokoscEkranu * 0.82 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.3 - margines, gruboscPaska, maxWysokosc * 0.7), Qt::white);

		if(this->glosnosc >= 8)
			painter.fillRect(QRectF(szerokoscEkranu * 0.84 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.2 - margines, gruboscPaska, maxWysokosc * 0.8), Qt::white);

		if(this->glosnosc >= 9)
			painter.fillRect(QRectF(szerokoscEkranu * 0.86 - margines, wysokoscEkranu * 0.4 + maxWysokosc * 0.1 - margines, gruboscPaska, maxWysokosc * 0.9), Qt::white);

		if(this->glosnosc >= 10)
			painter.fillRect(QRectF(szerokoscEkranu * 0.88 - margines, wysokoscEkranu * 0.4 - margines, gruboscPaska, maxWysokosc), Qt::white);

		if(pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5, 500, 100), "Rozdzielczość");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.5, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->rozdzielczosc, this->rozdzielczosc != "800x600", this->rozdzielczosc != "1920x1080");

		if(pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.6, 500, 100), "Jakość");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.6, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->jakosc, this->jakosc != "niska", this->jakosc != "wysoka");

		if(pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.7, 500, 100), "Sterowanie");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.7, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->sterowanie, this->sterowanie != "gamepad", this->sterowanie != "klawiatura");

		painter.setPen(Qt::white);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu * 0.9, 100), QString("Akceptuj [%1]      Anuluj [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignRight));
	} else if(this->tryb == AUTORZY) {
		painter.setFont(czcionkaTytulu);
		this->cieniowanyTekst(painter, obszarTytulu, "Autorzy", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaSrednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.15, szerokoscEkranu, 100), "Programowanie:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.20, szerokoscEkranu, 100), "Paweł Jarzyński", QTextOption(Qt::AlignHCenter));
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.24, szerokoscEkranu, 100), "Artur Pacholec", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaSrednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.30, szerokoscEkranu, 100), "Grafika:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.35, szerokoscEkranu, 100), "Michał Wolski", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaSrednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.41, szerokoscEkranu, 100), "Dodatkowe zadania:", QTextOption(Qt::AlignHCenter));

		painter.setFont(czcionkaNormalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + wysokoscEkranu * 0.47, szerokoscEkranu, 100), "Ewelina Kunikowska", QTextOption(Qt::AlignHCenter));

		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu * 0.9, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), QTextOption(Qt::AlignRight));
	} else if(this->tryb == POMOC) {
		if(this->pozycja == 1) {
			painter.drawPixmap(0, 0, szerokoscEkranu, wysokoscEkranu, QPixmap(qApp->applicationDirPath() + QString("/dane/pomoc/%1.png").arg((sterowanie == "gamepad")?"pad":"klawiatura")));
		} else if(this->pozycja == 2) {

		}
	}

	painter.end();

	this->ekran->update();
}

void Menu::ladujMuzyke()
{
	this->muzyka = Phonon::createPlayer(Phonon::MusicCategory, qApp->applicationDirPath() + "/dane/muzyka/menu.mp3");
	this->muzyka->setTransitionTime(-2000);

	Phonon::Path sciezka = this->muzyka->outputPaths().first();
	Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
	wyjscie->setVolume(this->bazaDanych->ustawienie("glosnosc", 5).toInt() / 10.0);

	connect(this->muzyka, SIGNAL(aboutToFinish()), SLOT(zapetlMuzyke()));
}

void Menu::zapetlMuzyke()
{
	this->muzyka->enqueue(qApp->applicationDirPath() + "/dane/muzyka/menu.mp3");
}

void Menu::cieniowanyTekst(QPainter &painter, const QRectF &r, const QString &text, const QTextOption &o) const
{
	QPen pen = painter.pen();

	int margines = painter.fontMetrics().height() / 10;

	painter.setPen(Qt::black);
	painter.drawText(r.adjusted(margines, margines, 0, 0), text, o);

	painter.setPen(pen);
	painter.drawText(r, text, o);
}

void Menu::przyciskUstawien(QPainter &painter, const QRectF &r, const QString &text, bool wstecz, bool dalej) const
{
	float wysokoscTrojkata = r.height() * 0.5;
	float szerokoscTrojkata = wysokoscTrojkata * 0.5;
	float margines = szerokoscTrojkata * 0.25;

	QPainterPath lewyTrojkat;
	lewyTrojkat.moveTo(0, wysokoscTrojkata / 2);
	lewyTrojkat.lineTo(szerokoscTrojkata, wysokoscTrojkata);
	lewyTrojkat.lineTo(szerokoscTrojkata, 0);
	lewyTrojkat.lineTo(0, wysokoscTrojkata / 2);
	lewyTrojkat.translate(r.left(), r.top() + (r.height() / 2 - wysokoscTrojkata / 2));

	QPainterPath prawyTrojkat;
	prawyTrojkat.lineTo(0, wysokoscTrojkata);
	prawyTrojkat.lineTo(szerokoscTrojkata, wysokoscTrojkata / 2);
	prawyTrojkat.lineTo(0, 0);

	prawyTrojkat.translate(r.right() - szerokoscTrojkata, r.top() + (r.height() / 2 - wysokoscTrojkata / 2));

	painter.fillPath(lewyTrojkat, Qt::black);
	if(wstecz)
		painter.fillPath(lewyTrojkat.translated(-margines, -margines), Qt::white);

	painter.fillPath(prawyTrojkat, Qt::black);
	if(dalej)
		painter.fillPath(prawyTrojkat.translated(-margines, -margines), Qt::white);

	painter.setPen(Qt::white);
	this->cieniowanyTekst(painter, r.adjusted(szerokoscTrojkata, 0, -szerokoscTrojkata - margines, 0), text, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

void Menu::listaWyboru(QPainter &painter, const QRectF &r, const QStringList &elementy, int zaznaczenie) const
{
	painter.setPen(QPen(Qt::white, 3.0));
	painter.drawRect(r);

	int wysokosc = painter.fontMetrics().height();
	for(int i = 0; i < qMin(elementy.size(), (int)r.height() / wysokosc); i++) {
		QRectF r_txt(r.left() + 10, r.top() + i * wysokosc, r.width() - 20, wysokosc);
		if(zaznaczenie == i+1) {
			painter.fillRect(QRectF(r.left() + 1.5, r.top() + i * wysokosc, r.width() - 3, wysokosc), Qt::white);
			painter.setPen(Qt::black);
			painter.drawText(r_txt, elementy.at(i));
		} else {
			painter.setPen(Qt::white);
			this->cieniowanyTekst(painter, r_txt, elementy.at(i));
		}
	}
}

void Menu::tabelaRekordow(QPainter &painter, const QRectF &r, const QList<QStringList> &rekordy, int strona) const
{
	painter.setPen(QPen(Qt::white, 3.0));
	painter.drawRect(r);

	int wysokosc = painter.fontMetrics().height();
	const int na_strone = 16;

	QRectF kolumnaGracza(r.left(), r.top(), r.width() * 0.4, wysokosc);
	painter.drawRect(kolumnaGracza);
	kolumnaGracza.adjust(10, 0, 0, 0);
	this->cieniowanyTekst(painter, kolumnaGracza, "Gracz");

	QRectF kolumnaMapy(r.left() + r.width() * 0.4, r.top(), r.width() * 0.4, wysokosc);
	painter.drawRect(kolumnaMapy);
	kolumnaMapy.adjust(10, 0, 0, 0);
	this->cieniowanyTekst(painter, kolumnaMapy, "Mapa");

	QRectF kolumnaWyniku(r.left() + r.width() * 0.8, r.top(), r.width() * 0.2, wysokosc);
	painter.drawRect(kolumnaWyniku);
	kolumnaWyniku.adjust(10, 0, 0, 0);
	this->cieniowanyTekst(painter, kolumnaWyniku, "Wynik");

	for(int i = na_strone * (strona - 1); i < qMin(rekordy.size(), na_strone * strona); i++) {
		kolumnaGracza.adjust(0, wysokosc, 0, wysokosc);
		kolumnaMapy.adjust(0, wysokosc, 0, wysokosc);
		kolumnaWyniku.adjust(0, wysokosc, 0, wysokosc);

		this->cieniowanyTekst(painter, kolumnaGracza, rekordy.at(i).at(0));
		this->cieniowanyTekst(painter, kolumnaMapy, rekordy.at(i).at(1));
		this->cieniowanyTekst(painter, kolumnaWyniku, rekordy.at(i).at(2));
	}
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

void Menu::wczytajGrafiki()
{
	QPixmap tlo("dane/grafika/tlo_menu.png");
	this->tloPixmapa = tlo.scaled(
		Obiekt::skala * tlo.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);

	QPixmap logo("dane/grafika/logo.png");
	this->logoPixmapa = logo.scaled(
		Obiekt::skala * logo.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);

	QPixmap demolka("dane/grafika/demolka.png");
	this->trybyPixmapa[0] = demolka.scaled(
		Obiekt::skala * demolka.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);

	QPixmap demolka_zaz("dane/grafika/demolka_zaznaczony.png");
	this->trybyPixmapa[1] = demolka_zaz.scaled(
		Obiekt::skala * demolka_zaz.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);

	QPixmap labirynt("dane/grafika/labirynt.png");
	this->trybyPixmapa[2] = labirynt.scaled(
		Obiekt::skala * labirynt.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);

	QPixmap labirynt_zaz("dane/grafika/labirynt_zaznaczony.png");
	this->trybyPixmapa[3] = labirynt_zaz.scaled(
		Obiekt::skala * labirynt_zaz.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);
}

void Menu::wczytajMapy()
{
	this->mapy = this->bazaDanych->mapy(this->trybGry);
}

void Menu::wczytajUstawienia()
{
	this->glosnosc = this->bazaDanych->ustawienie("glosnosc", 5).toInt();
	this->jakosc = this->bazaDanych->ustawienie("jakosc", "niska").toString();
	this->rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "800x600").toString();
	this->sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();
}

void Menu::zapiszUstawienia()
{
	//transakcja!
	QSqlQuery("BEGIN;");
	this->bazaDanych->zapiszUstawienie("glosnosc", this->glosnosc);
	this->bazaDanych->zapiszUstawienie("jakosc", this->jakosc);
	this->bazaDanych->zapiszUstawienie("rozdzielczosc", this->rozdzielczosc);
	this->bazaDanych->zapiszUstawienie("sterowanie", this->sterowanie);
	QSqlQuery("END;");

	Dzwiek::glosnosc = this->glosnosc;
	this->ekran->ustawJakosc(this->jakosc);
}
