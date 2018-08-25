#include <iostream>
#include "CImg.h"

#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Genera un degrade en escala de grises
CImg<unsigned char> generarDegrade(unsigned int size){
	CImg<unsigned char> salida(size,size);
	
	cimg_forXY(salida,i,j){
		salida(i,j)=i;
	}
	
	// Devuelve el degrade
	return salida;
}

// Aplica una paleta a una imagen en escala de grises
// Una paleta tiene ancho=1 y alto=256
CImg<float> aplicarPaleta(CImg<unsigned char> img_grises,CImg<float> paleta){
	CImg<float> salida(img_grises.width(),img_grises.height(),1,3);
	
	// Hace el mapeo de los grises con la paleta
	cimg_forXYC(img_grises,i,j,c){
		salida(i,j,c)=paleta(0,img_grises(i,j),c);
	}
	
	// Devuelve la imagen con falso color
	return salida;
}

// Colorea una imagen binaria a partir de dos vectores de color
CImg<unsigned char> aplicarColorBin(CImg<bool> img_bin,unsigned int color1[],unsigned int color2[]){
	CImg<unsigned char> salida(img_bin.width(),img_bin.height(),1,3);
	
	// Colorea la imagen binaria
	cimg_forXYC(img_bin,i,j,c){
		if(img_bin(i,j,c))
			salida(i,j,c)=color1[c];
		else
			salida(i,j,c)=color2[c];
	}
	
	// Devuelve la imagen coloreada
	return salida;
}


int main(int argc, char** argv) {
	// Paletas
	// bone.pal
	// cool.pal
	// copper.pal
	// gray.pal
	// hot.pal
	// hsv.pal
	// jet.pal
	// pink.pal
	
	// Carga una paleta
	CImg<float> paleta_bone=cargar_paleta("../paletas/bone.pal");
	CImg<float> paleta_cool=cargar_paleta("../paletas/cool.pal");
	CImg<float> paleta_copper=cargar_paleta("../paletas/copper.pal");
	CImg<float> paleta_gray=cargar_paleta("../paletas/gray.pal");
	CImg<float> paleta_hot=cargar_paleta("../paletas/hot.pal");
	CImg<float> paleta_hsv=cargar_paleta("../paletas/hsv.pal");
	CImg<float> paleta_jet=cargar_paleta("../paletas/jet.pal");
	CImg<float> paleta_pink=cargar_paleta("../paletas/pink.pal");
	
	// Item 1
	// Genera el degrade de prueba
	CImg<unsigned char> degrade=generarDegrade(256);
	
	// Aplica la paleta al degrade
	CImg<float> degrade_color_bone=aplicarPaleta(degrade,paleta_bone);
	CImg<float> degrade_color_cool=aplicarPaleta(degrade,paleta_cool);
	CImg<float> degrade_color_copper=aplicarPaleta(degrade,paleta_copper);
	CImg<float> degrade_color_gray=aplicarPaleta(degrade,paleta_gray);
	CImg<float> degrade_color_hot=aplicarPaleta(degrade,paleta_hot);
	CImg<float> degrade_color_hsv=aplicarPaleta(degrade,paleta_hsv);
	CImg<float> degrade_color_jet=aplicarPaleta(degrade,paleta_jet);
	CImg<float> degrade_color_pink=aplicarPaleta(degrade,paleta_pink);
	
	// Visualiza los resultados
	(degrade,paleta_bone,degrade_color_bone).display("Bone");
	(degrade,paleta_cool,degrade_color_cool).display("Cool");
	(degrade,paleta_copper,degrade_color_copper).display("Copper");
	(degrade,paleta_gray,degrade_color_gray).display("Gray");
	(degrade,paleta_hot,degrade_color_hot).display("Hot");
	(degrade,paleta_hsv,degrade_color_hsv).display("HSV");
	(degrade,paleta_jet,degrade_color_jet).display("Jet");
	(degrade,paleta_pink,degrade_color_pink).display("Pink");
	
	// Item 2
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> cameraman(filename1);
	
	const char* filename2 = cimg_option("-i","imagenC.tif","Image file\n");
	CImg<unsigned char> imagenC(filename2);
	
	const char* filename3 = cimg_option("-i","imagenD.tif","Image file\n");
	CImg<unsigned char> imagenD(filename3);
	
	// Aplica la paleta a la imagen
	// cameraman.tif
	CImg<float> cameraman_color_bone=aplicarPaleta(cameraman,paleta_bone);
	CImg<float> cameraman_color_hot=aplicarPaleta(cameraman,paleta_hot);
	CImg<float> cameraman_color_hsv=aplicarPaleta(cameraman,paleta_hsv);
	
	// imagenC.tif
	CImg<float> imagenC_color_copper=aplicarPaleta(imagenC,paleta_copper);
	CImg<float> imagenC_color_jet=aplicarPaleta(imagenC,paleta_jet);
	
	// imagenD.tif
	CImg<float> imagenD_color_hot=aplicarPaleta(imagenD,paleta_hot);
	CImg<float> imagenD_color_pink=aplicarPaleta(imagenD,paleta_pink);
	
	// Visualiza los resultados
	(cameraman,cameraman_color_bone,cameraman_color_hot,cameraman_color_hsv).display("cameraman.tif");
	(imagenC,imagenC_color_copper,imagenC_color_jet).display("imagenC.tif");
	(imagenD,imagenD_color_hot,imagenD_color_pink).display("imagenD.tif");
	
	// Item 3
	// Para imagenes binarias, hay que asignarle un color a cada valor
	// Ejemplo: rojo=0, azul=1.
	
	// Carga la imagen en filename
	const char* filename4 = cimg_option("-i","star64.bmp","Image file\n");
	CImg<bool> star64(filename4);
	
	// Vectores de colores
	unsigned int rojo[]={255,0,0};
	unsigned int azul[]={0,0,255};
	unsigned int amarillo[]={255,204,0};
	unsigned int verde[]={153,255,0};
	
	// Aplica los colores a la imagen
	CImg<unsigned char> combinacion1=aplicarColorBin(star64,rojo,azul);
	CImg<unsigned char> combinacion2=aplicarColorBin(star64,amarillo,verde);
	CImg<unsigned char> combinacion3=aplicarColorBin(star64,rojo,amarillo);
	
	// Visualiza los resultados
	(star64,combinacion1,combinacion2,combinacion3).display("star64.bmp");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
