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
		db.exec("CREATE TABLE bronie ("
		"  bron_id integer PRIMARY KEY NOT NULL, "
		"  rodzaj smallint, "
		"  promienRazenie integer, "
		"  silaRazenie integer, "
		"  zasieg integer, "
		"  predkosc integer "
		");");

		db.exec("CREATE TABLE mapy ("
		"  mapa_id integer PRIMARY KEY NOT NULL, "
		"  tryb smallint, "
		"  plik varchar(255) "
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

QMap<int, QString> BazaDanych::profile() const
{
	QSqlQuery query("SELECT * FROM profile ORDER BY nazwa ASC;");

	QMap<int, QString> profile;
	while(query.next()) {
		profile.insert(query.value(0).toInt(), query.value(1).toString());
	}

	return profile;
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
