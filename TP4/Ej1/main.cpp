#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Invierte los colores de una imagen HSI
CImg<float> invertirHue(CImg<float> img){
	CImg<float> salida(img);
	
	// Ancho
	unsigned int ancho=img.width();
	
	// Invierte el plano H
	cimg_forXY(img,i,j){
		salida(i,j,0,0)=img(ancho-1-i,j,0,0);
	}
	
	// Devuelve la imagen modificada
	return salida;
}


int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","patron.tif","Image file\n");
	CImg<float> patron(filename1);
	
	CImgList<float> lista;
	
	// Planos de RGB
	CImg<float> r, g, b;
	
	r=patron.get_channel(0);
	g=patron.get_channel(1);
	b=patron.get_channel(2);
	
	lista.assign(patron,r,g,b);
	lista.display("Patron - R - G - B");
	
	lista.clear();
	
	// Planos de HSI
	CImg<float> h, s, i, patron_hsi;
	
	// Convierte de RGB a HSI
	patron_hsi=patron.get_RGBtoHSI();
	
	h=patron_hsi.get_channel(0); // Color (0-360)
	s=patron_hsi.get_channel(1); // Saturacion (0-1)
	i=patron_hsi.get_channel(2); // Intensidad (0-1)
	
	lista.assign(patron,h,s,i);
	lista.display("Patron - H - S - I");
	
	lista.clear();
	
	// Variacion de azul a rojo
	CImg<float> patron_hsi_invertido=invertirHue(patron_hsi);
	
	// Convierte a RGB para visualizar
	CImg<float> patron_rgb_invertido=patron_hsi_invertido.get_HSItoRGB();
	
	lista.assign(patron,h,patron_rgb_invertido,patron_hsi_invertido.get_channel(0));
	lista.display("Variacion de azul a rojo");
	
	lista.clear();
	
	// Saturacion y Brillo maximo
	cimg_forXY(patron_hsi_invertido,i,j){
		patron_hsi_invertido(i,j,0,1)=1.0; // Saturacion
		patron_hsi_invertido(i,j,0,2)=1.0; // Brillo
	}
	
	patron_rgb_invertido=patron_hsi_invertido.get_HSItoRGB();
	
	CImg<float> s_max=patron_hsi_invertido.get_channel(1), i_max=patron_hsi_invertido.get_channel(2);
	
	lista.assign(patron,s,i,patron_rgb_invertido,s_max,i_max);
	lista.display("Saturacion y Brillo maximo");
	
	lista.clear();
	
	// Item 2
	// Carga la imagen en filename
	const char* filename2 = cimg_option("-i","pattern.tif","Image file\n");
	CImg<float> pattern(filename2);
	
	// Pasa de RGB a HSI
	CImg<float> pattern_hsi=pattern.get_RGBtoHSI();
	
	// Imagen complementada
	CImg<float> pattern_hsi_complementada(pattern_hsi);
	
	// Calcula el color complementario a partir del plano h
	float h_val=0.0;
	cimg_forXY(pattern_hsi,i,j){
		h_val=pattern_hsi(i,j,0,0);
		
		// El rango de h varia entre 0 y 360
		if(h_val<180)
			pattern_hsi_complementada(i,j,0,0)=h_val+180;
		else
			pattern_hsi_complementada(i,j,0,0)=h_val-180;
	}
	
	// Convierte de HSI a RGB para visualizar
	CImg<float> pattern_complementada=pattern_hsi_complementada.get_HSItoRGB();
	
	lista.assign(pattern,pattern_complementada);
	lista.display("Original y Complementada");
	
	lista.clear();
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
