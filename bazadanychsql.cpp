#include "bazadanychsql.h"

#include <QDomDocument>

#include <QStandardPaths>
#include <QApplication>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDir>

BazaDanychSql::BazaDanychSql()
{
}

bool BazaDanychSql::polacz()
{
	if(!BazaDanych::polacz())
		return false;

	// baza ustawien
	QSqlDatabase dbUstawienia = QSqlDatabase::addDatabase("QSQLITE", "dbUstawienia");
	QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation)).mkpath(".");
	dbUstawienia.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/ustawienia.db");
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
		"  rekord_id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"  profil_id integer, "
		"  wynik float(4,2), "
		"  mapa_id integer, "
		"  FOREIGN KEY (profil_id) REFERENCES profile(profil_id) "
		");");

		dbUstawienia.exec("CREATE TABLE ustawienia ("
		"  nazwa text PRIMARY KEY NOT NULL UNIQUE, "
		"  wartosc text "
		");");
	}

	return true;
}

QStringList BazaDanychSql::profile() const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.exec("SELECT * FROM profile ORDER BY LOWER(nazwa) ASC;");

	QStringList profile;
	while(query.next()) {
		profile << query.value(1).toString();
	}

	return profile;
}

void BazaDanychSql::dodajProfil(const QString &nazwa) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("INSERT INTO profile(nazwa) VALUES (:nazwa);");
	query.bindValue(":nazwa", nazwa);
	query.exec();
}

void BazaDanychSql::usunProfil(int id) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("DELETE FROM profile WHERE profil_id = :id;");
	query.bindValue(":id", id);
	query.exec();

	query.prepare("DELETE FROM rekordy WHERE profil_id = :id;");
	query.bindValue(":id", id);
	query.exec();
}

int BazaDanychSql::idProfilu(const QString &nazwa) const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("SELECT * FROM profile where nazwa = :nazwa ;");
	query.bindValue(":nazwa", nazwa);
	query.exec();
	query.next();

	return query.value(0).toInt();
}

QList<QStringList> BazaDanychSql::rekordy() const
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.exec("SELECT "
			   "  profile.nazwa, "
			   "  rekordy.mapa_id, "
			   "  rekordy.wynik "
			   "FROM "
			   "  rekordy "
			   "  INNER JOIN profile ON (rekordy.profil_id = profile.profil_id) "
			   "ORDER BY "
			   "  wynik DESC;");

	QList<QStringList> rekordy;

	QSqlQuery query2;
	while(query.next()) {
		QStringList rekord;

		query2.prepare("SELECT plik FROM plansze WHERE mapa_id = :mapa_id ;");
		query2.bindValue(":mapa_id", query.value(1));
		query2.exec();
		if(query2.next()) {
			rekord << query.value(0).toString();
			rekord << query2.value(0).toString();
			rekord << query.value(2).toString();
		}

		rekordy << rekord;
	}

	return rekordy;
}

void BazaDanychSql::zapiszRekord(int idGracza, int idPlanszy, float wynik)
{
	QSqlQuery query(QSqlDatabase::database("dbUstawienia"));
	query.prepare("INSERT INTO rekordy(profil_id, wynik, mapa_id) VALUES (:profil_id, :wynik, :mapa_id);");
	query.bindValue(":profil_id", idGracza);
	query.bindValue(":wynik", wynik);
	query.bindValue(":mapa_id", idPlanszy);
	query.exec();
}

QVariant BazaDanychSql::ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna) const
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

void BazaDanychSql::zapiszUstawienie(const QString &nazwa, const QVariant &wartosc)
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
