#include <iostream>
#include <complex>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

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

// Genera un filtro homomorfico normalizado
CImgList<double> filtroHomomorfico(CImg<double> img,double frec_corte,double gammaL=0.5,double gammaH=2.0,double c=1.0){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Espacios de frecuencia U y V
	CImg<double> U(M,N,1,1);
	CImg<double> V(M,N,1,1);
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro homomorfico normalizado
	cimg_forXY(H[0],i,j){
		// Variables de frecuencia U y V
		U(i,j)=-1.0+i*2.0/(M-1);
		V(i,j)=-1.0+j*2.0/(N-1);
		
		D(i,j)=sqrt(pow(U(i,j),2)+pow(V(i,j),2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=(gammaH-gammaL)*(1-exp(-c*pow(D(i,j),2)/pow(frec_corte,2)))+gammaL;
	}
	
	// Desplaza el filtro
	H[0].shift(M/2,N/2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Aplica un filtro homomorfico
CImg<double> filtradoHomomorfico(CImg<double> img,CImgList<double> HH){
	// La imagen se define como f(x,y)=i(x,y)*r(x,y), con i=iluminacion y 
	// r=reflectancia, por lo que se aplica logaritmo a la imagen
	
	// Normaliza los valores de la imagen para evitar logaritmo de cero
	CImg<double> img_normalizada=img.get_normalize(1,100);
	
	// Calcula el logaritmo de la imagen normalizada
	CImg<double> img_log=img_normalizada.get_log();
	
	// Una vez separadas las componentes teoricamente, aplica TDF (dentro de la
	// funcion) y filtrado S(u,v)=H(u,v)*Fi(u,v)+H(u,v)*Fr(u,v)
	CImg<double> filtrada=filtradoFrec(img_log,HH);
	
	// Normaliza
	filtrada.normalize(0,1);
	
	// Aplica antilogaritmo (exponencial)
	filtrada.exp();
	
	// Normaliza
	filtrada.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return filtrada;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","casilla.tif","Image file\n");
	CImg<double> casilla(filename1);
	
	const char* filename2 = cimg_option("-i","reunion.tif","Image file\n");
	CImg<double> reunion(filename2);
	
	// Item 1 y 2
	// El filtro homomorfico se define:
	// H(u,v)=(gammaH-gammaL)*(1-exp(-c*pow(D(i,j),2)/pow(frec_corte,2)))+gammaL
	// Con gammaH = efecto en altas frecuencias
	//     gammaL = efecto en bajas frecuencias
	//     c = constante de sharpness
	//     frec_corte = frecuencia de corte del filtro
	//     D = distancia euclidea del punto (u,v) al centro
	
	// Crea el filtro homommorfico
	//CImgList<double> HH_casilla=filtroHomomorfico(casilla,1.0,0.9,1.1,1.0);
	//CImgList<double> HH_reunion=filtroHomomorfico(reunion,0.7,0.9,1.7,1.0);
	CImgList<double> HH_casilla=filtroHomomorfico(casilla,0.4,0.5,1.1,1.0);
	CImgList<double> HH_reunion=filtroHomomorfico(reunion,0.7,0.6,1.7,1.0);
	
	// Aplica el filtro homomorfico
	CImg<double> casilla_filtrada=filtradoHomomorfico(casilla,HH_casilla);
	CImg<double> reunion_filtrada=filtradoHomomorfico(reunion,HH_reunion);
	
	// Muestra los resultados
	(casilla,casilla_filtrada).display("Original - Filtrado homomorfico");
	(reunion,reunion_filtrada).display("Original - Filtrado homomorfico");
	
	// Item 3
	// Imagen => Ecualizacion => Filtrado
	// Ecualiza las imagenes originales
	CImg<double> casilla_eq=casilla.get_equalize(256);
	CImg<double> reunion_eq=reunion.get_equalize(256);
	
	// Aplica el filtro homomorfico
	CImg<double> casilla_eq_filtrada=filtradoHomomorfico(casilla_eq,HH_casilla);
	CImg<double> reunion_eq_filtrada=filtradoHomomorfico(reunion_eq,HH_reunion);
	
	// Imagen => Filtrado => Ecualizacion
	// Ecualiza las imagenes filtradas
	CImg<double> casilla_filtrada_eq=casilla_filtrada.get_equalize(256);
	CImg<double> reunion_filtrada_eq=reunion_filtrada.get_equalize(256);
	
	// Muestra los resultados
	(casilla,casilla_eq,casilla_eq_filtrada,casilla_filtrada_eq).display("Original - Original ecualizada - Ecualizacion y filtrado - Filtrado y ecualizacion");
	(reunion,reunion_eq,reunion_eq_filtrada,reunion_filtrada_eq).display("Original - Original ecualizada - Ecualizacion y filtrado - Filtrado y ecualizacion");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
