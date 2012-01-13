#include <QApplication>
#include <QTextCodec>

#include "oknoglowne.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QApplication a(argc, argv);

	OknoGlowne w;
	w.show();

	return a.exec();
}
