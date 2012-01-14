#ifndef LADOWANIE_H
#define LADOWANIE_H

#include <QPixmap>

class Ekran;

class Ladowanie : public QObject
{
	Q_OBJECT

	public:
		Ladowanie(Ekran *ekran);

		void rysuj() const;

		void odswiez(float odswiez, const QString &opisPostepu = QString());
	private:
		Ekran* ekran;
		float postep;
		QString opisPostepu;
		QPixmap logoPixmapa;
};

#endif // LADOWANIE_H
