#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Funcion de medios tonos
CImg<bool> halftoning(CImg<unsigned char> img){
	// Alternativa sin usar un factor de interpolacion
	// La deja en 10 niveles y la normaliza entre 0 y 9
	// img=img.get_quantize(10).get_normalize(0,9);
	
	// Tamaño de la imagen
	unsigned int alto=img.height();
	unsigned int ancho=img.width();
	
	// Normaliza la imagen de entrada
	img.normalize(0,255);
	
	// Crea la imagen de medios tonos
	CImg<bool> img_salida(ancho*3,alto*3);
	
	// Factor para interpolar los niveles de gris
	float factor=256.0/10.0;
	
	// Patron de grises para el halftoning
	bool patron_grises[10][3][3]={
			{{1,1,1},{1,1,1},{1,1,1}}, // Tono 0
			{{1,0,1},{1,1,1},{1,1,1}}, // Tono 1
			{{1,0,1},{1,1,1},{1,1,0}}, // Tono 2
			{{0,0,1},{1,1,1},{1,1,0}}, // Tono 3
			{{0,0,1},{1,1,1},{0,1,0}}, // Tono 4
			{{0,0,0},{1,1,1},{0,1,0}}, // Tono 5
			{{0,0,0},{1,1,0},{0,1,0}}, // Tono 6
			{{0,0,0},{1,1,0},{0,0,0}}, // Tono 7
			{{0,0,0},{0,1,0},{0,0,0}}, // Tono 8
			{{0,0,0},{0,0,0},{0,0,0}}  // Tono 9
	};
	
	cimg_forXY(img,x,y){
		// Segun el nivel de gris del pixel actual, le aplica el factor para llevarlo
		// al rango 0-9 de medios tonos, y lo invierte porque el tono 0 es negro
		// y en CImg el negro es 1 (bases invertidas)
		unsigned int nivel=9-floor(img(x,y)/factor);
		
		// Construye la imagen de medios tonos haciendo un mapeo de cada pixel
		// de la imagen original a cada submatriz
		// Columna 1
		img_salida(3*x,3*y)=patron_grises[nivel][0][0];
		img_salida(3*x+1,3*y)=patron_grises[nivel][0][1];
		img_salida(3*x+2,3*y)=patron_grises[nivel][0][2];
		
		// Columna 2
		img_salida(3*x,3*y+1)=patron_grises[nivel][1][0];
		img_salida(3*x+1,3*y+1)=patron_grises[nivel][1][1];
		img_salida(3*x+2,3*y+1)=patron_grises[nivel][1][2];
		
		// Columna 3
		img_salida(3*x,3*y+2)=patron_grises[nivel][2][0];
		img_salida(3*x+1,3*y+2)=patron_grises[nivel][2][1];
		img_salida(3*x+2,3*y+2)=patron_grises[nivel][2][2];
	}
	
	// Devuelve la imagen procesada
	return img_salida;
}

int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	//const char* filename = cimg_option("-i","lake.jpg","Image file\n"); // lago
	const char* filename = cimg_option("-i","nature.jpg","Image file\n"); // naturaleza
	CImg<unsigned char> img(filename); // imagen
	
	// Calcula la imagen de medios tonos a partir de la original
	CImg<bool> salida=halftoning(img);
	
	// Muestra la imagen de medios tonos
	CImgDisplay ventana1(salida,"Imagen de medios tonos");
	
	// Item 2
	// Crea la imagen para el degrade
	CImg<unsigned int> degrade(256,256);
	
	cimg_forXY(degrade,x,y)
		degrade(x,y)=x;
	
	// Calcula la imagen de medios tonos
	CImg<bool> salida_degrade=halftoning(degrade);
	
	// Muestra la imagen de medios tonos
	CImgDisplay ventana2(salida_degrade,"Imagen de medios tonos");
	
	// Item 3
	// Carga las imagenes
	const char* filename1 = cimg_option("-i","huang1.jpg","Image file\n");
	const char* filename2 = cimg_option("-i","huang2.jpg","Image file\n");
	const char* filename3 = cimg_option("-i","huang3.jpg","Image file\n");
	CImg<unsigned char> huang1(filename1);
	CImg<unsigned char> huang2(filename2);
	CImg<unsigned char> huang3(filename3);
	
	// Calcula las imagenes de medios tonos
	CImg<bool> salida_huang1=halftoning(huang1);
	CImg<bool> salida_huang2=halftoning(huang2);
	CImg<bool> salida_huang3=halftoning(huang3);
	
	// Guarda las imagenes en un archivo
	// Salidas
	//salida_huang1.save("huang1_halftone.jpg");
	
	// no me deja guardar los archivos como jpg
	
	salida_huang1.display();
	salida_huang2.display();
	salida_huang3.display();
	
	while(!ventana1.is_closed()){};
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
