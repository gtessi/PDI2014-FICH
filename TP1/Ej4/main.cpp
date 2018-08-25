#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Funcion Item 4
void item4(int n, int r){
	// n = tamaño del cuadrado
	// r = radio del circulo
	
	// Crea la imagen
	CImg<bool> img(n,n,1,1,0);
	
	// Fondo negro
	img.fill(0);
	
	// Calcula el centro
	int centro=n/2;
	
	// Calcula el radio al cuadrado
	int radio2=r*r;
	
	// Dibuja el circulo de radio r
	cimg_forXY(img,x,y)
		if ((x-centro)*(x-centro)+(y-centro)*(y-centro)<=radio2) // Si esta dentro del circulo
			img(x,y)=true;
		else
			img(x,y)=false;
	
	// Hace el circulo con metodo de la clase CImg
	//img.draw_circle(centro,centro,r,blanco,1);
	/*
	CImg<T>& cimg_library::CImg< T >::draw_circle	(	const int 	x0,
	const int 	y0,
	int 	radius,
	const tc *const  	color,
	const float 	opacity = 1	 
	)			 [inline]
	Draw a filled circle.
	
	Parameters:
	x0 	X-coordinate of the circle center.
	y0 	Y-coordinate of the circle center.
	radius 	Circle radius.
	color 	Array of spectrum() values of type T, defining the drawing color.
	opacity 	Drawing opacity.
	*/
	
	// Muestra el resultado
	img.display("Item 4");
}

int main(int argc, char** argv) {
	// Item 1
	CImg<unsigned char> img1(256,256,1,1,0);
	
	// Le asigna valores de forma que cada columna vaya de 0 a 255
	// Recorre primero por columnas
	cimg_forXY(img1,i,j)
		img1(i,j)=i;
	
	// Muestra el resultado
	img1.display("Item 1");
	
	// Item 2
	CImg<unsigned char> img2(100,100,1,1,0);
	
	// Le asigna valores de forma que cada columna vaya de 0 a 100
	// Recorre primero por columnas
	cimg_forXY(img2,i,j)
		img2(i,j)=i;
	
	// Muestra el resultado
	img2.display("Item 2 - 0-100");
	img2.normalize(0,255);
	img2.display("Item 2 - 0-255 Normalizado");
	
	// Item 3
	CImg<unsigned char> img3(100,100,1,1,0);
	
	// Le asigna valores de forma que cada columna tome valores intercalados 0 y 1
	// Recorre primero por columnas
	bool flip=0;
	
	cimg_forXY(img3,i,j){
		if(flip==true)
			img3(i,j)=1;
		else
			img3(i,j)=0;
		flip=!flip;
	}
	
	// Muestra el resultado
	img3.display("Item 3");
	
	// Item 4
	item4(200,50); // Tamaño 200x200, Radio 50
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
