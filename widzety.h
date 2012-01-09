#ifndef WIDZETY_H
#define WIDZETY_H

#include <QPainter>

class Widzety
{
	public:
		static void cieniowanyTekst(QPainter &painter, const QRectF &r, const QString &text, const QTextOption &o = QTextOption());
		static void przyciskUstawien(QPainter &painter, const QRectF &r, const QString &text, bool wstecz = true, bool dalej = true);
		static void listaWyboru(QPainter &painter, const QRectF &r, const QStringList &elementy, int zaznaczenie = 1);
		static void tabelaRekordow(QPainter &painter, const QRectF &r, const QList<QStringList> &rekordy, int strona = 1);
		static void lustrzanyObrazek(QPainter &painter, const QRectF &r, const QPixmap &obrazek);
		static void schodkiGlosnosci(QPainter &painter, const QRectF &r, int wartosc);
};

#endif // WIDZETY_H
