#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

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
	
	// *** no se aplica el factor de escala
	// si se aplica, no se como se calcula dicho factor
	
////	// Aplica el factor de escala a la imagen
////	unsigned int factor_escala=cuentaUnos(n,patron);
////	
////	mascara/=factor_escala;
	
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


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	const char* filename2 = cimg_option("-i","hubble.tif","Image file\n");
	CImg<unsigned char> img2(filename2);
	
	const char* filename3 = cimg_option("-i","imagenC.tif","Image file\n");
	CImg<unsigned char> img3(filename3);
	
	CImg<float> salida1, salida2, salida3;
	
	CImgList<> lista;
	
	// Los filtros pasa-altos resaltan los detalles finos y las variaciones
	// rapidas (saltos bruscos) de intensidad entre pixeles vecinos
	
	// Suma=1: realce de altas frecuencias sin alterar las bajas frecuencias
	// Suma=0: extraccion de altas frecuencias, eliminando las bajas frecuencias
	
	// Los Suma=1 resaltan algunos detalles y bordes
	// Los Suma=0 resaltan los bordes eliminando las zonas homogeneas
	
	// Mascaras
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
	
	// Aplica las mascaras a las imagenes
	salida1=obtenerFiltro(img1,3,patron1_1);
	salida2=obtenerFiltro(img1,3,patron1_2);
	salida3=obtenerFiltro(img1,3,patron1_3);
	
	lista.push_back(img1);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	salida1=obtenerFiltro(img2,3,patron1_1);
	salida2=obtenerFiltro(img2,3,patron1_2);
	salida3=obtenerFiltro(img2,3,patron1_3);
	
	lista.push_back(img2);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	salida1=obtenerFiltro(img3,3,patron1_1);
	salida2=obtenerFiltro(img3,3,patron1_2);
	salida3=obtenerFiltro(img3,3,patron1_3);
	
	lista.push_back(img3);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	// -------------- Fin Suma=1 --------------
	cout<<"\n\n*********************************************************\n\n";
	
	salida1=obtenerFiltro(img1,3,patron0_1);
	salida2=obtenerFiltro(img1,3,patron0_2);
	salida3=obtenerFiltro(img1,3,patron0_3);
	
	lista.push_back(img1);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	salida1=obtenerFiltro(img2,3,patron0_1);
	salida2=obtenerFiltro(img2,3,patron0_2);
	salida3=obtenerFiltro(img2,3,patron0_3);
	
	lista.push_back(img2);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	salida1=obtenerFiltro(img3,3,patron0_1);
	salida2=obtenerFiltro(img3,3,patron0_2);
	salida3=obtenerFiltro(img3,3,patron0_3);
	
	lista.push_back(img3);
	lista.push_back(salida1);
	lista.push_back(salida2);
	lista.push_back(salida3);
	lista.display();
	lista.clear();
	
	// -------------- Fin Suma=0 --------------
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
