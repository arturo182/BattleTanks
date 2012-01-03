#include <QApplication>
#include <QTextCodec>
#include "silnik.h"

int main(int argc, char* argv[]){
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QApplication app(argc, argv);

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
