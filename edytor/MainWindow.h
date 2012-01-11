#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scena.h"

#include <QMainWindow>
#include <QHash>

namespace Ui{
	class MainWindow;
}

class QTreeWidgetItem;

class MainWindow : public QMainWindow{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void nowaPlansza();
		void wczytajPlansze();
		bool zapiszPlansze();
		bool zapiszPlanszeJako();
		void zaznaczGalaz();
		void wybierzTlo();
		void usunZaznaczony();
		void scenaZmieniona();
		void aktualizujTryb();
		void aktualizujDrzewkoPrzeszkod();
		void aktualizujDrzewkoWaypointow();

		void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
		void on_actionPrzybliz_triggered();
		void on_actionOddal_triggered();
		void on_actionPrzesuwanie_triggered();
		void on_actionEdycjaWierzcholkow_triggered();
		void on_actionZaznaczanie_triggered();
		void on_actionPrzesuwanieWidoku_triggered();
		void on_actionOryginalnyRozmiar_triggered();
		void on_actionDodaj_triggered();
		void on_actionDodajPunktRuchu_triggered();
		void on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	private:
		bool sprawdzZapis();
		bool zapiszPlik(const QString &nazwaPliku);
		void ustawTryb(Scena::Tryby tryb);

	private:
		Ui::MainWindow *ui;
		QString plikPlanszy;
		Scena *scena;
		Scena::Tryby tryb;
		QHash<QTreeWidgetItem*, class QGraphicsItem*> tabelaElementow;
};

#endif // MAINWINDOW_H
