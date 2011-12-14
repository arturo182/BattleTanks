#include "specyfikacjapojazdu.h"

SpecyfikacjaPojazdu::SpecyfikacjaPojazdu(const QPixmap& teksturaKorpus, const QPixmap& teksturaWieza, int przesuniecieWiezy, int predkoscMaksymalna, int wytrzymalosc):
	teksturaKorpus(teksturaKorpus),
	teksturaWieza(teksturaWieza),
	przesuniecieWiezy(przesuniecieWiezy),
	predkoscMaksymalna(predkoscMaksymalna),
	wytrzymalosc(wytrzymalosc){
}
