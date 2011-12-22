#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Ekran: public QWidget{
	Q_OBJECT
	
	private:
		QString jakosc;
	
	public:
		QPixmap buforObrazu;
		
		Ekran();
		void ustawRozdzielczosc(const QSize&);
		void ustawJakosc(const QString &);
	
	private:
		void paintEvent(QPaintEvent*);
};

#endif // EKRAN_H
