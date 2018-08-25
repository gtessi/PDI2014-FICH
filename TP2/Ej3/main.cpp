#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Item 1
// a) suma
CImg<unsigned char> suma(CImg<unsigned char> img1,CImg<unsigned char> img2){
	CImg<unsigned char> s=img1;
	s.fill(0);
	
	cimg_forXY(img1,i,j){
		// Suma y normaliza
		s(i,j)=(img1(i,j)+img2(i,j))/2;
	}
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img1,img2,s);
	lista.display();
	
	// Devuelve la imagen modificada
	return s;
}

// b) resta
CImg<unsigned char> resta1(CImg<unsigned char> img1,CImg<unsigned char> img2){
	CImg<unsigned char> s=img1;
	s.fill(0);
	
	cimg_forXY(img1,i,j){
		// Resta y normaliza
		s(i,j)=((img1(i,j)-img2(i,j))+255)/2;
	}
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img1,img2,s);
	lista.display();
	
	// Devuelve la imagen modificada
	return s;
}

CImg<unsigned char> resta2(CImg<unsigned char> img1,CImg<unsigned char> img2){
	CImg<unsigned char> s=img1;
	s.fill(0);
	
	cimg_forXY(img1,i,j){
		// Resta
		s(i,j)=img1(i,j)-img2(i,j);
	}
	
	// Le resta el minimo a toda la matriz
	s=s-s.min();
	
	// Normaliza
	// falta ver que onda con esto
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img1,img2,s);
	lista.display();
	
	// Devuelve la imagen modificada
	return s;
}

// c) multiplicacion
CImg<unsigned char> multiplicacion(CImg<unsigned char> img1,CImg<bool> img2){
	CImg<unsigned char> s=img1;
	s.fill(0);
	
	cimg_forXY(img1,i,j){
		// Multiplicacion
		s(i,j)=img1(i,j)*img2(i,j);
	}
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img1,img2,s);
	lista.display();
	
	// Devuelve la imagen modificada
	return s;
}

// d) division
CImg<unsigned char> division(CImg<unsigned char> img1,CImg<unsigned char> img2){
	CImg<unsigned char> s=img1;
	s.fill(0);
	
	cimg_forXY(img1,i,j){
		// Division es multiplicacion de una por la reciproca de la otra
		if(img2(i,j)==0)
			s(i,j)=img1(i,j);
		else
			s(i,j)=img1(i,j)*(1/img2(i,j));
	}
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img1,img2,s);
	lista.display();
	
	// Devuelve la imagen modificada
	return s;
}

// Item 2
CImg<int> ruido_promediado(CImg<int> img,double sigma,int tipo){
	CImg<int> promedio(img), temp(img), suma(img);
	
	// Crea 50 imagenes con ruido y las acumula
	for(int i=0;i<50;i++){
		// Aplica ruido con distribucion normal
		// Parametros: sigma es un double, tipo es un int
		// (0 es gaussiana, 1 es normal, 2 es sal y pimienta)
		temp=img.get_noise(sigma,tipo);
		
		// Acumula las imagenes con ruido
		suma=suma+temp;
	}
	
	// Calcula el promedio de la suma
	promedio=suma/50;
	
	// Muestra las imagenes
	CImgList<unsigned char> lista(img,temp,promedio);
	lista.display();
	
	// Devuelve la imagen modificada
	return promedio;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","huang2.jpg","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	const char* filename2 = cimg_option("-i","star.jpg","Image file\n");
	CImg<unsigned char> img2(filename2);
	
	const char* filename3 = cimg_option("-i","mask.bmp","Image file\n");
	CImg<bool> mascara(filename3);
	
	CImg<unsigned char> salida;
	CImg<int> salida2;
	
	// Item 1
	// Suma
	salida=suma(img1,img2);
	
	// Resta 1
	salida=resta1(img1,img2);
	
	// Resta 2
	salida=resta2(img1,img2);
	
	// Multiplicacion
	salida=multiplicacion(img1,mascara);
	
	// Division
	salida=division(img1,img2);
	
	// Item 2
	salida2=ruido_promediado(img1,0.05,1);
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
