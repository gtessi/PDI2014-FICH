#include <iostream>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Genera la mascara a partir de una imagen, del centroide de la esfera RGB y
// el radio a considerar
CImg<bool> generarMascaraRGB(CImg<double> img,double centroide[],double radio){
	CImg<bool> mascara(img);
	mascara.fill(0);
	
	// Variables
	double comp_r, comp_g, comp_b, distancia;
	
	// Calcula si el pixel actual pertenece a la esfera o no
	cimg_forXY(img,i,j){
		// Calcula la diferencia al cuadrado del pixel actual al centroide de
		// la esfera (|z-a|^2)
		comp_r=pow(img(i,j,0)-centroide[0],2);
		comp_g=pow(img(i,j,1)-centroide[1],2);
		comp_b=pow(img(i,j,2)-centroide[2],2);
		
		// Calcula la distancia euclidea del pixel actual al centroide
		distancia=pow(comp_r+comp_g+comp_b,0.5);
		
		// Si el pixel esta en la esfera, mascara en 1
		if(distancia<=radio){
			mascara(i,j,0)=1;
			mascara(i,j,1)=1;
			mascara(i,j,2)=1;
		}
	}
	
	// Devuelve la mascara RGB
	return mascara;
}

// Crea una mascara de NxN
CImg<double> crearMascara(unsigned int n){
	CImg<double> mascara(n,n);
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=1;
	}
	
	// Aplica el factor de escala a la imagen
	mascara/=(n*n);
	
	// Devuelve la mascara
	return mascara;
}

// Obtiene el centro de un objeto a partir de un valor
CImg<int> recortaObjeto(CImg<int> img,int valor,int &x,int &y){
	// Variables para los limites
	int x_ini=img.width();
	int y_ini=img.height();
	
	int x_fin=0;
	int y_fin=0;
	
	// Busca el objeto
	cimg_forXY(img,i,j){
		// Calcula el objeto si coincide con el valor
		if(img(i,j)==valor){
			if(i>x_fin)
				x_fin=i;
			if(i<x_ini)
				x_ini=i;
			if(j>y_fin)
				y_fin=j;
			if(j<y_ini)
				y_ini=j;
		}
	}
	
	// Recorta el objeto
	CImg<int> recorte=img.get_crop(x_ini,y_ini,x_fin,y_fin);
	
	// Calcula el centro del objeto
	x=x_ini+recorte.width()/2;
	y=y_ini+recorte.height()/2;
	
	// Devuelve el objeto recortado
	return recorte;
}

// Dibuja circulos en los objetos detectados
CImg<double> dibujarCirculos(CImg<double> img,CImg<int> etiquetado,unsigned int radio){
	// Vector de color
	double color[]={20,20,255};
	
	// Contador de objetos
	unsigned int cant_objetos=1;
	
	// Variables para los centros
	int x;
	int y;
	
	// Cuenta los objetos hasta que no quede ninguno
	while(etiquetado.max()>0){
		// Obtiene el recorte
		CImg<double> recorte(recortaObjeto(etiquetado,etiquetado.max(),x,y));
		
		// Guarda el valor maximo
		int maximo=etiquetado.max();
		
		// Elimina el objeto
		unsigned int x_min=max(x-recorte.width(),0);
		unsigned int y_min=max(y-recorte.height(),0);
		unsigned int x_max=min(x+recorte.width(),etiquetado.width());
		unsigned int y_max=min(y+recorte.height(),etiquetado.height());
		
		for(unsigned int i=x_min;i<x_max;i++)
			for(unsigned int j=y_min;j<y_max;j++)
				if(etiquetado(i,j)==maximo)
					etiquetado(i,j)=0;
		
		// Muestra en consola los datos de los objetos
		cout<<"Objeto "<<cant_objetos<<", centro ("<<x<<","<<y<<")\n";
		
		// Incrementa el contador de objetos
		cant_objetos++;
		
		// Dibuja un circulo sobre la imagen
		img.draw_circle(x,y,radio,color,.6);
	}
	
	// Muestra en consola la cantidad total de objetos
	cout<<"\n\nTotal de objetos: "<<cant_objetos-1<<"\n\n";
	
	// Devuelve la imagen
	return img;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","rosas.jpg","Image file\n");
	CImg<double> rosas(filename);
	
	// Item 1
	// Rojo: 120 a 220
	// Verde: 10 a 80
	// Azul: 50 a 100
	
	// Define el centroide de la esfera
	double centroide[]={170,45,75};
	
	// Define el radio de la esfera
	double radio=70;
	
	// Crea la mascara
	CImg<bool> mascara=generarMascaraRGB(rosas,centroide,radio);
	
	// Visualiza los resultados
	(rosas,mascara).display("Original - Mascara RGB");
	
	// Item 2
	// Crea una mascara suma 1
	CImg<double> filtroPB=crearMascara(15);
	
	// Aplica el filtro a la imagen
	CImg<double> rosas_filtrada=rosas.get_convolve(filtroPB);
	
	// Genera una nueva mascara
	mascara=generarMascaraRGB(rosas_filtrada,centroide,radio);
	
	// Visualiza los resultados
	(rosas,rosas_filtrada,mascara).display("Original - Filtrada PB - Mascara RGB");
	
	// Item 3
	// Genera el etiquetado de componentes conectadas a partir de una mascara
	// binaria
	CImg<int> etiquetado(label_cc(mascara.get_channel(0)));
	
	// Visualiza las componentes etiquetadas
	etiquetado.display("Etiquetado");
	
	// Item 4
	// Radio del circulo
	double radio_circulo=20;
	
	// Dibuja los circulos sobre las rosas
	CImg<double> rosas_circulos=dibujarCirculos(rosas,etiquetado,radio_circulo);
	
	// Visualiza los resultados
	(rosas,mascara,etiquetado,rosas_circulos).display("Original - Mascara binaria - Etiquetado - Resultado final");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
