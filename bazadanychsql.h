#ifndef BAZADANYCHSQL_H
#define BAZADANYCHSQL_H

#include "bazadanych.h"

class BazaDanychSql : public BazaDanych
{
  public:
	BazaDanychSql();
	bool polacz();

	QStringList profile() const;
	void dodajProfil(const QString &nazwa) const;
	void usunProfil(int id) const;
	int idProfilu(const QString &nazwa) const;

	QList<QStringList> rekordy() const;
	void zapiszRekord(int idGracza, int idPlanszy, float wynik);

	QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const;
	void zapiszUstawienie(const QString &nazwa, const QVariant &wartosc);
};

#endif // BAZADANYCHSQL_H
