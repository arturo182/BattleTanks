#ifndef WIERZCHOLEK_H
#define WIERZCHOLEK_H

#include <QPoint>
#include <QList>

class Wierzcholek{
	private:
		QPoint pozycja;
		QList<int> krawedzie;
		
	public:
		Wierzcholek(QPoint);
	
	friend class Graf;
	friend class Logika;
	// DEBUG
	friend class Plansza;
};

#endif // WIERZCHOLEK_H
