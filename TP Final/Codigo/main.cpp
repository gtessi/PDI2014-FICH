#include <iostream>
#include <CImg.h> // -lm -lpthread -lX11 -lfftw3
#include <vector>

using namespace cimg_library;
using namespace std;

#include "FuncGeneral.h"
#include "FuncSegmentacion.h"
#include "FuncGeneralesTPFinal.h"


int main(int argc, char *argv[]) {
	
	//CImg<double> img(cimg_option("-i","pique2/pique20000.png","Image file\n"));
	CImg<double> img(cimg_option("-i","reales2.png","Image file\n"));
	//CImg<double> img(cimg_option("-i","penal1_original.png","Image file\n"));
	
	CImg<double> imgGris,hsi,cancha;
	if(!(img.width()%2))
		img.crop(0,0,0,0,img.width()-2,img.height()-1,0,2,0);
	
	
	CImg<double> F(3,3,1,1,0);
	
	double color[] = {255,255,255};
	
	imgGris = get_grisProm(img);
	
	
	
	/// Inicializamos los filtros
	CImg<double> Gx(3,3,1,1,0),Gy(3,3,1,1,0);
	/// Horizontal y vertical
	Gx(0,0)=-1;Gx(0,1)=-1;Gx(0,2)=-1;Gx(2,0)=1;Gx(2,1)=1;Gx(2,2)=1;
	Gy(0,0)=-1;Gy(1,0)=-1;Gy(2,0)=-1;Gy(0,2)=1;Gy(1,2)=1;Gy(2,2)=1;
	
	
	/// Hacemos la convolucion
	CImg<double> Vert = imgGris.get_convolve(Gx),Hori = imgGris.get_convolve(Gy);

	
	/// Umbralizamos las imagenes
	int u=40;
	umbral255(Hori,u);
	umbral255(Vert,u);
	
	/// cancha binarisada
	cancha = get_canchaBinarisada(img);
	
	
	/// producto para sacarnos de ensima la tribuna y usamos el dilate
	/// y el erode para unificar las lineas
	Vert = productoBin(cancha,Vert);
	Hori = productoBin(cancha,Hori);
	Vert.dilate(6); Vert.erode(5);
	Hori.dilate(6); Hori.erode(5);

	/// calculamos los limites y las emes y equis de cada recta
	double ro1, ro2, m1,m2,x1,x2;
	get_limites(Hori,ro1,ro2);
	get_emesYequis(Vert,m1,m2,x1,x2);
	
	/// Detectamos los equipos
	CImgList<double> Equipos = get_Equipos(cancha,img);
	
	Equipos.display("Equipo 0                           Equipo 1");
	cout<<"DECIR CUAL ES EL EQUIPO DEFENSOR"<<endl;
	int equipoDefensor = 0;
	cin>>equipoDefensor;
	/// mostramos el tirolibre
	tirolibre(img, Equipos[equipoDefensor]).display("TIRO LIBRE");
	
	
	
	
	CImg<double> lineasOffside(Equipos[0]);
	lineasOffside.fill(0);
	
	double color_def[] = {255,255,0},
		color_ata[] = {255,0,0};
	
	int x,y;
	
	cout<<"DECIR SI EL ATAQUE ES HACIA LA DERECHA (1) O A LA IZQUIERDA (0)"<<endl;
	cin>>equipoDefensor;
	/// EQUIPO DEFENDIENDO
	ultimoJugador(Equipos[equipoDefensor],x,y,equipoDefensor);
	
	cout<<" valores de x y "<<x<<", "<<y<<endl;
	
	drawLineOffsideJugador(img, m1, m2, x1, x2, ro1,ro2, x,y,color_def);
	drawLineOffsideJugador(lineasOffside, m1, m2, x1, x2, ro1,ro2, x,y,color_def);
	
	
	/// EQUIPO ATACANDO
	ultimoJugador(Equipos[abs(equipoDefensor-1)],x,y,equipoDefensor);
	
	cout<<" valores de x y "<<x<<", "<<y<<endl;
	
	drawLineOffsideJugador(img, m1, m2, x1, x2, ro1,ro2, x,y,color_ata);
	drawLineOffsideJugador(lineasOffside, m1, m2, x1, x2, ro1,ro2, x,y,color_ata);
	
	if(offside(lineasOffside, color_def)){
		cout<<endl<<endl<<"***************************************"<<endl;
		cout<<"EL JUGADOR SE ENCUENTRA EN OFFFFSIDE DON NIEMBRO"<<endl;
		cout<<"***************************************"<<endl<<endl;
	}else{
		cout<<endl<<endl<<"***************************************"<<endl;
		cout<<"EL JUGADOR SE ENCUENTRA HABILITADIIISIMO DON NIEMBRO"<<endl;
		cout<<"***************************************"<<endl<<endl;
	}
	
	img.display("Imagen final");
	
	return 0;//*/
}
