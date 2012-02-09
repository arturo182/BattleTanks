#include "ekran.h"
#include "dotyk.h"

#include <QPainter>
#include <QDebug>
#include <QTouchEvent>

Ekran::Ekran(const QSize &rozdzielczosc, const QString &jakosc):
	QWidget(),
	jakosc(jakosc),
	buforObrazu(QPixmap(rozdzielczosc)){
	this->setAttribute(Qt::WA_AcceptTouchEvents);
	this->setAttribute(Qt::WA_LockLandscapeOrientation);
	this->setWindowState(Qt::WindowFullScreen);
	this->setCursor(Qt::BlankCursor);
}

void Ekran::ustawRozdzielczosc(const QSize &rozdzielczosc){
	this->buforObrazu = QPixmap(rozdzielczosc);
}

void Ekran::ustawJakosc(const QString &jakosc){
	this->jakosc = jakosc;
}

bool Ekran::event(QEvent *event)
{
	if((event->type() == QEvent::TouchBegin) || (event->type() == QEvent::TouchUpdate) || (event->type() == QEvent::TouchEnd)) {
		if(this->dotyk) {
			QTouchEvent *touch = static_cast<QTouchEvent*>(event);
			this->dotyk->poke(touch);
		}
	}

	return QWidget::event(event);
}

void Ekran::paintEvent(QPaintEvent*){
	QPainter painter(this);

	if(this->jakosc == "wysoka")
		painter.setRenderHint(QPainter::SmoothPixmapTransform);

	painter.drawPixmap(this->rect(), this->buforObrazu);
	painter.end();
}
