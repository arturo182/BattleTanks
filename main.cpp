#include <QApplication>
#include <QTextCodec>
#include "silnik.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("BattleTanks");

	Silnik silnik;
	silnik.uruchom();
	forever {
		app.processEvents();
		silnik.odswiez();

		if(silnik.czyWyjsc())
			break;
	}

	return 0;
}
