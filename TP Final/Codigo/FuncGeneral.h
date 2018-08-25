
///********************************************************
///                 LOGARITMO
///********************************************************
template <typename T>

void logImg(CImg<T> &img){
	cimg_forXY(img,x,y){
		if(img(x,y)<0)
			img(x,y) = -1*log10(img(x,y)*-1+1);
		else
			img(x,y) = log10(img(x,y)+1);
	}
}

template <typename T>
CImg<T> get_logImg(CImg<T> img){
	CImg<T> r(img.get_fill(0));
	cimg_forXY(r,x,y){
		if(img(x,y)<0)
			r(x,y) = -1*log10(img(x,y)*-1+1);
		else
			r(x,y) = log10(img(x,y)+1);
	}
	return r;
}

///********************************************************
///                 GET CHAR (anda maso)
///********************************************************
template <typename T>
CImg<char> get_char(CImg<T> img){
	CImg<char> r(img.get_fill(0));
	cimg_forXY(r,x,y){
		r(x,y) = abs (char(img(x,y)));
	}
	return r;
}

///********************************************************
///                    SUMA CERO
///********************************************************
template <typename T>
void sumaUno(CImg<T> &img){
	double suma=0,val=0;
	cimg_forXY(img,x,y){
		suma+=img(x,y);
	}
	img = img/suma;
}

///********************************************************
///                    SUMA CERO
///********************************************************
template <typename T>
void sumaCero(CImg<T> &img){
	double suma=0,x=img.width(),y=img.height();
	cimg_forXY(img,x,y){
		suma+=sqrt(img(x,y)*img(x,y));
	}
	img = (img - 1/(x*y))/suma;
}


///********************************************************
///                    UMBRAL
///********************************************************
template <typename T>
void umbral255(CImg<T> &img, int umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y)=0;
		if(img(x,y)>=umbral)
			aux(x,y)=1;
	}
	img=aux;
}

template <typename T>
void umbral01(CImg<T> &img, double umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y)=0;
		if(img(x,y)>=umbral)
			aux(x,y)=1;
	}
	img=aux;
}


template <typename T>
CImg<T> get_umbral255(CImg<T> img, int umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y)=0;
		if(img(x,y)>=umbral)
			aux(x,y)=1;
	}
	
	return aux;
}

template <typename T>
CImg<T> get_umbral01(CImg<T> img, double umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y)=0;
		if(img(x,y)>=umbral)
			aux(x,y)=1;
	}
	return aux;
}

///              RGB
template <typename T>
CImg<T> get_umbralRGB(CImg<T> img, T *umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y,0)=0;aux(x,y,1)=0;aux(x,y,2)=0;
		if(img(x,y,0)>=umbral[0] && img(x,y,1)>=umbral[1] && img(x,y,2)>=umbral[2])
		{aux(x,y,0)=1;aux(x,y,1)=1;aux(x,y,2)=1;}
	}
	return aux;
}

template <typename T>
void umbralRGB(CImg<T> &img, T *umbral){
	CImg<T> aux(img);
	cimg_forXY(img,x,y){
		aux(x,y,0)=0;aux(x,y,1)=0;aux(x,y,2)=0;
		if(img(x,y,0)>=umbral[0] && img(x,y,1)>=umbral[1] && img(x,y,2)>=umbral[2])
		{aux(x,y,0)=1;aux(x,y,1)=1;aux(x,y,2)=1;}
	}
	img=aux;
}


template <typename T>
void umbralRGB(CImg<T> &img, T *mas, T *menos){
	CImg<T> aux(img);
	bool mass, menoss;
	cimg_forXY(img,x,y){
		aux(x,y,0)=0;aux(x,y,1)=0;aux(x,y,2)=0;
		mass = img(x,y,0)>=mas[0] && img(x,y,1)>=mas[1] && img(x,y,2)>=mas[2];
		menoss = img(x,y,0)<=menos[0] && img(x,y,1)<=menos[1] && img(x,y,2)<=menos[2];
		if(mass && menoss)
		{aux(x,y,0)=1;aux(x,y,1)=1;aux(x,y,2)=1;}
	}
	img=aux;
}


template <typename T>
void umbralRGBColor(CImg<T> &img, T *mas, T *menos){
	CImg<T> aux(img);
	bool mass, menoss;
	cimg_forXY(img,x,y){
		aux(x,y,0)=0;aux(x,y,1)=0;aux(x,y,2)=0;
		mass = img(x,y,0)>=mas[0] && img(x,y,1)>=mas[1] && img(x,y,2)>=mas[2];
		menoss = img(x,y,0)<=menos[0] && img(x,y,1)<=menos[1] && img(x,y,2)<=menos[2];
		if(mass && menoss)
		{aux(x,y,0)=img(x,y,0);aux(x,y,1)=img(x,y,1);aux(x,y,2)=img(x,y,2);}
	}
	img=aux;
}


template <typename T>
void delUmbral255(CImg<T> &img, int umbral,bool masMenos){
	CImg<T> aux(img);
	if(masMenos){
		cimg_forXY(img,x,y){
			if(img(x,y)>=umbral)
				aux(x,y)=0;
	}}else{
		cimg_forXY(img,x,y){
			if(img(x,y)<=umbral)
				aux(x,y)=0;
	}}
	img=aux;
}

///********************************************************
///                    Eliminar un color
///********************************************************
template <typename T>
void delRGB(CImg<T> &img, T *color){
	cimg_forXY(img,x,y){
		if(img(x,y,0)==color[0] && img(x,y,1)==color[1] && img(x,y,2)==color[2])
			{img(x,y,0)=0;img(x,y,1)=0;img(x,y,2)=0;}
	}
}


template <typename T>
void delTonos(CImg<T> &img, double umbral){
	cimg_forXY(img,x,y){
		double media = (img(x,y,0) + img(x,y,1) + img(x,y,2))/3.0;
		if(fabs(img(x,y,0)-media)<umbral && fabs(img(x,y,1)-media)<umbral && fabs(img(x,y,2)-media)<umbral)
		{img(x,y,0)=0;img(x,y,1)=0;img(x,y,2)=0;}
		
	}
}


///********************************************************
///                    PROMEDIOS
///********************************************************
template <typename T>
double get_prom(CImg<T> &img){
	double prom=0;
	cimg_forXY(img,x,y){
		prom+=img(x,y);
	}
	return prom/(img.width() * img.height());
	
}

template <typename T>
int get_promh(CImg<T> &img){
	CImg<T> H = img.get_histogram(256,0,255);
	double max=H(0,0);
	int pos=0;
	cimg_forXY(H,x,y){
		if(max<H(x,y)){
			max=H(x,y);
			pos=x;}
	}
	return pos;
	
}


///********************************************************
///                    FIND POS
///********************************************************

template <typename T>
int findPos(T *dist, T value, int size){
	for(int i=0;i<size;i++){
		if(value == dist[i])
			return i;
	}}


///********************************************************
///                    FIND POS IMG
///********************************************************

template <typename T>
void findPosImg(CImg<T> img, T value, int &posx, int &posy){
	cimg_forXY(img,x,y){
		if(img(x,y) == value){
			posx=x;
			posy=y;
			return;}
}}

///********************************************************
///                MINIMO DE UN VECTOR
///********************************************************

template <typename T>
T minimo(T *dist, int size){
	T aux=dist[0];
	for(int i=1;i<size;i++){
		if(aux>dist[i])
			aux=dist[i];
	}
	return aux;
}


///********************************************************
///                UMBRALIZAR MAXIMOS EN ZONA
///********************************************************

template <typename T>
void umbraMaximoZona(CImg<T> &img, T val,int x, int y, double u){
	
	if(img.width()-1<x || img.height()-1<y || x<0 || y<0)
		return;
	if(img(x,y)==0)
		return;
	if(img(x,y)>val || img(x,y)<val*u)
		return;
	
	img(x,y)=0;
	
	umbraMaximoZona(img,val,x+1,y,u);
	umbraMaximoZona(img,val,x-1,y,u);
	umbraMaximoZona(img,val,x,y+1,u);
	umbraMaximoZona(img,val,x,y-1,u);
	
	
	umbraMaximoZona(img,val,x+1,y+1,u);
	umbraMaximoZona(img,val,x+1,y-1,u);
	umbraMaximoZona(img,val,x-1,y+1,u);
	umbraMaximoZona(img,val,x-1,y-1,u);
	
	
}


///********************************************************
///                OPERACIONES ARITMETICAS
///********************************************************
template <typename T>
CImg<T> restaImg(CImg<T> img,CImg<T> menos){
	if(img.spectrum() != menos.spectrum())
		return CImg<T>();
	CImg<T> r(img);
	int spectrum = img.spectrum();
	cimg_forXY(img,x,y){
		for(int i=0;i<spectrum;i++)
			r(x,y,i) = img(x,y,i) - menos(x,y,i);}
	return r;
}


///********************************************************
///                OPERACIONES BINARIAS
///********************************************************
template <typename T>
CImg<T> XOR(CImg<T> bin1,CImg<T> bin2){
	CImg<T> r (bin1);
	cimg_forXY(r,x,y){
		r(x,y) =( bin1(x,y) || bin2(x,y) ) - ( bin1(x,y) && bin2(x,y) );} return r;
}



