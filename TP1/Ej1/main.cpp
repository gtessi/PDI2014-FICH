#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename);
	
	// Coordenadas para el corte
	int xini=90, xfin=140;
	int yini=30, yfin=90;
	
	// Inicializa las variables de imagen
	int ancho=xfin-xini, alto=yfin-yini;
	CImg<unsigned char> img2(ancho,alto,1,1,0);
	
	// Recorta el area elegida de la imagen original
	for(int i=0;i<ancho;i++)
		for(int j=0;j<alto;j++)
			img2(i,j)=img1(xini+i,yini+j);
	
	// Muestra las dos imagenes a la vez
	CImgDisplay vent1(img1,"original"), vent2(img2,"procesada");
	while (!vent1.is_closed()){}
	
	// Guarda el recorte como BMP
	img2.save("recorte.bmp");
	
	// 54 bytes son de cabecera de BMP
	// En escala de grises, son 256 tonos por pixel = 1 byte por pixel
	// En BMP hay 3 canales, por lo tanto hay 3 bytes por pixel
	// 1 byte * ancho imagen * alto imagen * 3 bytes + 54 = total de bytes
	
	int total=1*ancho*alto*3+54;
	
	cout<<endl<<endl<<"El tamaño del recorte en BMP es: "<<total<<"."<<endl<<endl;
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
