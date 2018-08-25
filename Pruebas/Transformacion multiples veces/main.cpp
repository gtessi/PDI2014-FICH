#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Trunca los valores entre 0 y 255 para normalizar
unsigned char truncar(float val){
	unsigned char res=0;
	
	if(val>255)
		res=255;
	else
		if(val<0)
		res=0;
		else
			res=floor(val);
	
	return res;
}

// Transformacion
unsigned char transformacion(unsigned char r){
	float s=0;
	
	if(r<100)
		s=0;
	else if(r>200)
		s=255;
		else
			s=255*(r-100)/100;
	
	return truncar(s);
}

CImg<unsigned char> aplicaT(CImg<unsigned char> r){
	CImg<unsigned char> salida(r);
	salida.fill(0);
	
	cimg_forXY(r,i,j){
		salida(i,j)=transformacion(r(i,j));
	}
	
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> cameraman(filename1);
	
	CImg<unsigned char> res=aplicaT(cameraman);
	
	(cameraman,res).display();
	
	res=aplicaT(res);
	
	(cameraman,res).display();
	
	res=aplicaT(res);
	
	(cameraman,res).display();
	
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
