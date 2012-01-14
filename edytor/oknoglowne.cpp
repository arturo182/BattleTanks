#include "oknoglowne.h"
#include "ui_oknoglowne.h"

#include "oknospecyfikacji.h"
#include "przeszkoda.h"
#include "waypoint.h"
#include "sciezka.h"
#include "gracz.h"
#include "scena.h"

#include <QGraphicsPolygonItem>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSqlError>
#include <qmath.h>
#include <QDebug>

OknoGlowne::OknoGlowne(QWidget *parent):
QMainWindow(parent),
pojazdGracza(1),
ui(new Ui::OknoGlowne)
{
	this->ui->setupUi(this);
	this->scena = new Scena(QRectF(0, 0, 1000, 1000));
	this->ui->graphicsView->setScene(this->scena);
	this->ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	this->ui->graphicsView->viewport()->installEventFilter(this);

	this->ustawTryb(Scena::ZAZNACZANIE);
	this->wczytajBazeDanych();

	connect(this->scena, SIGNAL(selectionChanged()), SLOT(zaznaczGalaz()));
	connect(this->scena, SIGNAL(trybZmieniony()), SLOT(aktualizujTryb()));
	connect(this->scena, SIGNAL(przeszkodaDodana()), SLOT(aktualizujDrzewkoPrzeszkod()));
	connect(this->scena, SIGNAL(waypointDodany()), SLOT(aktualizujDrzewkoWaypointow()));
	connect(this->scena, SIGNAL(sciezkaDodana()), SLOT(scenaZmieniona()));
	connect(this->scena, SIGNAL(elementPrzesuniety()), SLOT(scenaZmieniona()));
}

OknoGlowne::~OknoGlowne()
{
	delete this->ui;
}

void OknoGlowne::closeEvent(QCloseEvent *event)
{
	if(sprawdzZapis())
		event->accept();
	else
		event->ignore();
}

void OknoGlowne::keyPressEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Space) && !event->isAutoRepeat()) {
		this->setProperty("poprzedniTryb", this->scena->tryb());
		this->ustawTryb(Scena::PRZESUWANIE_WIDOKU);
	}

	QMainWindow::keyPressEvent(event);
}

void OknoGlowne::keyReleaseEvent(QKeyEvent *event)
{
	if((event->key() == Qt::Key_Space) && !event->isAutoRepeat())
		this->ustawTryb((Scena::Tryb)this->property("poprzedniTryb").toInt());

	QMainWindow::keyReleaseEvent(event);
}

bool OknoGlowne::eventFilter(QObject *obj, QEvent *event)
{
	if(obj == this->ui->graphicsView->viewport()) {
		if(event->type() == QEvent::Wheel) {
			if(QWheelEvent *wheel = static_cast<QWheelEvent*>(event)) {
				if(wheel->modifiers() & Qt::ControlModifier) {
					if(wheel->delta() < 0)
						this->oddalZoom();
					else
						this->przyblizZoom();

					return true;
				}
			}
		}
	}

	return QMainWindow::eventFilter(obj, event);
}

void OknoGlowne::nowaPlansza()
{
	if(this->sprawdzZapis()) {
		qDeleteAll(this->ui->treeWidget->topLevelItem(0)->takeChildren());
		qDeleteAll(this->ui->treeWidget2->topLevelItem(0)->takeChildren());

		this->scena->clear();
		this->scena->setSceneRect(QRectF(0, 0, 1000, 1000));
		this->tabelaElementow.clear();
		this->plikPlanszy = "";
		this->plikTla = "";
		this->pojazdGracza = 0;
		this->pociskiGracza.clear();
		this->setWindowModified(false);
	}
}

void OknoGlowne::wczytajPlansze()
{
	this->sprawdzZapis();
	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wczytaj plansze", "", "Plansze (*.dat)");
	if(!nazwaPliku.isEmpty()) {
		this->nowaPlansza();

		QFileInfo plik(nazwaPliku);

		this->plikTla = plik.absolutePath() + "/" + plik.completeBaseName() + ".png";
		this->scena->addPixmap(QPixmap(this->plikTla));

		QFile mapaSpecyfikacjaPlik(nazwaPliku);
		if(!mapaSpecyfikacjaPlik.open(QIODevice::ReadOnly))
			return;

		int iloscElementow, iloscWierzcholkow;
		QPoint punkt;

		QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);

		//przeszkody
		mapaSpecyfikacjaDane >> iloscElementow;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> iloscWierzcholkow;

			QPolygon przeszkoda(iloscWierzcholkow);
			for(int j = 0; j < iloscWierzcholkow; j++){
				mapaSpecyfikacjaDane >> punkt;
				przeszkoda.setPoint(j, punkt);
			}

			this->scena->dodajPrzeszkode(przeszkoda);
		}

		QList<Waypoint*> waypointy;

		//waypointy
		mapaSpecyfikacjaDane >> iloscElementow;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> punkt;

			Waypoint *waypoint = this->scena->dodajWaypoint(punkt);

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

		float zwrotGracza;

		mapaSpecyfikacjaDane >> this->pojazdGracza;
		mapaSpecyfikacjaDane >> punkt;
		mapaSpecyfikacjaDane >> zwrotGracza;

		if(!punkt.isNull())
			this->scena->dodajGracza(punkt)->setRotation(-(zwrotGracza * 180 / M_PI));

		mapaSpecyfikacjaDane >> iloscElementow;
		int id, ilosc;
		for(int i = 0; i < iloscElementow; i++){
			mapaSpecyfikacjaDane >> id;
			mapaSpecyfikacjaDane >> ilosc;

			this->pociskiGracza.insert(id, ilosc);
		}

		mapaSpecyfikacjaPlik.close();

		if(this->scena->items().count())
			this->scena->setSceneRect(this->scena->itemsBoundingRect());

		this->plikPlanszy = nazwaPliku;
		this->setWindowModified(false);
	}
}

bool OknoGlowne::zapiszPlansze()
{
	if(this->plikPlanszy.isEmpty())
		return this->zapiszPlanszeJako();
	else
		return this->zapiszPlik(this->plikPlanszy);
}

void OknoGlowne::zaznaczGalaz()
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

void OknoGlowne::edytujSpecyfikacje()
{
	OknoSpecyfikacji spec(this->plikTla, this->pojazdGracza, this->pociskiGracza, this);
	int ret = spec.exec();

	if(ret == QDialog::Accepted) {
		if(this->plikTla != spec.plikTla) {
			this->plikTla = spec.plikTla;
			this->scenaZmieniona();

			if(!this->plikTla.isEmpty()) {
				bool ustawiony = false;
				foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
					if(item->type() == QGraphicsPixmapItem::Type) {
						if(QGraphicsPixmapItem *pixmap = qgraphicsitem_cast<QGraphicsPixmapItem*>(item)) {
							pixmap->setPixmap(QPixmap(this->plikTla));
							ustawiony = true;
							break;
						}
					}
				}

				if(!ustawiony)
					this->scena->addPixmap(QPixmap(this->plikTla))->setZValue(-100);
			}
		}

		if(this->pojazdGracza != spec.pojazdGracza) {
			this->pojazdGracza = spec.pojazdGracza;
			this->scenaZmieniona();
		}

		if(this->pociskiGracza != spec.pociskiGracza) {
			this->pociskiGracza = spec.pociskiGracza;
			this->scenaZmieniona();
		}
	}
}

void OknoGlowne::usunZaznaczony()
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

	this->przenumerujWaypointy();
	this->przenumerujPrzeszkody();
}

void OknoGlowne::scenaZmieniona()
{
	this->setWindowModified(true);
}

void OknoGlowne::aktualizujTryb()
{
	this->ustawTryb(this->scena->tryb());
}

void OknoGlowne::aktualizujDrzewkoPrzeszkod()
{
	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Przeszkoda::Type) {
			if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(item)) {
				if(!this->tabelaElementow.key(przeszkoda)) {
					QTreeWidgetItem *galaz = new QTreeWidgetItem(ui->treeWidget->topLevelItem(0));
					galaz->setText(0, "Przeszkoda #x");
					galaz->setIcon(0, QIcon(":/ikony/shape_square.png"));

					this->tabelaElementow.insert(galaz, przeszkoda);
					this->ui->treeWidget->expandAll();

					this->scenaZmieniona();
				}
			}
		}
	}

	this->przenumerujPrzeszkody();
}

void OknoGlowne::aktualizujDrzewkoWaypointow()
{
	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Waypoint::Type) {
			if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item)) {
				if(!this->tabelaElementow.key(waypoint)) {
					QTreeWidgetItem *galaz = new QTreeWidgetItem(this->ui->treeWidget2->topLevelItem(0));
					galaz->setText(0, "Punkt #x");
					galaz->setIcon(0, QIcon(":/ikony/flag_yellow.png"));

					this->tabelaElementow.insert(galaz, waypoint);
					this->ui->treeWidget2->expandAll();

					this->scenaZmieniona();
				}
			}
		}
	}

	this->przenumerujWaypointy();
}

bool OknoGlowne::sprawdzZapis()
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

bool OknoGlowne::zapiszPlanszeJako()
{
	QString nazwaPliku = QFileDialog::getSaveFileName(this, "Zapisz plansze", "", "Plansze (*.dat)");
	if(nazwaPliku.isEmpty())
		return false;

	return this->zapiszPlik(nazwaPliku);
}

bool OknoGlowne::zapiszPlik(const QString &nazwaPliku)
{
	QFile mapaSpecyfikacjaPlik(nazwaPliku);
	if(!mapaSpecyfikacjaPlik.open(QFile::WriteOnly)) {
		return false;
	}

	QFileInfo plik(nazwaPliku);
	QString nazwaGrafiki = plik.absolutePath() + "/" + plik.completeBaseName() + ".png";
	if(!QFile::exists(nazwaGrafiki)) {
		QFile::copy(this->plikTla, nazwaGrafiki);
	}

	QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);

	QList<Przeszkoda*> przeszkody;
	QList<Waypoint*> waypointy;
	QList<Sciezka*> sciezki;
	QPoint pozycjaGracza;
	float zwrotGracza;

	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Przeszkoda::Type) {
			if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(item))
				przeszkody << przeszkoda;
		} else if(item->type() == Waypoint::Type) {
			if(Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item))
				waypointy << waypoint;
		} else if(item->type() == Sciezka::Type) {
			if(Sciezka *sciezka = qgraphicsitem_cast<Sciezka*>(item))
				sciezki << sciezka;
		} else if(item->type() == Gracz::Type) {
			pozycjaGracza = item->pos().toPoint();
			zwrotGracza = -item->rotation();
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

	mapaSpecyfikacjaDane << this->pojazdGracza;
	mapaSpecyfikacjaDane << pozycjaGracza;
	mapaSpecyfikacjaDane << zwrotGracza * M_PI / 180;

	mapaSpecyfikacjaDane << this->pociskiGracza.count();
	QMapIterator<int, int> it(this->pociskiGracza);
	while(it.hasNext()) {
		it.next();

		mapaSpecyfikacjaDane << it.key() << it.value();
	}


	mapaSpecyfikacjaPlik.close();

	this->plikPlanszy = nazwaPliku;
	this->setWindowModified(false);

	this->ui->statusBar->showMessage("Plansza została zapisana.", 10 * 1000);

	return true;
}

void OknoGlowne::ustawTryb(Scena::Tryb tryb)
{
	this->ui->graphicsView->setCursor((tryb == Scena::DODAWANIE_PRZESZKODY || tryb == Scena::DODAWANIE_WAYPOINTU) ? Qt::CrossCursor : Qt::ArrowCursor);

	this->ui->actionZaznaczanie->setChecked(tryb == Scena::ZAZNACZANIE);
	this->ui->actionPrzesuwanieWidoku->setChecked(tryb == Scena::PRZESUWANIE_WIDOKU);
	this->ui->actionPrzesuwanie->setChecked(tryb == Scena::PRZESUWANIE_ELEMENTU);
	this->ui->actionEdycjaWierzcholkow->setChecked(tryb == Scena::EDYCJA_WIERZCHOLKOW);
	this->ui->actionDodaj->setChecked(tryb == Scena::DODAWANIE_PRZESZKODY);
	this->ui->actionDodajPunktRuchu->setChecked(tryb == Scena::DODAWANIE_WAYPOINTU);
	this->ui->actionLaczeniePunktowRuchu->setChecked(tryb == Scena::LACZENIE_WAYPOINTOW);
	this->ui->actionUstawPunktPoczatkowy->setChecked(tryb == Scena::POZYCJA_GRACZA);

	this->ui->graphicsView->setDragMode((tryb == Scena::PRZESUWANIE_WIDOKU) ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);

	this->scena->ustawTryb(tryb);
	this->scena->update();
}

void OknoGlowne::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
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

void OknoGlowne::przyblizZoom()
{
	this->ui->graphicsView->scale(1.15, 1.15);
}

void OknoGlowne::oddalZoom()
{
	this->ui->graphicsView->scale(1.0 / 1.15, 1.0 / 1.15);
}

void OknoGlowne::trybPrzesuwania()
{
	this->ustawTryb(Scena::PRZESUWANIE_ELEMENTU);
}

void OknoGlowne::trybEdycjiWierzcholkow()
{
	this->ustawTryb(Scena::EDYCJA_WIERZCHOLKOW);
}

void OknoGlowne::trybZaznaczania()
{
	this->ustawTryb(Scena::ZAZNACZANIE);
}

void OknoGlowne::trybPrzesuwaniaWidoku()
{
	this->ustawTryb(Scena::PRZESUWANIE_WIDOKU);
}

void OknoGlowne::zerujZoom()
{
  this->ui->graphicsView->resetTransform();
}

void OknoGlowne::trybDodawaniaPrzeszkod()
{
	bool ok = false;
	int boki = QInputDialog::getInt(this, "Podaj ilosc bokow", "Ilosc bokow:", 3, 3, 100, 1, &ok);
	if(ok) {
		this->ustawTryb(Scena::DODAWANIE_PRZESZKODY);
		this->scena->setProperty("boki", boki);
	} else {
		this->aktualizujTryb();
	}
}

void OknoGlowne::trybDodawaniaWaypointow()
{
	this->ustawTryb(Scena::DODAWANIE_WAYPOINTU);
}

void OknoGlowne::on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
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

void OknoGlowne::trybLaczeniaWaypointow()
{
	this->ustawTryb(Scena::LACZENIE_WAYPOINTOW);
}

void OknoGlowne::trybPozycjiGracza()
{
	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Gracz::Type) {
			this->ui->graphicsView->ensureVisible(item);
			QMessageBox::information(this, "Punkt już ustawiony", "Punkt początkowy jest już wstawiony na planszy.<br>Jeśli chcesz go przenieść, użyj narzędzia przesuwania lub usuń go i dodaj nowy.");
			this->aktualizujTryb();
			return;
		}
	}

	this->ustawTryb(Scena::POZYCJA_GRACZA);
}

void OknoGlowne::wczytajBazeDanych()
{
	//	nawiazanie polaczenia z baza
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("baza.db");

	if(!db.open())
		qDebug() << "Nie udalo sie poalczyc z baza danych" << db.databaseName() << db.lastError().text();
}

void OknoGlowne::przenumerujWaypointy()
{
	int id = 0;
	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Waypoint::Type) {
			Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item);
			waypoint->id = id;
			this->tabelaElementow.key(waypoint)->setText(0, "Przeszkoda #" + QString::number(id));
			waypoint->update();
			id++;
		}
	}
}

void OknoGlowne::przenumerujPrzeszkody()
{
	int id = 0;
	foreach(QGraphicsItem *item, this->scena->items(Qt::AscendingOrder)) {
		if(item->type() == Waypoint::Type) {
			Waypoint *waypoint = qgraphicsitem_cast<Waypoint*>(item);
			waypoint->id = id;
			this->tabelaElementow.key(waypoint)->setText(0, "Przeszkoda #" + QString::number(id));
			waypoint->update();
			id++;
		}
	}
}
