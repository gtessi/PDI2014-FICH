#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Obtiene el perfil de una imagen dados dos puntos alineados (horizontal)
CImg<unsigned char> obtenerPerfilH(CImg<unsigned char> img,unsigned int y){
	// Define la imagen
	CImg<unsigned char> salida(img);
	
	// Horizontal
	//cout<<"Horizontal\n";
	salida=img.get_crop(0,y,img.width()-2,y);
	// Fija la y y se mueve a lo largo de todo el eje x
	
	// Devuelve la imagen de salida
	return salida;
}

// Obtiene el perfil de una imagen dados dos puntos alineados (vertical)
CImg<unsigned char> obtenerPerfilV(CImg<unsigned char> img,unsigned int x){
	// Define la imagen
	CImg<unsigned char> salida(img);
	
	// Vertical
	//cout<<"Vertical\n";
	salida=img.get_crop(x,45,x,193);
	// En y=45 la botella deberia estar llena
	// En y=193 esta la base de la botella
	
	// Devuelve la imagen de salida
	return salida;
}

int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","botellas.tif","Image file\n");
	CImg<unsigned char> img(filename); // imagen
	
	// Crea una ventana y dibuja la imagen
	CImgDisplay ventana(img,"Imagen");
	
	// Calcula el perfil de intensidad de la imagen en la altura de llenado
	// En y=45 la botella deberia estar llena
	CImg<unsigned char> perfil_original=obtenerPerfilH(img,45);
	CImg<unsigned char> perfil=perfil_original;
	
	// Normalizo los pixeles del perfil: negro=0, grises=127, blanco=255
	cimg_forX(perfil,i){
		// Pixel negro
		if(perfil(i)<=100)
			perfil(i)=0;
		else
			// Pixel blanco
			if(perfil(i)>=240)
				perfil(i)=255;
			// Pixel gris
			else
				perfil(i)=127;
	}
	
	// Guarda el perfil en un archivo
	//perfil_original.save("perfil_original.bmp");
	//perfil.save("perfil.bmp");
	
	// Analizando el perfil de intensidad, hay que ubicar los pixeles blancos
	// Pixel negro indica que no hay botella
	// Pixel gris indica que hay botella y que esta llena
	// Pixel blanco indica que la botella esta vacia
	
	// Pixel siguiente
	unsigned char pixel_sig=0;
	
	// Cantidad de botellas, de botellas vacias
	int cant_botellas=0, cant_botellas_vacias=0;
	
	// Posicion de la botella vacia
	int pos_botella_vacia=0;
	
	// Ancho del perfil
	int ancho_perfil=perfil.width();
	
	// Pixeles que definen donde esta la botella vacia
	int pixel_ini=0, pixel_fin=0;
	
	for(int i=0;i<ancho_perfil-1;i++){
		// Actualiza el pixel siguiente
		pixel_sig=perfil(i+1);
		
		// Hay botella llena
		if(perfil(i)==127 && pixel_sig==0)
			// Botella llena
			cant_botellas++;
		// Hay botella vacia
		if(perfil(i)==255 && pixel_sig==0){
			// Botella vacia
			cant_botellas++;
			cant_botellas_vacias++;
			
			// Pixel final de la botella vacia
			pixel_fin=i;
			
			// Posicion de la botella
			pos_botella_vacia=cant_botellas;
		}
		
		// Pixel inicial de la botella vacia
		if(perfil(i)==0 && pixel_sig==255)
			pixel_ini=i;
	}
	
	// Verifica el ultimo pixel para determinar si hay botella
	// Botella llena
	if(pixel_sig==127)
		cant_botellas++;
	// Botella vacia
	if(pixel_sig==255){
		cant_botellas++;
		cant_botellas_vacias++;
		
		// Pixel final de la botella vacia
		pixel_fin=ancho_perfil;
	}
	
	// Muestra en consola la cantidad de botellas y la cantidad de botellas vacias
	cout<<"\n\nCantidad de botellas: "<<cant_botellas<<"\nCantidad de botellas vacias: "<<cant_botellas_vacias<<"\n\n";
	
	// Muestra en consola la posicion de la botella vacia
	cout<<"Posicion de la botella vacia (de izquierda a derecha): "<<pos_botella_vacia<<"\n\n";
	
	// Busca la mitad entre los extremos de una botella vacia (pixeles blancos)
	int mitad=(pixel_fin-pixel_ini)/2;
	
	// Calcula el perfil de intensidad vertical de la botella vacia
	// En x=mitad esta la botella vacia
	CImg<unsigned char> perfil_botella_vacia=obtenerPerfilV(img,mitad);
	
	// Normalizo los pixeles del perfil: negro=0, blanco=255
	cimg_forY(perfil_botella_vacia,j){
		// Pixel blanco
		if(perfil_botella_vacia(j)>=130)
			perfil_botella_vacia(j)=255;
		// Pixel negro
		else
			perfil_botella_vacia(j)=0;
	}
	
	// Alto del perfil
	int alto_perfil=perfil_botella_vacia.height();
	
	// Corrige el perfil
	int j=0;
	while(j<alto_perfil){
		// Si el pixel anterior es blanco y el siguiente negro = siguiente es blanco
		if(perfil_botella_vacia(j)==255 && perfil_botella_vacia(j+1)==0){
			perfil_botella_vacia(j+1)=255;
			
			// Incrementa el contador para que no relea el pixel siguiente
			j++;
		}
		// Si el pixel anterior es negro y el siguiente blanco = siguiente es negro
		if(perfil_botella_vacia(j)==0 && perfil_botella_vacia(j+1)==255)
			perfil_botella_vacia(j+1)=0;
		
		// Incrementa el contador
		j++;
	}
	
	// Pixeles blancos y negros
	int blancos=0, negros=0;
	
	// Cuenta pixeles blancos y negros
	j=0;
	while(perfil_botella_vacia(j)==255){
		blancos++;
		j++;
	}
	negros=alto_perfil-blancos;
	
	// Calcula el porcentaje de llenado de la botella
	// Total
	int total=alto_perfil;
	
	// Resultado
	double res=(negros*100.0)/total;
	
	// Muestra en consola el porcentaje de llenado de la botella
	cout<<"Botella llena al "<<res<<"%\n\n";
	
//	// Dibuja el perfil
//	// Crea la imagen donde se va a calcular la curva de intensidad
//	CImg<unsigned char> visu(500,400,1,3,0);
//	
//	// Crea un display donde se va a mostrar el perfil de intensidad
//	CImgDisplay ventana2(visu,"Perfil de intensidad");
//	
//	// Vector color rojo
//	const unsigned char rojo[]={255,0,0};
//	
//	// Fondo negro
//	visu.fill(0);
//	// Calcula la curva de intensidad
//	visu.draw_graph(perfil_original,rojo,1,1,1,0,255);
//	// Muestra la curva en la ventana
//	visu.display(ventana2);
	
	while(!ventana.is_closed()){}
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
