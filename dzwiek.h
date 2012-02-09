#ifndef DZWIEK_H
#define DZWIEK_H

//#include <Phonon/MediaObject>
#include <QObject>
#include <QString>

class Dzwiek: public QObject
{
	Q_OBJECT

	public:
		static int glosnosc;

	public:
		Dzwiek(const QString &nazwaPliku);
		~Dzwiek();

		void odtworz() const;

		static void odtworz(const QString &nazwaPliku);

	signals:
		void zakonczony();

	private:
		QString nazwaPliku;
		//Phonon::MediaObject *media;
};

#endif // DZWIEK_H
