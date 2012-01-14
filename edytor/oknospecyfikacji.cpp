#include "oknospecyfikacji.h"
#include "ui_oknospecyfikacji.h"

#include <QFileDialog>
#include <QSqlQuery>
#include <QDebug>

OknoSpecyfikacji::OknoSpecyfikacji(const QString &plikTla, const int &pojazdGracza, const QMap<int, int> &pociskiGracza, QWidget *parent) :
QDialog(parent),
plikTla(plikTla),
pojazdGracza(pojazdGracza),
pociskiGracza(pociskiGracza),
ui(new Ui::OknoSpecyfikacji)
{
	this->ui->setupUi(this);

	connect(this->ui->buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(this->ui->buttonBox, SIGNAL(rejected()), SLOT(reject()));

	this->ui->tloEdit->setText(this->plikTla);

	QSqlQuery query("SELECT * FROM pociski;");
	while(query.next()) {
		int id = query.value(0).toInt();
		QString nazwa = query.value(1).toString();

		this->pociskiBaza.insert(id, nazwa);
		this->ui->pociskiComboBox->addItem(QIcon("grafika/bronie/" + nazwa + "Pocisk.png"), nazwa, id);

		if(pociskiGracza.contains(id)) {
			QTreeWidgetItem *item = new QTreeWidgetItem(this->ui->pociskiTreeWidget);
			item->setIcon(0, QIcon("grafika/bronie/" + nazwa + "Pocisk.png"));
			item->setText(0, nazwa);
			item->setText(1, QString::number(pociskiGracza.value(id)));
			item->setText(2, QString::number(id));
		}
	}

	query.exec("SELECT * FROM pojazdy;");
	while(query.next()) {
		int id = query.value(0).toInt();
		QString nazwa = query.value(1).toString();
		this->ui->graczComboBox->addItem(QIcon("grafika/pojazdy/" + nazwa + "Korpus.png"), nazwa, id);

		if(id == pojazdGracza)
			this->ui->graczComboBox->setCurrentIndex(this->ui->graczComboBox->count() - 1);
	}
}

OknoSpecyfikacji::~OknoSpecyfikacji()
{
	delete this->ui;
}

void OknoSpecyfikacji::przegladajTlo()
{
	QString nazwaPliku = QFileDialog::getOpenFileName(this, "Wybierz tło", "", "Pliki graficzne (*.bmp;*.png;*.jpg;*.jpeg)");
	if(!nazwaPliku.isEmpty()) {
		this->plikTla = nazwaPliku;
		this->ui->tloEdit->setText(this->plikTla);
	}
}

void OknoSpecyfikacji::dodajPocisk()
{
	int ilosc = this->ui->pociskiIloscSpinBox->value();
	int index = this->ui->pociskiComboBox->currentIndex();
	int id = this->ui->pociskiComboBox->itemData(index).toInt();

	QTreeWidgetItem *item = new QTreeWidgetItem(this->ui->pociskiTreeWidget);
	item->setIcon(0, this->ui->pociskiComboBox->itemIcon(index));
	item->setText(0, this->pociskiBaza.value(id));
	item->setText(1, QString::number(ilosc));
	item->setText(2, QString::number(id));

	this->pociskiGracza.insert(id, ilosc);

	this->ui->pociskiIloscSpinBox->setValue(0);
}

void OknoSpecyfikacji::usunPocisk()
{
	QTreeWidgetItem *current = this->ui->pociskiTreeWidget->currentItem();

	int id = current->text(3).toInt();
	this->pociskiGracza.remove(id);

	delete current;
}

void OknoSpecyfikacji::sprawdzDodajPocisk(int)
{
	int ilosc = this->ui->pociskiIloscSpinBox->value();
	int index = this->ui->pociskiComboBox->currentIndex();
	int id = this->ui->pociskiComboBox->itemData(index).toInt();

	this->ui->pociskiDodajButton->setEnabled((ilosc != 0) && (!this->pociskiGracza.contains(id)));
}

void OknoSpecyfikacji::zmienGracza(int index)
{
	this->pojazdGracza = this->ui->graczComboBox->itemData(index).toInt();
}

void OknoSpecyfikacji::on_pociskiTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	this->ui->pociskiUsunButton->setEnabled(current);
}
