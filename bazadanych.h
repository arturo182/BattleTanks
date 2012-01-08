#ifndef BAZADANYCH_H
#define BAZADANYCH_H

#include <QVariant>
#include <QMap>

class BazaDanych{
  public:
	BazaDanych();
	bool polacz();

	QStringList profile() const;
	int idProfilu(const QString &nazwa) const;

	QList<QStringList> rekordy() const;

	QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const;
	void zapiszUstawienie(const QString &nazwa, const QVariant &wartosc);
};

#endif // BAZADANYCH_H
