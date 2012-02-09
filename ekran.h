#ifndef EKRAN_H
#define EKRAN_H

#include "Dotyk.h"

#include <QWidget>

class Ekran: public QWidget
{
	Q_OBJECT

	private:
		QString jakosc;

	public:
		QPixmap buforObrazu;
		Dotyk* dotyk;

		Ekran(const QSize &, const QString &);
		void ustawRozdzielczosc(const QSize&);
		void ustawJakosc(const QString &);

	protected:
		bool event(QEvent *event);

	private:
		void paintEvent(QPaintEvent*);
};

#endif // EKRAN_H
