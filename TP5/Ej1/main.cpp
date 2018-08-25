#include <iostream>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Item 2
// Linea horizontal
CImg<bool> lineaHorizontal(unsigned int w,unsigned int h,unsigned int pos){
	CImg<bool> salida(w,h);
	salida.fill(0);
	
	if(pos>=0 && pos<=h)
		cimg_forX(salida,i){
			salida(i,pos)=1;
		}
	
	// Devuelve la imagen
	return salida;
}

// Linea vertical
CImg<bool> lineaVertical(unsigned int w,unsigned int h,unsigned int pos){
	CImg<bool> salida(w,h);
	salida.fill(0);
	
	if(pos>=0 && pos<=w)
		cimg_forX(salida,i){
			salida(pos,i)=1;
		}
	
	// Devuelve la imagen
	return salida;
}

// Cuadrado centrado
// Se toma como un rectangulo equilatero

// Rectangulo centrado
CImg<bool> rectanguloCentrado(unsigned int w,unsigned int h,unsigned int ancho,unsigned int alto){
	CImg<bool> salida(w,h);
	salida.fill(0);
	
	unsigned int centro_x=w/2;
	unsigned int centro_y=h/2;
	
	for(unsigned int i=centro_x-ancho/2;i<centro_x+ancho/2;i++)
		for(unsigned int j=centro_y-alto/2;j<centro_y+alto/2;j++)
			salida(i,j)=1;
	
	// Devuelve la imagen
	return salida;
}

// Circulo centrado
CImg<bool> circuloCentrado(unsigned int lado,unsigned int radio){
	CImg<bool> salida(lado,lado);
	salida.fill(0);
	
	unsigned int centro=lado/2;
	
	unsigned int comp1=0, comp2=0, radio2=radio*radio;
	
	cimg_forXY(salida,i,j){
		comp1=(i-centro)*(i-centro);
		comp2=(j-centro)*(j-centro);
		
		if((comp1+comp2)<=radio2)
			salida(i,j)=1;
	}
	
	// Devuelve la imagen
	return salida;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","FFT2_Patrones.png","Image file\n");
	CImg<double> patrones(filename1);
	
	const char* filename2 = cimg_option("-i","FFT2_Activacion.png","Image file\n");
	CImg<double> activacion(filename2);
	
	// Muestra los patrones y la activacion
	(patrones,activacion).display();
	
	// Item 2
	// Variables
	unsigned int w=256, h=256, pos_h=127, pos_v=127, ancho=40, alto=128, radio=64;
	
	// Linea horizontal
	CImg<double> lh=lineaHorizontal(w,h,pos_h).get_normalize(0,255);
	//lh.display();
	
	// Linea vertical
	CImg<double> lv=lineaVertical(w,h,pos_v).get_normalize(0,255);
	//lv.display();
	
	// Cuadrado centrado
	CImg<double> cuad=rectanguloCentrado(w,h,alto,alto).get_normalize(0,255);
	//cuad.display();
	
	// Rectangulo centrado
	CImg<double> rect=rectanguloCentrado(w,h,ancho,alto).get_normalize(0,255);
	//rect.display();
	
	// Circulo
	CImg<double> circ=circuloCentrado(w,radio).get_normalize(0,255);
	//circ.display();
	
	// Item 3
	// TDF linea horizontal
	// Da como resultado un espectro de frecuencias en el eje x. A medida que
	// se desplaza hacia el origen, el espectro tiende a la continua. De lo
	// contrario, tiende a una senoidal.
	CImg<double> lh_tdf_magn=magn_tdf(lh,true,"../gray.pal");
	lh_tdf_magn.display("TDF Magnitud - Linea horizontal");
	
	// TDF linea vertical
	// Igual que para la linea horizontal, solo que el espectro de frecuencias
	// aparece sobre el eje y
	CImg<double> lv_tdf_magn=magn_tdf(lv,true,"../gray.pal");
	lv_tdf_magn.display("TDF Magnitud - Linea vertical");
	
	// TDF cuadrado centrado
	// Ocurre lo mismo que en las lineas horizontales y verticales, solo que en
	// ambos ejes, formando un espectro de frecuencias en 2D
	CImg<double> cuad_tdf_magn=magn_tdf(cuad,true,"../gray.pal");
	cuad_tdf_magn.display("TDF Magnitud - Cuadrado");
	
	// TDF rectangulo centrado
	// Ocurre lo mismo que en el cuadrado, solo que habra frecuencias mas
	// juntas en el eje opuesto al lado mas grande del rectangulo (si lado
	// mayor es en y, esas frecuencias aparecen en eje x)
	CImg<double> rect_tdf_magn=magn_tdf(rect,true,"../gray.pal");
	rect_tdf_magn.display("TDF Magnitud - Rectangulo");
	
	// TDF circulo centrado
	// Da como resultado un espectro de frecuencias radial, que dependera del
	// radio del circulo
	CImg<double> circ_tdf_magn=magn_tdf(circ,true,"../gray.pal");
	circ_tdf_magn.display("TDF Magnitud - Circulo");
	
	// Item 4
	CImg<double> lv512=lineaVertical(512,512,255);
	CImg<double> lv512_rotada=lv512.get_rotate(20.0);
	
	CImg<double> lv512_recorte=lv512.get_crop(512/4,512/4,3*511/4,3*511/4);
	CImg<double> lv512_rotada_recorte=lv512_rotada.get_crop(512/4+100,512/4,3*511/4+100,3*511/4);
	
	//(lv512,lv512_rotada,lv512_recorte,lv512_rotada_recorte).display("Original - Rotada - Recortada - Recortada Rotada");
	
	// Calcula la TDF de los recortes
	CImg<double> lv512_recorte_tdf_magn=magn_tdf(lv512_recorte,true,"../gray.pal");
	CImg<double> lv512_rotada_recorte_tdf_magn=magn_tdf(lv512_rotada_recorte,true,"../gray.pal");
	
	(lv512_recorte_tdf_magn,lv512_rotada_recorte_tdf_magn).display("TDF Recortada - TDF Recortada Rotada");
	
	// Cuando se rota la linea, aparecen componentes armonicas en el espectro
	// de frecuencias. De todas formas se nota claramente la componente
	// frecuencial principal, perpendicular a la linea rotada
	
	// Item 5
	const char* filename3 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<double> cameraman(filename3);
	
	const char* filename4 = cimg_option("-i","huang3.jpg","Image file\n");
	CImg<double> huang3(filename4);
	
	const char* filename5 = cimg_option("-i","rmn.jpg","Image file\n");
	CImg<double> rmn(filename5);
	
	// Calcula la TDF de las imagenes
	CImg<double> cameraman_tdf_magn=magn_tdf(cameraman,true,"../gray.pal");
	CImg<double> huang3_tdf_magn=magn_tdf(huang3,true,"../gray.pal");
	CImg<double> rmn_tdf_magn=magn_tdf(rmn,true,"../gray.pal");
	
	// Visualiza las imagenes con sus espectros de magnitudes asociados
	(cameraman,cameraman_tdf_magn).display("Cameraman - TDF Cameraman");
	(huang3,huang3_tdf_magn).display("huang3 - TDF huang3");
	(rmn,rmn_tdf_magn).display("rmn - TDF rmn");

//	magn_tdf(imagen,true,"../gray.pal"); //metodo para ver la magnitud de una
//	tdf en escala logaritmica, el input debe ser double
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
