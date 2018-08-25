#include <iostream>
#include "CImg.h"

#include "funciones.h"

using namespace cimg_library;
using namespace std;

// Extrae una porcion de la imagen dados dos puntos
CImg<unsigned char> obtenerCuadro(CImg<unsigned char> img,unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1){
	CImg<unsigned char> salida(x1-x0,y1-y0);
	
	// Corta el area especificada
	salida=img.get_crop(x0,y0,x1,y1);
	
	// Devuelve la imagen recortada
	return salida;
}

// Pega una imagen sobre otra, dados dos puntos
CImg<unsigned char> pegarImagen(CImg<unsigned char> target,CImg<unsigned char> obj,unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1){
	CImg<unsigned char> salida(target);
	
	// *** NO CONTROLA SI LOS PUNTOS SON VALIDOS ***
	
	cimg_forXY(obj,i,j){
		salida(i+x0,j+y0)=obj(i,j);
	}
	
	// Devuelve la imagen modificada
	return salida;
}


int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cuadros.tif","Image file\n");
	CImg<unsigned char> cuadros(filename1);
	
	CImgList<> lista;
	
	// Cuadros
	// El tamaño de la ventana es importante, considerar un borde blanco
	// 1: (0;0)     - (147;147)
	// 2: (362;0)   - (509;147)
	// 3: (182;182) - (329;329)
	// 4: (0;364)   - (147;511)
	// 5: (362;364) - (509;511)
	
	// Cuadros extraidos
	CImg<unsigned char> cuadro1=obtenerCuadro(cuadros,0,0,147,147);
	CImg<unsigned char> cuadro2=obtenerCuadro(cuadros,362,0,509,147);
	CImg<unsigned char> cuadro3=obtenerCuadro(cuadros,182,182,329,329);
	CImg<unsigned char> cuadro4=obtenerCuadro(cuadros,0,364,147,511);
	CImg<unsigned char> cuadro5=obtenerCuadro(cuadros,362,364,509,511);
	
	// Cuadros ecualizados
	CImg<unsigned char> cuadro1_eq=cuadro1.get_equalize(256,0,255);
	CImg<unsigned char> cuadro2_eq=cuadro2.get_equalize(256,0,255);
	CImg<unsigned char> cuadro3_eq=cuadro3.get_equalize(256,0,255);
	CImg<unsigned char> cuadro4_eq=cuadro4.get_equalize(256,0,255);
	CImg<unsigned char> cuadro5_eq=cuadro5.get_equalize(256,0,255);
	
	// Pega los cuadros ecualizados localmente en la imagen original
	CImg<unsigned char> cuadros_mod(cuadros);
	
	cuadros_mod=pegarImagen(cuadros_mod,cuadro1_eq,0,0,147,147);
	cuadros_mod=pegarImagen(cuadros_mod,cuadro2_eq,362,0,509,147);
	cuadros_mod=pegarImagen(cuadros_mod,cuadro3_eq,182,182,329,329);
	cuadros_mod=pegarImagen(cuadros_mod,cuadro4_eq,0,364,147,511);
	cuadros_mod=pegarImagen(cuadros_mod,cuadro5_eq,362,364,509,511);
	
	// Ecualiza la imagen original globalmente
	CImg<unsigned char> cuadros_eq=cuadros.get_equalize(256,0,255);
	
	lista.push_back(cuadros);
	lista.push_back(cuadros_mod);
	lista.push_back(cuadros_eq);
	lista.display();
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Item 2
	// Carga la imagen en filename
	const char* filename2 = cimg_option("-i","esqueleto.tif","Image file\n");
	CImg<float> esqueleto(filename2);
	
	CImgList<> lista1(esqueleto), lista2(esqueleto), lista3(esqueleto), lista4(esqueleto);
	
	// Transformacion logaritmica
	CImg<float> esqueleto_log=logaritmica(esqueleto,1.0);
	lista1.push_back(esqueleto_log);
	
	// Transformacion potencia
	CImg<float> esqueleto_pot=potencia(esqueleto,1.0,2.0);
	lista1.push_back(esqueleto_pot);
	
	lista1.display();
	
	// Filtro de promediado
	CImg<float> esqueleto_filtrado3=filtrarImagen(esqueleto,3,patron_1);
	lista2.push_back(esqueleto_filtrado3);
	
	CImg<float> esqueleto_filtrado5=filtrarImagen(esqueleto,5,patron_2);
	lista2.push_back(esqueleto_filtrado5);
	
	// Filtro de acentuado
	CImg<float> esqueleto_acentuado3=restaPB(esqueleto,esqueleto_filtrado3);
	lista2.push_back(esqueleto_acentuado3);
	
	CImg<float> esqueleto_acentuado5=restaPB(esqueleto,esqueleto_filtrado5);
	lista2.push_back(esqueleto_filtrado5);
	
	lista2.display();
	
	// Filtrado de alta potencia (high-boost)
	CImg<float> esqueleto_hb3=restaPB(esqueleto,esqueleto_filtrado3,1.2);
	lista3.push_back(esqueleto_hb3);
	
	CImg<float> esqueleto_hb5=restaPB(esqueleto,esqueleto_filtrado5,1.2);
	lista3.push_back(esqueleto_hb5);
	
	lista3.display();
	
	// Ecualizacion global de la imagen
	CImg<float> esqueleto_eq256=esqueleto.get_equalize(256,0,255);
	lista4.push_back(esqueleto_eq256);
	
	CImg<float> esqueleto_eq64=esqueleto.get_equalize(64,0,255);
	lista4.push_back(esqueleto_eq64);
	
	lista4.display();
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
