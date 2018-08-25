#include <iostream>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Colorea una region segmentada
CImg<double> colorearRegion(CImg<double> region){
	CImg<double> region_coloreada(region);
	
	// Toma un color al azar
	double color=rand()%360;
	
	// Colorea la imagen usando el modelo HSI
	region_coloreada=region.get_fill(color);
	region_coloreada.append(region.get_fill(1),'c');
	region_coloreada.append(region.get_normalize(0,1),'c');
	
	// Pasa de HSI a RGB
	region_coloreada.HSItoRGB();
	
	// Devuelve la imagen coloreada
	return region_coloreada;
}

// Captura una semilla a partir de un punto y colorea la region
CImg<unsigned char> region_segmentada(CImg<unsigned char> img,int x,int y,int delta_semilla,int etiqueta,CImg<double> &recorte,CImg<double> &segmentaciones){
	// Realiza el segmentado por crecimiento de regiones a partir de la
	// semilla (x;y)
	CImg<unsigned char> segmentada=region_growing(img,x,y,delta_semilla,etiqueta);
	
	// Aisla la region segmentada de la imagen original
	recorte=img-segmentada;
	
	// Colorea la region segmentada con un color aleatorio
	CImg<double> region_coloreada=colorearRegion(recorte);
	
	// Acumula las regiones segmentadas
	segmentaciones+=region_coloreada;
	
	// Elimina la zona segmentada de la imagen original
	img-=recorte;
	
	// Devuelve la imagen sin la zona segmentada
	return img;
}

// Captura una semilla a partir de un click para determinar la region
void capturarClick(CImg<unsigned char> img,int delta_semilla,int etiqueta){
	// Define la ventana
	CImgDisplay ventana(img,"Capturar semilla");
	
	// Comienza la captura del click
	while(!ventana.is_closed() && !ventana.is_keyQ()){
		// Espera eventos
		ventana.wait();
		
		// Verifica si se hizo click y que este dentro de los limites de la
		// imagen
		if(ventana.button() && ventana.mouse_x()>=0 && ventana.mouse_y()>=0){
			// Captura las coordenadas del punto
			unsigned int x=ventana.mouse_x();
			unsigned int y=ventana.mouse_y();
			
			// Realiza el segmentado por crecimiento de regiones a partir de la
			// semilla (x;y)
			CImg<unsigned char> segmentada=region_growing(img,x,y,delta_semilla,etiqueta);
			
			// Redibuja en la ventana
			ventana.resize();
			ventana.render(segmentada);
			ventana.paint();
		}
	}
}

// Segmentacion automatica (semilla aleatoria)
CImg<double> segmentacionAutomatica(CImg<unsigned char> img,int delta,int etiqueta,int maximo){
	CImg<unsigned char> segmentacion(img);
	
	CImg<double> recorte;
	
	// Acumulador de las zonas segmentadas
	CImg<double> segmentaciones(img.width(),img.height(),1,3,0);
	
	// Coordenadas del punto aleatorio
	unsigned int x;
	unsigned int y;
	
	// Contador de la cantidad de segmentaciones
	int cant_segmentaciones=0;
	
	// Calcula la segmentacion
	while(segmentacion.max()>0 && cant_segmentaciones<maximo){
		// Calcula valores aleatorios para los pixels dentro del rango
		x=rand()%segmentacion.width();
		y=rand()%segmentacion.height();
		
		// Segmenta solo regiones distintas de cero
		if(segmentacion(x,y)!=0){
			// Calcula la zona segmentada
			segmentacion=region_segmentada(img,x,y,delta,etiqueta,recorte,segmentaciones);
			
			// Incrementa el contador
			cant_segmentaciones++;
		}
	}
	
	// Devuelve las regiones segmentadas acumuladas
	return segmentaciones;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","bone.tif","Image file\n");
	CImg<unsigned char> bone(filename);
	
	// Delta de la semilla
	int delta=20;
	
	// Define la etiqueta
	int etiqueta=256;
	
	// Maxima cantidad de segmentaciones
	int maximo=10;
	
	// Segmenta a partir de un punto elegido por el usuario
	//capturarClick(bone,delta,etiqueta);
	
	// Segmenta en forma automatica
	CImg<double> segmentaciones=segmentacionAutomatica(bone,delta,etiqueta,maximo);
	
	// Visualiza los resultados en pantalla
	(bone,segmentaciones).display();
	
	// La diferencia es que al elegir los puntos el usuario, la segmentacion es
	// mas precisa, debido a que se sabe donde hacer click para elegir mejor
	// las regiones, dentro de zonas de color mas uniforme, que al azar.
	// La diferencia es que el automatico lo hace mas rapido, pero con muchas
	// mas semillas.
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
