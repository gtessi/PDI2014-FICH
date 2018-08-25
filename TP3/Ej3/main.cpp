#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Cuenta la cantidad de unos en el patron
unsigned int cuentaUnos(unsigned int n,double patron[]){
	unsigned int total=0;
	
	unsigned int n2=n*n;
	
	for(unsigned int i=0;i<n2;i++)
		if(patron[i]==1.0)
			total++;
	
	// Devuelve la cantidad de unos en el patron
	return total;
}

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

// Devuelve una imagen convolucionada con una matriz de NxN, a partir de un patron
CImg<float> obtenerFiltro(CImg<unsigned char> img,unsigned int n,double patron[],bool acotar=false){
	CImg<float> mascara(n,n), salida;
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=patron[i+j*n];
	}
	
	// Aplica el factor de escala a la imagen
	unsigned int factor_escala=cuentaUnos(n,patron);
	
	mascara/=factor_escala;
	
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

CImg<float> gaussian_mask(unsigned int n,float sigma){
	CImg<float> mascara(n,n), matriz_covarianza(2,2); // ???
	
	// Vector color
	const char color[]={1};
	
	// Crea la matriz de covarianza
	matriz_covarianza(0,0)=sigma;
	matriz_covarianza(1,1)=sigma;
	
	// Calcula la distribucion gaussiana
	mascara.draw_gaussian(n/2,n/2,matriz_covarianza,color);
	
	// Normaliza
	mascara.normalize(0,255);
	
	// Devuelve la mascara
	return mascara;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","hubble.tif","Image file\n");
	CImg<unsigned char> hubble(filename);
	
	// Item 1
	// Filtrado promedio
	CImg<float> filtrado_promedio, mascara_gaussiana, gaussiana;
	
	// Patrones
	double patron3[]={
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0
	};
	double patron5[]={
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0
	};
	
	double patron32[]={
		0.0, 1.0, 0.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 0.0
	};
	
	//filtrado_promedio=obtenerFiltro(hubble,3,patron3,false);
	filtrado_promedio=obtenerFiltro(hubble,5,patron5,false);
	//filtrado_promedio=obtenerFiltro(hubble,3,patron32,false);
	
	filtrado_promedio.display();
	
	// Aplica la mascara gaussiana
	// Mas tamaño, mas borroso
	// El desvio no afecta mucho cuando el tamaño es pequeño
	mascara_gaussiana=gaussian_mask(11,3);
	gaussiana=hubble.get_convolve(mascara_gaussiana);
	gaussiana/=gaussiana.sum();
	
	gaussiana.display();
	
	// Item 2
	// Deteccion de objetos de mayor tamaño aplicando un filtrado promedio
	filtrado_promedio.threshold(150);
	
	filtrado_promedio.display();
	
	// No aparecen objetos con el filtrado gaussiano aplicado, hay que revisar
	// los parametros y verificar si aparecen los objetos grandes
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
