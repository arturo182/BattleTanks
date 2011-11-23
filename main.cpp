#include <QApplication>
#include "silnik.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	
	Silnik silnik;
	silnik.uruchom();
	
	return app.exec();
}
