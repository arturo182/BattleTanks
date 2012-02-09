#include "ladowanie.h"
#include "obiekt.h"
#include "ekran.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>

Ladowanie::Ladowanie(Ekran *ekran) :
QObject(),
ekran(ekran),
postep(0.0f)
{
//	QPixmap tlo("grafika/tlo_menu.png");
//	this->tloPixmapa = tlo.scaled(
//		Obiekt::skala * tlo.size(),
//		Qt::IgnoreAspectRatio,
//		(this->jakosc == "wysoka") ? Qt::SmoothTransformation : Qt::FastTransformation
//	);

	QPixmap logo("grafika/logo.png");
	this->logoPixmapa = logo.scaled(Obiekt::skala * logo.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void Ladowanie::rysuj() const
{
	//	rysowanie na ekranie :P
	QPainter painter(&this->ekran->buforObrazu);
	painter.fillRect(this->ekran->buforObrazu.rect(), Qt::black);

	int szerokoscEkranu = this->ekran->buforObrazu.width();
	int wysokoscEkranu = this->ekran->buforObrazu.height();

	QFont czcionka = painter.font();
	czcionka.setFamily("Trebuchet MS");
	czcionka.setPixelSize(wysokoscEkranu * 0.05);

	painter.drawPixmap((szerokoscEkranu - this->logoPixmapa.width()) * 0.5, (wysokoscEkranu - this->logoPixmapa.height()) * 0.5, this->logoPixmapa);

	painter.setPen(Qt::white);
	painter.setFont(czcionka);
	painter.drawText(QRectF(0, wysokoscEkranu * 0.85, szerokoscEkranu, 100), "Ładowanie (" + this->opisPostepu + ")", QTextOption(Qt::AlignHCenter));

	QLinearGradient grad(0, 0, 20, 20);
	grad.setSpread(QGradient::RepeatSpread);
	grad.setColorAt(0.00, QColor("#278fbb"));
	grad.setColorAt(0.49, QColor("#278fbb"));
	grad.setColorAt(0.51, QColor("#1c7195"));
	grad.setColorAt(1.00, QColor("#1c7195"));

	painter.setPen(Qt::NoPen);
	painter.setBrush(grad);
	painter.drawRect(szerokoscEkranu * 0.25, wysokoscEkranu * 0.95, (szerokoscEkranu * 0.5) * this->postep, wysokoscEkranu * 0.02);

	painter.setPen(QPen(Qt::white, 2));
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(szerokoscEkranu * 0.25, wysokoscEkranu * 0.95, szerokoscEkranu * 0.5, wysokoscEkranu * 0.02);

	painter.end();
	this->ekran->update();
}

void Ladowanie::odswiez(float postep, const QString &opisPostepu)
{
	this->postep = postep;

	if(!opisPostepu.isNull())
		this->opisPostepu = opisPostepu;

	this->rysuj();
	qApp->processEvents();
}
