#include "specyfikacjapojazdu.h"

SpecyfikacjaPojazdu::SpecyfikacjaPojazdu(const QPixmap& teksturaKorpus, const QPixmap& teksturaWieza, int przesuniecieOsiDlaKorpusu, int przesuniecieOsiDlaWiezy, int predkoscMaksymalnaPojazdu, float predkoscRotacjiWiezy, int wytrzymalosc):
	teksturaKorpus(teksturaKorpus),
	teksturaWieza(teksturaWieza),
	rozmiarKorpus(this->teksturaKorpus.teksturaOryginalna.size()),
	rozmiarWieza(this->teksturaWieza.teksturaOryginalna.size()),
	przesuniecieOsiDlaKorpusu(przesuniecieOsiDlaKorpusu),
	przesuniecieOsiDlaWiezy(przesuniecieOsiDlaWiezy),
	predkoscMaksymalnaPojazdu(predkoscMaksymalnaPojazdu),
	predkoscRotacjiWiezy(predkoscRotacjiWiezy),
	wytrzymalosc(wytrzymalosc){}
