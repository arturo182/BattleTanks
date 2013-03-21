#ifndef DZWIEK_H
#define DZWIEK_H

#include <QObject>
#include <QString>

class QAudioOutput;

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
		QAudioOutput *media;
};

#endif // DZWIEK_H
