#include <iostream>
#include <cassert>
#include <ctime>
#include <complex>
#include "CImg.h"

using namespace cimg_library;
using namespace std;


// Crea una mascara de NxN
CImg<double> crearMascara(unsigned int n){
	CImg<double> mascara(n,n);
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=1;
	}
	
	// Aplica el factor de escala a la imagen
	mascara/=(n*n);
	
	// Devuelve la mascara
	return mascara;
}

CImg<double> crearMascaraGaussiana(unsigned int n,double sigma){
	CImg<double> mascara(n,n), matriz_covarianza(2,2); // ???
	
	// Vector color
	const char color[]={1};
	
	// Crea la matriz de covarianza
	matriz_covarianza(0,0)=sigma;
	matriz_covarianza(1,1)=sigma;
	
	// Calcula la distribucion gaussiana
	mascara.draw_gaussian(n/2,n/2,matriz_covarianza,color);
	
	// Devuelve la mascara
	return mascara;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> img(filename);
	
	// Variables de tiempo
	time_t tini, tfin;
	
	// Define el valor de la varianza
	double sigma=2.0;
	
	// Crea la mascara de 21x21
	CImg<double> mascara=crearMascaraGaussiana(21,sigma);
	//CImg<double> mascara=crearMascara(21);
	
	// Espacios de imagen
	CImg<double> filtrado_espacial, filtrado_frecuencial;
	
	// Aplica la mascara a la imagen en el dominio espacial
	tini=time(NULL);
	
	filtrado_espacial=img.get_convolve(mascara);
	
	tfin=time(NULL);
	
	cout<<"\nEl filtrado espacial toma "<<difftime(tfin,tini)<<" segundos.\n\n";
	
	// Para el filtrado frecuencial, hay que normalizar los tamaños de la
	// imagen y del filtro, rellenando con ceros
	// El ancho total es ancho_img+ancho_mascara-1
	// El alto total es alto_img+alto_mascara-1
	unsigned int ancho_total=img.width()+mascara.width()-1;
	unsigned int alto_total=img.height()+mascara.height()-1;
	
	// Redimensiona las imagenes para poder hacer la multiplicacion en
	// frecuencias, rellenando con ceros (zero-padding)
	// M + N - 1
	CImg<double> img_resize=img.get_resize(ancho_total,alto_total,-100,-100,0);
	CImg<double> mascara_resize=mascara.get_resize(ancho_total,alto_total,-100,-100,0);
	
	// Calcula las TDFs de la imagen y la mascara redimensionadas
	CImgList<double> img_resize_tdf=img_resize.get_FFT();
	CImgList<double> mascara_resize_tdf=mascara_resize.get_FFT();
	
	// Crea la lista de la TDF para el producto
	CImgList<double> temp(2,img_resize.width(),img_resize.height(),img_resize.depth(),img_resize.spectrum(),0);
	
	// Multiplica en frecuencia
	tini=time(NULL);
	
	cimg_forXY(img_resize,i,j){
		// Obtiene los valores complejos de las TDFs
		complex<double> valor_img(img_resize_tdf[0](i,j),img_resize_tdf[1](i,j));
		complex<double> valor_mascara(mascara_resize_tdf[0](i,j),mascara_resize_tdf[1](i,j));
		
		// Realiza el producto entre los valores
		complex<double> prod=valor_img*valor_mascara;
		
		// Crea los espectros
		temp[0](i,j)=real(prod);
		temp[1](i,j)=imag(prod);
	}
	
	// Calcula la TDF inversa y toma la parte real
	filtrado_frecuencial=temp.get_FFT(true)[0];
	
	tfin=time(NULL);
	
	cout<<"\nEl filtrado frecuencial toma "<<difftime(tfin,tini)<<" segundos.\n\n";
	
	// Calcula el grosor de los bordes para el recorte
	unsigned int deltaX=floor(mascara.width()/2.0);
	unsigned int deltaY=floor(mascara.height()/2.0);
	
	// Recorta los bordes que sobran
	filtrado_frecuencial.crop(deltaX,deltaY,filtrado_frecuencial.width()-deltaX,filtrado_frecuencial.height()-deltaY);
	
	// Muestra los resultados
	(img,filtrado_espacial,filtrado_frecuencial).display();
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
