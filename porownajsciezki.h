#ifndef POROWNAJSCIEZKI_H
#define POROWNAJSCIEZKI_H

class PorownajSciezki{
	private:
		static float* odleglosci;
		
	public:
		inline static void ustawTabliceOdleglosci(float*);
		inline bool operator()(const int&, const int&) const;
};

void PorownajSciezki::ustawTabliceOdleglosci(float* odleglosci){
	PorownajSciezki::odleglosci = odleglosci;
}

bool PorownajSciezki::operator()(const int& v, const int& w) const{
	if(PorownajSciezki::odleglosci[v] != PorownajSciezki::odleglosci[w])
		return  PorownajSciezki::odleglosci[v] < PorownajSciezki::odleglosci[w];
	else
		return v < w;
}

#endif // POROWNAJSCIEZKI_H
