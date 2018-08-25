#include <iostream>
#include <vector>
#include <cmath>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

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

// Variable de control, solo se hacen dos clicks
unsigned int cant_click=0;

// Variables para el primer y segundo click
click click1;
click click2;

// Distancia euclidea entre dos clicks
float dist(click c1,click c2){
	float dx=c2.x-c1.x;
	float dy=c2.y-c1.y;
	
	float distancia=sqrt(dx*dx+dy*dy);
	
	return distancia;
}

// Obtiene las coordenadas de Bresenham para los puntos inicial y final, y los
// tamaños de la imagen. Los devuelve en un vector<vector<unsigned int> >
void get_bresenham_coords(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,vector<vector<unsigned int> > & coords){
	coords.clear();
	int dx=x1-x0,
		dy=y1-y0,
		y=y0,
		eps=0;
	for (unsigned int x=x0;x<=x1;x++){
		vector<unsigned int> coord;
		coord.push_back(x);
		coord.push_back(y);
		coords.push_back(coord);
		eps+=dy;
		if((eps<<1)>=dx){
			y++;
			eps-=dx;
		}
	}
}

// Obtiene el perfil de intensidad para un segmento en cualquier direccion
// Usa las variables globales click1 y click2
CImg<unsigned char> obtenerPerfilDiagonal(CImg<unsigned char> img){
	// Vector con las coordenadas para los puntos
	vector<vector<unsigned int> > puntos_a_incluir; 
	
	// Obtiene las coordenadas y las guarda en el vector
	get_bresenham_coords(click1.x,click1.y,click2.x,click2.y,puntos_a_incluir);
	
	// Crea una imagen de una sola fila, con tantos pixeles como puntos del vector
	CImg<unsigned char> recortado(puntos_a_incluir.size(),1);
	
	// Recorre el vector de coordenadas y copia los valores de cada pixel de la imagen
	for(unsigned int i=0;i<puntos_a_incluir.size();i++){
		// Control
		//cout<<"Coordenadas: "<<puntos_a_incluir[i][0]<<", "<<puntos_a_incluir[i][1]<<"\n";
		recortado(i,0)=img(puntos_a_incluir[i][0],puntos_a_incluir[i][1]);
	}
	
	// Devuelve la imagen
	return recortado;
}

// Obtiene el perfil de una imagen dados dos puntos alineados en cualquier direccion
bool obtenerPerfil(CImg<unsigned char> &img,CImg<unsigned char> &salida){
	// Si la distancia euclidea entre dos puntos es muy corta, no calcula nada
	if(dist(click1,click2)<4){
		cout<<"Muy cerca, no sirve\n";
		return false;
	}
	
	// Diferentes casos a considerar
	if(fabs(click2.x-click1.x)<3){
		// Vertical
		cout<<"Vertical\n";
		salida=img.get_crop(click1.x,click1.y,click1.x,click2.y); 
	}
	else if(fabs(click2.y-click1.y)<3){
		// Horizontal
		cout<<"Horizontal\n";
		salida=img.get_crop(click1.x,click1.y,click2.x,click1.y);
	}
	else{
		// Diagonal
		cout<<"Diagonal\n";
		salida=obtenerPerfilDiagonal(img); 
	}
	
	return true;
}

// Funcion simplificada que calcula el perfil de intensidad cuando ya se obtuvieron
// los puntos
CImg<unsigned char> perfilIntesidad(CImg<unsigned char> img) {
	CImg<unsigned char> perfil;
	
	if(obtenerPerfil(img,perfil))  // obtenerPerfil tiene salida de control
		return perfil;
	else
		return img;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<unsigned char> img(filename); // imagen
	
	// Crea una ventana y dibuja la imagen
	CImgDisplay ventana(img,"Imagen");
	
	// Crea la imagen donde se va a calcular la curva de intensidad
	CImg<unsigned char> visu(500,400,1,3,0);
	
	// Crea un display donde se va a mostrar el perfil de intensidad
	CImgDisplay ventana2(visu,"Perfil de intensidad");
	
	// Vector color rojo
	const unsigned char rojo[]={255,0,0};
	
	// Comienza el while para capturar eventos
	while(!ventana.is_closed() && !ventana.is_keyQ()){
		// Espera algun evento en la imagen
		ventana.wait();
		
		// Controla si se presiono algun boton del mouse dentro de la imagen
		if (ventana.button() && ventana.mouse_y()>=0){ 
			// Detecta cuantos clicks fueron hechos
			// Primer click
			if((cant_click % 2)==0){
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
				int temp=0;
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
				
				// Como ya hay dos clicks, se puede calcular el perfil
				CImg<unsigned char> perfil=perfilIntesidad(img);
				
				// Dibuja el perfil
				// Fondo negro
				visu.fill(0);
				// Calcula la curva de intensidad
				visu.draw_graph(perfil,rojo,1,1,1,0,255);
				// Muestra la curva en la ventana
				visu.display(ventana2);
				
				// Reinicia el contador de clicks
				cant_click=0;
			}
		}
	}
	
	// hay un crash cerca de los bordes y luego de jugar mucho con el programa
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
