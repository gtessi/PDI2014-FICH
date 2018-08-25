#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Calcula la media geometrica de una vecindad
double mediaGeometrica(CImg<double> vecindad){
	double productoria=1;
	
	// Cantidad de vecinos
	double mn=vecindad.width()*vecindad.height();
	
	// Calcula la productoria de los vecinos
	cimg_forXY(vecindad,i,j){
		productoria*=vecindad(i,j);
	}
	
	// Calcula la raiz m-esima si la productoria es positiva, de lo contrario
	// el valor es cero
	if(productoria<0)
		productoria=0;
	else
		productoria=pow(productoria,(1/mn));
	
	// Devuelve la media geometrica
	return productoria;
}

// Aplica el filtro de media geometrica
CImg<double> filtradoMediaGeometrica(CImg<double> img,int m=3,int n=3){
	CImg<double> salida(img);
	
	// Tamaño de la imagen
	int ancho=img.width();
	int alto=img.height();
	
	// Calcula los pasos de la vecindad centrada
	int paso_x=m/2;
	int paso_y=n/2;
	
	// Calcula la imagen filtrada
	cimg_forXY(img,i,j){
		// Calcula la vecindad con pixels validos
		int x_ini=(i-paso_x<0) ? 0 : i-paso_x;
		int y_ini=(j-paso_y<0) ? 0 : j-paso_y;
		int x_fin=(i+paso_x>=ancho) ? ancho-1 : i+paso_x;
		int y_fin=(j+paso_y>=alto) ? alto-1 : j+paso_y;
		
		// Obtiene la vecindad
		CImg<double> vecindad=img.get_crop(x_ini,y_ini,x_fin,y_fin);
		
		// Aplica el filtro de media geometrica
		salida(i,j)=mediaGeometrica(vecindad);
	}
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}

// Calcula la mediana de una vecindad
double mediana(CImg<double> vecindad){
	double salida=0;
	
	// Ordena la vecindad
	vecindad.sort();
	
	// Obtiene el valor del centro
	salida=vecindad(vecindad.width()/2,vecindad.height()/2);
	
	// Devuelve la mediana
	return salida;
}

// Calcula el punto medio de una vecindad
double puntoMedio(CImg<double> vecindad){
	// Obtiene el valor maximo y minimo de la vecindad
	double max=vecindad.max();
	double min=vecindad.min();
	
	// Calcula el valor medio
	double resultado=(max+min)*0.5;
	
	// Devuelve el resultado
	return resultado;
}

// Aplica el filtro de orden
// Tipo de filtro
// 0 = Filtro de la mediana
// 1 = Filtro del punto medio
CImg<double> filtradoOrden(CImg<double> img,unsigned int tipo_filtro=0,int m=3,int n=3){
	CImg<double> salida(img);
	
	// Tamaño de la imagen
	int ancho=img.width();
	int alto=img.height();
	
	// Calcula los pasos de la vecindad centrada
	int paso_x=m/2;
	int paso_y=n/2;
	
	// Calcula la imagen filtrada
	cimg_forXY(img,i,j){
		// Calcula la vecindad con pixels validos
		int x_ini=(i-paso_x<0) ? 0 : i-paso_x;
		int y_ini=(j-paso_y<0) ? 0 : j-paso_y;
		int x_fin=(i+paso_x>=ancho) ? ancho-1 : i+paso_x;
		int y_fin=(j+paso_y>=alto) ? alto-1 : j+paso_y;
		
		// Obtiene la vecindad
		CImg<double> vecindad=img.get_crop(x_ini,y_ini,x_fin,y_fin);
		
		// Aplica el filtro elegido
		// Mediana
		if(tipo_filtro==0)
			salida(i,j)=mediana(vecindad);
		// Punto medio
		if(tipo_filtro==1)
			salida(i,j)=puntoMedio(vecindad);
	}
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","FAMILIA_a.jpg","Image file\n");
	CImg<double> FAMILIA_a(filename1);
	
	const char* filename2 = cimg_option("-i","FAMILIA_b.jpg","Image file\n");
	CImg<double> FAMILIA_b(filename2);
	
	const char* filename3 = cimg_option("-i","FAMILIA_c.jpg","Image file\n");
	CImg<double> FAMILIA_c(filename3);
	
	// Visualiza las imagenes
	(FAMILIA_a,FAMILIA_b,FAMILIA_c).display("FAMILIA_a - FAMILIA_b - FAMILIA_c");
	
	// Tipos de ruido de las imagenes
	// FAMILIA_a = Ruido gaussiano
	// FAMILIA_b = Ruido uniforme
	// FAMILIA_c = Ruido impulsivo
	
	// Solucion para cada tipo de ruido
	// Ruido gaussiano = media aritmetica, media geometrica, media armonica, punto medio, media-alfa recortado
	// Ruido uniforme = punto medio
	// Ruido impulsivo = mediana, media-alfa recortado
	
	// Aplica los filtros a las imagenes
	CImg<double> FAMILIA_a_media_geometrica=filtradoMediaGeometrica(FAMILIA_a,3,3);
	CImg<double> FAMILIA_b_punto_medio=filtradoOrden(FAMILIA_b,1,3,3);
	CImg<double> FAMILIA_c_mediana=filtradoOrden(FAMILIA_c,0,3,3);
	
	// Visualiza las imagenes filtradas
	(FAMILIA_a,FAMILIA_a_media_geometrica).display("FAMILIA_a - Filtro media geometrica");
	(FAMILIA_b,FAMILIA_b_punto_medio).display("FAMILIA_b - Filtro punto medio");
	(FAMILIA_c,FAMILIA_c_mediana).display("FAMILIA_c - Filtro mediana");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
