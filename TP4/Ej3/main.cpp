#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","rio.jpg","Image file\n");
	CImg<float> rio(filename);
	
	// Muestra la imagen
	rio.display();
	
	// Valores de grises correspondientes al agua en el rango 0-20
	float umbral=20.0;
	
	// Calcula el histograma de la imagen
	CImg<float> rio_histograma=rio.get_histogram(256,0,255);
	rio_histograma.display_graph("Histograma",3);
	
	// Pinta de amarillo el rango de grises correspondiente al agua
	CImg<float> rio_color(rio.width(),rio.height(),1,3);
	
	cimg_forXY(rio,i,j){
		if(rio(i,j)<=umbral){
			rio_color(i,j,0)=255.0;
			rio_color(i,j,1)=255.0;
			rio_color(i,j,2)=0.0;
		}
		else{
			rio_color(i,j,0)=rio(i,j);
			rio_color(i,j,1)=rio(i,j);
			rio_color(i,j,2)=rio(i,j);
		}
	}
	
	// Muestra el resultado en pantalla
	CImgList<> lista(rio,rio_color);
	lista.display("Rio - Rio Color");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
