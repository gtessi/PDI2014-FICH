#include <iostream>
#include <cassert>
#include "CImg.h"
#include "../transformaciones.h"

using namespace cimg_library;
using namespace std;

// Calcula el negativo de la imagen
CImg<unsigned char> negativoImg(CImg<unsigned char> img){
	CImg<unsigned char> salida(img.width(),img.height());
	
	// Aplica la transformacion lineal a la imagen
	cimg_forXY(img,i,j){
		salida(i,j)=lineal(img(i,j),0,-1.0,0.0);
	}
	
	// Devuelve la imagen modificada
	return salida;
}

// Suma dos imagenes
CImg<unsigned char> sumaImg(CImg<unsigned char> img1,CImg<unsigned char> img2){
	// Verifica que las imagenes tengan las mismas dimensiones
	assert(img1.is_sameXY(img2));
	
	CImg<unsigned char> salida(img1.width(),img1.height());
	
	// Suma las imagenes
	cimg_forXY(img1,i,j){
		// Hace el promedio del valor para evitar que se vaya de rango
		salida(i,j)=(img1(i,j)+img2(i,j))/2;
	}
	
	// Devuelve la imagen modificada
	return salida;
}

// Filtro emboss
CImg<unsigned char> emboss(CImg<unsigned char> img,int corrimiento_x=1,int corrimiento_y=1){
	// Calcula el negativo de la imagen de entrada
	CImg<unsigned char> negativo=negativoImg(img);
	
	// Aplica el corrimiento
	negativo.shift(corrimiento_x,corrimiento_y);
	
	// Suma las imagenes
	CImg<unsigned char> salida=sumaImg(img,negativo);
	
	// Muestra los resultados
	CImgList<unsigned char> lista(img,negativo,salida);
	lista.display();
	
	// Devuelve la imagen modificada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img(filename);
	
	CImg<unsigned char> salida;
	
	// Aplica el filtro emboss a la imagen con diferentes desplazamientos
	// Relieve interior
	salida=emboss(img,2,2);
	
	// Relieve exterior
	salida=emboss(img,-2,-2);
	
	// Relives mezclados
	salida=emboss(img,-1,3);
	salida=emboss(img,4,-2);
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
