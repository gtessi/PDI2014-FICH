#include <iostream>
#include <cassert>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Acota el rango
CImg<int> clipping(CImg<int> img){
	CImg<int> salida(img);
	
	// Aplica el clipping
	cimg_forXY(salida,i,j){
		if (salida(i,j)>255)
			salida(i,j)=255;
		else
			if(salida(i,j)<0)
			salida(i,j)=0;
	}
	
	// Devuelve la imagen acotada
	return salida;
}

// Crea una mascara a partir de un patron dado
CImg<float> crearMascara(unsigned int n,double patron[]){
	CImg<float> mascara(n,n);
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=patron[i+j*n];
	}
	
	// Aplica el factor de escala a la imagen
	//mascara/=(n*n);
	
	// Devuelve la mascara
	return mascara;
}

// Aplica una mascara a una imagen para filtrarla
CImg<int> aplicarMascara(CImg<int> img,CImg<float> mascara){
	CImg<int> salida;
	
	// Convolucion entre la imagen y la mascara
	salida=img.get_convolve(mascara);
	
	// Acota el rango (clipping)
	salida=clipping(salida);
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}

// Filtro pasa-altos calculado como la diferencia entre la imagen original y una
// version suavizada (filtrada con un filtro pasa-bajos)
// Si se pasa el parametro A, se obtiene el filtrado de alta potencia (high-boost)
CImg<int> restaPB(CImg<int> original,CImg<int> filtrada,double A=1.0){
	assert(original.is_sameXY(filtrada));
	
	CImg<int> salida(original.width(),original.height());
	
	if(A<1.0){
		cout<<"\n\n *** ERROR - A menor que 1. ***\n\n";
		return salida;
	}
	
	// Calcula la resta normalizada entre las imagenes
	double temp=0;
	cimg_forXY(original,i,j){
		temp=A*original(i,j)-filtrada(i,j);
		
		// Clipping
		if(temp>255)
			temp=255;
		else
			if(temp<0)
				temp=0;
		
		salida(i,j)=temp;
	}
	
	// Devuelve la imagen modificada
	return salida;
}

// Filtro de alta potencia (segunda version)
CImg<int> restaPBv2(CImg<int> original,CImg<int> filtrada,double A=1.0){
	assert(original.is_sameXY(filtrada));
	
	CImg<int> salida(original.width(),original.height());
	
	if(A<1.0){
		cout<<"\n\n *** ERROR - A menor que 1. ***\n\n";
		return salida;
	}
	
	// Calcula la resta normalizada entre las imagenes
	double temp=0;
	cimg_forXY(original,i,j){
		// (A-1)*f(x,y) + f(x,y) - PB(f(x,y))
		temp=(A-1.0)*original(i,j)+original(i,j)-filtrada(i,j);
		
		// PA(f(x,y)) = f(x,y) - PB(f(x,y)), siendo PA la restaPB entre la
		// imagen original y la que tiene el filtro de promediado
		
		// Clipping
		if(temp>255)
			temp=255;
		else
			if(temp<0)
			temp=0;
		
		salida(i,j)=temp;
	}
	
	// Devuelve la imagen modificada
	return salida;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","camaleon.tif","Image file\n");
	CImg<int> img1(filename1);
	
	const char* filename2 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<int> img2(filename2);
	
	CImgList<> lista;
	
	// Espacios para las imagenes
	CImg<int> filtrada1_3, filtrada1_5, filtrada2_3, filtrada2_5,
		resta1_3, resta1_5, resta2_3, resta2_5;
	
	// Mascaras
	double patron3[]={
		0.11, 0.11, 0.11,
		0.11, 0.11, 0.11,
		0.11, 0.11, 0.11
	}, patron5[]={
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04,
		0.04, 0.04, 0.04, 0.04, 0.04
	};
	
	CImg<float> mascara3=crearMascara(3,patron3);
	CImg<float> mascara5=crearMascara(5,patron5);
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Item 1
	// Imagenes filtradas
	filtrada1_3=aplicarMascara(img1,mascara3);
	filtrada1_5=aplicarMascara(img1,mascara5);
	
	// Restas
	resta1_3=restaPB(img1,filtrada1_3);
	resta1_5=restaPB(img1,filtrada1_5);
	
	lista.push_back(img1);
	lista.push_back(filtrada1_3);
	lista.push_back(resta1_3);
	lista.push_back(filtrada1_5);
	lista.push_back(resta1_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Imagenes filtradas
	filtrada2_3=aplicarMascara(img2,mascara3);
	filtrada2_5=aplicarMascara(img2,mascara5);
	
	// Restas
	resta2_3=restaPB(img2,filtrada2_3);
	resta2_5=restaPB(img2,filtrada2_5);
	
	lista.push_back(img2);
	lista.push_back(filtrada2_3);
	lista.push_back(resta2_3);
	lista.push_back(filtrada2_5);
	lista.push_back(resta2_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Item 2
	CImg<int> resta1A_3, resta1A_5, resta2A_3, resta2A_5;
	
	// Coeficiente A
	//double A=1.2;
	double A=2.0;
	
	// Restas
	resta1A_3=restaPB(img1,filtrada1_3,A);
	resta1A_5=restaPB(img1,filtrada1_5,A);
	
	lista.push_back(img1);
	lista.push_back(resta1_3);
	lista.push_back(resta1A_3);
	lista.push_back(resta1_5);
	lista.push_back(resta1A_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Restas
	resta2A_3=restaPB(img2,filtrada2_3,A);
	resta2A_5=restaPB(img2,filtrada2_5,A);
	
	lista.push_back(img2);
	lista.push_back(resta2_3);
	lista.push_back(resta2A_3);
	lista.push_back(resta2_5);
	lista.push_back(resta2A_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Otra version del filtro de alta potencia
	CImg<int> resta1A2_3, resta1A2_5, resta2A2_3, resta2A2_5;
	
	// Restas
	resta1A2_3=restaPBv2(img1,filtrada1_3,A);
	resta1A2_5=restaPBv2(img1,filtrada1_5,A);
	
	lista.push_back(img1);
	lista.push_back(resta1A_3);
	lista.push_back(resta1A2_3);
	lista.push_back(resta1A_5);
	lista.push_back(resta1A2_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Restas
	resta2A2_3=restaPBv2(img2,filtrada2_3,A);
	resta2A2_5=restaPBv2(img2,filtrada2_5,A);
	
	lista.push_back(img2);
	lista.push_back(resta2A_3);
	lista.push_back(resta2A2_3);
	lista.push_back(resta2A_5);
	lista.push_back(resta2A2_5);
	lista.display();
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
