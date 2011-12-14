#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Ekran: public QWidget{
	Q_OBJECT
	
	public:
		QPixmap buforObrazu;
		
		Ekran();
		void ustawRozdzielczosc(QSize);
	
	private:
		void paintEvent(QPaintEvent*);
};

#endif // EKRAN_H
