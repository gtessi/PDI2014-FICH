#include <iostream>
#include <cassert>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Calcula el plano de bit n de la imagen
CImg<bool> obtenerPlano(CImg<unsigned char> img,unsigned int n){
	CImg<bool> plano(img.width(),img.height());
	plano.fill(0);
	
	// Bit significativo
	unsigned int bitSignificativo=pow(2,n);
	
	// Aplica un AND entre la imagen y el bit significativo
	plano=img & bitSignificativo;
	
	// Devuelve el plano de bit n
	return plano;
}

// Funcion que calcula el error cuadratico medio de una imagen
float obtenerECM(CImg<unsigned char> img1,CImg<unsigned char> img2){
	assert(img1.is_sameXY(img2));
	
	float res=0;
	
	// Acumula las diferencias al cuadrado de cada pixel
	int diferencia=0;
	cimg_forXY(img1,i,j){
		// Calcula la diferencia entre dos pixeles
		diferencia=img1(i,j)-img2(i,j);
		
		// Acumula la diferencia al cuadrado
		res+=pow(diferencia,2);
	}
	
	// Calcula el promedio de la sumatoria para normalizar
	res/=(img1.width()*img1.height());
	
	// Devuelve el resultado
	return res;
}

// Funcion que muestra las rodajas de planos de bits
void graficarPlanos(CImg<unsigned char> img){
	CImg<bool> salida;
	
	CImgDisplay ventana1, ventana2;
	
	CImgList<unsigned char> lista;
	lista.push_back(img);
	
	// Calcula los 8 planos de bits
	for(int i=0;i<8;i++){
		// Calcula el plano de bits
		salida=obtenerPlano(img,i);
		
		// Agrega el plano a la lista
		lista.push_back(salida);
		
		// Calcula e imprime en consola el error cuadratico medio
		cout<<"\nPlano #"<<i<<"\t ECM: "<<obtenerECM(img,salida)<<"\t MSE: "<<img.MSE(salida)<<endl;
	}
	
	// Muestra la imagen original y los planos de bits
	lista.display(ventana1,false);
	
	// Acumula los diferentes planos y calcula el error cuadratico medio
	// Toma el ultimo plano calculado
	CImg<int> temp=lista.at(1);
	
	// Crea la lista donde se van a ir mostrando las sumas parciales de los planos
	CImgList<int> acumulado(temp);
	
	// Recorre los planos del bit mas significativo hasta el menos significativo
	for(int i=2;i<9;i++){
		// Acumula los planos
		temp+=lista.at(i)*pow(2,i); // Multiplica por 4, 8, 16, 32, 64 y 128 (bits significativos)
		acumulado.push_back(temp);
		
		// Calcula e imprime en consola el error cuadratico medio
		cout<<"\nPlano #"<<i<<"\t ECM: "<<obtenerECM(img,temp)<<"\t MSE: "<<img.MSE(temp)<<endl;
	}
	
	// Muestra la imagen original y los planos de bits
	acumulado.display(ventana2,false);
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img(filename);
	
	// Grafica los diferentes planos de bits y calcula su error cuadratico medio
	graficarPlanos(img);
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
