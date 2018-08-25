#include <iostream>
#include <CImg.h>

using namespace std;
using namespace cimg_library;

int main(int argc, char **argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","esqueleto.tif","Image file\n");
	CImg<double> esqueleto(filename1);
	
	esqueleto.display();
	
	return 0;
}
