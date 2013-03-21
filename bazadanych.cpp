#include "bazadanych.h"

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

bool BazaDanych::polacz()
{
	QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).mkpath(".");

	//	nawiazanie polaczenia z baza
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("baza.db");

	if(!db.open()) {
		qDebug() << "Nie udalo sie poalczyc z baza danych" << db.databaseName() << db.lastError().text();
		return false;
	}

	// sprawdzamy czy tabele w bazie danych istnieją
	if(!db.tables().count()) {
		db.exec("CREATE TABLE pociski ("
		"  pocisk_id          integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  nazwa              varchar(255), "
		"  zasieg             integer, "
		"  predkosc           integer, "
		"  silaRazenie        integer, "
		"  promienRazenie     integer, "
		"  animacjaSzerokosc  smallint, "
		"  animacjaWysokosc   smallint, "
		"  animacjaCzas       integer "
		");");

		db.exec("CREATE TABLE plansze ("
		"  mapa_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  tryb smallint, "
		"  plik varchar(255) "
		");");

		db.exec("CREATE TABLE pojazdy ("
		"  pojazd_id             integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  nazwa                 varchar(255), "
		"  przesuniecieOsKorpus  float(6,2), "
		"  przesuniencieOsWieza  float(6,2), "
		"  predkoscMaksymalna    float(6,2), "
		"  predkoscWiezy         float(6,2), "
		"  wytrzymalosc          integer "
		");");
	}

	return true;
}

QList<QStringList> BazaDanych::plansze(int tryb) const
{
	QSqlQuery query;
	if(tryb > -1) {
		query.prepare("SELECT * FROM plansze WHERE tryb = :tryb ORDER BY LOWER(plik) ASC;");
		query.bindValue(":tryb", tryb);
	} else {
		query.prepare("SELECT * FROM plansze ORDER BY LOWER(plik) ASC;");
	}

	query.exec();

	QList<QStringList> plansze;
	while(query.next()) {
		QStringList plansza;

		plansza << query.value(0).toString();
		plansza << query.value(1).toString();
		plansza << query.value(2).toString();

		plansze << plansza;
	}

	return plansze;
}
