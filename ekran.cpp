#include <QPainter>
#include "ekran.h"

Ekran::Ekran():
	QWidget(){
	//this->setWindowState(Qt::WindowFullScreen);
}

void Ekran::ustawRozdzielczosc(QSize rozdzielczosc){
	this->buforObrazu = QPixmap(rozdzielczosc);
}

void Ekran::paintEvent(QPaintEvent*){
	QPainter painter(this);
	painter.drawPixmap(this->rect(), this->buforObrazu);
	painter.end();
}
