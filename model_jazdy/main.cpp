#include <QApplication>
#include "plansza.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	Plansza plansza;
	plansza.show();
	
	return app.exec();
}
