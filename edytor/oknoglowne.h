#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scena.h"

#include <QMainWindow>
#include <QHash>
#include <QMap>

namespace Ui{
	class OknoGlowne;
}

class QTreeWidgetItem;

class OknoGlowne : public QMainWindow{
	Q_OBJECT

	public:
		enum TrybGry
		{
			DEMOLKA = 0,
			LABIRYNT
		};

	public:
		explicit OknoGlowne(QWidget *parent = 0);
		~OknoGlowne();

	protected:
		void closeEvent(QCloseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);
		bool eventFilter(QObject *obj, QEvent *event);

	private slots:
		void nowaPlansza();
		void wczytajPlansze();
		bool zapiszPlansze();
		bool zapiszPlanszeJako();
		void zaznaczGalaz();
		void edytujSpecyfikacje();
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
		void trybPozycjiGracza();
		void trybObcegoPojazdu();
		void trybMety();
		void wczytajBazeDanych();
		void przenumerujWaypointy();
		void przenumerujPrzeszkody();

	private:
		bool sprawdzZapis();
		bool zapiszPlik(const QString &nazwaPliku);
		void ustawTryb(Scena::Tryb tryb);

	private:
		QString plikPlanszy;
		QString plikTla;
		int trybGry;
		int pojazdGracza;
		QMap<int, int> pociskiGracza;
		Scena *scena;
		QHash<QTreeWidgetItem*, class QGraphicsItem*> tabelaElementow;
		Ui::OknoGlowne *ui;
};

#endif // MAINWINDOW_H
