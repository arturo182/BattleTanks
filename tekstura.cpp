#include "tekstura.h"
#include "obiekt.h"

QList<Tekstura*> Tekstura::tekstury;

Tekstura::Tekstura(const QPixmap& tekstura):
	teksturaOryginalna(tekstura){
	this->przeskaluj(Obiekt::skala);
	Tekstura::tekstury.append(this);
}

Tekstura::Tekstura(const Tekstura& tekstura):
	teksturaOryginalna(tekstura.teksturaOryginalna),
	teksturaPrzeskalowana(tekstura.teksturaPrzeskalowana){
	Tekstura::tekstury.append(this);
}

Tekstura::~Tekstura(){
	Tekstura::tekstury.removeOne(this);
}

void Tekstura::przeskaluj(double skala){
	this->teksturaPrzeskalowana = this->teksturaOryginalna.scaled(
		skala * this->teksturaOryginalna.size(),
		Qt::IgnoreAspectRatio,
		Qt::SmoothTransformation
	);
}

void Tekstura::przeskalujWszystko(double skala){
	for(QList<Tekstura*>::iterator i = Tekstura::tekstury.begin(); i != Tekstura::tekstury.end(); i++)
		(*i)->przeskaluj(skala);
}
