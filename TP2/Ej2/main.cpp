#include <iostream>
#include <vector>
#include "CImg.h"
#include "../transformaciones.h"

using namespace cimg_library;
using namespace std;

// Cadenas con los parametros para las funciones
// Estructura: inicio final gamma offset
struct parametros{
	unsigned int inicio;
	unsigned int final;
	
	float gamma;
	float offset;
};

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacionLOG(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=logaritmica(i,offset);
		
		// Agrega el valor al vector LUT
		LUT.push_back(valor);
	}
}

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacionPOT(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float gamma=nombre.gamma;
	float offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=potencia(i,offset,gamma);
		
		// Agrega el valor al vector LUT
		LUT.push_back(valor);
	}
}

// Dada una imagen y un vector LUT, aplica la transformacion
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img,vector<unsigned char> LUT){
	CImg<unsigned char> s(img.width(),img.height());
	
	// Aplica la LUT a toda la imagen
	cimg_forXY(img,i,j){
		// El vector LUT tiene un mapeo de valores en el rango 0-255 para cada pixel
		s(i,j)=LUT[img(i,j)];
	}
	
	// Devuelve la imagen modificada
	return s;
}

// Devuelve una imagen que representa el mapeo de la LUT
CImg<unsigned char> imagenMapeo(vector<unsigned char> LUT){
	unsigned int ancho=LUT.size()+1;
	
	CImg<unsigned char> mapeo(ancho,ancho,1,1,0);
	
	for(unsigned int i=0;i<ancho;i++){
		// Crea la curva de la LUT
		mapeo(i,ancho-LUT[i]-1)=255;
	}
	
	// Devuelve la imagen
	return mapeo;
}

// Calcula y grafica el mapeo del vector LUT y la imagen resultante
void graficas(CImg<unsigned char> img,vector<unsigned char> LUT){
	// Aplica LUT a la imagen
	CImg<unsigned char> salida=aplicarLUT(img,LUT);
	
	// Calcula la curva del mapeo LUT
	CImg<unsigned char> mapeo=imagenMapeo(LUT);
	
	// Muestra en pantalla los resultados
	CImgList<unsigned char> lista(img,mapeo,salida);
	lista.display();
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","rmn.jpg","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	// Vector LUT
	vector<unsigned char> LUT;
	
	// Item 1
	// En transformaciones.h
	
	// Item 2
	// Logaritmica
	// Muestra cosas que no se ven en el fondo negro
	parametros log1;
	log1.inicio=0;
	log1.final=255;
	log1.gamma=0.0; // El gamma no se usa en logaritmo
	log1.offset=5.0;
	
	crearTransformacionLOG(log1,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	// Potencia
	// Intensifica el fondo pero se satura demasiado
	parametros pot1;
	pot1.inicio=0;
	pot1.final=255;
	pot1.gamma=2.0;
	pot1.offset=1.0;
	
	crearTransformacionPOT(pot1,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	parametros pot2;
	pot2.inicio=0;
	pot2.final=255;
	pot2.gamma=3.0;
	pot2.offset=1.0;
	
	crearTransformacionPOT(pot2,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	// No remarca nada de la imagen, muy saturada
	parametros pot3;
	pot3.inicio=0;
	pot3.final=255;
	pot3.gamma=-2.0;
	pot3.offset=1.0;
	
	crearTransformacionPOT(pot3,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	// Resalta cosas en el fondo oscuro
	parametros pot4;
	pot4.inicio=0;
	pot4.final=255;
	pot4.gamma=0.5;
	pot4.offset=1.0;
	
	crearTransformacionPOT(pot4,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	parametros pot5;
	pot5.inicio=0;
	pot5.final=255;
	pot5.gamma=0.25;
	pot5.offset=1.0;
	
	crearTransformacionPOT(pot5,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
