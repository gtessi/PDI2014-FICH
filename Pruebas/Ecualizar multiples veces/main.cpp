#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> cameraman(filename1);
	
	const char* filename2 = cimg_option("-i","stdSLR.jpg","Image file\n");
	CImg<double> autos(filename2);
	
	CImg<double> autos_eq=autos.get_equalize(256,0,255);
	
	(autos,autos_eq).display();
	
	autos_eq=autos_eq.get_equalize(256,0,255);
	
	(autos,autos_eq).display();
	
	autos_eq=autos_eq.get_equalize(256,0,255);
	
	(autos,autos_eq).display();
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
