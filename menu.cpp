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
	//	obsluga akcji

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
		}
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

void Menu::
rysuj() const{
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
	QRectF obszarTytulu = QRectF(0, wysokoscEkranu / 12, szerokoscEkranu, 100);

	QTextOption opcjeTekstu(Qt::AlignHCenter);

	QString sterowanie = this->bazaDanych->ustawienie("sterowanie", "gamepad").toString();

	if(this->tryb == TWORZENIE_PROFILU) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Tworzenie profilu", opcjeTekstu);

		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Profil: " + this->nowyProfil + "_", opcjeTekstu);

		painter.setFont(normalna);
		int odstepX = szerokoscEkranu / 30;
		int odstepY = wysokoscEkranu / 15;
		int offsetY = obszarTytulu.y() + (wysokoscEkranu / 2.5);

		int wiersz = 0;
		int kolumna = 0;
		foreach(const QString &litera, this->alfabet) {
			if(this->pozycja - 1 == wiersz * 10 + kolumna) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
			cieniowanyTekst(painter, QRectF((szerokoscEkranu / 2) + (odstepX * (kolumna - 6)), offsetY + odstepY * wiersz, 100, 100), (this->shift)?litera.toUpper():litera);

			if(kolumna < 9) {
				kolumna++;
			} else {
				kolumna = 0;
				wiersz++;
			}
		}

		if(this->pozycja == 37) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF((szerokoscEkranu / 2) + (odstepX * 4), offsetY, 100, 100), "SPACJA");

		if(this->pozycja == 38) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF((szerokoscEkranu / 2) + (odstepX * 4), offsetY + odstepY, 100, 100), "SHIFT");

		if(this->pozycja == 39) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF((szerokoscEkranu / 2) + (odstepX * 4), offsetY + odstepY * 2, 100, 100), "USUŃ");

		if(this->pozycja == 40) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF((szerokoscEkranu / 2) + (odstepX * 4), offsetY + odstepY * 3, 100, 100), "GOTOWE");

	} else if(this->tryb == WYBOR_PROFILU) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Wybierz profil", opcjeTekstu);

		QPoint offset((szerokoscEkranu - this->drzewko->width()) / 2, wysokoscEkranu / 10 + 50);

		this->drzewko->render(&painter, offset);

		painter.setFont(normalna);

		if(this->drzewko->topLevelItemCount()) {
			cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25), szerokoscEkranu, 100), QString("Wybierz [%1]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter"), opcjeTekstu);
			cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25) + 40, szerokoscEkranu, 100), QString("Usuń [%1]").arg((sterowanie == "gamepad")?"Przycisk 1":"Del"), opcjeTekstu);
		}

		cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25) + 80, szerokoscEkranu, 100), QString("Stwórz nowy [%1]").arg((sterowanie == "gamepad")?"Przycisk 3":"Spacja"), opcjeTekstu);
		cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25) + 120, szerokoscEkranu, 100), QString("Wyjdź [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), opcjeTekstu);

	} else if(this->tryb == MENU_GLOWNE) {
		painter.setFont(normalna);

		if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2, 500, 100), "Graj");

		if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 40, 500, 100), "Ustawienia i pomoc");

		if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 80, 500, 100), "Rekordy");

		if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 120, 500, 100), "Wyjdź");
	} else if(this->tryb == REKORDY) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Rekordy", opcjeTekstu);

		QPoint offset((szerokoscEkranu - this->drzewko->width()) / 2, wysokoscEkranu / 10 + 50);
		this->drzewko->render(&painter, offset);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25), szerokoscEkranu, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"), opcjeTekstu);
	} else if(this->tryb == WYBOR_TRYBU) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Tryb gry:", opcjeTekstu);

		QStyleOptionButton opt;
		opt.rect = QRect(szerokoscEkranu / 20, obszarTytulu.y() + wysokoscEkranu / 5, szerokoscEkranu, 100);
		opt.state = ((this->pozycja == 1) ? QStyle::State_On : QStyle::State_None) | QStyle::State_Enabled;
		opt.text = "Demolka";
		qApp->style()->drawControl(QStyle::CE_RadioButton, &opt, &painter);

		opt.rect = QRect(szerokoscEkranu / 20, obszarTytulu.y() + wysokoscEkranu / 5 * 2.5, szerokoscEkranu, 100);
		opt.state = ((this->pozycja == 2) ? QStyle::State_On : QStyle::State_None) | QStyle::State_Enabled;
		opt.text = "Labirynt";
		qApp->style()->drawControl(QStyle::CE_RadioButton, &opt, &painter);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 13, obszarTytulu.y() + wysokoscEkranu / 5 * 1.75, szerokoscEkranu, 100), "Zniszcz wszystkich wrogu w jak najszybszym czasie.");
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 13, obszarTytulu.y() + wysokoscEkranu / 5 * 3.25, szerokoscEkranu, 100), "Dojedź do celu zanim zostaniesz zniszczony.");

		cieniowanyTekst(painter, QRectF(szerokoscEkranu /2, obszarTytulu.y() + wysokoscEkranu / 5 * 4, szerokoscEkranu, 100), QString("Dalej [%1]   Wróć [%2]").arg((sterowanie == "gamepad")?"Przycisk 2":"Enter").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
	} else if(this->tryb == USTAWIENIA_POMOC) {
		painter.setFont(normalna);

		if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2, 500, 100), "Ustawienia");

		if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 40, 500, 100), "Pomoc");

		if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 80, 500, 100), "Autorzy");

		if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 120, 500, 100), "Wróć");
	} else if(this->tryb == USTAWIENIA) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Ustawienia:", opcjeTekstu);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 3 * 2, obszarTytulu.y() + wysokoscEkranu / 5 * 4, szerokoscEkranu, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
	} else if(this->tryb == AUTORZY) {
		painter.setFont(tytul);
		cieniowanyTekst(painter, obszarTytulu, "Autorzy", opcjeTekstu);

		painter.setFont(srednia);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom(), szerokoscEkranu, 100), "Programowanie:", opcjeTekstu);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 50, szerokoscEkranu, 100), "Paweł Jarzyński", opcjeTekstu);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 90, szerokoscEkranu, 100), "Artur Pacholec", opcjeTekstu);

		painter.setFont(srednia);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 140, szerokoscEkranu, 100), "Grafika:", opcjeTekstu);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 180, szerokoscEkranu, 100), "Michał Wolski", opcjeTekstu);

		painter.setFont(srednia);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 230, szerokoscEkranu, 100), "Dodatkowe zadania:", opcjeTekstu);

		painter.setFont(normalna);
		cieniowanyTekst(painter, QRectF(0, obszarTytulu.bottom() + 270, szerokoscEkranu, 100), "Ewelina Kunikowska", opcjeTekstu);

		cieniowanyTekst(painter, QRectF(szerokoscEkranu / 3 * 2, obszarTytulu.y() + wysokoscEkranu / 5 * 4, szerokoscEkranu, 100), QString("Wróć [%1]").arg((sterowanie == "gamepad")?"Przycisk 4":"Backspace"));
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
