#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename);
	
	filename = cimg_option("-i","colores.jpg","Image file\n");
	CImg<unsigned char> img2(filename);
	
	filename = cimg_option("-i","manzana.jpg","Image file\n");
	CImg<unsigned char> img3(filename);
	
	// Muestra informacion en consola de cada imagen (print)
	img1.print("cameraman.tif");
	
	cout<<endl<<"*********************************************************"<<endl<<endl;
	
	img2.print("colores.jpg");
	
	cout<<endl<<"*********************************************************"<<endl<<endl;
	
	img3.print("manzana.jpg");
	
	cout<<endl<<"*********************************************************"<<endl<<endl;
	
	// Muestra informacion en consola de cada imagen (stats)
	// stats = Compute statistics vector from the pixel values
	
	// Muestra las imagenes en ventanas separadas a la vez (CImgDisplay)
	CImgDisplay vent1(img1,"cameraman.tif"), vent2(img2,"colores.jpg"), vent3(img3,"manzana.jpg");
	while (!vent1.is_closed()){}
	
	// Muestra las imagenes en ventanas separadas de a una con informacion adicional (CImg&display)
	img1.display("cameraman.tif");
	img2.display("colores.jpg");
	img3.display("manzana.jpg");
	
	cout<<endl<<"*********************************************************"<<endl<<endl;
	
	// Muestra varias imagenes en una sola ventana (CImgList)
	// CImgList permite poner varias imagenes en una misma ventana, por resolucion, por copia de una imagen, como mosaico
	CImgList<unsigned char> lista(img1,img2,img3,true);
	
	lista.display("Lista");
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
