#include <iostream>
#include <vector>
#include <cassert>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Cadenas con los parametros para las funciones
// Estructura: inicio final valor
struct parametros{
	unsigned int inicio;
	unsigned int final;
	
	bool valor;
};

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacion(parametros nombre,vector<bool> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	bool valor=nombre.valor;
	
	// Redimensiona el vector LUT
	LUT.resize(256,0);
	
	// Crea el vector LUT segun los parametros
	for(unsigned int i=0;i<LUT.size();i++){
		if(i>=inicio && i<=final)
			LUT[i]=valor;
		else
			LUT[i]=!valor;
	}
}

// Dada una imagen y un vector LUT, aplica la transformacion
CImg<bool> aplicarLUT(CImg<unsigned char> img,vector<bool> LUT){
	CImg<bool> s(img.width(),img.height());
	
	// Aplica la LUT a toda la imagen
	cimg_forXY(img,i,j){
		// El vector LUT tiene un mapeo de valores en el rango 0-1 para cada pixel
		s(i,j)=LUT[img(i,j)];
	}
	
	// Devuelve la imagen modificada
	return s;
}

// Devuelve una imagen que representa el mapeo de la LUT
CImg<bool> imagenMapeo(vector<bool> LUT){
	unsigned int ancho=LUT.size()+1;
	
	CImg<bool> mapeo(ancho,ancho,1,1,0);
	
	for(unsigned int i=0;i<ancho;i++){
		// Hace un corrimiento de 5 pixeles en y para que se vean las lineas
		if(LUT[i])
			mapeo(i,5)=true;
		else
			mapeo(i,250)=true;
	}
	
	// Devuelve la imagen
	return mapeo;
}

//----------------------------------------------------------------------
// Operaciones logicas
// AND
CImg<bool> andImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) and img2(i,j);
	}
	
	return salida;
}

// OR
CImg<bool> orImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) or img2(i,j);
	}
	
	return salida;
}

// XOR
CImg<bool> xorImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) xor img2(i,j);
	}
	
	return salida;
}

// NOT
CImg<bool> notImg(CImg<bool> img){
	CImg<bool> salida(img.width(),img.height());
	
	cimg_forXY(img,i,j){
		salida(i,j)=!img(i,j);
		//salida(i,j)=not(img(i,j));
	}
	
	return salida;
}
//----------------------------------------------------------------------
// Operaciones relacionales
// Menor <
CImg<bool> menorImg(CImg<unsigned char> img1,CImg<unsigned char> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) < img2(i,j);
	}
	
	return salida;
}

// Menor o igual <=
CImg<bool> menorIgualImg(CImg<unsigned char> img1,CImg<unsigned char> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) <= img2(i,j);
	}
	
	return salida;
}

// Mayor >=
CImg<bool> mayorImg(CImg<unsigned char> img1,CImg<unsigned char> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) > img2(i,j);
	}
	
	return salida;
}

// Mayor o igual >=
CImg<bool> mayorIgualImg(CImg<unsigned char> img1,CImg<unsigned char> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) >= img2(i,j);
	}
	
	return salida;
}
//----------------------------------------------------------------------


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	const char* filename2 = cimg_option("-i","clown.jpg","Image file\n");
	CImg<unsigned char> img2(filename2);
	
	CImg<bool> mapeo, salida;
	
	CImgList<unsigned char> lista;
	
	// Vector LUT
	vector<bool> LUT;
	
	// Item 1
	// Parametros para los umbrales
	parametros invertido;
	invertido.inicio=0;
	invertido.final=150;
	invertido.valor=1;
	
	// Aplica la transformacion
	crearTransformacion(invertido,LUT);
	mapeo=imagenMapeo(LUT);
	salida=aplicarLUT(img1,LUT);
	
	LUT.clear();
	
	// Normaliza
	mapeo.normalize(0,255);
	salida.normalize(0,255);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(mapeo);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Aplica la transformacion
	crearTransformacion(invertido,LUT);
	mapeo=imagenMapeo(LUT);
	salida=aplicarLUT(img2,LUT);
	
	LUT.clear();
	
	// Normaliza
	mapeo.normalize(0,255);
	salida.normalize(0,255);
	
	// Muestra los resultados
	lista.push_back(img2);
	lista.push_back(mapeo);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	
	// Parametros para los umbrales
	parametros intervalo;
	intervalo.inicio=127;
	intervalo.final=192;
	intervalo.valor=0;
	
	// Aplica la transformacion
	crearTransformacion(intervalo,LUT);
	mapeo=imagenMapeo(LUT);
	salida=aplicarLUT(img1,LUT);
	
	LUT.clear();
	
	// Normaliza
	mapeo.normalize(0,255);
	salida.normalize(0,255);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(mapeo);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Aplica la transformacion
	crearTransformacion(intervalo,LUT);
	mapeo=imagenMapeo(LUT);
	salida=aplicarLUT(img2,LUT);
	
	LUT.clear();
	
	// Normaliza
	mapeo.normalize(0,255);
	salida.normalize(0,255);
	
	// Muestra los resultados
	lista.push_back(img2);
	lista.push_back(mapeo);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Item 2
	// Parametros para los umbrales
	parametros umbral;
	umbral.inicio=0;
	umbral.final=127;
	umbral.valor=0;
	
	// Aplica la transformacion
	crearTransformacion(umbral,LUT);
	
	CImg<bool> img1bool=aplicarLUT(img1,LUT);
	CImg<bool> img2bool=aplicarLUT(img2,LUT);
	
	LUT.clear();
	
	// Operaciones logicas
	// AND
	salida=andImg(img1bool,img2bool);
	
	// Muestra los resultados
	lista.push_back(img1bool);
	lista.push_back(img2bool);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// OR
	salida=orImg(img1bool,img2bool);
	
	// Muestra los resultados
	lista.push_back(img1bool);
	lista.push_back(img2bool);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// XOR
	salida=xorImg(img1bool,img2bool);
	
	// Muestra los resultados
	lista.push_back(img1bool);
	lista.push_back(img2bool);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// NOT
	salida=notImg(img1bool);
	
	lista.push_back(img1bool);
	lista.push_back(salida);
	
	salida=notImg(img2bool);
	
	lista.push_back(img2bool);
	lista.push_back(salida);
	
	// Muestra los resultados
	lista.display();
	
	lista.clear();
	
	// Operacion compuesta
	salida=andImg(xorImg(img1bool,img2bool),orImg(notImg(img1bool),img2bool));
	salida.display();	
	// Item 3
	// Operaciones relacionales
	// Menor
	salida=menorImg(img1,img2);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(img2);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Menor o igual
	salida=menorIgualImg(img1,img2);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(img2);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Mayor
	salida=mayorImg(img1,img2);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(img2);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	// Mayor o igual
	salida=mayorIgualImg(img1,img2);
	
	// Muestra los resultados
	lista.push_back(img1);
	lista.push_back(img2);
	lista.push_back(salida);
	lista.display();
	
	lista.clear();
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
