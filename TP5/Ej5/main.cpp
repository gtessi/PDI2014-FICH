#include <iostream>
#include <complex>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Genera un filtro pasa-altos ideal normalizado
CImgList<double> filtroPAIdeal(CImg<double> img,double frec_corte){
	// Tama�o del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Espacios de frecuencia U y V
	CImg<double> U(M,N,1,1);
	CImg<double> V(M,N,1,1);
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro pasa-altos ideal normalizado
	cimg_forXY(H[0],i,j){
		// Variables de frecuencia U y V
		U(i,j)=-1.0+i*2.0/(M-1);
		V(i,j)=-1.0+j*2.0/(N-1);
		
		D(i,j)=sqrt(pow(U(i,j),2)+pow(V(i,j),2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		if(D(i,j)<=frec_corte)
			H[0](i,j)=0.0;
		else
			H[0](i,j)=1.0;
	}
	
	// Desplaza el filtro
	H[0].shift(M/2,N/2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Genera un filtro pasa-altos Butterworth normalizado
CImgList<double> filtroPAButterworth(CImg<double> img,double frec_corte,unsigned int orden){
	// Tama�o del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Espacios de frecuencia U y V
	CImg<double> U(M,N,1,1);
	CImg<double> V(M,N,1,1);
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro pasa-altos Butterworth normalizado
	cimg_forXY(H[0],i,j){
		// Variables de frecuencia U y V
		U(i,j)=-1.0+i*2.0/(M-1);
		V(i,j)=-1.0+j*2.0/(N-1);
		
		D(i,j)=sqrt(pow(U(i,j),2)+pow(V(i,j),2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1/(1+pow((frec_corte/D(i,j)),2*orden));
	}
	
	// Desplaza el filtro
	H[0].shift(M/2,N/2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Genera un filtro pasa-altos gaussiano normalizado
CImgList<double> filtroPAgaussiano(CImg<double> img,double frec_corte){
	// Tama�o del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Espacios de frecuencia U y V
	CImg<double> U(M,N,1,1);
	CImg<double> V(M,N,1,1);
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro pasa-altos gaussiano normalizado
	cimg_forXY(H[0],i,j){
		// Variables de frecuencia U y V
		U(i,j)=-1.0+i*2.0/(M-1);
		V(i,j)=-1.0+j*2.0/(N-1);
		
		D(i,j)=sqrt(pow(U(i,j),2)+pow(V(i,j),2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1-exp(-pow(D(i,j),2)/(2*pow(frec_corte,2)));
	}
	
	// Desplaza el filtro
	H[0].shift(M/2,N/2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Aplica un filtro en el dominio frecuencial
CImg<double> filtradoFrec(CImg<double> img,CImgList<double> H){
	CImg<double> filtrada;
	
	// Calcula la TDF de la imagen
	CImgList<double> img_tdf=img.get_FFT();
	
	// Realiza la multiplicacion en el dominio frecuencial
	CImgList<double> filtrada_tdf=img_tdf;
	
	cimg_forXY(img_tdf[0],i,j){
		// Valor complejo de la TDF de la imagen original
		complex<double> factor1(img_tdf[0](i,j),img_tdf[1](i,j));
		
		// Valor complejo de la TDF del filtro
		complex<double> factor2(H[0](i,j),H[1](i,j));
		
		// Realiza el producto entre los numeros complejos
		complex<double> producto=factor1*factor2;
		
		// Guarda la parte real e imaginaria de la imagen filtrada
		filtrada_tdf[0](i,j)=real(producto); // Parte real
		filtrada_tdf[1](i,j)=imag(producto); // Parte imaginaria
	}
	
	// Calcula la TDF inversa y toma solo la parte real
	filtrada=filtrada_tdf.get_FFT(true)[0];
	
	// Devuelve la imagen filtrada
	return filtrada;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> cameraman(filename);
	
	// Item 1
	// Crea el filtro pasa-altos normalizado ideal
	CImgList<double> HPA_ideal=filtroPAIdeal(cameraman,0.3);
	
	// Aplica el filtro ideal a la imagen en el dominio frecuencial
	CImg<double> filtrada_ideal=filtradoFrec(cameraman,HPA_ideal);
	
	// Muestra los resultados
	(cameraman,filtrada_ideal).display("Original - Filtrada en frecuencia PA ideal");
	
	// Item 2
	// Crea el filtro pasa-altos normalizado Butterworth
	CImgList<double> HPA_Butterworth=filtroPAButterworth(cameraman,0.3,2);
	
	// Aplica el filtro Butterworth a la imagen en el dominio frecuencial
	CImg<double> filtrada_Butterworth=filtradoFrec(cameraman,HPA_Butterworth);
	
	// Muestra los resultados
	(cameraman,filtrada_Butterworth).display("Original - Filtrada en frecuencia PA Butterworth");
	
	// Item 3
	// ***********
	// Hay que hacer una mascara gaussiana invertida
	// ***********
////	// Crea el filtro pasa-altos gaussiano en el dominio espacial
////	CImg<double> mascara_gaussiana=gaussian_mask(5,3);
////	
////	// Tama�o normalizado
////	unsigned int P=cameraman.width()+mascara_gaussiana.width()-1;
////	unsigned int Q=cameraman.height()+mascara_gaussiana.height()-1;
////	
////	// Normaliza el tama�o de la imagen
////	CImg<double> cameraman_resize=cameraman.get_resize(P,Q,-100,-100,0);
////	
////	// Normaliza el tama�o del filtro (zero-padding)
////	CImg<double> mascara_gaussiana_resize=mascara_gaussiana.get_resize(P,Q,-100,-100,0);
////	
////	// Calcula la TDF del filtro gaussiano
////	CImgList<double> mascara_gaussiana_tdf=mascara_gaussiana_resize.get_FFT();
////	
////	// Aplica la mascara gaussiana en el dominio frecuencial
////	CImg<double> filtrada_gaussiano_esp=filtradoFrec(cameraman_resize,mascara_gaussiana_tdf);
////	
////	// Calcula el grosor de los bordes para el recorte
////	unsigned int deltaX=floor(mascara_gaussiana.width()/2.0);
////	unsigned int deltaY=floor(mascara_gaussiana.height()/2.0);
////	
////	// Recorta los bordes que sobran
////	filtrada_gaussiano_esp.crop(deltaX,deltaY,filtrada_gaussiano_esp.width()-deltaX-1,filtrada_gaussiano_esp.height()-deltaY-1);
////	
////	// Muestra las TDFs de la imagen y de la filtrada
////	(magn_tdf(cameraman,true,"../gray.pal"),magn_tdf(filtrada_gaussiano_esp,true,"../gray.pal")).display("TDF original - TDF filtrada en frecuencia PA gaussiano a partir de una mascara");
////	
////	// Muestra los resultados
////	(cameraman,filtrada_gaussiano_esp).display("Original - Filtrada en frecuencia PA gaussiano a partir de una mascara");
	
	// Item 4
	// Crea el filtro pasa-altos normalizado gaussiano
	CImgList<double> HPA_gaussiano=filtroPAgaussiano(cameraman,0.3);
	
	// Aplica el filtro gaussiano a la imagen en el dominio frecuencial
	CImg<double> filtrada_gaussiano=filtradoFrec(cameraman,HPA_gaussiano);
	
	// Muestra los resultados
	(cameraman,filtrada_gaussiano).display("Original - Filtrada en frecuencia PA gaussiano");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
