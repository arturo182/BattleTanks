#include "bazadanychxml.h"

#include <QStandardPaths>
#include <QDomDocument>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

BazaDanychXml::BazaDanychXml()
{
}

bool BazaDanychXml::polacz()
{
	if(!BazaDanych::polacz())
		return false;

	return true;
}

QStringList BazaDanychXml::profile() const
{
	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/profiles.xml");
	plik.open(QIODevice::ReadOnly);

	QStringList profile;

	QDomDocument doc;
	doc.setContent(&plik);
	QDomElement korzen = doc.documentElement();
	for(QDomNode elem = korzen.firstChild(); !elem.isNull(); elem = elem.nextSibling()) {
		QDomText tekst = elem.firstChild().toText();

		profile << tekst.nodeValue();
	}

	plik.close();

	return profile;
}

void BazaDanychXml::dodajProfil(const QString &nazwa) const
{
	QStringList profile = this->profile();
	profile << nazwa;
	profile.removeDuplicates();

	QDomDocument doc;
	QDomElement root = doc.createElement("profile");

	foreach(const QString &profile, profile) {
		QDomElement elem = doc.createElement("profil");
		elem.appendChild(doc.createTextNode(profile));
		root.appendChild(elem);
	}

	doc.appendChild(root);

	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/profiles.xml");
	plik.open(QIODevice::WriteOnly);
	plik.write(doc.toString(2).toLatin1());
	plik.close();
}

void BazaDanychXml::usunProfil(int id) const
{
	QStringList profile = this->profile();
	profile.removeAt(id);

	QDomDocument doc;
	QDomElement root = doc.createElement("profile");

	foreach(const QString &profile, profile) {
		QDomElement elem = doc.createElement("profil");
		elem.appendChild(doc.createTextNode(profile));
		root.appendChild(elem);
	}

	doc.appendChild(root);

	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/profiles.xml");
	plik.open(QIODevice::WriteOnly);
	plik.write(doc.toString(2).toLatin1());
	plik.close();
}

int BazaDanychXml::idProfilu(const QString &nazwa) const
{
	return this->profile().indexOf(nazwa);
}

QList<QStringList> BazaDanychXml::rekordy() const
{
	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/highscores.xml");
	plik.open(QIODevice::ReadOnly);

	QList<QStringList> rekordy;

	QDomDocument doc;
	doc.setContent(&plik);
	QDomElement korzen = doc.documentElement();
	for(QDomNode elem = korzen.firstChild(); !elem.isNull(); elem = elem.nextSibling()) {
		QDomElement e = elem.toElement();

		QSqlQuery query;
		query.prepare("SELECT plik FROM plansze WHERE mapa_id = :mapa_id ;");
		query.bindValue(":mapa_id", e.attribute("mapaId"));
		query.exec();

		if(!query.isValid())
			qDebug() << query.lastError().text();

		QStringList rekord;
		rekord << profile().at(e.attribute("profilId").toInt());
		rekord << query.value(0).toString();
		rekord << e.attribute("wynik");

		rekordy << rekord;
	}

	plik.close();

	return rekordy;
}

void BazaDanychXml::zapiszRekord(int idGracza, int idPlanszy, float wynik)
{
	QList<QStringList> rekordy = this->rekordy();

	QStringList rekord;
	rekord << profile().at(idGracza);
	rekord << QString::number(idPlanszy);
	rekord << QString::number(wynik);
	rekordy << rekord;

	QDomDocument doc;
	QDomElement root = doc.createElement("rekordy");

	foreach(const QStringList &rekord, rekordy) {
		QDomElement rElem = doc.createElement("rekord");
		rElem.setAttribute("mapaId", rekord[1]);
		rElem.setAttribute("profilId", idProfilu(rekord[0]));
		rElem.setAttribute("wynik", rekord[2]);

		root.appendChild(rElem);
	}

	doc.appendChild(root);

	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/highscores.xml");
	plik.open(QIODevice::WriteOnly);
	plik.write(doc.toString(2).toLatin1());
	plik.close();
}

QMap<QString, QVariant> BazaDanychXml::ustawienia() const
{
	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/settings.xml");
	plik.open(QIODevice::ReadOnly);

	QMap<QString, QVariant> ustawienia;

	QDomDocument doc;
	doc.setContent(&plik);
	QDomElement korzen = doc.documentElement();
	for(QDomNode elem = korzen.firstChild(); !elem.isNull(); elem = elem.nextSibling()) {
		QDomElement e = elem.toElement();

		ustawienia.insert(e.attribute("nazwa"), e.attribute("wartosc"));
	}

	plik.close();

	return ustawienia;
}

QVariant BazaDanychXml::ustawienie(const QString &nazwa, const QVariant &wartoscDomyslna) const
{
	return ustawienia().value(nazwa, wartoscDomyslna);
}

void BazaDanychXml::zapiszUstawienie(const QString &nazwa, const QVariant &wartosc)
{
	QMap<QString, QVariant> ustawienia = this->ustawienia();
	ustawienia.insert(nazwa, wartosc);

	QDomDocument doc;
	QDomElement root = doc.createElement("ustawienia");

	QMapIterator<QString, QVariant> it(ustawienia);
	while(it.hasNext()) {
		it.next();

		QDomElement elem = doc.createElement("ustawienie");
		elem.setAttribute("wartosc", it.value().toString());
		elem.setAttribute("nazwa", it.key());
		root.appendChild(elem);
	}

	doc.appendChild(root);

	QFile plik(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/settings.xml");
	plik.open(QIODevice::WriteOnly);
	plik.write(doc.toString(2).toLatin1());
	plik.close();
}
