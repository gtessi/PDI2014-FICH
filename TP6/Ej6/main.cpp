#include <iostream>
#include <utility>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Item 2
// Calcula las distancias entre todos los puntos de una vecindad
CImg<double> distanciasVecindad(CImg<double> vecindad,unsigned int tipo_distancia=0){
	// Tamaños
	unsigned int M=vecindad.width();
	unsigned int N=vecindad.height();
	
	CImg<double> distancias(M,N);
	
	// Acumulador
	double acum;
	
	// Calcula la distancia entre todos los puntos de la vecindad
	cimg_forXY(vecindad,x,y){
		// Inicializa el acumulador
		acum=0.0;
		
		// Recorre los puntos de la vecindad
		cimg_forXY(vecindad,i,j){
			// Calcula la distancia para cada canal
			cimg_forC(vecindad,c){
				double diferencia=vecindad(x,y,0,c)-vecindad(i,j,0,c);
				
				// Calcula la distancia segun el tipo elegido
				switch(tipo_distancia){
				// Manhattan
				case 0:
					acum+=fabs(diferencia);
					break;
				// Euclidea
				case 1:
					acum+=pow(diferencia,2);
					break;
				}
			}
		}
		
		// Si la distancia es Euclidea, calcula la raiz cuadrada
		if(tipo_distancia==1)
			acum=sqrt(acum);
		
		// Guarda la distancia en la matriz
		distancias(x,y)=acum;
	}
	
	// Devuelve la matriz con las distancias mas cortas
	return distancias;
}

// Devuelve el punto con la menor distancia de la vecindad
pair<unsigned int,unsigned int> menorDistancia(CImg<double> distancias){
	// Coordenadas
	unsigned int x=0;
	unsigned int y=0;
	
	double menor=distancias(x,y);
	
	// Busca la menor distancia
	cimg_forXY(distancias,i,j){
		if(distancias(i,j)<menor){
			// Guarda las coordenadas
			x=i;
			y=j;
			
			// Actualiza el acumulador
			menor=distancias(i,j);
		}
	}
	
	// Devuelve el punto
	return make_pair(x,y);
}

// Aplica el filtro de mediana generalizado
// Tipo de distancia
// 0 = Manhattan
// 1 = Euclidea
CImg<double> filtradoMedianaGeneralizado(CImg<double> img,unsigned int tipo_distancia=0,int m=3,int n=3){
	CImg<double> salida(img);
	
	// Tamaño de la imagen
	int ancho=img.width();
	int alto=img.height();
	
	// Calcula los pasos de la vecindad centrada
	int paso_x=m/2;
	int paso_y=n/2;
	
	// Calcula la imagen filtrada
	cimg_forXY(img,i,j){
		// Calcula la vecindad con pixels validos
		int x_ini=(i-paso_x<0) ? 0 : i-paso_x;
		int y_ini=(j-paso_y<0) ? 0 : j-paso_y;
		int x_fin=(i+paso_x>=ancho) ? ancho-1 : i+paso_x;
		int y_fin=(j+paso_y>=alto) ? alto-1 : j+paso_y;
		
		// Obtiene la vecindad
		CImg<double> vecindad=img.get_crop(x_ini,y_ini,x_fin,y_fin);
		
		// Obtiene las distancias
		CImg<double> distancias=distanciasVecindad(vecindad,tipo_distancia);
		
		// Busca el punto con menor distancia
		pair<unsigned int,unsigned int> punto_min=menorDistancia(distancias);
		
		// Copia el valor del canal de la menor distancia
		cimg_forC(vecindad,c){
			salida(i,j,0,c)=vecindad(punto_min.first,punto_min.second,0,c);
		}
	}
	
	// Devuelve la imagen filtrada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","sangre.jpg","Image file\n");
	CImg<double> sangre(filename);
	
	// Item 1
	// Para implementar filtros de ordenamiento en espacio de color vectorial
	// es necesario encontrar un esquema de ordenamiento de vectores que
	// permita que la mediana (u otro filtro de orden) tenga sentido. Algo que 
	// es sencillo en escalares, es complicado con vectores. No son adecuados
	// ya que no cumplen con la propiedad de aplicabilidad tanto para escalares
	// como para vectores.
	
	// Item 3
	// Desvios para ruido
	double desv_impulsivo=20.0;
	double desv_gaussiano=20.0;
	
	// Imagen contaminada con ruido
	CImg<double> sangre_ruido=sangre.get_noise(desv_impulsivo,2); // Ruido impulsivo
	//CImg<double> sangre_ruido=sangre.get_noise(desv_gaussiano,0); // Ruido gaussiano
	
	// Aplica el filtro de mediana generalizado para varias distancias
	CImg<double> sangre_filtrada_manhattan=filtradoMedianaGeneralizado(sangre_ruido,0,3,3);
	CImg<double> sangre_filtrada_euclidea=filtradoMedianaGeneralizado(sangre_ruido,1,3,3);
	
	// Visualiza los resultados
	(sangre,sangre_ruido,sangre_filtrada_manhattan,sangre_filtrada_euclidea).display("Original - Ruido - Distancia Manhattan - Distancia Euclidea");
	
	// Calcula los ECMs de las imagenes
	cout<<"\n\n";
	cout<<"ECM Imagen con ruido:      "<<sangre.MSE(sangre_ruido)<<"\n";
	cout<<"ECM Distancia Manhattan:   "<<sangre.MSE(sangre_filtrada_manhattan)<<"\n";
	cout<<"ECM Distancia Euclidea:    "<<sangre.MSE(sangre_filtrada_euclidea)<<"\n";
	
	// Item 4
	
	// Item 5
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
