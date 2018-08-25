#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Crea una mascara dado una dimension n
CImg<float> crearMascara(unsigned int n){
	CImg<float> mascara(n,n,1,1);
	
	// Llena la matriz de unos
	mascara.fill(1);
	
	// Promedia para que la suma sea uno
	mascara/=(n*n);
	
	// Devuelve la mascara
	return mascara;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img(filename);
	
	// Espacios de imagen
	CImg<float> mascara;
	CImg<unsigned char> convolucion;
	
	// Mascara 3x3 llena
	// Hace la convolucion entre la imagen y la mascara, y la normaliza
	mascara=crearMascara(3);
	convolucion=img.get_convolve(mascara);
	convolucion.normalize(0,255);
	convolucion.display();
	
	// Mascara 5x5 llena
	mascara=crearMascara(5);
	convolucion=img.get_convolve(mascara);
	convolucion.normalize(0,255);
	convolucion.display();
	
	// Mascara 11x11 llena
	mascara=crearMascara(11);
	convolucion=img.get_convolve(mascara);
	convolucion.normalize(0,255);
	convolucion.display();
	
	// Matriz cruz 3x3
	// 0 1 0
	// 1 1 1
	// 0 1 0
	mascara(0,1)=1;
	mascara(1,0)=1; mascara(1,1)=1; mascara(1,2)=1;
	mascara(2,1)=1;
	mascara/=5;
	
	convolucion=img.get_convolve(mascara);
	convolucion.normalize(0,255);
	convolucion.display();
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
