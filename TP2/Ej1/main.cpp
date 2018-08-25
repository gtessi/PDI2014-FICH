#include <iostream>
#include <vector>
#include "CImg.h"
#include "../transformaciones.h"

using namespace cimg_library;
using namespace std;

// Cadenas con los parametros para las funciones
// Estructura: inicio final pendiente offset
struct parametros{
	unsigned int inicio;
	unsigned int final;
	
	float pendiente;
	int offset;
};

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacion(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float pendiente=nombre.pendiente;
	int offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=lineal(i,inicio,pendiente,offset);
		
		// Agrega el valor al vector LUT
		LUT.push_back(valor);
	}
}

// Dada una imagen y un vector LUT, aplica la transformacion
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img,vector<unsigned char> LUT){
	CImg<unsigned char> s(img.width(),img.height());
	
	// Aplica la LUT a toda la imagen
	cimg_forXY(img,i,j){
		// El vector LUT tiene un mapeo de valores en el rango 0-255 para cada pixel
		s(i,j)=LUT[img(i,j)];
	}
	
	// Devuelve la imagen modificada
	return s;
}

// Devuelve una imagen que representa el mapeo de la LUT
CImg<unsigned char> imagenMapeo(vector<unsigned char> LUT){
	unsigned int ancho=LUT.size()+1;
	
	CImg<unsigned char> mapeo(ancho,ancho,1,1,0);
	
	for(unsigned int i=0;i<ancho;i++){
		// Crea la curva de la LUT
		mapeo(i,ancho-LUT[i]-1)=255;
	}
	
	// Devuelve la imagen
	return mapeo;
}

// Calcula y grafica el mapeo del vector LUT y la imagen resultante
void graficas(CImg<unsigned char> img,vector<unsigned char> LUT){
	// Aplica LUT a la imagen
	CImg<unsigned char> salida=aplicarLUT(img,LUT);
	
	// Calcula la curva del mapeo LUT
	CImg<unsigned char> mapeo=imagenMapeo(LUT);
	
	// Muestra en pantalla los resultados
	CImgList<unsigned char> lista(img,mapeo,salida);
	lista.display();
}

// Item 5
// Struct que representa un click del mouse
typedef struct click{
	// Coordenadas
	int x;
	int y;
	
	// Imprime en consola los valores
	void print(bool cr) {
		cout<<x<<' '<<y;
		if (cr)
			cout<<'\n';
	}
}click;

// Dados dos puntos elegidos por el usuario, calcula la curva LUT que pasa por
// ellos y se la aplica a la imagen
void curvaInteractiva(CImg<unsigned char> img){
	// Crea las imagenes
	CImg<unsigned char> salida(255,255,1,1,0);
	CImg<unsigned char> mapeo=salida;
	
	// Crea la lista donde se muestra la imagen, la curva LUT y el resultado
	CImgList<unsigned char> lista;
	
	// Crea una ventana donde se eligen los puntos
	CImgDisplay ventana(mapeo);
	
	// Vector LUT
	vector<unsigned char> LUT;
	
	// Variables
	float dx1, dx2, dx3,
		  dy1, dy2, dy3,
		  pendiente1, pendiente2, pendiente3;
	
	unsigned int offset1, offset2, offset3;
	
	int temp, valor=0;
	
	// Variable de control, solo se hacen dos clicks
	unsigned int cant_click=0;
	
	// Variables para el primer y segundo click
	click click1;
	click click2;
	
	// Comienza el while para capturar eventos
	while(!ventana.is_closed() && !ventana.is_keyQ()){
		// Espera algun evento en la imagen
		ventana.wait();
		
		// Controla si se presiono algun boton del mouse dentro de la imagen
		if (ventana.button() && ventana.mouse_y()>=0){
			// Detecta cuantos clicks fueron hechos
			// Primer click
			if(cant_click==0){
				// Captura las coordenadas
				click1.x=ventana.mouse_x();
				click1.y=ventana.mouse_y();
				
				// Imprime las coordenadas en consola
				cout<<"Click1 en "; click1.print(true);
				
				// Incrementa el contador de clicks
				cant_click++;
			}
			// Segundo click
			else{
				// Captura las coordenadas
				click2.x=ventana.mouse_x();
				click2.y=ventana.mouse_y();
				
				// Imprime las coordenadas en consola
				cout<<"Click2 en "; click2.print(true);
				
				// Controla las coordenadas de los puntos para evitar raices de
				// numeros negativos
				temp=0;
				if(click1.x>click2.x || click1.y>click2.y){
					// Coordenada x
					temp=click1.x;
					click1.x=click2.x;
					click2.x=temp;	
					
					// Coordenada y
					temp=click1.y;
					click1.y=click2.y;
					click2.y=temp;
				}
				
				// Cuando se tienen los dos puntos, puede empezar a calcular
				// Calculo de las pendientes
				dx1=click1.x;
				dx2=click2.x-click1.x;
				dx3=255-click2.x;
				
				dy1=click1.y;
				dy2=click2.y-click1.y;
				dy3=255-click2.y;
				
				pendiente1=dy1/dx1;
				pendiente2=dy2/dx2;
				pendiente3=dy3/dx3;
				
				// Offsets
				offset1=0;
				offset2=click1.y;
				offset3=click2.y;
				
				// Valor temporal
				valor=0;
				
				// Crea la curva LUT
				for(int i=0;i<256;i++){
					// Primer tramo
					if (i<click1.x)
						// Calcula la LUT para el punto actual
						valor=lineal(i,0,pendiente1,offset1);
					else
						// Segundo tramo
						if (i<click2.x)
							// Calcula la LUT para el punto actual
							valor=lineal(i,click1.x,pendiente2,offset2);
						// Tercer tramo
						else
							// Calcula la LUT para el punto actual
							valor=lineal(i,click2.x,pendiente3,offset3);
					
					// Agrega el valor al vector LUT
					LUT.push_back(valor);
					
					// Limpia la variable
					valor=0;
				}
				
				// Aplica LUT a la imagen
				salida=aplicarLUT(img,LUT);
				
				// Calcula la curva del mapeo LUT
				mapeo=imagenMapeo(LUT);
				
				// Dibuja la recta lineal en el mapeo
				cimg_forX(mapeo,x){
					mapeo(x,255-x)=127; // color gris
				}
				
				// Muestra en pantalla los resultados
				lista.push_back(img);
				lista.push_back(mapeo);
				lista.push_back(salida);
				lista.display();
				
				// Reinicia el contador de clicks
				cant_click=0;
				
				// Limpia variables
				LUT.clear();
				lista.clear();
			}
		}
	}
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img1(filename1);
	
	const char* filename2 = cimg_option("-i","rmn.jpg","Image file\n");
	CImg<unsigned char> img2(filename2);
	
	const char* filename3 = cimg_option("-i","huang3.jpg","Image file\n");
	CImg<unsigned char> img3(filename3);
	
	// Vector LUT
	vector<unsigned char> LUT;
	
	// Transformaciones
	parametros lineal1;
	lineal1.inicio=0;
	lineal1.final=255;
	lineal1.pendiente=2.0;
	lineal1.offset=0;
	
	parametros lineal2;
	lineal2.inicio=0;
	lineal2.final=255;
	lineal2.pendiente=0.5;
	lineal2.offset=0;
	
	parametros lineal3;
	lineal3.inicio=0;
	lineal3.final=255;
	lineal3.pendiente=1.0;
	lineal3.offset=30;
	
	// Item 1
	// En transformaciones.h
	
	// Item 2
	// Imagen 1
	crearTransformacion(lineal1,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	crearTransformacion(lineal2,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	crearTransformacion(lineal3,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	// Imagen 2
	crearTransformacion(lineal1,LUT);
	graficas(img2,LUT);
	LUT.clear();
	
	crearTransformacion(lineal2,LUT);
	graficas(img2,LUT);
	LUT.clear();
	
	crearTransformacion(lineal3,LUT);
	graficas(img2,LUT);
	LUT.clear();
	
	// Imagen 3
	crearTransformacion(lineal1,LUT);
	graficas(img3,LUT);
	LUT.clear();
	
	crearTransformacion(lineal2,LUT);
	graficas(img3,LUT);
	LUT.clear();
	
	crearTransformacion(lineal3,LUT);
	graficas(img3,LUT);
	LUT.clear();
	
	// Item 3
	parametros negativa;
	negativa.inicio=0;
	negativa.final=255;
	negativa.pendiente=-1.0;
	negativa.offset=0;
	
	crearTransformacion(negativa,LUT);
	graficas(img1,LUT);
	LUT.clear();
	
	// Item 4
	parametros tramo1;
	tramo1.inicio=0;
	tramo1.final=100;
	tramo1.pendiente=0.0;
	tramo1.offset=0;
	
	parametros tramo2;
	tramo2.inicio=100;
	tramo2.final=200;
	tramo2.pendiente=3;
	tramo2.offset=0;
	
	parametros tramo3;
	tramo3.inicio=200;
	tramo3.final=255;
	tramo3.pendiente=0.0;
	tramo3.offset=255;
	
	crearTransformacion(tramo1,LUT);
	crearTransformacion(tramo2,LUT);
	crearTransformacion(tramo3,LUT);
	
	graficas(img1,LUT);
	//graficas(img2,LUT);
	//graficas(img3,LUT);
	
	LUT.clear();
	
	// Item 5
	curvaInteractiva(img1);
	//curvaInteractiva(img2);
	//curvaInteractiva(img3);
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
