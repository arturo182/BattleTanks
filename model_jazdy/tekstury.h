#ifndef TEKSTURY_H
#define TEKSTURY_H

#include <QPixmap>

class Tekstury: private QPixmap{
	private:
		QSize ilosc_klatek;
		QSize rozmiar_klatki;
		
	public:
		Tekstury(QString, QSize);
		int liczba_klatek() const;
	
	friend class Animacja;
};

inline int Tekstury::liczba_klatek() const{
	return this->ilosc_klatek.width() * this->ilosc_klatek.height();
}

#endif // TEKSTURY_H
