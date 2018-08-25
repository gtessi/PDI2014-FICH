#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
	// Cargamos la imagen en filename
	const char* filename = cimg_option("-i","futbol.jpg","Image file\n");
	
	/*
	CImg<unsigned char> img(450,382,1,3);
	img.fill(0);
	unsigned char rojo[]={ 55,0,0};
	img.draw_text(0,0,"hola mundo",rojo);
	img.display("");
	*/	
	
	// Inicializamos las variables de imagen
	CImg<unsigned char> img1(filename);
	CImg<float> img2(img1);
	
 	int ancho=img1.width(), alto=img1.height();
	img2.normalize(0,1);
	img2.sqrt();
	img2.resize(ancho/4,alto/4);
	
	// Mostramos los valores de RGB del pixel 0,0	
	for (int i=0;i<1;i++){
		for(int j=0;j<1;j++){
			cout<<img2(i,j,0)<<" ";
			cout<<img2(i,j,1)<<" ";
			cout<<img2(i,j,2)<<" ";
		}
		cout<<endl;
	}
	
	// Le agregamos una linea en el medio de la imagen reducida
	for (int i=0;i<ancho/4;i++){
		img2(i,alto/8,0) = 1;
		img2(i,alto/8,1) = 1;
		img2(i,alto/8,2) = 1;}
	img2.display("");
	
	CImgDisplay vent1(img1,"original"), vent2(img2,"procesada");
	while (!vent1.is_closed()){}
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
