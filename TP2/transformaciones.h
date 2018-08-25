#include <cmath>

// Trunca los valores entre 0 y 255 para normalizar
unsigned char truncar(float val){
	unsigned char res=0;
	
	if(val>255)
		res=255;
    else
		if(val<0)
			res=0;
		else
			res=floor(val);
	
	return res;
}

// Transformacion lineal
unsigned char lineal(unsigned int r,unsigned int ini,float a,float c){
	float s=0;
	
	// Verifica si la pendiente es positiva o negativa
	if(a<0) // Negativa
		s=255+a*(r-ini)+c;
	else // Positiva
		s=a*(r-ini)+c;
	
	return truncar(s);
}

// Transformacion logaritmica
unsigned char logaritmica(unsigned int r,float c=1.0){
	float s=0;
	
	s=c*log(1+r);
	
	return truncar(s);
}

// Transformacion potencia
unsigned char potencia(unsigned int r,float c=1.0,float gamma=1.0){
	float s=0;
	
	s=c*pow(r,gamma);
	
	return truncar(s);
}

//-------------------------------------------------------------------------
