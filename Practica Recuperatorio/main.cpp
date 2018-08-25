#include <iostream>
#include <vector>
#include <set>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

///****************************************
/// Crecimiento de regiones
///****************************************
CImg<unsigned char> region_growing(CImg<unsigned char> orig,int x,int y,int delta, int etiqueta){
	// orig: imagen a procesar
	// x,y: posición de la semilla
	// delta: define el rango de pertenencia como [semilla-delta/2, semilla+delta/2]
	// etiqueta: nro de la etiqueta, no debe pertenecer al rango
	
	//Basicamente pinto la semilla y la pongo en la cola
	//despues tomo el primer nodo de la cola como referencia (a su vez lo elimino de la cola),
	// miro sus vecinos, los pinto y los agrego al final de la cola
	
	struct node{
		int x;
		int y;
	}node;
	// armo el rango
	int valor=orig(x,y);
	int rango_min=valor-delta/2,
		rango_max=valor+delta/2+delta%2; //para delta impar suma 1 para delta par suma 0
	if (rango_min<0) rango_min=0;
	if (rango_max>255) rango_max=255;
	
////	//muestro en pantalla y controlo etiqueta
////	cout<<"semilla: "<<valor<<" rango: ["<<rango_min<<","<<rango_max<<"] etiqueta: "<<etiqueta<<endl;
////	if (etiqueta>=rango_min && etiqueta<=rango_max){
////		cout<<"error: la etiqueta ("<<etiqueta<<") no puede pertenecer al rango, vuelva a elegir"<<endl;
////		cout<<"etiqueta: ";
////		cin>>etiqueta;
////	}  
	
	
	CImg<> dest(orig);
	vector<int> camino;
	
	dest(x,y)=etiqueta;
	//agrego el nodo (x,y) al final de la cola sólo por una cuestion de inicializacion
	//para que el erase del while tenga sentido en la primer ejecucion
	camino.push_back(x);
	camino.push_back(y);
	//mientras exista cola
	while(camino.size()!=0){
		//     cout<<camino.size()<<" ";
		//tomo los dos primeros valores de la cola
		node.x=camino[0];
		node.y=camino[1];
		
		//elimino el nodo de la cola xq ya lo tengo pintado
		camino.erase(camino.begin(),camino.begin()+2); 
		
		//miro a la derecha
		if ((node.x+1 > 0) && (node.x+1 < dest.width()))
			if ((node.y > 0) && (node.y < dest.height()))
				if (dest(node.x+1,node.y) >= rango_min && dest(node.x+1,node.y) <= rango_max){
					//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
					dest(node.x+1,node.y)=etiqueta;
					camino.push_back(node.x+1);
					camino.push_back(node.y);
				}
		//miro a la izquierda
		if ((node.x-1 > 0) && (node.x-1 < dest.width()))
					if ((node.y > 0) && (node.y < dest.height()))
					if (dest(node.x-1,node.y) >= rango_min && dest(node.x-1,node.y) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x-1,node.y)=etiqueta;
						camino.push_back(node.x-1);
						camino.push_back(node.y);
					}
		//miro abajo
		if ((node.x > 0) && (node.x < dest.width()))
					if ((node.y+1 > 0) && (node.y+1 < dest.height()))
					if (dest(node.x,node.y+1) >= rango_min && dest(node.x,node.y+1) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x,node.y+1)=etiqueta;
						camino.push_back(node.x);
						camino.push_back(node.y+1);
					}
		//miro arriba
		if ((node.x > 0) && (node.x < dest.width()))
					if ((node.y-1 > 0) && (node.y-1 < dest.height()))
					if (dest(node.x,node.y-1) >= rango_min && dest(node.x,node.y-1) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x,node.y-1)=etiqueta;
						camino.push_back(node.x);
						camino.push_back(node.y-1);
					}
	}
	return dest;
}

// Etiquetado con crecimiento de regiones
CImg<int> etiquetado_cr(CImg<int> img,int blanco=1,int delta=1,int nueva_etiqueta=1){
	// Verifica si el valor del pixel no tiene etiqueta
	if(blanco==nueva_etiqueta)
		if(blanco==1){
			img*=255;
			blanco=255;
		}
	
	// Recorre la imagen
	cimg_forXY(img,x,y){
		if(img(x,y)==blanco){
			// Le asigna la misma etiqueta a toda la region por crecimiento de
			// regiones
			img=region_growing(img,x,y,1,nueva_etiqueta);
			
			// Incrementa la etiqueta
			nueva_etiqueta++;
		}
	} 
	
	// Devuelve la imagen etiquetada
	return img;
}

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

// Genera la mascara a partir de una imagen en HSI y del subespacio HS
CImg<bool> generarMascaraHS(CImg<double> img_hsi,double sub_H[],double sub_S[]){
	CImg<bool> mascara(img_hsi);
	mascara.fill(0);
	
	// Obtiene los espacios H y S
	CImg<double> H=img_hsi.get_channel(0);
	CImg<double> S=img_hsi.get_channel(1);
	
	// Verifica si el pixel actual pertenece al subespacio HS
	cimg_forXY(img_hsi,i,j){
		// Si el pixel actual pertenece al subespacio HS
		if((H(i,j)>=sub_H[0] && H(i,j)<=sub_H[1]) && (S(i,j)>=sub_S[0] && S(i,j)<=sub_S[1])){
			// Crea la mascara
			mascara(i,j,0)=1;
			mascara(i,j,1)=1;
			mascara(i,j,2)=1;
		}
	}
	
	// Devuelve la mascara HS
	return mascara;
}

// Calcula el promedio de una region
double calcularPromedio(CImg<double> h){
	double promedio=0;
	int contador=0;
	
	// Cuenta los valores de h y los acumula
	cimg_forXY(h,i,j){
		promedio+=h(i,j);
		contador++;
	}
	
	// Calcula el promedio
	promedio/=contador;
	
	// Devuelve el resultado
	return promedio;
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

// Cuenta la cantidad de etiquetas
unsigned int cantEtiquetas(CImg<int> img){
	unsigned int total=0;
	
	// Usa un conjunto porque no permite elementos repetidos
	set<int> etiquetas;
	
	// Inserta cada etiqueta en el conjunto
	cimg_forXY(img,i,j){
		etiquetas.insert(img(i,j));
	}
	
	// Elimina la etiqueta correspondiente al fondo
	total=etiquetas.size()-1;
	
	// Devuelve el total de etiquetas
	return total;
}

// Cuenta la cantidad de etiquetas contenidas en un radio
unsigned int cantEtiquetasRadio(CImg<int> img,unsigned int centro_x,unsigned int centro_y,double radio){
	unsigned int total=0;
	
	// Usa un conjunto porque no permite elementos repetidos
	set<int> etiquetas;
	
	double factor1,factor2,distancia;
	
	cimg_forXY(img,i,j){
		// Calcula los factores para la distancia euclidea
		factor1=pow(i-centro_x,2);
		factor2=pow(j-centro_y,2);
		
		// Calcula la distancia euclidea del pixel actual al centroide
		distancia=factor1+factor2;
		
		// Inserta la etiqueta solo si esta contenida en la circunferencia
		if(distancia<=radio*radio)
			etiquetas.insert(img(i,j));
	}
	
	// Elimina la etiqueta correspondiente al fondo
	total=etiquetas.size()-1;
	
	// Devuelve el total de etiquetas
	return total;
}

// Genera una mascara de radio y centro dados
CImg<bool> generarMascaraCirculo(unsigned int w,unsigned int h,unsigned int cx,unsigned int cy,double radio){
	CImg<bool> salida(w,h,1,3,0);
	
	unsigned int color[]={255,0,0};
	
	CImg<unsigned int> circulo(w,h,1,3,0);
	circulo.draw_circle(cx,cy,radio,color);
	
	cimg_forXY(salida,i,j){
		if(circulo(i,j,0,0)==255){
			salida(i,j,0,0)=true;
			salida(i,j,0,1)=true;
			salida(i,j,0,2)=true;
		}
	}
	
	return salida;
}

// AND
CImg<bool> andImg(CImg<bool> img1,CImg<bool> img2){
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) and img2(i,j);
	}
	
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","Platos01.jpg","Image file\n");
	//const char* filename = cimg_option("-i","Platos02.jpg","Image file\n");
	//const char* filename = cimg_option("-i","Platos03.jpg","Image file\n");
	//const char* filename = cimg_option("-i","Platos04.jpg","Image file\n");
	
	CImg<double> img(filename);
	
	// *** TIPO DE SOPA ***
	// Convierte la imagen de RGB a HSI
	CImg<double> img_hsi=img.get_RGBtoHSI();
	
	// Visualiza los canales H y S para obtener los valores de colores
	//(img_hsi.get_channel(0),img_hsi.get_channel(1)).display("Canal H - Canal S");
	
	// Recorta una region en el centro para saber el tipo de sopa
	CImg<double> img_recortada=img_hsi.get_crop(450,450,550,550);
	
	// Calcula el promedio de h del recorte
	double h_promedio=calcularPromedio(img_recortada.get_channel(0));
	
	// *** SEGMENTACION DE LAS MOSCAS ***
	// Crea una mascara suma 1
	CImg<double> filtroPB=crearMascara(15);
	
	// Aplica el filtro a la imagen
	CImg<double> img_PB=img.get_convolve(filtroPB);
	
	// Segmentacion RGB de las moscas
	double centroide[]={30,30,30};
	double radio=60;
	
	// Crea la mascara RGB para las moscas
	CImg<bool> mascara_moscas=generarMascaraRGB(img_PB,centroide,radio);
	
	// Elimina los pixels que quedan asilados (dilatacion y erosion)
	mascara_moscas.dilate(2).erode(2);
	
	mascara_moscas.display("Mascara moscas");
	
	// Mascaras circulares
	CImg<bool> plato_sopa=generarMascaraCirculo(img.width(),img.height(),500,500,442);
	CImg<bool> sopa=generarMascaraCirculo(img.width(),img.height(),500,500,320);
	
	// AND logico entre la mascara de las moscas y la mascara de la sopa
	CImg<bool> moscas_sopa=andImg(mascara_moscas,sopa);
	
	// AND logico entre la mascara de las moscas y la mascara del plato
	CImg<bool> moscas_plato_sopa=andImg(mascara_moscas,plato_sopa);
	
	// Genera el etiquetado de componentes conectadas a partir de una mascara
	// binaria
	CImg<int> etiquetado(etiquetado_cr(mascara_moscas.get_channel(0)));
	CImg<int> etiquetado_sopa(etiquetado_cr(moscas_sopa.get_channel(0)));
	CImg<int> etiquetado_plato_sopa(etiquetado_cr(moscas_plato_sopa.get_channel(0)));
	
	// Cuenta la cantidad de moscas a partir de las etiquetas
	unsigned int cantidad_moscas=cantEtiquetas(etiquetado);
	
	// Cuenta la cantidad de moscas en la sopa a partir de las etiquetas
	unsigned int cantidad_moscas_sopa=cantEtiquetas(etiquetado_sopa);
	
	// Cuenta la cantidad de moscas en el plato a partir de las etiquetas
	unsigned int cantidad_moscas_plato_sopa=cantEtiquetas(etiquetado_plato_sopa);
	
	// Cuenta la cantidad de moscas en el borde del plato
	unsigned int cantidad_moscas_plato=cantidad_moscas_plato_sopa-cantidad_moscas_sopa;
	
	// *** INFORMACION ***
	// Verifica que tipo de sopa es
	int tipo_sopa=0;
	
	if(h_promedio>=15 && h_promedio<=28){
		// Zapallo
		tipo_sopa=1;
		
		cout<<"\n\n TIPO DE SOPA: ZAPALLO\n";
	}
	else{
		// De la casa
		tipo_sopa=2;
		
		cout<<"\n\n TIPO DE SOPA: DE LA CASA\n";
	}
	
	// Informa la cantidad total de moscas, las moscas en el plato y las moscas
	// en la sopa
	cout<<"CANTIDAD TOTAL DE MOSCAS: "<<cantidad_moscas<<endl;
	cout<<"CANTIDAD TOTAL DE MOSCAS EN EL PLATO: "<<cantidad_moscas_plato<<endl;
	cout<<"CANTIDAD TOTAL DE MOSCAS EN LA SOPA: "<<cantidad_moscas_sopa<<endl;
	
	// Informa si la sopa esta bien servida
	if(tipo_sopa==1 && cantidad_moscas_sopa<=3)
		// Contiene sopa de zapallo y una cantidad maxima de 3 moscas
		cout<<"\n PLATO DE SOPA DE ZAPALLO BIEN SERVIDO \n\n";
	if(tipo_sopa==2 && cantidad_moscas_sopa<=4)
		// Contiene sopa de la casa y una cantidad maxima de 4 moscas
		cout<<"\n PLATO DE SOPA DE LA CASA BIEN SERVIDO \n\n";
	
	
	return 0;
}
