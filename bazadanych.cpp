#include "bazadanych.h"

#include <QSqlDatabase>
#include <QApplication>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

BazaDanych::BazaDanych(){
}

bool BazaDanych::polacz(){
	//	nawiazanie polaczenia z baza
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(qApp->applicationDirPath() + "/baza.db");

	if(!db.open()) {
		qDebug() << "Nie udalo sie poalczyc z baza danych" << db.databaseName() << db.lastError().text();
		return false;
	}

	// sprawdzamy czy tabele w bazie danych istnieją
	if(!db.tables().count()) {
		db.exec("CREATE TABLE pociski ("
		"  pocisk_id       integer PRIMARY KEY NOT NULL, "
		"  nazwa           varchar(255), "
		"  rodzaj          smallint, "
		"  zasieg          integer, "
		"  predkosc        integer, "
		"  silaRazenie     integer, "
		"  promienRazenie  integer "
		");");

		db.exec("CREATE TABLE mapy ("
		"  mapa_id integer PRIMARY KEY NOT NULL, "
		"  tryb smallint, "
		"  plik varchar(255) "
		");");

		db.exec("CREATE TABLE pojazdy ("
		"  pojazd_id             integer PRIMARY KEY NOT NULL, "
		"  nazwa                 varchar(255), "
		"  przesuniecieOsKorpus  float(6,2), "
		"  przesuniencieOsWieza  float(6,2), "
		"  predkoscMaksymalna    float(6,2), "
		"  predkoscWiezy         float(6,2), "
		"  wytrzymalosc          integer "
		");");

		db.exec("CREATE TABLE profile ("
		"  profil_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  nazwa varchar(80) UNIQUE "
		");");

		db.exec("CREATE TABLE rekordy ("
		"  rekord_id integer, "
		"  profil_id integer, "
		"  wynik float(4,2), "
		"  mapa_id integer, "
		"  FOREIGN KEY (mapa_id) REFERENCES profile(mapa_id), "
		"  FOREIGN KEY (profil_id) REFERENCES mapy(profil_id) "
		");");

		db.exec("CREATE TABLE ustawienia ("
		"  nazwa text PRIMARY KEY NOT NULL UNIQUE, "
		"  wartosc text "
		");");
	}


	return true;
}

QStringList BazaDanych::profile() const
{
	QSqlQuery query("SELECT * FROM profile ORDER BY LOWER(nazwa) ASC;");

	QStringList profile;
	while(query.next()) {
		profile << query.value(1).toString();
	}

	return profile;
}

int BazaDanych::idProfilu(const QString &nazwa) const
{
	QSqlQuery query;
	query.prepare("SELECT * FROM profile where nazwa = :nazwa");
	query.bindValue(":nazwa", nazwa);
	query.exec();
	query.next();

	return query.value(0).toInt();
}

QList<QStringList> BazaDanych::rekordy() const
{
	QSqlQuery query("SELECT "
					"  profile.nazwa, "
					"  mapy.plik, "
					"  rekordy.wynik "
					"FROM "
					"  rekordy "
					"  INNER JOIN profile ON (rekordy.profil_id = profile.profil_id) "
					"  INNER JOIN mapy ON (rekordy.mapa_id = mapy.mapa_id) "
					"ORDER BY "
					"  wynik DESC;");

	QList<QStringList> rekordy;

	while(query.next()) {
		QStringList rekord;

		rekord << query.value(0).toString();
		rekord << query.value(1).toString();
		rekord << query.value(2).toString();


		rekordy << rekord;
	}

	return rekordy;
}

QVariant BazaDanych::ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna) const
{
	QSqlQuery query;
	query.prepare("SELECT wartosc FROM ustawienia WHERE nazwa = :nazwa");
	query.bindValue(":nazwa", nazwa);
	query.exec();

	QVariant wartosc;
	if(query.first())
		wartosc = query.value(0);

	return (wartosc.isNull()) ? wartoscDomyslna : wartosc;
}

void BazaDanych::zapiszUstawienie(const QString &nazwa, const QVariant &wartosc)
{
	QSqlQuery query;
	query.prepare("UPDATE ustawienia SET wartosc = :wartosc WHERE nazwa = :nazwa");
	query.bindValue(":nazwa", nazwa);
	query.bindValue(":wartosc", wartosc);
	query.exec();

	if(!query.numRowsAffected()) {
		query.prepare("INSERT INTO ustawienia(nazwa, wartosc) VALUES (:nazwa, :wartosc)");
		query.bindValue(":nazwa", nazwa);
		query.bindValue(":wartosc", wartosc);
		query.exec();
	}
}
