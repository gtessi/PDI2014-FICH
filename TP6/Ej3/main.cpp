#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Define una constante para evitar casos cercanos a cero
const double epsilon=0.001;

// Item 1
// Calcula la mediana de una vecindad
double mediana(CImg<double> vecindad){
	double salida=0;
	
	// Ordena la vecindad
	vecindad.sort();
	
	// Obtiene el valor del centro
	salida=vecindad(vecindad.width()/2,vecindad.height()/2);
	
	// Devuelve la mediana
	return salida;
}

// Calcula el punto medio de una vecindad
double puntoMedio(CImg<double> vecindad){
	// Obtiene el valor maximo y minimo de la vecindad
	double max=vecindad.max();
	double min=vecindad.min();
	
	// Calcula el valor medio
	double resultado=(max+min)*0.5;
	
	// Devuelve el resultado
	return resultado;
}

// Calcula la media-alfa recortada de una vecindad
double mediaAlfaRecortado(CImg<double> vecindad,unsigned int d){
	double salida=0;
	
	// Tamaño de la vecindad
	unsigned int m=vecindad.width();
	unsigned int n=vecindad.height();
	
	// Calcula el factor
	int factor=m*n-d;
	
	// Si el valor del factor es menor que cero, retorna el pixel original sin
	// procesar
	if(factor<=0)
		salida=vecindad(m/2,n/2);
	else{
		double coeficiente=1.0/factor;
		double sum=0;
		
		// Ordena la vecindad
		vecindad.sort();
		
		// Calcula la sumatoria solo de los elementos que esten dentro de la
		// vecindad mn-d
		for(unsigned int i=d/2;i<(m*n-d/2);i++){
			// La division da la fila
			unsigned int fila=floor(i/m);
			// El modulo da la columna
			unsigned int columna=i % n;
			
			// Calcula la sumatoria
			sum+=vecindad(fila,columna);
		}
		
		// Calcula el valor de la media
		salida=coeficiente*sum;
	}
	
	// Devuelve la media
	return salida;
}

// Aplica el filtro de orden
// Tipo de filtro
// 0 = Filtro de la mediana
// 1 = Filtro del punto medio
// 2 = Filtro de la media-alfa recortado
CImg<double> filtradoOrden(CImg<double> img,unsigned int tipo_filtro=0,unsigned int d=2,int m=3,int n=3){
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
		
		// Aplica el filtro elegido
		// Mediana
		if(tipo_filtro==0)
			salida(i,j)=mediana(vecindad);
		// Punto medio
		if(tipo_filtro==1)
			salida(i,j)=puntoMedio(vecindad);
		// Media-alfa recortado
		if(tipo_filtro==2)
			salida(i,j)=mediaAlfaRecortado(vecindad,d);
	}
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}

// Reemplaza la intensidad en una imagen RGB
CImg<double> reemplazarIntensidad(CImg<double> img,CImg<double> i){
	CImg<double> salida;
	
	// Convierte la imagen RGB a HSI
	CImg<double> img_hsi=img.get_normalize(0,1).get_RGBtoHSI();
	
	// Canales
	CImg<double> h=img_hsi.get_channel(0);
	CImg<double> s=img_hsi.get_channel(1);
	
	// Recompone la imagen
	CImg<double> salida_hsi=h;
	salida_hsi.append(s,'c');
	
	// Normaliza la intensidad
	salida_hsi.append(i.get_normalize(0,1),'c');
	
	// Convierte la imagen HSI en RGB
	salida=salida_hsi.get_HSItoRGB().get_normalize(0,255);
	
	// Devuelve la imagen
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","sangre.jpg","Image file\n");
	CImg<double> sangre(filename);
	
	// Desvios para ruido
	double desv_impulsivo=20.0;
	double desv_gaussiano=20.0;
	
	// Obtiene el canal de intensidad (HSI)
	CImg<double> sangre_hsi=sangre.get_RGBtoHSI();
	CImg<double> sangre_i=sangre_hsi.get_channel(2);
	sangre_i.normalize(0,255);
	
	// Item 2
	// Aplica ruido impulsivo y gaussiano a la imagen
	CImg<double> sangre_i_ruido=sangre_i.get_noise(desv_impulsivo,2);
	sangre_i_ruido=sangre_i_ruido.get_noise(desv_gaussiano,0);
	
	// Imagen a color con ruido
	CImg<double> sangre_ruido=reemplazarIntensidad(sangre,sangre_i_ruido);
	
	// Normaliza
	sangre_i_ruido.normalize(0,255);
	
	// Item 3
	// Valor de d, debe ser par
	unsigned int d=2;
	
	// Aplica los diferentes filtros
	// Mediana
	CImg<double> filtrada_mediana=filtradoOrden(sangre_i_ruido,0,d,3,3);
	
	// Punto medio
	CImg<double> filtrada_punto_medio=filtradoOrden(sangre_i_ruido,1,d,3,3);
	
	// Media-alfa recortado
	CImg<double> filtrada_media_alfa_recortado=filtradoOrden(sangre_i_ruido,2,d,3,3);
	
	// Resultados en color
	CImg<double> mediana_color=reemplazarIntensidad(sangre,filtrada_mediana);
	CImg<double> punto_medio_color=reemplazarIntensidad(sangre,filtrada_punto_medio);
	CImg<double> media_alfa_recortado_color=reemplazarIntensidad(sangre,filtrada_media_alfa_recortado);
	
	// Visualiza los resultados
	(sangre,sangre_ruido,mediana_color,punto_medio_color,media_alfa_recortado_color).display("Original - Ruido - Mediana - Punto medio - Media-alfa recortado");
	
	// Calcula los ECMs de las imagenes
	cout<<"\n\n";
	cout<<"ECM Imagen con ruido:      "<<sangre.MSE(sangre_ruido)<<"\n";
	cout<<"ECM Mediana:               "<<sangre.MSE(mediana_color)<<"\n";
	cout<<"ECM Punto medio:           "<<sangre.MSE(punto_medio_color)<<"\n";
	cout<<"ECM Media-alfa recortado:  "<<sangre.MSE(media_alfa_recortado_color)<<"\n";
	
	// El ruido impulsivo no se puede filtrar facilmente, o se filtra la sal o
	// la pimienta. Ademas, algunos filtros pueden eliminar el ruido gaussiano
	// y una parte del impulsivo (sal)
	// Ruido gaussiano varianza=20; impulsivo varianza=20
	
	// ECM Imagen con ruido:       1695.89
	// ECM Mediana:                 402.81
	// ECM Punto medio:             799.006
	// ECM Media-alfa recortado:    568.786
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
