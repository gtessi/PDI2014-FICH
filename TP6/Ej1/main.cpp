#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Crea una imagen con un patron de grises constante
CImg<double> crearPatronGrises(unsigned int n,unsigned int cant_pat){
	CImg<double> salida(n,n,1,1);
	
	// Calcula el salto de los patrones
	unsigned int salto_patron=floor(256/cant_pat);
	
	// Crea los patrones
	for(unsigned int pat=0;pat<cant_pat;pat++)
		for(unsigned int i=salto_patron*pat;i<=salto_patron*(pat+1);i++)
			for(unsigned int j=0;j<n;j++)
				salida(i,j)=salto_patron*pat;
	
	
	//for(unsigned int j=0;j<n;j++)
	
	// Devuelve la imagen
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> cameraman(filename);
	
	// Item 1
	// Potencia del ruido
	double sigma_gaussiano=10.0;
	double sigma_uniforme=20.0;
	double sigma_impulsivo=2.0; // Sal y pimienta
	
	// Genera imagenes con diferentes tipos de ruido
	CImg<double> cameraman_gaussiano=cameraman.get_noise(sigma_gaussiano,0);
	CImg<double> cameraman_uniforme=cameraman.get_noise(sigma_uniforme,1);
	CImg<double> cameraman_impulsivo=cameraman.get_noise(sigma_impulsivo,2);
	
	// Normaliza
	cameraman_gaussiano.normalize(0,255);
	cameraman_uniforme.normalize(0,255);
	cameraman_impulsivo.normalize(0,255);
	
	// Muestra los resultados
	(cameraman,cameraman_gaussiano,cameraman_uniforme,cameraman_impulsivo).display("Original - Ruido gaussiano - Ruido uniforme - Ruido impulsivo");
	
	// Calcula los histogramas
	CImg<double> cameraman_hist=cameraman.get_histogram(256);
	CImg<double> cameraman_gaussiano_hist=cameraman_gaussiano.get_histogram(256);
	CImg<double> cameraman_uniforme_hist=cameraman_uniforme.get_histogram(256);
	CImg<double> cameraman_impulsivo_hist=cameraman_impulsivo.get_histogram(256);
	
	// Muestra los histogramas
	CImgDisplay ventana1;
	
	cameraman_hist.display_graph(ventana1,3,0);
	cameraman_gaussiano_hist.display_graph(ventana1,3,0);
	cameraman_uniforme_hist.display_graph(ventana1,3,0);
	cameraman_impulsivo_hist.display_graph(ventana1,3,0);
	
	// Item 2
	// La media esta dada por la cantidad de patrones de grises elegidos
	// El desvio se centra en el cambio de patrones
	
	// Genera el patron de grises
	CImg<double> patron=crearPatronGrises(256,4);
	
	// Genera imagenes con diferentes tipos de ruido
	CImg<double> patron_gaussiano=patron.get_noise(sigma_gaussiano,0);
	CImg<double> patron_uniforme=patron.get_noise(sigma_uniforme,1);
	CImg<double> patron_impulsivo=patron.get_noise(sigma_impulsivo,2);
	
	// Normaliza
	patron_gaussiano.normalize(0,255);
	patron_uniforme.normalize(0,255);
	patron_impulsivo.normalize(0,255);
	
	// Muestra los resultados
	(patron,patron_gaussiano,patron_uniforme,patron_impulsivo).display("Original - Ruido gaussiano - Ruido uniforme - Ruido impulsivo");
	
	// Calcula los histogramas
	CImg<double> patron_hist=patron.get_histogram(256);
	CImg<double> patron_gaussiano_hist=patron_gaussiano.get_histogram(256);
	CImg<double> patron_uniforme_hist=patron_uniforme.get_histogram(256);
	CImg<double> patron_impulsivo_hist=patron_impulsivo.get_histogram(256);
	
	// Muestra los histogramas
	CImgDisplay ventana2;
	
	patron_hist.display_graph(ventana2,3,0);
	patron_gaussiano_hist.display_graph(ventana2,3,0);
	patron_uniforme_hist.display_graph(ventana2,3,0);
	patron_impulsivo_hist.display_graph(ventana2,3,0);
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
