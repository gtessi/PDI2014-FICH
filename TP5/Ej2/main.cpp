#include <iostream>
#include <cmath>
#include <complex>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Calcula la magnitud de la TDF de una imagen
CImg<double> magnitud(CImgList<double> tdf){
	CImg<double> salida(tdf[0]);
	
	cimg_forXY(salida,i,j){
		// Magnitud = raiz cuadrada (real al cuadrado + imaginaria al cuadrado)
		salida(i,j)=sqrt(pow(tdf[0](i,j),2)+pow(tdf[1](i,j),2));
	}
	
	// Devuelve la magnitud
	return salida;
}

// Calcula la fase de la TDF de una imagen
CImg<double> fase(CImgList<double> tdf){
	CImg<double> salida(tdf[0]);
	
	cimg_forXY(salida,i,j){
		// Fase = arctan (imaginaria / real)
		// atan2 es el arcotangente de dos parametros
		salida(i,j)=atan2(tdf[1](i,j),tdf[0](i,j));
	}
	
	// Devuelve la fase
	return salida;
}


int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> cameraman(filename1);
	
	// Calcula la TDF de la imagen
	CImgList<double> cameraman_tdf=cameraman.get_FFT();
	
	// Calcula la magnitud de la TDF de la imagen
	CImg<double> cameraman_tdf_magn=magnitud(cameraman_tdf);
	
	// Calcula la fase de la TDF de la imagen
	CImg<double> cameraman_tdf_fase=fase(cameraman_tdf);
	
	// Crea las listas de las TDFs de solo magnitud y de solo fase
	CImgList<double> tdf_solo_magnitud(2,cameraman.width(),cameraman.height(),cameraman.depth(),cameraman.spectrum(),0);
	CImgList<double> tdf_solo_fase(2,cameraman.width(),cameraman.height(),cameraman.depth(),cameraman.spectrum(),0);
	
	// Define la componente imaginaria i
	complex<double> I(0,1);
	
	// Crea las TDFs
	cimg_forXY(cameraman,i,j){
		// Obtiene las componentes complejas para la magnitud unitaria y la
		// fase cero
		complex<double> fase_cero=cameraman_tdf_magn(i,j);
		complex<double> magnitud_unitaria=exp(I*cameraman_tdf_fase(i,j));
		
		// Crea los espectros
		tdf_solo_magnitud[0](i,j)=real(fase_cero);
		tdf_solo_magnitud[1](i,j)=imag(fase_cero);
		
		tdf_solo_fase[0](i,j)=real(magnitud_unitaria);
		tdf_solo_fase[1](i,j)=imag(magnitud_unitaria);
	}
	
	// Calcula la TDF inversa y toma la parte real para ver los resultados
	// get_FFT(true) calcula la TDF inversa
	// [0] toma la parte real
	CImg<double> cameraman_solo_magnitud=tdf_solo_magnitud.get_FFT(true)[0];
	CImg<double> cameraman_solo_fase=tdf_solo_fase.get_FFT(true)[0];
	
	// Visualiza los resultados
	(cameraman,magn_tdf(cameraman,true,"../gray.pal"),cameraman_solo_magnitud,cameraman_solo_fase).display("Original - Original Magnitud - Solo Magnitud - Solo Fase");
	
	// -----------------------------------------------------------------------
	
	// Item 2
	const char* filename2 = cimg_option("-i","rmn.jpg","Image file\n");
	CImg<double> rmn(filename2);
	
	// Calcula la TDF de la imagen
	CImgList<double> rmn_tdf=rmn.get_FFT();
	
	// Calcula la magnitud de la TDF de la imagen
	CImg<double> rmn_tdf_magn=magnitud(rmn_tdf);
	
	// Calcula la fase de la TDF de la imagen
	CImg<double> rmn_tdf_fase=fase(rmn_tdf);
	
	// Crea las listas de las TDFs
	CImgList<double> resultado1(2,cameraman.width(),cameraman.height(),cameraman.depth(),cameraman.spectrum(),0);
	CImgList<double> resultado2(2,rmn.width(),rmn.height(),rmn.depth(),rmn.spectrum(),0);
	
	// Crea las TDFs
	cimg_forXY(rmn,i,j){
		// Calcula los valores de la TDF con las magnitudes y fases cruzadas
		complex<double> valor1=cameraman_tdf_magn(i,j)*exp(I*rmn_tdf_fase(i,j));
		complex<double> valor2=rmn_tdf_magn(i,j)*exp(I*cameraman_tdf_fase(i,j));
		
		// Crea los espectros
		resultado1[0](i,j)=real(valor1);
		resultado1[1](i,j)=imag(valor1);
		
		resultado2[0](i,j)=real(valor2);
		resultado2[1](i,j)=imag(valor2);
	}
	
	// Calcula la TDF inversa y toma la parte real para ver los resultados
	CImg<double> resultado1_invertido=resultado1.get_FFT(true)[0];
	CImg<double> resultado2_invertido=resultado2.get_FFT(true)[0];
	
	// Visualiza los resultados
	(cameraman,rmn,resultado1_invertido).display("Cameraman - RMN - Magnitud Cameraman, Fase RMN");
	(cameraman,rmn,resultado2_invertido).display("Cameraman - RMN - Magnitud RMN, Fase Cameraman");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
