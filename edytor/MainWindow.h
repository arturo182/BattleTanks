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
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

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
		void przyblizZoom();
		void oddalZoom();
		void trybPrzesuwania();
		void trybEdycjiWierzcholkow();
		void trybZaznaczania();
		void trybPrzesuwaniaWidoku();
		void zerujZoom();
		void trybDodawaniaPrzeszkod();
		void trybDodawaniaWaypointow();
		void on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
		void trybLaczeniaWaypointow();

	private:
		bool sprawdzZapis();
		bool zapiszPlik(const QString &nazwaPliku);
		void ustawTryb(Scena::Tryb tryb);

	private:
		Ui::MainWindow *ui;
		QString plikPlanszy;
		Scena *scena;
		QHash<QTreeWidgetItem*, class QGraphicsItem*> tabelaElementow;
};

#endif // MAINWINDOW_H
