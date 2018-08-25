#include <iostream>
#include <vector>
#include "CImg.h"
#include "PDI_functions.h"

using namespace cimg_library;
using namespace std;

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

// Struct punto
struct punto{
	unsigned int x;
	unsigned int y;
};

// Aplica una mascara sobre la imagen
CImg<double> aplicarMascara(CImg<double> img,CImg<bool> mascara){
	CImg<double> salida(img);
	
	// Aplica la mascara en todos los canales
	cimg_forXY(img,i,j){
		salida(i,j,0)*=mascara(i,j,0);
		salida(i,j,1)*=mascara(i,j,1);
		salida(i,j,2)*=mascara(i,j,2);
	}
	
	// Devuelve la imagen enmascarada
	return salida;
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

// Verifica si un valor no pertenece al vector
bool pertenece(vector<int> v,int valor){
	bool salida=false;
	
	// Recorre el vector
	for(unsigned int i=0;i<v.size();i++)
		// Compara el valor buscado con el del vector
		if(v[i]==valor)
			salida=true;
	
	return salida;
}

// Crea una lista con todas las etiquetas de una imagen
vector<int> obtenerEtiquetas(CImg<int> etiquetado){
	vector<int> lista_etiquetas;
	
	// Primer elemento
	lista_etiquetas.push_back(0);
	
	// Primera etiqueta
	int valor_anterior=etiquetado(0,0);
	
	// Recorre la imagen buscando las etiquetas
	cimg_forXY(etiquetado,i,j){
		if(etiquetado(i,j)!=0)
			if(!pertenece(lista_etiquetas,etiquetado(i,j))){
				lista_etiquetas.push_back(etiquetado(i,j));
				//cout<<etiquetado(i,j)<<"\n";
			}
	}
	
	// Borra el primer elemento
	lista_etiquetas.erase(lista_etiquetas.begin());
	
	// Devuelve el vector con las etiquetas
	return lista_etiquetas;
}

// Calcula el area de una region a partir de una etiqueta
double calcularArea(CImg<int> etiquetado,int etiqueta,double pixel_area=0.5){
	double resultado=0.0;
	
	// Contador
	int c=0;
	
	// Recorre la imagen buscando los pixels correspondientes a la etiqueta
	cimg_forXY(etiquetado,i,j){
		if(etiquetado(i,j)==etiqueta)
			c++;
	}
	
	// Calcula el area segun la cantidad de pixels
	resultado=double(c)*pixel_area;
	
	// Devuelve el resultado
	return resultado;
}

// Calcula las areas para cada etiqueta
vector<double> obtenerAreas(CImg<int> etiquetado,vector<int> &etiquetas,double pixel_area=0.5,double valor=40){
	vector<double> vector_areas;
	
	// Vector temporal
	vector<int> e;
	
	// Area
	double area=0.0;
	
	// Para cada etiqueta, calcula el area
	for(unsigned int i=0;i<etiquetas.size();i++){
		area=calcularArea(etiquetado,etiquetas[i],pixel_area);
		
		// Si el area es mayor a un valor, la guarda junto con su etiqueta
		if(area>=valor){
			e.push_back(etiquetas[i]);
			vector_areas.push_back(area);
		}
	}
	
	// Reemplaza el vector de etiquetas
	etiquetas=e;
	
	// Devuelve el vector con las areas
	return vector_areas;
}

// Obtiene la posicion del maximo dBZ de cada area etiquetada
vector<punto> obtenerdBZ(CImg<double> img_hsi,CImg<int> etiquetado,vector<int> etiquetas){
	vector<punto> dBZs;
	
	// Variable punto
	punto p;
	
	// Valor maximo
	double maximo=0.0;
	
	// Recorre cada etiqueta
	for(unsigned int e=0;e<etiquetas.size();e++){
		// Recorre la imagen buscando los maximos dBZs por area
		cimg_forXY(etiquetado,i,j){
			// Verifica que la etiqueta corresponda y que el valor del canal H
			// sea mayor que el maximo
			if(etiquetado(i,j)!=0 && etiquetado(i,j)==etiquetas[e])
				if(img_hsi(i,j,0,0)>maximo && img_hsi(i,j,0,0)>254.921){
					// Actualiza el maximo
					maximo=img_hsi(i,j,0,0);
					
					// Crea el punto
					p.x=i;
					p.y=j;
				}
		}
		
		// Guarda el punto en el vector
		dBZs.push_back(p);
		
		// Resetea el maximo a cero
		maximo=0.0;
	}
	
	// Devuelve las posiciones de los dBZs
	return dBZs;
}

// Marca los puntos con mayor dBZ
CImg<double> marcardBZs(CImg<double> img,vector<punto> dBZs){
	CImg<double> salida(img);
	
	// Vector de color
	unsigned int color[]={255,0,0};
	
	// Marca los puntos del vector
	for(unsigned int i=0;i<dBZs.size();i++)
		salida.draw_circle(dBZs[i].x,dBZs[i].y,10,color,1,1);
	
	// Devuelve la imagen
	return salida;
}

// Muestra la informacion por consola
void mostrarInformacion(CImg<double> img_hsi,vector<int> etiquetas,vector<double> areas,vector<punto> dBZs){
	// Variables
	double a=0.0; // Area
	double dbz=0.0; // dBZ
	punto p; // Punto
	double longitud=0.0; // Longitud
	double latitud=0.0; // Latitud
	
	// Limites de la longitud
	double lon_min=-61.75;
	double lon_max=-59.25;
	
	// Limites de la latitud
	double lat_min=-32.9;
	double lat_max=-30.79;
	
	// Tamaño de la imagen
	unsigned int M=img_hsi.width();
	unsigned int N=img_hsi.height();
	
	// Saltos
	double paso_x=(abs(lon_min)-abs(lon_max))/M;
	double paso_y=(abs(lat_min)-abs(lat_max))/N;
	
	cout<<"\n\n **************************** \n\n"; 
	
	// Recorre cada etiqueta
	for(unsigned int e=0;e<etiquetas.size();e++){
		// Area
		a=areas[e];
		
		// Punto dBZ maximo
		p.x=dBZs[e].x;
		p.y=dBZs[e].y;
		
		// dBZ
		dbz=img_hsi(p.x,p.y,0,0);
		
		// Longitud (x crece hacia la izquierda)
		longitud=p.x*paso_x+lon_min;
		
		// Latitud (y crece hacia abajo)
		latitud=lat_max-p.y*paso_y;
		
		// Muestra la informacion en consola
		cout<<"Extension: "<<a<<" Kms; Maximo dBZ: "<<dbz<<" dBZ; Latitud del maximo: "<<latitud<<"; Longitud del maximo: "<<longitud<<"\n";
	}
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","1.png","Image file\n");
	//const char* filename2 = cimg_option("-i","2.png","Image file\n");
	//const char* filename3 = cimg_option("-i","3.png","Image file\n");
	
	CImg<double> img_original(filename);
	
	// Copia
	CImg<double> img=img_original;
	
	// *** SEGMENTACION COLOR ***
	// Convierte la imagen de RGB a HSI
	CImg<double> img_hsi=img.get_RGBtoHSI();
	
	// Visualiza los canales H y S para obtener los valores de colores
	(img_hsi.get_channel(0),img_hsi.get_channel(1)).display("Canal H - Canal S");
	
	// Recorta la imagen
	// x1=86;31
	// x2=428;373
	img.crop(86,31,428,373);
	img_hsi.crop(86,31,428,373);
	
	// Define el subespacio HS
	// H=[254.921;329.0]
	// S=[1.0;1.0]
	double sub_H[]={254.921,329.0};
	double sub_S[]={1.0,1.0};
	
	// Crea la mascara HS
	CImg<bool> mascara=generarMascaraHS(img_hsi,sub_H,sub_S);
	
	// Elimina los pixels de division politica (dilatacion y erosion)
	mascara.dilate(2).erode(2);
	
	// Aplica la mascara a la imagen
	CImg<double> img_enmascarada=aplicarMascara(img,mascara);
	
	// Visualiza la segmentacion
	(img,img_enmascarada).display("Original - Enmascarada");
	
	// *** ETIQUETADO DE REGIONES ***
	// Genera el etiquetado de componentes conectadas a partir de una mascara
	// binaria
	CImg<int> etiquetado(etiquetado_cr(mascara.get_channel(0)));
	
	// Visualiza las componentes etiquetadas
	etiquetado.display("Etiquetado");
	
	// Crea la lista con todas las etiquetas
	vector<int> etiquetas=obtenerEtiquetas(etiquetado);
	
	// Calcula las areas para cada etiqueta, solo si es mayor a 40 km2
	vector<double> areas=obtenerAreas(etiquetado,etiquetas,0.5,40);
	
	// *** IDENTIFICAR EL PIXEL CON MAYOR dBZ ***
	// Obtiene las coordenadas de los puntos con mayor dBZ
	vector<punto> dBZs=obtenerdBZ(img_hsi,etiquetado,etiquetas);
	
	// *** MUESTRA LA INFORMACION POR CONSOLA ***
	// Extension: area Kms; Maximo dBZ: dbz dBZ; Latitud del maximo: ; Longitud del maximo:
	// Muestra en consola los datos
	mostrarInformacion(img_hsi,etiquetas,areas,dBZs);
	
	// Corrige cada cada punto dBZ debido al recorte de la imagen
	// x=86
	// y=31
	for(unsigned int i=0;i<dBZs.size();i++){
		dBZs[i].x+=86;
		dBZs[i].y+=31;
	}
	
	// Marca los pixels con mayor dBZ de cada area
	CImg<double> img_mayordBZ=marcardBZs(img_original,dBZs);
	
	// Visualiza los puntos de mayor dBZ para cada area
	img_mayordBZ.display();
	
	return 0;
}
