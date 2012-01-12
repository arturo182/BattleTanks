#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "przeszkoda.h"
#include "waypoint.h"
#include "sciezka.h"
#include "scena.h"

#include <QGraphicsPolygonItem>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <qmath.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);
	this->scena = new Scena();
	this->ui->graphicsView->setScene(this->scena);
	this->plikPlanszy = "";
	this->ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	this->ustawTryb(Scena::ZAZNACZANIE);

	connect(this->scena, SIGNAL(selectionChanged()), SLOT(zaznaczGalaz()));
	connect(this->scena, SIGNAL(trybZmieniony()), SLOT(aktualizujTryb()));
	connect(this->scena, SIGNAL(przeszkodaDodana()), SLOT(aktualizujDrzewkoPrzeszkod()));
	connect(this->scena, SIGNAL(waypointDodany()), SLOT(aktualizujDrzewkoWaypointow()));
	connect(this->scena, SIGNAL(sciezkaDodana()), SLOT(scenaZmieniona()));
	connect(this->scena, SIGNAL(elementPrzesuniety()), SLOT(scenaZmieniona()));
	connect(this->scena, SIGNAL(zoomOddalony()), SLOT(oddalZoom()));
	connect(this->scena, SIGNAL(zoomPrzyblizony()), SLOT(przyblizZoom()));
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(sprawdzZapis())
		event->accept();
	else
		event->ignore();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Space) && !event->isAutoRepeat()) {
		this->setProperty("poprzedniTryb", this->scena->tryb());
		this->ustawTryb(Scena::PRZESUWANIE_WIDOKU);
	}

	QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Space) && !event->isAutoRepeat())
		this->ustawTryb((Scena::Tryb)this->property("poprzedniTryb").toInt());

	QMainWindow::keyReleaseEvent(event);
}

void MainWindow::nowaPlansza()
{
	if(this->sprawdzZapis()) {
		qDeleteAll(this->ui->treeWidget->topLevelItem(0)->takeChildren());
		qDeleteAll(this->ui->treeWidget2->topLevelItem(0)->takeChildren());

		this->scena->przeszkody = 0;
		this->scena->waypointy = 0;

		this->scena->clear();
		this->tabelaElementow.clear();
		this->plikPlanszy = "";
		this->setWindowModified(false);
	}
}

void MainWindow::wczytajPlansze()
{
	this->sprawdzZapis();
	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wczytaj plansze", "", "Plansze (*.dat)");
	if(!nazwaPliku.isEmpty()) {
		this->nowaPlansza();

		QFileInfo plik(nazwaPliku);

		this->scena->addPixmap(QPixmap(plik.absolutePath() + "/" + plik.completeBaseName() + ".png"));

		QFile mapaSpecyfikacjaPlik(nazwaPliku);
		if(!mapaSpecyfikacjaPlik.open(QIODevice::ReadOnly))
			return;

		int iloscElementow, iloscWierzcholkow;
		QPoint wierzcholek;

		QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);

		//przeszkody
		mapaSpecyfikacjaDane >> iloscElementow;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> iloscWierzcholkow;

			QPolygon przeszkoda(iloscWierzcholkow);
			for(int j = 0; j < iloscWierzcholkow; j++){
				mapaSpecyfikacjaDane >> wierzcholek;
				przeszkoda.setPoint(j, wierzcholek);
			}

			this->scena->dodajPrzeszkode(przeszkoda);
		}

		QList<Waypoint*> waypointy;

		//waypointy
		mapaSpecyfikacjaDane >> iloscElementow;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> wierzcholek;

			Waypoint *waypoint = this->scena->dodajWaypoint(wierzcholek);

			waypointy << waypoint;
		}

		int poczatek, koniec;

		//sciezki
		mapaSpecyfikacjaDane >> iloscElementow;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> poczatek;
			mapaSpecyfikacjaDane >> koniec;

			this->scena->dodajSciezke(waypointy.at(poczatek), waypointy.at(koniec));
		}

		mapaSpecyfikacjaPlik.close();

		this->plikPlanszy = nazwaPliku;
		this->setWindowModified(false);
	}
}

bool MainWindow::zapiszPlansze()
{
	if(this->plikPlanszy.isEmpty())
		return this->zapiszPlanszeJako();
	else
		return this->zapiszPlik(this->plikPlanszy);
}

void MainWindow::zaznaczGalaz()
{
	QList<QGraphicsItem*> zaznaczenie = this->scena->selectedItems();
	if(zaznaczenie.count()) {
		if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(zaznaczenie.first())) {
			QTreeWidgetItem *galaz = this->tabelaElementow.key(przeszkoda);

			if(galaz)
				this->ui->treeWidget->setCurrentItem(galaz);
		} else if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(zaznaczenie.first())) {
			QTreeWidgetItem *galaz = this->tabelaElementow.key(waypoint);

			if(galaz)
				this->ui->treeWidget2->setCurrentItem(galaz);
		}
	}

	this->ui->actionUsun->setEnabled(zaznaczenie.count());
}

void MainWindow::wybierzTlo()
{
	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wybierz tło", "", "Pliki graficzne (*.bmp;*.png;*.jpg;*.jpeg)");
	if(!nazwaPliku.isEmpty()) {
		if(this->scena->items().count()) {
			if(QGraphicsPixmapItem *item = qgraphicsitem_cast<QGraphicsPixmapItem*>(this->scena->items().first()))
				item->setPixmap(QPixmap(nazwaPliku));
		} else {
			this->scena->addPixmap(QPixmap(nazwaPliku));
		}
	}
}

void MainWindow::usunZaznaczony()
{
	foreach(QGraphicsItem *element, this->scena->selectedItems()) {
		if(element->type() == Sciezka::Type) {
			this->scena->removeItem(element);

			Sciezka *sciezka = qgraphicsitem_cast<Sciezka*>(element);
			sciezka->poczatek()->usunSciezke(sciezka);
			sciezka->koniec()->usunSciezke(sciezka);

			delete element;
		}
	}

	foreach(QGraphicsItem *element, this->scena->selectedItems()) {
		this->scena->removeItem(element);

		if(element->type() == Waypoint::Type) {
			qgraphicsitem_cast<Waypoint*>(element)->usunSciezki();
		}

		QTreeWidgetItem *galaz = this->tabelaElementow.key(element);
		if(galaz) {
			this->tabelaElementow.remove(galaz);
			delete galaz;
		}

		delete element;
		this->scenaZmieniona();
	}
}

void MainWindow::scenaZmieniona()
{
	this->setWindowModified(true);
}

void MainWindow::aktualizujTryb()
{
	this->ustawTryb(this->scena->tryb());
}

void MainWindow::aktualizujDrzewkoPrzeszkod()
{
	for(int i = this->scena->items().count() - 1;  i >= 0; i--) {
		QGraphicsItem *item = this->scena->items().at(i);
		if(item->type() == Przeszkoda::Type) {
			if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(item)) {
				if(!this->tabelaElementow.key(przeszkoda)) {
					QTreeWidgetItem *galaz = new QTreeWidgetItem(ui->treeWidget->topLevelItem(0));
					galaz->setText(0, QString("Przeszkoda #%1").arg(this->scena->przeszkody));
					galaz->setIcon(0, QIcon(":/ikony/shape_square.png"));

					this->tabelaElementow.insert(galaz, przeszkoda);
					this->ui->treeWidget->expandAll();

					this->scenaZmieniona();
				}
			}
		}
	}
}

void MainWindow::aktualizujDrzewkoWaypointow()
{
	for(int i = this->scena->items().count() - 1;  i >= 0; i--) {
		QGraphicsItem *item = this->scena->items().at(i);
		if(item->type() == Waypoint::Type) {
			if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item)) {
				if(!this->tabelaElementow.key(waypoint)) {
					QTreeWidgetItem *galaz = new QTreeWidgetItem(this->ui->treeWidget2->topLevelItem(0));
					galaz->setText(0, QString("Punkt #%1").arg(this->scena->waypointy));
					galaz->setIcon(0, QIcon(":/ikony/flag_yellow.png"));

					this->tabelaElementow.insert(galaz, waypoint);
					this->ui->treeWidget2->expandAll();

					this->scenaZmieniona();
				}
			}
		}
	}
}

bool MainWindow::sprawdzZapis()
{
	if(this->isWindowModified()) {
		QMessageBox::StandardButton ret = QMessageBox::warning(this, "Zapisz zmiany", "Plansza została zmieniona, czy zapisać zmiany?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if(ret == QMessageBox::Save)
			return zapiszPlansze();
		else if(ret == QMessageBox::Cancel)
			return false;
	}

	return true;
}

bool MainWindow::zapiszPlanszeJako()
{
	QString nazwaPliku = QFileDialog::getSaveFileName(this, "Zapisz plansze", "", "Plansze (*.dat)");
	if(nazwaPliku.isEmpty())
		return false;

	return this->zapiszPlik(nazwaPliku);
}

bool MainWindow::zapiszPlik(const QString &nazwaPliku)
{
	QFile mapaSpecyfikacjaPlik(nazwaPliku);
	if(!mapaSpecyfikacjaPlik.open(QFile::WriteOnly)) {
		return false;
	}

	QFileInfo plik(nazwaPliku);

	QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);

	QList<Przeszkoda*> przeszkody;
	QList<Waypoint*> waypointy;
	QList<Sciezka*> sciezki;

	for(int i = this->scena->items().count() - 1;  i >= 0; i--) {
		QGraphicsItem *item = this->scena->items().at(i);
		if(item->type() == Przeszkoda::Type) {
			if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(item))
				przeszkody << przeszkoda;
		} else if(item->type() == Waypoint::Type) {
			if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item))
				waypointy << waypoint;
		} else if(item->type() == Sciezka::Type) {
			if(Sciezka *sciezka = qgraphicsitem_cast<Sciezka*>(item))
				sciezki << sciezka;
		} else if(item->type() == QGraphicsPixmapItem::Type) {
			if(QGraphicsPixmapItem *tlo = static_cast<QGraphicsPixmapItem*>(item)) {
				QString nazwaGrafiki = plik.absolutePath() + "/" + plik.completeBaseName() + ".png";
				if(!QFile::exists(nazwaGrafiki))
					tlo->pixmap().save(nazwaGrafiki);
			}
		}
	}

	mapaSpecyfikacjaDane << przeszkody.count();
	foreach(Przeszkoda *poly, przeszkody) {
		mapaSpecyfikacjaDane << poly->polygon().count();

		QPoint wierzcholek;
		for(int j = 0; j < poly->polygon().count(); j++) {
			poly->polygon().toPolygon().point(j, &wierzcholek.rx(), &wierzcholek.ry());
			wierzcholek += poly->pos().toPoint();
			mapaSpecyfikacjaDane << wierzcholek;
		}
	}

	mapaSpecyfikacjaDane << waypointy.count();
	foreach(Waypoint *waypoint, waypointy)
		mapaSpecyfikacjaDane << waypoint->pos().toPoint();

	mapaSpecyfikacjaDane << sciezki.count();
	foreach(Sciezka *sciezka, sciezki)
		mapaSpecyfikacjaDane << waypointy.indexOf(sciezka->poczatek()) << waypointy.indexOf(sciezka->koniec());

	mapaSpecyfikacjaPlik.close();

	this->plikPlanszy = nazwaPliku;
	this->setWindowModified(false);

	QMessageBox::information(this, "Zapisano", "Plansza została zapisana.");

	return true;
}

void MainWindow::ustawTryb(Scena::Tryb tryb)
{
	this->ui->graphicsView->setCursor((tryb == Scena::DODAWANIE_PRZESZKODY || tryb == Scena::DODAWANIE_WAYPOINTU) ? Qt::CrossCursor : Qt::ArrowCursor);

	this->ui->actionZaznaczanie->setChecked(tryb == Scena::ZAZNACZANIE);
	this->ui->actionPrzesuwanieWidoku->setChecked(tryb == Scena::PRZESUWANIE_WIDOKU);
	this->ui->actionPrzesuwanie->setChecked(tryb == Scena::PRZESUWANIE_ELEMENTU);
	this->ui->actionEdycjaWierzcholkow->setChecked(tryb == Scena::EDYCJA_WIERZCHOLKOW);
	this->ui->actionDodaj->setChecked(tryb == Scena::DODAWANIE_PRZESZKODY);
	this->ui->actionDodajPunktRuchu->setChecked(tryb == Scena::DODAWANIE_WAYPOINTU);
	this->ui->actionLaczeniePunktowRuchu->setChecked(tryb == Scena::LACZENIE_WAYPOINTOW);

	this->ui->graphicsView->setDragMode((tryb == Scena::PRZESUWANIE_WIDOKU) ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);

	this->scena->ustawTryb(tryb);
	this->scena->update();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if(current) {
		if(current->parent()) {
			this->scena->clearSelection();

			if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(this->tabelaElementow.value(current))) {
				przeszkoda->setSelected(true);
				this->ui->graphicsView->ensureVisible(przeszkoda);
			}
		}
	}
}

void MainWindow::przyblizZoom()
{
	this->ui->graphicsView->scale(1.15, 1.15);
}

void MainWindow::oddalZoom()
{
	this->ui->graphicsView->scale(1.0 / 1.15, 1.0 / 1.15);
}

void MainWindow::trybPrzesuwania()
{
	this->ustawTryb(Scena::PRZESUWANIE_ELEMENTU);
}

void MainWindow::trybEdycjiWierzcholkow()
{
	this->ustawTryb(Scena::EDYCJA_WIERZCHOLKOW);
}

void MainWindow::trybZaznaczania()
{
	this->ustawTryb(Scena::ZAZNACZANIE);
}

void MainWindow::trybPrzesuwaniaWidoku()
{
	this->ustawTryb(Scena::PRZESUWANIE_WIDOKU);
}

void MainWindow::zerujZoom()
{
  this->ui->graphicsView->resetTransform();
}

void MainWindow::trybDodawaniaPrzeszkod()
{
	bool ok = false;
	int boki = QInputDialog::getInt(this, "Podaj ilosc bokow", "Ilosc bokow:", 3, 3, 100, 1, &ok);
	if(ok) {
		this->ustawTryb(Scena::DODAWANIE_PRZESZKODY);
		this->scena->setProperty("boki", boki);
	} else {
		this->ustawTryb(this->scena->tryb());
	}
}

void MainWindow::trybDodawaniaWaypointow()
{
	this->ustawTryb(Scena::DODAWANIE_WAYPOINTU);
}

void MainWindow::on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if(current) {
		if(current->parent()) {
			this->scena->clearSelection();

			if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(this->tabelaElementow.value(current))) {
				waypoint->setSelected(true);
				this->ui->graphicsView->ensureVisible(waypoint);
			}
		}
	}
}

void MainWindow::trybLaczeniaWaypointow()
{
	this->ustawTryb(Scena::LACZENIE_WAYPOINTOW);
}
