#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

CImg<unsigned char> calcularHistograma(CImg<unsigned char> img){
	CImg<unsigned char> histograma(img.width(),img.height());
	histograma.fill(255);
	
	// Patron de grises
	CImg<unsigned char> patron;
	
	// Vector color
	int color[]={0,0,0}; // R G B
	
	// Calcula el histograma de la imagen
	patron=img.get_histogram(256,0,255);
	
	// Dibuja el histograma
	histograma.draw_graph(patron,color,1,3,0);
	
	// Devuelve el histograma
	return histograma;
}

int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	// Listas
	CImgList<unsigned char> lista1,lista2;
	
	// Calcula el histograma de la imagen original
	CImg<unsigned char> histo1=calcularHistograma(img1);
	
	// Ecualiza la imagen
	CImg<unsigned char> img1_eq;
	img1_eq=img1.get_equalize(256,0,255);
	//img1_eq=img1.get_equalize(128,0,255);
	//img1_eq=img1.get_equalize(32,0,255);
	//img1_eq=img1.get_equalize(16,0,255);
	
	// Calcula el histograma de la imagen ecualizada
	CImg<unsigned char> histo1_eq=calcularHistograma(img1_eq);
	
	// Muestra las imagenes y sus histogramas
	lista1.push_back(img1);
	lista1.push_back(img1_eq);
	
	lista2.push_back(histo1);
	lista2.push_back(histo1_eq);
	
	lista1.get_append('x').append(lista2.get_append('x'),'y').display("Lista");
	
	cout<<"*********************************************************"<<endl<<endl;
	
	// Item 2
	// histo1.tif es una imagen oscura, con poco contraste ?
	// histo2.tif es una imagen clara, con mucho contraste ?
	// histo3.tif es una imagen muy oscura
	// histo4.tif es una imagen muy clara, con poco contraste ?
	// histo5.tif es una imagen clara, con mucho contraste ?
	
	// Correspondencia
	// histo1.tif = ImagenA.tif
	// histo2.tif = ImagenC.tif
	// histo3.tif = ImagenE.tif
	// histo4.tif = ImagenB.tif
	// histo5.tif = imagenD.tif
	
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","ImagenA.tif","Image file\n");
	CImg<unsigned char> imgA(filename);
	
	const char* filenameB = cimg_option("-i","ImagenB.tif","Image file\n");
	CImg<unsigned char> imgB(filenameB);
	
	const char* filenameC = cimg_option("-i","ImagenC.tif","Image file\n");
	CImg<unsigned char> imgC(filenameC);
	
	const char* filenameD = cimg_option("-i","ImagenD.tif","Image file\n");
	CImg<unsigned char> imgD(filenameD);
	
	const char* filenameE = cimg_option("-i","ImagenE.tif","Image file\n");
	CImg<unsigned char> imgE(filenameE);
	
	// Histograma
	CImg<unsigned char> histo;
	
	// Calcula el histograma de la imagen
	histo=imgA.get_histogram(256,0,255);
	histo.display_graph(0,3);
	
	histo=imgB.get_histogram(256,0,255);
	histo.display_graph(0,3);
	
	histo=imgC.get_histogram(256,0,255);
	histo.display_graph(0,3);
	
	histo=imgD.get_histogram(256,0,255);
	histo.display_graph(0,3);
	
	histo=imgE.get_histogram(256,0,255);
	histo.display_graph(0,3);
	
	// Histogramas reales
	// ImagenA.tif = histo2.tif
	// ImagenB.tif = histo4.tif
	// ImagenC.tif = histo1.tif
	// ImagenD.tif = histo5.tif
	// imagenE.tif = histo3.tif
	
	// Correspondencia
	// histo1.tif = ImagenA.tif -- incorrecto
	// histo2.tif = ImagenC.tif -- incorrecto
	// histo3.tif = ImagenE.tif -- correcto
	// histo4.tif = ImagenB.tif -- correcto
	// histo5.tif = imagenD.tif -- correcto
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
