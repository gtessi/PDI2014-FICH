#include <iostream>
#include <cassert>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Funciones de filtrado (Item 2)
// Acota el rango
CImg<float> clipping(CImg<float> img){
	CImg<float> salida(img);
	
	// Aplica el clipping
	cimg_forXY(salida,i,j){
		if (salida(i,j)>255)
			salida(i,j)=255;
		else
			if(salida(i,j)<0)
			salida(i,j)=0;
	}
	
	// Devuelve la imagen acotada
	return salida;
}

// Devuelve una imagen convolucionada con una matriz de NxN, a partir de un patron
CImg<float> filtrarImagen(CImg<float> img,unsigned int opcion_patron=2,bool acotar=false){
	unsigned int n=3;
	
	CImg<float> mascara(n,n), salida;
	
	double patron[]={0,0,0,0,0,0,0,0,0};
	
	// Arma el patron a partir de la opcion
	// Mascaras suma=1
	if(opcion_patron==2){
		patron[0]=-1.0; patron[1]=-1.0; patron[2]=-1.0;
		patron[3]=-1.0; patron[4]=9.0; patron[5]=-1.0;
		patron[6]=-1.0; patron[7]=-1.0; patron[8]=-1.0;
	}
	else
		if(opcion_patron==3){
			patron[0]=1.0; patron[1]=-2.0; patron[2]=1.0;
			patron[3]=-2.0; patron[4]=5.0; patron[5]=-2.0;
			patron[6]=1.0; patron[7]=-2.0; patron[8]=1.0;
		}
		else{
			patron[0]=0.0; patron[1]=-1.0; patron[2]=0.0;
			patron[3]=-1.0; patron[4]=5.0; patron[5]=-1.0;
			patron[6]=0.0; patron[7]=-1.0; patron[8]=0.0;
		}
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=patron[i+j*n];
	}
	
	// Convolucion entre la imagen y la mascara
	salida=img.get_convolve(mascara);
	
	// Acota el rango (clipping)
	if(acotar)
		salida=clipping(salida);
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}


int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","chairs_oscura.jpg","Image file\n");
	CImg<float> chairs_oscura(filename1);
	
	const char* filename2 = cimg_option("-i","chairs.jpg","Image file\n");
	CImg<float> chairs(filename2);
	
	// Grafica el histograma original
	CImg<float> chairs_oscura_histograma=chairs_oscura.get_histogram(256,0,255);
	chairs_oscura_histograma.display_graph(0,3);
	
	// Ecualiza la imagen
	// En RGB
	// Canales
	CImg<float> r=chairs_oscura.get_channel(0);
	CImg<float> g=chairs_oscura.get_channel(1);
	CImg<float> b=chairs_oscura.get_channel(2);
	
	// Ecualiza cada canal
	r.equalize(256,0,255);
	g.equalize(256,0,255);
	b.equalize(256,0,255);
	
	// Recompone la imagen
	CImg<float> chairs_oscura_rgb_eq=r;
	chairs_oscura_rgb_eq.append(g,'c');
	chairs_oscura_rgb_eq.append(b,'c');
	
	// En HSI
	// Convierte la imagen de RGB a HSI
	CImg<float> chairs_oscura_hsi=chairs_oscura.get_RGBtoHSI(); 
	
	// Canales
	CImg<float> h=chairs_oscura_hsi.get_channel(0);
	CImg<float> s=chairs_oscura_hsi.get_channel(1);
	CImg<float> i=chairs_oscura_hsi.get_channel(2);
	
	// Ecualiza solo el canal de intensidad (rango de 0 a 1)
	i.equalize(256,0,1);
	
	// Recompone la imagen
	CImg<float> chairs_oscura_hsi_eq=h;
	chairs_oscura_hsi_eq.append(s,'c');
	chairs_oscura_hsi_eq.append(i,'c');
	
	// Convierte la imagen HSI a RGB para visualizar
	chairs_oscura_hsi_eq.HSItoRGB();
	
	// Visualiza los resultados
	(chairs_oscura,chairs_oscura_rgb_eq,chairs_oscura_hsi_eq,chairs).display("Oscura - RGB ecualizada - HSI ecualizada - Original");
	
	// Es mas efectivo ecuualizar solo el canal de intensidad para mejorar el
	// brillo, que ecualizar los tres canales RGB a la vez, sin saber cuanto
	// contribuye cada color al brillo de la imagen. Sin embargo, la imagen RGB
	// parece mas vivida, ya que al modificar la intensidad solamente, cambia
	// la "percepcion" de la imagen. Para mejorar la calidad visual de la
	// imagen HSI, se deberia saturar un poquito mas.
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Item 2
	// Carga la imagen en filename
	const char* filename3 = cimg_option("-i","camino.tif","Image file\n");
	CImg<float> camino(filename3);
	
	// Opcion de filtrado
	unsigned int opcion=2;
	
	// En RGB
	// Canales
	r=camino.get_channel(0);
	g=camino.get_channel(1);
	b=camino.get_channel(2);
	
	// Filtra cada canal
	r=filtrarImagen(r,opcion);
	g=filtrarImagen(g,opcion);
	b=filtrarImagen(b,opcion);
	
	// Recompone la imagen
	CImg<float> camino_filtrada=r;
	camino_filtrada.append(g,'c');
	camino_filtrada.append(b,'c');
	
	// En HSI
	// Convierte la imagen RGB a HSI
	CImg<float> camino_hsi=camino.get_RGBtoHSI();
	
	// Canales
	h=camino_hsi.get_channel(0);
	s=camino_hsi.get_channel(1);
	i=camino_hsi.get_channel(2);
	
	// Filtra solo el canal de intensidad
	i=filtrarImagen(i,opcion);
	
	// Normaliza el rango de la intensidad (0 a 1)
	i.normalize(0.0,1.0);
	
	// Recompone la imagen
	CImg<float> camino_hsi_filtrada=h;
	camino_hsi_filtrada.append(s,'c');
	camino_hsi_filtrada.append(i,'c');
	
	// Convierte la imagen HSI a RGB para visualizar
	camino_hsi_filtrada.HSItoRGB();
	
	// Visualiza los resultados
	(camino,camino_filtrada,camino_hsi_filtrada).display("Original - RGB filtrada - HSI filtrada");
	
	// Ocurre algo similar a lo anterior, en HSI los colores se mantienen
	// intactos. El filtro patron1_2 oscurece la imagen, pero preserva los
	// colores y la vuelve mas nitida.
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
