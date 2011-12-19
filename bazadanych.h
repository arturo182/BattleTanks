#ifndef BAZADANYCH_H
#define BAZADANYCH_H

#include <QVariant>
#include <QMap>

class BazaDanych{
  public:
	BazaDanych();
	bool polacz();

	QMap<int, QString> profile() const;

	QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const;
};

#endif // BAZADANYCH_H
