#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Ekran: public QWidget{
	Q_OBJECT
	
	public:
		QPixmap buforObrazu;
		
		Ekran();
};

#endif // EKRAN_H
