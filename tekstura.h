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
		void przeskaluj(double);
		static void przeskalujWszystko(double);
};

#endif // TEKSTURA_H
