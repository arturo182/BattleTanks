#ifndef OKNOPOJAZDUOBCEGO_H
#define OKNOPOJAZDUOBCEGO_H

#include "waypoint.h"

#include <QDialog>

namespace Ui
{
	class OknoPojazduObcego;
}

class OknoPojazduObcego : public QDialog
{
	Q_OBJECT

	public:
		explicit OknoPojazduObcego(int pojazd, int pocisk, QWidget *parent = 0);
		~OknoPojazduObcego();

		ObcyPojazd obcyPojazd;

	private slots:
		void ustawPocisk(int index);
		void ustawPojazd(int index);

	private:
		Ui::OknoPojazduObcego *ui;
};

#endif // OKNOPOJAZDUOBCEGO_H
