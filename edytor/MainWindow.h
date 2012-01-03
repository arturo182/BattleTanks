#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
		void zaznaczGalaz();
		void wybierzTlo();
		void usunZaznaczony();
		void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	private:
		void dodajPrzeszkode(const QPolygon &poly);
		bool sprawdzZapis();
		bool zapiszPlanszeJako();
		bool zapiszPlik(const QString &nazwaPliku);

	private:
		Ui::MainWindow *ui;
		QString plikPlanszy;
		QHash<QTreeWidgetItem*, class Przeszkoda*> tabelaElementow;
};

#endif // MAINWINDOW_H
