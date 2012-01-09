#include "pauza.h"
#include "widzety.h"
#include "plansza.h"
#include "ekran.h"

#include <QTextOption>
#include <QPainter>

Pauza::Pauza(Ekran *ekran, Plansza *plansza):
ekran(ekran),
plansza(plansza),
pozycja(1)
{
}

Silnik::Tryb Pauza::odswiez(int milisekundy, Silnik::Akcja akcja){
	if(akcja == Silnik::START) {
		this->pozycja = 1;
		return Silnik::ROZGRYWKA;
	} else if(akcja == Silnik::WYBIERZ) {
		if(this->pozycja == 1) {
			this->pozycja = 1;
			return Silnik::ROZGRYWKA;
		} else if(this->pozycja == 2) {
			this->pozycja = 1;
		} else if(this->pozycja == 3) {

		} else if(this->pozycja == 4) {
			this->pozycja = 1;
			this->plansza->czysc();
			return Silnik::MENU;
		}
	} else if(akcja == Silnik::GORA) {
		if(this->pozycja > 1)
			this->pozycja--;
		else
			this->pozycja = 4;
	} else if(akcja == Silnik::DOL) {
		if(this->pozycja < 4)
			this->pozycja++;
		else
			this->pozycja = 1;
	}

	return Silnik::PAUZA;
}

void Pauza::rysuj() const{
	QPainter painter(&this->ekran->buforObrazu);
	painter.fillRect(this->ekran->buforObrazu.rect(), QColor(0, 0, 0, 128));
	painter.setPen(Qt::white);

	const QColor kolorZaznaczenia("#278fbb");

	int szerokoscEkranu = this->ekran->buforObrazu.width();
	int wysokoscEkranu = this->ekran->buforObrazu.height();
	QRectF obszarTytulu = QRectF(0, wysokoscEkranu * 0.08, szerokoscEkranu, 100);

	QFont czcionkaTytulu = painter.font();
	czcionkaTytulu.setFamily("Trebuchet MS");
	czcionkaTytulu.setPixelSize(wysokoscEkranu * 0.04);

	QFont czcionkaNormalna = painter.font();
	czcionkaNormalna.setFamily("Trebuchet MS");
	czcionkaNormalna.setPixelSize(wysokoscEkranu * 0.03);

	painter.setFont(czcionkaTytulu);
	Widzety::cieniowanyTekst(painter, obszarTytulu, "PAUZA", QTextOption(Qt::AlignHCenter));

	painter.setFont(czcionkaNormalna);

	if(this->pozycja == 1) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
	Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.45, szerokoscEkranu, 500), "Kontynuuj", QTextOption(Qt::AlignHCenter));

	if(this->pozycja == 2) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
	Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.5, szerokoscEkranu, 500), "Zrestartuj", QTextOption(Qt::AlignHCenter));

	if(this->pozycja == 3) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
	Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.55, szerokoscEkranu, 500), "Ustawienia", QTextOption(Qt::AlignHCenter));

	if(this->pozycja == 4) { painter.setPen(kolorZaznaczenia); } else { painter.setPen(Qt::white); }
	Widzety::cieniowanyTekst(painter, QRectF(0, wysokoscEkranu * 0.6, szerokoscEkranu, 500), "Wróć do menu", QTextOption(Qt::AlignHCenter));

	painter.end();
	this->ekran->update();
}
