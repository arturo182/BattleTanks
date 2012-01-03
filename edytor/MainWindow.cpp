#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Przeszkoda.h"
#include "Scena.h"

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
	this->ui->graphicsView->setScene(new Scena);
	this->plikPlanszy = "";


	this->ustawTryb(Scena::ZAZNACZANIE);

	connect(this->ui->graphicsView->scene(), SIGNAL(selectionChanged()), this, SLOT(zaznaczGalaz()));
	connect(this->ui->graphicsView->scene(), SIGNAL(changed(QList<QRectF>)), this, SLOT(scenaZmieniona()));
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

void MainWindow::nowaPlansza()
{
	if(this->sprawdzZapis()) {
		qDeleteAll(this->ui->treeWidget->topLevelItem(0)->takeChildren());

		this->ui->graphicsView->scene()->clear();
		this->tabelaElementow.clear();
		this->plikPlanszy = "";
		this->setWindowModified(false);
	}
}

void MainWindow::wczytajPlansze()
{
	this->nowaPlansza();

	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wczytaj plansze", "", "Plansze (*.dat)");
	if(!nazwaPliku.isEmpty()) {
		QFileInfo plik(nazwaPliku);

		this->ui->graphicsView->scene()->addPixmap(QPixmap(plik.absolutePath() + "/" + plik.completeBaseName() + ".png"));

		QFile mapaSpecyfikacjaPlik(nazwaPliku);
		if(!mapaSpecyfikacjaPlik.open(QIODevice::ReadOnly))
			return;

		int iloscPrzeszkod, iloscWierzcholkow;
		QPoint wierzcholek;

		QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);
		mapaSpecyfikacjaDane >> iloscPrzeszkod;
		for(int i = 0; i < iloscPrzeszkod; i++){
			mapaSpecyfikacjaDane >> iloscWierzcholkow;

			QPolygon przeszkoda(iloscWierzcholkow);
			for(int j = 0; j < iloscWierzcholkow; j++){
				mapaSpecyfikacjaDane >> wierzcholek;
				przeszkoda.setPoint(j, wierzcholek);
			}

			this->dodajPrzeszkode(przeszkoda);
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
	QList<QGraphicsItem*> zaznaczenie = this->ui->graphicsView->scene()->selectedItems();
	if(zaznaczenie.count()) {
		if(Przeszkoda *przeszkoda = static_cast<Przeszkoda*>(zaznaczenie.first())) {
			QTreeWidgetItem *galaz = this->tabelaElementow.key(przeszkoda);

			if(galaz)
				this->ui->treeWidget->setCurrentItem(galaz);
		}
	}

	this->ui->actionUsun->setEnabled(zaznaczenie.count());
}

void MainWindow::wybierzTlo()
{
	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wybierz tło", "", "Pliki graficzne (*.bmp;*.png;*.jpg;*.jpeg)");
	if(!nazwaPliku.isEmpty()) {
		if(this->ui->graphicsView->scene()->items().count()) {
			if(QGraphicsPixmapItem *item = qgraphicsitem_cast<QGraphicsPixmapItem*>(this->ui->graphicsView->scene()->items().first()))
				item->setPixmap(QPixmap(nazwaPliku));
		} else {
			this->ui->graphicsView->scene()->addPixmap(QPixmap(nazwaPliku));
		}
	}
}

void MainWindow::usunZaznaczony()
{
	QList<QGraphicsItem*> zaznaczenie = this->ui->graphicsView->scene()->selectedItems();
	if(zaznaczenie.count()) {
		if(Przeszkoda *przeszkoda = qgraphicsitem_cast<Przeszkoda*>(zaznaczenie.first())) {
			this->ui->graphicsView->scene()->removeItem(przeszkoda);

			QTreeWidgetItem *item = this->tabelaElementow.key(przeszkoda);
			this->tabelaElementow.remove(item);
			delete item;
		}
	}
}

void MainWindow::scenaZmieniona()
{
	this->setWindowModified(true);
}

void MainWindow::dodajPrzeszkode(const QPolygon &poly)
{
	Przeszkoda *item = new Przeszkoda(poly);
	this->ui->graphicsView->scene()->addItem(item);

	QTreeWidgetItem *galaz = new QTreeWidgetItem(ui->treeWidget->topLevelItem(0));
	galaz->setText(0, QString("Przeszkoda #%1").arg(ui->graphicsView->scene()->items().count() - 1));
	galaz->setIcon(0, QIcon(":/ikony/shape_square.png"));

	this->tabelaElementow.insert(galaz, item);
	this->ui->treeWidget->expandAll();
	this->setWindowModified(true);
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
	mapaSpecyfikacjaDane << this->ui->graphicsView->items().count() - 1;

	for(int i = this->ui->graphicsView->items().count() - 1;  i >= 0; i--) {
		QGraphicsItem *item = this->ui->graphicsView->items().at(i);
		if(item->type() == Przeszkoda::Type) {
			if(Przeszkoda *poly = qgraphicsitem_cast<Przeszkoda*>(item)) {
				mapaSpecyfikacjaDane << poly->polygon().count();

				QPoint wierzcholek;
				for(int j = 0; j < poly->polygon().count(); j++) {
					poly->polygon().toPolygon().point(j, &wierzcholek.rx(), &wierzcholek.ry());
					wierzcholek += poly->pos().toPoint();
					mapaSpecyfikacjaDane << wierzcholek;
				}
			}
		} else if(item->type() == QGraphicsPixmapItem::Type) {
			if(QGraphicsPixmapItem *tlo = static_cast<QGraphicsPixmapItem*>(item)) {
				tlo->pixmap().save(plik.absolutePath() + "/" + plik.completeBaseName() + ".png");
			}
		}
	}

	mapaSpecyfikacjaPlik.close();

	this->plikPlanszy = nazwaPliku;
	this->setWindowModified(false);

	return true;
}

void MainWindow::ustawTryb(Scena::Tryby tryb)
{
	this->tryb = tryb;

	this->ui->actionZaznaczanie->setChecked(tryb == Scena::ZAZNACZANIE);
	this->ui->actionPrzesuwanieWidoku->setChecked(tryb == Scena::PRZESUWANIE_WIDOKU);
	this->ui->actionPrzesuwanie->setChecked(tryb == Scena::PRZESUWANIE_PRZESZKODY);
	this->ui->actionEdycjaWierzcholkow->setChecked(tryb == Scena::EDYCJA_WIERZCHOLKOW);

	this->ui->graphicsView->setDragMode((tryb == Scena::PRZESUWANIE_WIDOKU) ? QGraphicsView::ScrollHandDrag : QGraphicsView::NoDrag);

	this->ui->graphicsView->scene()->setProperty("tryb", tryb);
	this->ui->graphicsView->scene()->update();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if(current) {
		if(current->parent()) {
			this->ui->graphicsView->scene()->clearSelection();
			this->tabelaElementow.value(current)->setSelected(true);
		}
	}
}

void MainWindow::on_actionPrzybliz_triggered()
{
	this->ui->graphicsView->scale(1.15, 1.15);
}

void MainWindow::on_actionOddal_triggered()
{
	this->ui->graphicsView->scale(1.0 / 1.15, 1.0 / 1.15);
}

void MainWindow::on_actionPrzesuwanie_triggered()
{
	this->ustawTryb(Scena::PRZESUWANIE_PRZESZKODY);
}

void MainWindow::on_actionEdycjaWierzcholkow_triggered()
{
	this->ustawTryb(Scena::EDYCJA_WIERZCHOLKOW);
}

void MainWindow::on_actionZaznaczanie_triggered()
{
	this->ustawTryb(Scena::ZAZNACZANIE);
}

void MainWindow::on_actionPrzesuwanieWidoku_triggered()
{
	this->ustawTryb(Scena::PRZESUWANIE_WIDOKU);
}

void MainWindow::on_actionOryginalnyRozmiar_triggered()
{
  this->ui->graphicsView->resetTransform();
}

void MainWindow::on_actionDodaj_triggered()
{
	bool ok = false;
	int boki = QInputDialog::getInt(this, "Podaj ilosc bokow", "Ilosc bokow:", 3, 3, 100, 1, &ok);
	if(ok) {
		QPointF srodek(0, 0);
		double r = 100.0;
		QPolygonF poly;
		float alfa;

		for(int i = 0; i < boki; i++) {
			alfa = 2.0 * M_PI * i / boki;
			poly.push_back(srodek + r * QPointF(qSin(alfa), -qCos(alfa)));
		}

		this->dodajPrzeszkode(poly.toPolygon());
	}
}
