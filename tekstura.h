#ifndef TEKSTURA_H
#define TEKSTURA_H

#include <QPixmap>

class Tekstura{
	private:
		static QList<Tekstura*> tekstury;
		
	public:
		QPixmap teksturaOryginalna;
		QPixmap teksturaPrzeskalowana;
		
		Tekstura(const QPixmap&);
		Tekstura(const Tekstura&);
		~Tekstura();
		void przeskaluj(float);
		static void przeskalujWszystko(float);
};

#endif // TEKSTURA_H
