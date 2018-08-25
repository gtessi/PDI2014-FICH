#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","rmn.jpg","Image file\n");
	CImg<unsigned char> img100(filename); // imagen
	
	// Dimensiones originales
	unsigned int alto=img100.height();
	unsigned int ancho=img100.width();
	
	// Aprovecha el pipeline, primero reduce la imagen y la vuelve a agrandar
	// como si se tratara de un objeto sobre otro (viola el encapsulamiento)
	
	// Escala la imagen original en un 50%
	CImg<unsigned char> img50=img100.get_resize(alto/2,ancho/2).get_resize(alto,ancho);
	
	// Escala la imagen original en un 25%
	CImg<unsigned char> img25=img100.get_resize(alto/4,ancho/4).get_resize(alto,ancho);
	
	// Escala la imagen original en un 12.5%
	CImg<unsigned char> img125=img100.get_resize(alto/8,ancho/8).get_resize(alto,ancho);
	
//	// Guarda las imagenes escaladas
//	img50.save("rmn50.jpg"); // 50%
//	img25.save("rmn25.jpg"); // 25%
//	img125.save("rmn125.jpg"); // 12.5%
	
	// no me deja guardar los archivos como jpg
	
	// Crea una lista donde muestra todas las imagenes
	CImgList<unsigned char> lista;
	lista.assign(img100,img50,img25,img125);
	lista.display("Lista de imagenes con diferente resolucion espacial");
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
