#ifndef BAZADANYCH_H
#define BAZADANYCH_H

#include <QStringList>
#include <QVariant>

class BazaDanych
{

  public:
	BazaDanych() { }
	virtual ~BazaDanych() { }

	virtual bool polacz();

	virtual QStringList profile() const = 0;
	virtual void dodajProfil(const QString &nazwa) const = 0;
	virtual void usunProfil(int id) const = 0;
	virtual int idProfilu(const QString &nazwa) const = 0;

	virtual QList<QStringList> rekordy() const = 0;
	virtual void zapiszRekord(int idGracza, int idPlanszy, float wynik) = 0;

	virtual QList<QStringList> plansze(int tryb = -1) const;

	virtual QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const = 0;
	virtual void zapiszUstawienie(const QString &nazwa, const QVariant &wartosc) = 0;
};

#endif // BAZADANYCH_H
