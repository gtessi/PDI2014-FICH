#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","huang2.jpg","Image file\n");
	CImg<unsigned char> img_original(filename); // imagen
	CImg<unsigned char> img_temp; // imagen temporal
	
	// Define las listas
	CImgList<unsigned char> lista1;
	CImgList<unsigned char> lista2;
	
	lista1.push_back(img_original);
	
	// Variable de bits
	int bits=0;
	
	// Cuantizaciones
	// de 7 a 5
	for(int i=7;i>4;i--){
		// Cantidad de niveles
		bits=pow(2,i);
		
		// Cuantiza y normaliza
		img_temp=img_original.get_quantize(bits).get_normalize(0,255);
		
		// Agrega la imagen a la lista
		lista1.push_back(img_temp);
	}
	
	// de 4 a 1
	for(int i=4;i>0;i--){
		// Cantidad de niveles
		bits=pow(2,i);
		
		// Cuantiza y normaliza
		img_temp=img_original.get_quantize(bits).get_normalize(0,255);
		
		// Agrega la imagen a la lista
		lista2.push_back(img_temp);
	}
	
	// no me deja guardar los archivos como jpg
	
	// Muestra las imagenes en secuencia
	// Arma el display con lista1 como primer fila y lista2 como segunda, luego lo muestra en pantalla
	// Las imagenes van de 8 bits a 5 (primer fila), y de 4 a 1 (segunda fila)
	lista1.get_append('x').append(lista2.get_append('x'),'y').display("Lista");
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
