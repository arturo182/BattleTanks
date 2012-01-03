#ifndef SCENA_H
#define SCENA_H

#include <QGraphicsScene>

class Scena : public QGraphicsScene
{
	Q_OBJECT

	public:
		enum Tryby{
			ZAZNACZANIE = 0,
			PRZESUWANIE_WIDOKU,
			PRZESUWANIE_PRZESZKODY,
			EDYCJA_WIERZCHOLKOW
		};

	public:
		Scena(): QGraphicsScene() { }
};

#endif // SCENA_H
