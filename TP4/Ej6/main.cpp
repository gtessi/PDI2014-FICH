#include <iostream>
#include <vector>
#include "CImg.h"

#include "funciones.h"

using namespace cimg_library;
using namespace std;

// Dado un pixel devuelve un vector con sus componentes RGB
vector<float> colorPixel(CImg<unsigned char> pixel,unsigned int i,unsigned int j){
	vector<float> color(3);
	
	// Componentes
	color[0]=pixel(i,j,0);
	color[1]=pixel(i,j,1);
	color[2]=pixel(i,j,2);
	
	// Devuelve el vector con las componentes
	return color;
}

// Devuelve verdadero si un vector de color esta en un rango
bool compararColor(vector<float> color,int color1,int color2){
	bool resultado=false;
	
	if((color[0]>=color1 && color[0]<=color2) && (color[1]>=color1 && color[1]<=color2) && (color[2]>=color1 && color[2]<=color2))
		resultado=true;
	
	// Devuelve el resultado
	return resultado;
}

// Detecta y separa el rectangulo de la imagen
CImg<float> detectarRectangulo(CImg<unsigned char> img){
	// Variables
	unsigned int ancho=img.width(), alto=img.height();
	vector<float> pixel, pixel_sig;
	
	// Coordenadas
	unsigned int x0=0, y0=0, x1=0, y1=0;
	
	// Rango de colores
	int color1=230, color2=232;
	
	// Detecta los puntos extremos que delimitan la zona
	// Punto 1
	for(unsigned int i=0;i<ancho-1;i++)
		for(unsigned int j=i;j<alto-1;j++){
			// Toma el valor del pixel
			// (i;j)
			pixel=colorPixel(img,i,j);
			// (i+1;j+1)
			pixel_sig=colorPixel(img,i+1,j+1);
			
			// Verifica si los colores coinciden
			if(compararColor(pixel,color1,color2) && !compararColor(pixel_sig,color1,color2)){
				x0=i+1;
				y0=j+1;
			}
		}
	
	// Punto 2
	for(unsigned int i=1;i<ancho;i++)
		for(unsigned int j=1;j<alto;j++){
			// Toma el valor del pixel
			// (i;j)
			pixel=colorPixel(img,i-1,j-1);
			// (i+1;j+1)
			pixel_sig=colorPixel(img,i,j);
			
			// Verifica si los colores coinciden
			if(!compararColor(pixel,color1,color2) && compararColor(pixel_sig,color1,color2)){
				x1=i+1;
				y1=j+1;
			}
		}
	
	// Hace el recorte del area detectada
	CImg<float> salida=img.get_crop(x0,y0,x1,y1);
	
	// Devuelve el area recortada
	return salida;
}

// Invierte una mascara
CImg<bool> invertirMascara(CImg<float> mascara){
	CImg<bool> salida(mascara);
	
	// Invierte cada valor del pixel de la mascara
	cimg_forXY(mascara,i,j){
		salida(i,j)=!mascara(i,j);
	}
	
	// Devuelve la mascara invertida
	return salida;
}

// Aplica una mascara sobre la imagen
CImg<float> aplicarMascara(CImg<float> img,CImg<bool> mascara){
	CImg<float> salida(img);
	
	// Aplica la mascara en todos los canales
	cimg_forXYC(img,i,j,c){
		salida(i,j,c)*=mascara(i,j);
	}
	
	// Devuelve la imagen enmascarada
	return salida;
}

// Segmenta con un color una imagen enmascarada
CImg<float> segmentarZona(CImg<float> img,float color[]){
	CImg<float> salida(img);
	
	// Aplica la segmentacion
	cimg_forXY(img,i,j){
		if(img(i,j,0)==0 && img(i,j,1)==0 && img(i,j,2)==0){
			salida(i,j,0)=color[0];
			salida(i,j,1)=color[1];
			salida(i,j,2)=color[2];
		}
	}
	
	// Devuelve la imagen segmentada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","Deforestacion.png","Image file\n");
	CImg<unsigned char> deforestacion(filename);
	
	// Muestra la imagen original
	deforestacion.display();
	
	// Aplica un recorte a la zona delimitada
	CImg<float> recorte=deforestacion.get_crop(152,275,729,705);
	
	// *** el metodo no funciona
	//CImg<float> recorte=detectarRectangulo(deforestacion);
	
	// Muestra la imagen recortada
	recorte.display();
	
	// Crea una especie de mascara filtrando la imagen y aplicando umbrales
	// para binarizar la imagen
	
	// *** Inicio del preproceso ***
	// Filtro pasa bajos
	CImg<float> recorte_filtrada=filtrarImagen(recorte,3,patron1_2);
	
	// Filtro de alta potencia
	CImg<float> mascara=restaPB(recorte,recorte_filtrada,2.0);
	
	// Aplica un umbral
	mascara.threshold(32);
	
	// Filtro de promediado
	mascara=filtrarImagen(mascara,5,patron_2);
	
	// Aplica un umbral
	mascara.threshold(200);
	
	// Invierte la mascara para separar la zona no deforestada
	CImg<bool> mascara_invertida=invertirMascara(mascara);
	// *** Fin del preproceso ***
	
	// Aplica la mascara a la imagen
	CImg<float> resultado=aplicarMascara(recorte,mascara_invertida);
	
	// Vector color
	float color[]={255.0,0.0,0.0};
	
	// Segmenta la zona enmascarada con un color
	resultado=segmentarZona(resultado,color);
	
	// Visualiza los resultados
	(recorte,resultado).display("Original - Segmentada");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
