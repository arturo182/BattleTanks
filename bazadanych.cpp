#include "bazadanych.h"

#include <QDesktopServices>
#include <QSqlDatabase>
#include <QApplication>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDir>

BazaDanych::BazaDanych(){
}

bool BazaDanych::polacz(){
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
		"  pocisk_id          integer PRIMARY KEY NOT NULL, "
		"  nazwa              varchar(255), "
		"  zasieg             integer, "
		"  predkosc           integer, "
		"  silaRazenie        integer, "
		"  promienRazenie     integer, "
		"  animacjaSzerokosc  smallint, "
		"  animacjaWysokosc   smallint, "
		"  animacjaCzas       integer "
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
	}

	// baza ustawien
	QSqlDatabase dbUstawienia = QSqlDatabase::addDatabase("QSQLITE", "dbUstawienia");
	QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).mkpath(".");
	dbUstawienia.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/ustawienia.db");
	if(!dbUstawienia.open()) {
		qDebug() << "Nie udalo sie poalczyc z baza danych" << dbUstawienia.databaseName() << dbUstawienia.lastError().text();
		return false;
	}

	if(!dbUstawienia.tables().count()) {
		dbUstawienia.exec("CREATE TABLE profile ("
		"  profil_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  nazwa varchar(80) UNIQUE "
		");");

		dbUstawienia.exec("CREATE TABLE rekordy ("
		"  rekord_id integer, "
		"  profil_id integer, "
		"  wynik float(4,2), "
		"  mapa_id integer, "
		"  FOREIGN KEY (mapa_id) REFERENCES profile(mapa_id), "
		"  FOREIGN KEY (profil_id) REFERENCES mapy(profil_id) "
		");");

		dbUstawienia.exec("CREATE TABLE ustawienia ("
		"  nazwa text PRIMARY KEY NOT NULL UNIQUE, "
		"  wartosc text "
		");");
	}

	return true;
}

QStringList BazaDanych::profile() const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.exec("SELECT * FROM profile ORDER BY LOWER(nazwa) ASC;");

	QStringList profile;
	while(query.next()) {
		profile << query.value(1).toString();
	}

	return profile;
}

void BazaDanych::dodajProfil(const QString &nazwa) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("INSERT INTO profile(nazwa) VALUES (:nazwa);");
	query.bindValue(":nazwa", nazwa);
	query.exec();
}

void BazaDanych::usunProfil(int id) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("DELETE FROM profile WHERE profil_id = :id;");
	query.bindValue(":id", id);
	query.exec();

	query.prepare("DELETE FROM rekordy WHERE profil_id = :id;");
	query.bindValue(":id", id);
	query.exec();
}

int BazaDanych::idProfilu(const QString &nazwa) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("SELECT * FROM profile where nazwa = :nazwa ;");
	query.bindValue(":nazwa", nazwa);
	query.exec();
	query.next();

	return query.value(0).toInt();
}

QList<QStringList> BazaDanych::rekordy() const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.exec("SELECT "
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

QList<QStringList> BazaDanych::mapy(int tryb) const
{
	QSqlQuery query;
	query.prepare("SELECT * FROM mapy WHERE tryb = :tryb ORDER BY LOWER(plik) ASC");
	query.bindValue(":tryb", tryb);
	query.exec();

	QList<QStringList> mapy;
	while(query.next()) {
		QStringList mapa;

		mapa << query.value(0).toString();
		mapa << query.value(1).toString();
		mapa << query.value(2).toString();

		mapy << mapa;
	}

	return mapy;
}

QVariant BazaDanych::ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("SELECT wartosc FROM ustawienia WHERE nazwa = :nazwa ;");
	query.bindValue(":nazwa", nazwa);
	query.exec();

	QVariant wartosc;
	if(query.first())
		wartosc = query.value(0);

	return (wartosc.isNull()) ? wartoscDomyslna : wartosc;
}

void BazaDanych::zapiszUstawienie(const QString &nazwa, const QVariant &wartosc)
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("UPDATE ustawienia SET wartosc = :wartosc WHERE nazwa = :nazwa ;");
	query.bindValue(":nazwa", nazwa);
	query.bindValue(":wartosc", wartosc);
	query.exec();

	if(!query.numRowsAffected()) {
		query.prepare("INSERT INTO ustawienia(nazwa, wartosc) VALUES (:nazwa, :wartosc);");
		query.bindValue(":nazwa", nazwa);
		query.bindValue(":wartosc", wartosc);
		query.exec();
	}
}
