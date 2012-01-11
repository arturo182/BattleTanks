#include "funkcje.h"

#include <QStringList>

QString qSizeToString(const QSize &size)
{
	return QString("%1x%2").arg(size.width()).arg(size.height());
}

QSize qStringToSize(const QString &str)
{
	int w = 0;
	int h = 0;

	QStringList lista = str.split("x");
	if(lista.size() == 2) {
		w = lista.at(0).toInt();
		h = lista.at(1).toInt();
	}

	return QSize(w, h);
}
