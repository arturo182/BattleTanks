#ifndef OKNOSPECYFIKACJI_H
#define OKNOSPECYFIKACJI_H

#include <QDialog>
#include <QMap>

namespace Ui
{
	class OknoSpecyfikacji;
}

class QTreeWidgetItem;

class OknoSpecyfikacji : public QDialog
{
	Q_OBJECT

	public:
		explicit OknoSpecyfikacji(const QString &plikTla, const int &trybGry, const int &pojazdGracza, const QMap<int, int> &pociskiGracza, QWidget *parent = 0);
		~OknoSpecyfikacji();

		QString plikTla;
		int trybGry;
		int pojazdGracza;
		QMap<int, int> pociskiGracza;


	private slots:
		void przegladajTlo();
		void dodajPocisk();
		void usunPocisk();
		void sprawdzDodajPocisk(int);
		void zmienGracza(int index);
		void zmienTryb(int index);
		void on_pociskiTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	private:
		Ui::OknoSpecyfikacji *ui;
		QMap<int, QString> pociskiBaza;
};

#endif // OKNOSPECYFIKACJI_H
