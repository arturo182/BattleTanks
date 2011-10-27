#include <QApplication>
#include <SDL/SDL.h>
#include "plansza.h"
#include <QMessageBox>

#undef main

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	
	QPixmap mapa("map.jpg");
	Plansza plansza(mapa);
	plansza.show();
	
	return app.exec();
}
