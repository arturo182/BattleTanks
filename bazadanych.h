#ifndef BAZADANYCH_H
#define BAZADANYCH_H

#include <QVariant>
#include <QMap>

class BazaDanych{
  public:
	BazaDanych();
	bool polacz();

	QStringList profile() const;
	void dodajProfil(const QString &nazwa) const;
	void usunProfil(int id) const;
	int idProfilu(const QString &nazwa) const;

	QList<QStringList> rekordy() const;
	void zapiszRekord(int idGracza, int idPlanszy, float wynik);

	QList<QStringList> plansze(int tryb = -1) const;

	QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const;
	void zapiszUstawienie(const QString &nazwa, const QVariant &wartosc);
};

#endif // BAZADANYCH_H
