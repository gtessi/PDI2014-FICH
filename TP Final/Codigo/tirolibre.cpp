#include <iostream>
#include <CImg.h> // -lm -lpthread -lX11 -lfftw3
#include <vector>

using namespace cimg_library;
using namespace std;


#include "FuncGeneral.h"
#include "FuncGeneralesTPFinal.h"


CImg<double> mascara_elpise(CImg<double> elipse, int x0, int y0, double r1_a, double r1_b, double r2, double angulo){
	unsigned char color[]={0,0,0};
	CImg<double> img_inferior(elipse.get_crop(0,y0,elipse.width(),elipse.height()));
	CImg<double> img_superior(elipse.get_crop(0,0,elipse.width(),y0));
	
	//Dibujo la misma elipse con r1_a pero de color negro.
	img_inferior.draw_ellipse(x0,0,r1_a,r2,angulo,color,1,1);
	
	//Dibujo la misma elipse con r2_a pero de color negro.
	img_superior.draw_ellipse(x0,y0,r1_b,r2,angulo,color,1,1);
//	(img_inferior,img_superior).display("");
	
	
	int ancho=img_inferior.width();
	int alto=img_inferior.height()+img_inferior.height();
	
	//Concateno las dos imagenes, img_superior e img_inferior:
	CImg<double> resultado(ancho,alto,1,3);
	resultado.fill(0);
	
	
	CImg<double> result(img_superior.width(), img_superior.height()+img_inferior.height(),1,3);
	for(int i=0; i<img_superior.width(); i++){
		for(int j=0; j<img_superior.height(); j++){
			result(i,j,0)=img_superior(i,j);			
			result(i,j,1)=img_superior(i,j);			
			result(i,j,2)=img_superior(i,j);			
		}
	}
	
	for(int i=0; i<img_inferior.width(); i++){
		for(int j=0; j<img_inferior.height(); j++){
			result(i,j+y0,0)=img_inferior(i,j);			
			result(i,j+y0,1)=img_inferior(i,j);			
			result(i,j+y0,2)=img_inferior(i,j);			
		}
	}
	
	//Paso todo lo que es 0 a 1, y lo que es 1 a 0
	
	for(int i=0; i<result.width(); i++){
		for(int j=0; j<result.height(); j++){
			if(result(i,j,0)==0) {
				result(i,j,0)=1;
				result(i,j,1)=1;
				result(i,j,2)=1;
			}
			else{
				result(i,j,0)=0;
				result(i,j,1)=0;
				result(i,j,2)=0;
			}
		}
	}
	
	
//	result.display("");
	
	
	return result;
}


void clic_mouse(CImg<double> img, int &semilla_x, int &semilla_y){
	//Evento del Mouse
	CImgDisplay ventana(img, "Seleccionar Posición de la pelota");
	while(!ventana.is_closed()){
//		cargo las coordenadas del primer click del mouse
		ventana.wait();
		if(ventana.button() == 1){
			semilla_x = ventana.mouse_x();
			semilla_y = ventana.mouse_y();
			cout<<"Posicion seleccionada: "<<"["<<semilla_x<<","<<semilla_y<<"]"<<endl;
			cout<<"Puede cerrar la vantana"<<endl<<endl;
		}		
	}	
}

CImg<double> tirolibre(CImg<double> img){
	CImg<double> elipse(img);
	elipse.fill(0);
	
	unsigned char color_elipse[]={255,255,255};
	int x0=760; int y0=200;
	
	clic_mouse(img,x0,y0);	
	
	double r1_a=75; double r1_b=76; double r2=162;
	double angulo=90;
	elipse.draw_ellipse(x0,y0,r1_a,r2,angulo,color_elipse);
	elipse.draw_ellipse(x0,y0,r1_b,r2,angulo,color_elipse);
	
	elipse=mascara_elpise(elipse,x0,y0,r1_a,r1_b,r2,angulo);
	
	//Detecto los jugadores que Defienden
	CImg<double> cancha(get_canchaBinarisada(img));
	CImgList<double> Equipos=get_Equipos(cancha,img);
	
	//MASCARA DEFENSIVOS
	Equipos[0].display("");//Equipo en Ataque
	Equipos[1].display("");//Equipo en Defensa
	elipse.display("");
	
	//Verifico si dentro de la elipse existe algun jugador en DEFENSA
	int contar_pixel=0;
	for(int i=0; i<elipse.width(); i++){
		for(int j=0; j<elipse.height(); j++){
			if(elipse(i,j)==0 && Equipos[1](i,j)!=0){
				contar_pixel++;
			}
		}
	}
	
	if(contar_pixel>5){
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-------EXISTE INVASION EN EL AREA DEL TIRO LIBRE------"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}else{
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-- /NO/ EXISTE INVASION EN EL AREA DEL TIRO LIBRE-----"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}
	
	
	
	
	// Aplico el circulo sobre la imagen
	
	CImg<double> concirculo(img);
	CImg<double> concirculoHSI(concirculo.get_RGBtoHSI());
	
	//Aplico el Circulo sobre la imagen
	
	for(int i=0; i<img.width(); i++){
		for(int j=0; j<img.height(); j++){
			if(img(i,j,0)*elipse(i,j,0)==0){
				concirculoHSI(i,j,2)=0.7*concirculoHSI(i,j,2);
			}
		}
	}
	
	return concirculoHSI.get_HSItoRGB();
	
}


int main(int argc, char *argv[]) {
	
	
//	CImg<double> img("tirolibre3.png");		
	CImg<double> img("tirolibre2.png");	
	//CImg<double> concirculo(tirolibre(img));
	
	
	CImg<double> elipse(img);
	elipse.fill(0);
	
	unsigned char color_elipse[]={255,255,255};
	int x0=760; int y0=200;
	
	clic_mouse(img,x0,y0);	
	
	double r1_a=75; double r1_b=76; double r2=162;
	double angulo=90;
	elipse.draw_ellipse(x0,y0,r1_a,r2,angulo,color_elipse);
	elipse.draw_ellipse(x0,y0,r1_b,r2,angulo,color_elipse);
	
	elipse=mascara_elpise(elipse,x0,y0,r1_a,r1_b,r2,angulo);
	
	//Detecto los jugadores que Defienden
	CImg<double> cancha(get_canchaBinarisada(img));
	CImgList<double> Equipos=get_Equipos(cancha,img);
	
	//MASCARA DEFENSIVOS
	Equipos[0].display("");//Equipo en Ataque
	Equipos[1].display("");//Equipo en Defensa
	elipse.display("");
	
	return 0;
	
	//Verifico si dentro de la elipse existe algun jugador en DEFENSA
	int contar_pixel=0;
	for(int i=0; i<elipse.width(); i++){
		for(int j=0; j<elipse.height(); j++){
			if(elipse(i,j)==0 && Equipos[1](i,j)!=0){
				contar_pixel++;
			}
		}
	}
	
	if(contar_pixel>5){
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-------EXISTE INVASION EN EL AREA DEL TIRO LIBRE------"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}else{
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-- /NO/ EXISTE INVASION EN EL AREA DEL TIRO LIBRE-----"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}
	
	
	
	
	// Aplico el circulo sobre la imagen
	
	CImg<double> concirculo(img);
	CImg<double> concirculoHSI(concirculo.get_RGBtoHSI());
	
	//Aplico el Circulo sobre la imagen
	
	for(int i=0; i<img.width(); i++){
		for(int j=0; j<img.height(); j++){
			if(img(i,j,0)*elipse(i,j,0)==0){
				concirculoHSI(i,j,2)=0.7*concirculoHSI(i,j,2);
			}
		}
	}
	
	
	concirculo.display("");
	
	
	return 0;
}

