#include "widzety.h"

void Widzety::cieniowanyTekst(QPainter &painter, const QRectF &r, const QString &text, const QTextOption &o)
{
	painter.save();

	int margines = painter.fontMetrics().height() / 10;

	painter.setPen(Qt::black);
	painter.drawText(r.adjusted(margines, margines, 0, 0), text, o);

	painter.restore();
	painter.drawText(r, text, o);
}

void Widzety::przyciskUstawien(QPainter &painter, const QRectF &r, const QString &text, bool wstecz, bool dalej)
{
	float wysokoscTrojkata = r.height() * 0.5;
	float szerokoscTrojkata = wysokoscTrojkata * 0.5;
	float margines = szerokoscTrojkata * 0.25;

	QPainterPath lewyTrojkat;
	lewyTrojkat.moveTo(0, wysokoscTrojkata / 2);
	lewyTrojkat.lineTo(szerokoscTrojkata, wysokoscTrojkata);
	lewyTrojkat.lineTo(szerokoscTrojkata, 0);
	lewyTrojkat.lineTo(0, wysokoscTrojkata / 2);
	lewyTrojkat.translate(r.left(), r.top() + (r.height() / 2 - wysokoscTrojkata / 2));

	QPainterPath prawyTrojkat;
	prawyTrojkat.lineTo(0, wysokoscTrojkata);
	prawyTrojkat.lineTo(szerokoscTrojkata, wysokoscTrojkata / 2);
	prawyTrojkat.lineTo(0, 0);

	prawyTrojkat.translate(r.right() - szerokoscTrojkata, r.top() + (r.height() / 2 - wysokoscTrojkata / 2));

	painter.fillPath(lewyTrojkat, Qt::black);
	if(wstecz)
		painter.fillPath(lewyTrojkat.translated(-margines, -margines), Qt::white);

	painter.fillPath(prawyTrojkat, Qt::black);
	if(dalej)
		painter.fillPath(prawyTrojkat.translated(-margines, -margines), Qt::white);

	painter.setPen(Qt::white);
	cieniowanyTekst(painter, r.adjusted(szerokoscTrojkata, 0, -szerokoscTrojkata - margines, 0), text, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

void Widzety::listaWyboru(QPainter &painter, const QRectF &r, const QStringList &elementy, int zaznaczenie)
{
	painter.setPen(QPen(Qt::white, 3.0));
	painter.drawRect(r);

	int wysokosc = painter.fontMetrics().height();
	for(int i = 0; i < qMin(elementy.size(), (int)r.height() / wysokosc); i++) {
		QRectF r_txt(r.left() + 10, r.top() + i * wysokosc, r.width() - 20, wysokosc);
		if(zaznaczenie == i+1) {
			painter.fillRect(QRectF(r.left() + 1.5, r.top() + i * wysokosc, r.width() - 3, wysokosc), Qt::white);
			painter.setPen(Qt::black);
			painter.drawText(r_txt, elementy.at(i));
		} else {
			painter.setPen(Qt::white);
			cieniowanyTekst(painter, r_txt, elementy.at(i));
		}
	}
}

void Widzety::tabelaRekordow(QPainter &painter, const QRectF &r, const QList<QStringList> &rekordy, int strona)
{
	painter.setPen(QPen(Qt::white, 3.0));
	painter.drawRect(r);

	int wysokosc = painter.fontMetrics().height();
	const int na_strone = 9;

	QRectF kolumnaGracza(r.left(), r.top(), r.width() * 0.4, wysokosc);
	painter.drawRect(kolumnaGracza);
	kolumnaGracza.adjust(10, 0, 0, 0);
	cieniowanyTekst(painter, kolumnaGracza, "Gracz");

	QRectF kolumnaPlansze(r.left() + r.width() * 0.4, r.top(), r.width() * 0.4, wysokosc);
	painter.drawRect(kolumnaPlansze);
	kolumnaPlansze.adjust(10, 0, 0, 0);
	cieniowanyTekst(painter, kolumnaPlansze, "Plansza");

	QRectF kolumnaWyniku(r.left() + r.width() * 0.8, r.top(), r.width() * 0.2, wysokosc);
	painter.drawRect(kolumnaWyniku);
	kolumnaWyniku.adjust(10, 0, 0, 0);
	cieniowanyTekst(painter, kolumnaWyniku, "Wynik");

	for(int i = na_strone * (strona - 1); i < qMin(rekordy.size(), na_strone * strona); i++) {
		kolumnaGracza.adjust(0, wysokosc, 0, wysokosc);
		kolumnaPlansze.adjust(0, wysokosc, 0, wysokosc);
		kolumnaWyniku.adjust(0, wysokosc, 0, wysokosc);

		cieniowanyTekst(painter, kolumnaGracza, rekordy.at(i).at(0));
		cieniowanyTekst(painter, kolumnaPlansze, rekordy.at(i).at(1));
		cieniowanyTekst(painter, kolumnaWyniku, rekordy.at(i).at(2));
	}
}

void Widzety::lustrzanyObrazek(QPainter &painter, const QRectF &r, const QPixmap &obrazek)
{
	painter.drawPixmap(r.translated(0, r.height() * 0.5).toRect(), obrazek);

	/*QLinearGradient gradient(0, 0, 0, obrazek.height());
	gradient.setColorAt(0.5, Qt::black);
	gradient.setColorAt(0, Qt::white);

	QPixmap mask = obrazek;
	QPainter p(&mask);
	p.fillRect(obrazek.rect(), gradient);
	p.end();

	QImage reflection = obrazek.toImage().mirrored();
	reflection.setAlphaChannel(mask.toImage());

	painter.setOpacity(0.5);
	painter.drawImage(r.translated(0, r.height()), reflection);
	painter.setOpacity(1.0);*/
}

void Widzety::schodkiGlosnosci(QPainter &painter, const QRectF &r, int wartosc)
{
	float gruboscPaska = r.width() * 0.05;
	float maxWysokosc = r.height();
	float margines = gruboscPaska * 0.3;

	//czarne paski
	for(int i = 0; i < 10; i++)
		painter.fillRect(QRectF(r.left() + gruboscPaska * (i * 2), r.top() + maxWysokosc * (9 - i) * 0.1, gruboscPaska, maxWysokosc * (i + 1) * 0.1), Qt::black);

	//biale paski
	for(int j = 0; j < wartosc; j++)
		painter.fillRect(QRectF(r.left() + gruboscPaska * (j * 2) - margines, r.top() + maxWysokosc * (9 - j) * 0.1 - margines, gruboscPaska, maxWysokosc * (j + 1) * 0.1), Qt::white);
}

void Widzety::przyciskGamepada(QPainter &painter, const QRectF &r, const QString &numer)
{
	painter.save();

	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	painter.drawEllipse(r.adjusted(-1, -1, 1, 1));

	painter.setPen(QColor("#171717"));
	painter.setBrush(QColor("#080808"));
	painter.drawEllipse(r);

	painter.setPen(Qt::white);
	QFont f = painter.font();
	f.setPixelSize(r.height() * 0.6);
	painter.setFont(f);
	painter.drawText(r, numer, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

	painter.restore();
}

void Widzety::przyciskKlawiatury(QPainter &painter, const QRectF &r, const QString &tekst)
{
	painter.save();

	painter.setPen(QColor("#bfbfbf"));
	painter.setBrush(QColor("#deddaf"));
	painter.drawRect(r);

	painter.setPen(Qt::black);
	QFont f = painter.font();
	f.setPixelSize(r.height() * 0.4);
	painter.setFont(f);

	painter.drawText(r, tekst, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

	painter.restore();
}
