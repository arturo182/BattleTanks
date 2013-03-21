#ifndef BAZADANYCHXML_H
#define BAZADANYCHXML_H

#include "bazadanychsql.h"
#include "bazadanych.h"

class BazaDanychXml : public BazaDanychSql
{
	public:
		BazaDanychXml();

		bool polacz();

		QStringList profile() const;
		void dodajProfil(const QString &nazwa) const;
		void usunProfil(int id) const;
		int idProfilu(const QString &nazwa) const;

		QList<QStringList> rekordy() const;
		void zapiszRekord(int idGracza, int idPlanszy, float wynik);

		QMap<QString, QVariant> ustawienia() const;
		QVariant ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna = QVariant()) const;
		void zapiszUstawienie(const QString &nazwa, const QVariant &wartosc);
};

#endif // BAZADANYCHXML_H
