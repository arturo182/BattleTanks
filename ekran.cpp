#include <QPainter>
#include "ekran.h"

Ekran::Ekran():
  QWidget(){
	this->setWindowState(Qt::WindowFullScreen);
}

void Ekran::ustawRozdzielczosc(const QSize &rozdzielczosc){
  this->buforObrazu = QPixmap(rozdzielczosc);
}

void Ekran::ustawJakosc(const QString &jakosc)
{
  this->jakosc = jakosc;
}

void Ekran::paintEvent(QPaintEvent*){
  QPainter painter(this);

//  if(this->jakosc == "wysoka")
//	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

  painter.drawPixmap(this->rect(), this->buforObrazu);
  painter.end();
}
