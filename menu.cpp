#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

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
drzewko(new QTreeWidget()),
ekran(ekran),
bazaDanych(bazaDanych),
plansza(plansza),
tryb(WYBOR_PROFILU),
pozycja(1),
shift(false){

	this->drzewko->setHeaderHidden(true);
	this->drzewko->setIndentation(0);
	this->drzewko->setStyleSheet("background-color: transparent; font-size: 12pt;");
	this->drzewko->resize(this->ekran->buforObrazu.width() / 3, this->ekran->buforObrazu.height() / 2);

	this->tloPixmapa = QPixmap(qApp->applicationDirPath() + "/tlo_menu.png");

	wczytajProfile();

	for(int i = 0; i <= 9; i++)
		this->alfabet.append(QString::number(i));

	for(int i = 'a'; i <= 'z'; i++)
		this->alfabet.append(QChar(i));
}

Silnik::Tryb Menu::odswiez(int milisekundy, Akcja akcja){
	if(tryb == WYBOR_PROFILU || tryb == WYBOR_TRYBU || tryb == WYBOR_MAPY || tryb == REKORDY) {
		if(akcja == GORA) {
			this->zaznaczenie--;

			if(this->zaznaczenie < 0)
				this->zaznaczenie = this->drzewko->topLevelItemCount() - 1;
		} else if(akcja == DOL) {
			this->zaznaczenie++;

			if(this->zaznaczenie >= this->drzewko->topLevelItemCount())
				this->zaznaczenie = 0;
		}

		QTreeWidgetItem *item = this->drzewko->topLevelItem(this->zaznaczenie);
		this->drzewko->scrollToItem(item);
		this->drzewko->setCurrentItem(item);
	}

	if(this->tryb == WYBOR_TRYBU) {
		if(akcja == WYBIERZ) {
			this->tryb = WYBOR_MAPY;
		} else if(akcja == GORA) {
			this->pozycja = 1;
		} else if(akcja == DOL) {
			this->pozycja = 2;
		} else if(akcja == COFNIJ) {
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		}
	} else if(this->tryb == WYBOR_MAPY) {
		this->muzyka->stop();
		this->plansza->zaladuj("planszaTestowa");
		return Silnik::ROZGRYWKA;
	} else if(this->tryb == REKORDY) {
		if(akcja == COFNIJ) {
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		}
	} else if(this->tryb == MENU_GLOWNE) {
		if(akcja == WYBIERZ) {
			if(this->pozycja == 1) {
				this->pozycja = 1;
				this->tryb = WYBOR_TRYBU;
			} else if(this->pozycja == 2) {
				this->pozycja = 1;
				this->tryb = USTAWIENIA_POMOC;
			} else if(this->pozycja == 3) {
				this->drzewko->resize(this->ekran->buforObrazu.width() / 1.5, this->ekran->buforObrazu.height() / 2);
				this->drzewko->setHeaderHidden(false);
				this->drzewko->setHeaderLabels(QStringList() << "Gracz" << "Mapa" << "Wynik");

				this->drzewko->clear();

				QSqlQuery query("SELECT "
				"  rekordy.rekord_id, "
				"  rekordy.wynik, "
				"  profile.nazwa, "
				"  profile.profil_id, "
				"  mapy.mapa_id, "
				"  mapy.tryb, "
				"  mapy.plik "
				"FROM "
				"  rekordy "
				"  INNER JOIN profile ON (rekordy.profil_id = profile.profil_id) "
				"  INNER JOIN mapy ON (rekordy.mapa_id = mapy.mapa_id);");

				while(query.next()) {
					QTreeWidgetItem *item = new QTreeWidgetItem(this->drzewko);
					item->setText(0,  query.value(2).toString());
					item->setText(1,  query.value(6).toString());
					item->setText(2,  query.value(1).toString());
				}

				this->tryb = REKORDY;
			} else if(pozycja == 4) {
				qApp->quit();
			}
		} else if(akcja == GORA) {
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		}
	} else if(this->tryb == USTAWIENIA_POMOC) {
		if(akcja == WYBIERZ) {
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
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == COFNIJ) {
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
		}
	} else if(this->tryb == USTAWIENIA) {
		if(akcja == COFNIJ) {
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		} else if(akcja == WYBIERZ) {
			this->zapiszUstawienia();
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		} else if(akcja == GORA) {
			this->pozycja--;

			if(this->pozycja < 1)
				this->pozycja = 4;
		} else if(akcja == DOL) {
			this->pozycja++;

			if(this->pozycja > 4)
				this->pozycja = 1;
		} else if(akcja == LEWO) {
			if(this->pozycja == 1) {
				if(this->glosnosc > 1)
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
		if(akcja == COFNIJ)
			this->tryb = USTAWIENIA_POMOC;
	} else if(this->tryb == POMOC) {
		if(akcja == WYBIERZ)
			this->pozycja++;

		if(this->pozycja == 3) {
			this->pozycja = 1;
			this->tryb = USTAWIENIA_POMOC;
		}
	} else if(this->tryb == WYBOR_PROFILU) {
		if(akcja == WYBIERZ && this->drzewko->topLevelItemCount()){
			this->idGracza = this->drzewko->currentItem()->text(1).toInt();
			this->pozycja = 1;
			this->tryb = MENU_GLOWNE;
			this->muzyka->play();
		} else if(akcja == WYBIERZ_ALT) {
			this->nowyProfil = "";
			this->tryb = TWORZENIE_PROFILU;
			this->pozycja = 1;
		} else if(akcja == USUN) {
			if(this->drzewko->topLevelItemCount()) {
				int id = this->drzewko->currentItem()->text(1).toInt();

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
			qApp->quit();
		}
	} else if(this->tryb == TWORZENIE_PROFILU) {
		if(akcja == PRAWO) {
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

	QFont tytul = painter.font();
	tytul.setFamily("Trebuchet MS");
	tytul.setPointSize(24);

	QFont normalna = painter.font();
	normalna.setFamily("Trebuchet MS");
	normalna.setPointSize(16);

	QFont srednia = painter.font();
	srednia.setFamily("Trebuchet MS");
	srednia.setPointSize(20);

	int szerokoscEkranu = this->ekran->buforObrazu.width();
	int wysokoscEkranu = this->ekran->buforObrazu.height();
	QRectF obszarTytulu = QRectF(0, wysokoscEkranu * 0.08, szerokoscEkranu, 100);

	QTextOption opcjeTekstu(Qt::AlignHCenter);

	QString sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();

	if(this->tryb == TWORZENIE_PROFILU) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Tworzenie profilu", opcjeTekstu);

		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Profil: " + this->nowyProfil + "_", opcjeTekstu);

		painter.setFont(normalna);
		int odstepX = szerokoscEkranu / 30;
		int odstepY = wysokoscEkranu / 15;
		int offsetY = obszarTytulu.y() + (wysokoscEkranu / 2.5);

		int wiersz = 0;
		int kolumna = 0;
		foreach(const QString &litera, this->alfabet) {
			if(this->pozycja - 1 == wiersz * 10 + kolumna) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
			this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * (kolumna - 6)), offsetY + odstepY * wiersz, 100, 100), (this->shift)?litera.toUpper():litera);

			if(kolumna < 9) {
				kolumna++;
			} else {
				kolumna = 0;
				wiersz++;
			}
		}

		if(this->pozycja == 37) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY, 100, 100), "SPACJA");

		if(this->pozycja == 38) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY, 100, 100), "SHIFT");

		if(this->pozycja == 39) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 2, 100, 100), "USUŃ");

		if(this->pozycja == 40) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF((szerokoscEkranu * 0.5) + (odstepX * 4), offsetY + odstepY * 3, 100, 100), "GOTOWE");

	} else if(this->tryb == WYBOR_PROFILU) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Wybierz profil", opcjeTekstu);

		QPoint offset((szerokoscEkranu - this->drzewko->width()) * 0.5, wysokoscEkranu / 10 + 50);

		this->drzewko->render(&painter, offset);

		painter.setFont(normalna);

		if(this->drzewko->topLevelItemCount()) {
			this->cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu * 0.04), szerokoscEkranu, 100), QString("Wybierz [%1]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter"), opcjeTekstu);
			this->cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu * 0.04) + 40, szerokoscEkranu, 100), QString("Usuń [%1]").arg((sterowanie == "gamepad")?"Przycisk 1":"Del"), opcjeTekstu);
		}

		this->cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu * 0.04) + 80, szerokoscEkranu, 100), QString("Stwórz nowy [%1]").arg((sterowanie == "gamepad")?"Przycisk 3":"Spacja"), opcjeTekstu);
		this->cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu * 0.04) + 120, szerokoscEkranu, 100), QString("Wyjdź [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), opcjeTekstu);

	} else if(this->tryb == MENU_GLOWNE) {
		painter.setFont(normalna);

		if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5, 500, 100), "Graj");

		if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 40, 500, 100), "Ustawienia i pomoc");

		if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 80, 500, 100), "Rekordy");

		if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 120, 500, 100), "Wyjdź");
	} else if(this->tryb == REKORDY) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Rekordy", opcjeTekstu);

		QPoint offset((szerokoscEkranu - this->drzewko->width()) * 0.5, wysokoscEkranu * 0.1 + 50);
		this->drzewko->render(&painter, offset);

		painter.setFont(normalna);
		this->cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu * 0.04), szerokoscEkranu, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), opcjeTekstu);
	} else if(this->tryb == WYBOR_TRYBU) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Tryb gry:", opcjeTekstu);

		QStyleOptionButton opt;
		opt.rect = QRect(szerokoscEkranu * 0.05, obszarTytulu.y() + wysokoscEkranu * 0.02, szerokoscEkranu, 100);
		opt.state = ((this->pozycja == 1) ? QStyle::State_On : QStyle::State_None) | QStyle::State_Enabled;
		opt.text = "Demolka";
		qApp->style()->drawControl(QStyle::CE_RadioButton, &opt, &painter);

		opt.text = "Labirynt";
		qApp->style()->drawControl(QStyle::CE_RadioButton, &opt, &painter);

		painter.setFont(normalna);
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.08, obszarTytulu.y() + wysokoscEkranu / 5 * 1.75, szerokoscEkranu, 100), "Zniszcz wszystkich wrogu w jak najszybszym czasie.");
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.08, obszarTytulu.y() + wysokoscEkranu / 5 * 3.25, szerokoscEkranu, 100), "Dojedź do celu zanim zostaniesz zniszczony.");

		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.5, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu, 100), QString("Dalej [%1]   Wróć [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
	} else if(this->tryb == USTAWIENIA_POMOC) {
		painter.setFont(normalna);

		if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5, 500, 100), "Ustawienia");

		if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 40, 500, 100), "Pomoc");

		if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 80, 500, 100), "Autorzy");

		if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5 + 120, 500, 100), "Wróć");
	} else if(this->tryb == USTAWIENIA) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Ustawienia:", opcjeTekstu);

		painter.setFont(normalna);
		if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
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

		if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.5, 500, 100), "Rozdzielczość");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.5, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->rozdzielczosc, this->rozdzielczosc != "800x600", this->rozdzielczosc != "1920x1080");

		if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.6, 500, 100), "Jakość");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.6, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->jakosc, this->jakosc != "niska", this->jakosc != "wysoka");

		if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.05, wysokoscEkranu * 0.7, 500, 100), "Sterowanie");
		this->przyciskUstawien(painter, QRectF(szerokoscEkranu * 0.7, wysokoscEkranu * 0.7, szerokoscEkranu * 0.19, wysokoscEkranu * 0.06), this->sterowanie, this->sterowanie != "gamepad", this->sterowanie != "klawiatura");

		painter.setPen(Qt::white);
		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.5, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu, 100), QString("Akceptuj [%1]      Anuluj [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
	} else if(this->tryb == AUTORZY) {
		painter.setFont(tytul);
		this->cieniowanyTekst(painter, obszarTytulu, "Autorzy", opcjeTekstu);

		painter.setFont(srednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Programowanie:", opcjeTekstu);

		painter.setFont(normalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 50, szerokoscEkranu, 100), "Paweł Jarzyński", opcjeTekstu);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 90, szerokoscEkranu, 100), "Artur Pacholec", opcjeTekstu);

		painter.setFont(srednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 140, szerokoscEkranu, 100), "Grafika:", opcjeTekstu);

		painter.setFont(normalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 180, szerokoscEkranu, 100), "Michał Wolski", opcjeTekstu);

		painter.setFont(srednia);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 230, szerokoscEkranu, 100), "Dodatkowe zadania:", opcjeTekstu);

		painter.setFont(normalna);
		this->cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 270, szerokoscEkranu, 100), "Ewelina Kunikowska", opcjeTekstu);

		this->cieniowanyTekst(painter, QRectF(szerokoscEkranu * 0.6, obszarTytulu.y() + wysokoscEkranu * 0.8, szerokoscEkranu, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
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

	painter.setPen(Qt::black);

	painter.fillPath(lewyTrojkat, Qt::black);
	if(wstecz)
		painter.fillPath(lewyTrojkat.translated(-margines, -margines), Qt::white);

	painter.fillPath(prawyTrojkat, Qt::black);
	if(dalej)
		painter.fillPath(prawyTrojkat.translated(-margines, -margines), Qt::white);

	painter.setPen(Qt::white);
	this->cieniowanyTekst(painter, r.adjusted(szerokoscTrojkata, 0, -szerokoscTrojkata - margines, 0), text, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

void Menu::wczytajProfile()
{
	this->drzewko->clear();

	QMapIterator<int, QString> it(this->bazaDanych->profile());
	while(it.hasNext()) {
		it.next();

		QTreeWidgetItem *item = new QTreeWidgetItem(this->drzewko);
		item->setText(0, it.value());
		item->setText(1, QString::number(it.key()));
	}

	if(this->drzewko->topLevelItemCount()) {
		this->drzewko->setCurrentItem(this->drzewko->topLevelItem(0));
		this->zaznaczenie = 0;
	}
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
	this->bazaDanych->zapiszUstawienie("glosnosc", this->glosnosc);
	this->bazaDanych->zapiszUstawienie("jakosc", this->jakosc);
	this->bazaDanych->zapiszUstawienie("rozdzielczosc", this->rozdzielczosc);
	this->bazaDanych->zapiszUstawienie("sterowanie", this->sterowanie);
}
