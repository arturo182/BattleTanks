#include "specyfikacjapojazdu.h"

SpecyfikacjaPojazdu::SpecyfikacjaPojazdu(const QPixmap& teksturaKorpus, const QPixmap& teksturaWieza, int przesuniecieOsiDlaKorpusu, int przesuniecieOsiDlaWiezy, int predkoscMaksymalnaPojazdu, float predkoscRotacjiWiezy, int wytrzymalosc):
	teksturaKorpus(teksturaKorpus),
	teksturaWieza(teksturaWieza),
	rozmiar(this->teksturaKorpus.teksturaOryginalna.size()),
	przesuniecieOsiDlaKorpusu(przesuniecieOsiDlaKorpusu),
	przesuniecieOsiDlaWiezy(przesuniecieOsiDlaWiezy),
	predkoscMaksymalnaPojazdu(predkoscMaksymalnaPojazdu),
	predkoscRotacjiWiezy(predkoscRotacjiWiezy),
	wytrzymalosc(wytrzymalosc){
}
