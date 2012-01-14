#include "oknopojazduobcego.h"
#include "ui_oknopojazduobcego.h"

#include <QSqlQuery>
#include <QDebug>

OknoPojazduObcego::OknoPojazduObcego(int pojazd, int pocisk, QWidget *parent) :
QDialog(parent),
ui(new Ui::OknoPojazduObcego)
{
	this->ui->setupUi(this);

	connect(this->ui->buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(this->ui->buttonBox, SIGNAL(rejected()), SLOT(reject()));

	this->obcyPojazd.pojazd = pojazd;
	this->obcyPojazd.pocisk = pocisk;

	QSqlQuery query("SELECT * FROM pociski;");
	while(query.next()) {
		int id = query.value(0).toInt();
		QString nazwa = query.value(1).toString();

		this->ui->pociskiComboBox->addItem(QIcon("grafika/bronie/" + nazwa + "Pocisk.png"), nazwa, id);
		if(id == pocisk)
			this->ui->pociskiComboBox->setCurrentIndex(this->ui->pociskiComboBox->count() - 1);
	}

	this->ui->pojazdyComboBox->addItem("Brak obcego pojazdu", "-1");

	query.exec("SELECT * FROM pojazdy;");
	while(query.next()) {
		int id = query.value(0).toInt();
		QString nazwa = query.value(1).toString();

		this->ui->pojazdyComboBox->addItem(QIcon("grafika/pojazdy/" + nazwa + "Korpus.png"), nazwa, id);

		if(id == pojazd)
			this->ui->pojazdyComboBox->setCurrentIndex(this->ui->pojazdyComboBox->count() - 1);
	}
}

OknoPojazduObcego::~OknoPojazduObcego()
{
	delete this->ui;
}

void OknoPojazduObcego::ustawPocisk(int index)
{
		qDebug() << this->ui->pociskiComboBox->itemData(index).toInt();
	this->obcyPojazd.pocisk = this->ui->pociskiComboBox->itemData(index).toInt();
}

void OknoPojazduObcego::ustawPojazd(int index)
{
	this->obcyPojazd.pojazd = this->ui->pojazdyComboBox->itemData(index).toInt();
}
