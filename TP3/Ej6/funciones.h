#include <iostream>
#include <cassert>
#include <cmath>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Patrones para las mascaras
// Promediado
double patron_1[]={
	0.11, 0.11, 0.11,
	0.11, 0.11, 0.11,
	0.11, 0.11, 0.11
};
double patron_2[]={
	0.04, 0.04, 0.04, 0.04, 0.04,
	0.04, 0.04, 0.04, 0.04, 0.04,
	0.04, 0.04, 0.04, 0.04, 0.04,
	0.04, 0.04, 0.04, 0.04, 0.04,
	0.04, 0.04, 0.04, 0.04, 0.04
};

// Suma=1
double patron1_1[]={
	0.0, -1.0, 0.0,
	-1.0, 5.0, -1.0,
	0.0, -1.0, 0.0
},
patron1_2[]={
	-1.0, -1.0, -1.0,
	-1.0, 9.0, -1.0,
	-1.0, -1.0, -1.0
},
patron1_3[]={
	1.0, -2.0, 1.0,
	-2.0, 5.0, -2.0,
	1.0, -2.0, 1.0
};

// Suma=0
double patron0_1[]={
	0.0, -1.0, 0.0,
	-1.0, 4.0, -1.0,
	0.0, -1.0, 0.0
},
patron0_2[]={
	-1.0, -1.0, -1.0,
	-1.0, 8.0, -1.0,
	-1.0, -1.0, -1.0
},
patron0_3[]={
	1.0, -2.0, 1.0,
	-2.0, 4.0, -2.0,
	1.0, -2.0, 1.0
};

// Acota el rango
CImg<float> clipping(CImg<float> img){
	CImg<float> salida(img);
	
	// Aplica el clipping
	cimg_forXY(salida,i,j){
		if (salida(i,j)>255)
			salida(i,j)=255;
		else
			if(salida(i,j)<0)
			salida(i,j)=0;
	}
	
	// Devuelve la imagen acotada
	return salida;
}

// Transformacion logaritmica
CImg<float> logaritmica(CImg<float> r,float c=1.0){
	CImg<float> s(r.width(),r.height());
	
	cimg_forXY(r,i,j){
		s(i,j)=c*log(1+r(i,j));
	}
	
	return clipping(s);
}

// Transformacion potencia
CImg<float> potencia(CImg<float> r,float c=1.0,float gamma=1.0){
	CImg<float> s(r.width(),r.height());
	
	cimg_forXY(r,i,j){
		s(i,j)=c*pow(r(i,j),gamma);
	}
	
	return clipping(s);
}

// Devuelve una imagen convolucionada con una matriz de NxN, a partir de un patron
CImg<float> filtrarImagen(CImg<float> img,unsigned int n,double patron[],bool acotar=false){
	CImg<float> mascara(n,n), salida;
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=patron[i+j*n];
	}
	
	// Convolucion entre la imagen y la mascara
	salida=img.get_convolve(mascara);
	
	// Acota el rango (clipping)
	if(acotar)
		salida=clipping(salida);
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}

// Filtro pasa-altos calculado como la diferencia entre la imagen original y una
// version suavizada (filtrada con un filtro pasa-bajos)
// Si se pasa el parametro A, se obtiene el filtrado de alta potencia (high-boost)
CImg<float> restaPB(CImg<float> original,CImg<float> filtrada,double A=1.0){
	assert(original.is_sameXY(filtrada));
	
	CImg<float> salida(original.width(),original.height());
	
	if(A<1.0){
		cout<<"\n\n *** ERROR - A menor que 1. ***\n\n";
		return salida;
	}
	
	// Calcula la resta normalizada entre las imagenes
	double temp=0;
	cimg_forXY(original,i,j){
		temp=A*original(i,j)-filtrada(i,j);
		
		// Clipping
		if(temp>255)
			temp=255;
		else
			if(temp<0)
			temp=0;
		
		salida(i,j)=temp;
	}
	
	// Devuelve la imagen modificada
	return salida;
}

//-------------------------------------------------------------------------
