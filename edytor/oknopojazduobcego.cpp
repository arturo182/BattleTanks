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

	int i = 0;
	QSqlQuery query("SELECT * FROM pociski;");
	while(query.next()) {
		QString nazwa = query.value(1).toString();

		this->ui->pociskiComboBox->addItem(QIcon("grafika/bronie/" + nazwa + "Pocisk.png"), nazwa, i);
		if(i == pocisk)
			this->ui->pociskiComboBox->setCurrentIndex(this->ui->pociskiComboBox->count() - 1);

		i++;
	}

	this->ui->pojazdyComboBox->addItem("Brak obcego pojazdu", "-1");

	i = 0;
	query.exec("SELECT * FROM pojazdy;");
	while(query.next()) {
		QString nazwa = query.value(1).toString();

		this->ui->pojazdyComboBox->addItem(QIcon("grafika/pojazdy/" + nazwa + "Korpus.png"), nazwa, i);

		if(i == pojazd)
			this->ui->pojazdyComboBox->setCurrentIndex(this->ui->pojazdyComboBox->count() - 1);

		i++;
	}
}

OknoPojazduObcego::~OknoPojazduObcego()
{
	delete this->ui;
}

void OknoPojazduObcego::ustawPocisk(int index)
{
	this->obcyPojazd.pocisk = this->ui->pociskiComboBox->itemData(index).toInt();
}

void OknoPojazduObcego::ustawPojazd(int index)
{
	this->obcyPojazd.pojazd = this->ui->pojazdyComboBox->itemData(index).toInt();
}
