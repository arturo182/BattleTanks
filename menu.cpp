#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

#include <QStyleOptionQ3ListView>
#include <QApplication>
#include <QTreeWidget>
#include <QTextOption>
#include <QSqlQuery>
#include <QStyle>

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza):
  drzewko(new QTreeWidget()),
  ekran(ekran),
  bazaDanych(bazaDanych),
  plansza(plansza),
  tryb(WYBOR_PROFILU),
  pozycja(1){

  this->drzewko->setHeaderHidden(true);
  this->drzewko->setIndentation(0);
  this->drzewko->setStyleSheet("background-color: transparent; font-size: 12pt;");
  this->drzewko->resize(this->ekran->buforObrazu.width() / 3, this->ekran->buforObrazu.height() / 2);

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
	this->plansza->zaladuj("planszaTestowa");
	return Silnik::ROZGRYWKA;
  } else if(this->tryb == REKORDY) {
	if(akcja == COFNIJ) {
	  this->pozycja = 1;
	  this->tryb = MENU_GLOWNE;
	}
  } else if(this->tryb == MENU_GLOWNE){
	if(akcja == WYBIERZ) {
	  if(this->pozycja == 1) {
		this->pozycja = 1;
		this->tryb = WYBOR_TRYBU;
	  } else if(this->pozycja == 2) {
		this->tryb = USTAWIENIA;
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
  } else if(this->tryb == WYBOR_PROFILU) {
	if(akcja == WYBIERZ && this->drzewko->topLevelItemCount()){
	  this->idGracza = this->drzewko->currentItem()->text(1).toInt();
	  this->tryb = MENU_GLOWNE;
	} else if(akcja == COFNIJ) {
	  qApp->quit();
	}
  }

  return Silnik::MENU;
}

void Menu::rysuj() const{
  //	rysowanie na ekranie :P
  QPainter painter(&this->ekran->buforObrazu);
  painter.fillRect(this->ekran->buforObrazu.rect(), Qt::red);
  painter.setPen(Qt::white);

  QFont tytul = painter.font();
  tytul.setFamily("Comic Sans MS");
  tytul.setPointSize(24);

  QFont normalna = painter.font();
  normalna.setFamily("Comic Sans MS");
  normalna.setPointSize(16);

  int szerokoscEkranu = this->ekran->buforObrazu.width();
  int wysokoscEkranu = this->ekran->buforObrazu.height();
  QRectF obszarTytulu = QRectF(0, wysokoscEkranu / 12, szerokoscEkranu, 100);

  QTextOption opcjeTekstu(Qt::AlignHCenter);

  if(this->tryb == WYBOR_PROFILU) {
	painter.setFont(tytul);
	painter.drawText(obszarTytulu, "Wybierz profil", opcjeTekstu);

	QPoint offset((szerokoscEkranu - this->drzewko->width()) / 2, wysokoscEkranu / 10 + 50);

	this->drzewko->render(&painter, offset);

	painter.setFont(normalna);

	if(this->drzewko->topLevelItemCount())
	  painter.drawText(QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25), szerokoscEkranu, 100), "Wybierz (Przycisk 2)", opcjeTekstu);

	painter.drawText(QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25) + 40, szerokoscEkranu, 100), "Stwórz nowy (Przycisk 3)", opcjeTekstu);
	painter.drawText(QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25) + 80, szerokoscEkranu, 100), "Wyjdź (Przycisk 4)", opcjeTekstu);

  } else if(this->tryb == MENU_GLOWNE) {
	painter.setFont(normalna);

	if(pozycja == 1) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
	painter.drawText(QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2, 500, 100), "Graj");

	if(pozycja == 2) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
	painter.drawText(QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 40, 500, 100), "Ustawienia");

	if(pozycja == 3) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
	painter.drawText(QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 80, 500, 100), "Rekordy");

	if(pozycja == 4) { painter.setPen(Qt::gray); } else { painter.setPen(Qt::white); }
	painter.drawText(QRectF(szerokoscEkranu / 20, wysokoscEkranu / 2 + 120, 500, 100), "Wyjdź");
  } else if(this->tryb == REKORDY) {
	painter.setFont(tytul);
	painter.drawText(obszarTytulu, "Rekordy", opcjeTekstu);

	QPoint offset((szerokoscEkranu - this->drzewko->width()) / 2, wysokoscEkranu / 10 + 50);
	this->drzewko->render(&painter, offset);

	painter.setFont(normalna);
	painter.drawText(QRectF(0, offset.y() + this->drzewko->height() + (wysokoscEkranu / 25), szerokoscEkranu, 100), "Wróć (Przycisk 4)", opcjeTekstu);
  } else if(this->tryb == WYBOR_TRYBU) {
	painter.setFont(tytul);
	painter.drawText(obszarTytulu, "Tryb gry:", opcjeTekstu);

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
	painter.drawText(QRectF(szerokoscEkranu / 13, obszarTytulu.y() + wysokoscEkranu / 5 * 1.75, szerokoscEkranu, 100), "Zniszcz wszystkich wrogu w jak najszybszym czasie.");
	painter.drawText(QRectF(szerokoscEkranu / 13, obszarTytulu.y() + wysokoscEkranu / 5 * 3.25, szerokoscEkranu, 100), "Dojedź do celu zanim zostaniesz zniszczony.");

	painter.drawText(QRectF(szerokoscEkranu /2, obszarTytulu.y() + wysokoscEkranu / 5 * 4, szerokoscEkranu, 100), "Dalej (Przycisk 2)   Wróć (Przycisk 4)");
  }

  painter.end();

  this->ekran->update();
}
