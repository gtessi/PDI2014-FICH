#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Define una constante para evitar los ceros
const double epsilon=0.001;

// Item 1
// Calcula la media geometrica de una vecindad
double mediaGeometrica(CImg<double> vecindad){
	double productoria=1;
	
	// Cantidad de vecinos
	double mn=vecindad.width()*vecindad.height();
	
	// Calcula la productoria de los vecinos
	cimg_forXY(vecindad,i,j){
		productoria*=vecindad(i,j);
	}
	
	// Calcula la raiz m-esima si la productoria es positiva, de lo contrario
	// el valor es cero
	if(productoria<0)
		productoria=0;
	else
		productoria=pow(productoria,(1/mn));
	
	// Devuelve la media geometrica
	return productoria;
}

// Calcula la media contra-armonica de una vecindad
double mediaContraArmonica(CImg<double> vecindad,double Q){
	double num=0.0;
	double den=0.0;
	
	double resultado=0.0;
	
	// Cantidad de vecinos
	double mn=vecindad.width()*vecindad.height();
	
	// Realiza las sumatorias
	cimg_forXY(vecindad,i,j){
		// Casos especiales segun el valor de Q
		// Q=0 -> Media aritmetica
		if(Q==0){
			// Solo suma el pixel si es distinto de cero
			if(vecindad(i,j)>epsilon){
				num+=vecindad(i,j);
			}
		}
		else
			// Q=-1 -> Media armonica
			if(Q==-1){
				// Controla que el pixel no sea nulo (division por cero)
				if(vecindad(i,j)>epsilon){
					den+=(1.0/vecindad(i,j));
				}
			}
			else
				// Media contra-armonica
				// Solo suma el pixel si es distinto de cero
				if(vecindad(i,j)>epsilon){
					num+=pow(vecindad(i,j),Q+1);
					den+=pow(vecindad(i,j),Q);
				}
	}
	
	// Normaliza si la vecindad es distinta de cero
	if(mn>0){
		num/=mn;
		den/=mn;
	}
	
	// Calcula la media segun el valor de Q
	// Media aritmetica
	if(Q==0)
		resultado=num;
	else
		// Media armonica
		if(Q==-1){
			if(abs(den)>epsilon)
				resultado=1/den;
			else
				resultado=0;
		}
		else{
			// Media contra-armonica
			if(abs(den)>epsilon)
				resultado=num/den;
			else
				resultado=0;
		}
	
	// Devuelve el resultado
	return resultado;
}

// Calcula la media aritmetica de una vecindad
double mediaAritmetica(CImg<double> vecindad){
	return mediaContraArmonica(vecindad,0);
}

// Calcula la media armonica de una vecindad
double mediaArmonica(CImg<double> vecindad){
	return mediaContraArmonica(vecindad,-1);
}

// Aplica el filtro de medias
// Tipo de filtro
// 0 = Filtro de la media aritmetica
// 1 = Filtro de la media geometrica
// 2 = Filtro de la media armonica
// 3 = Filtro de la media contra-armonica
CImg<double> filtradoMedias(CImg<double> img,unsigned int tipo_filtro=0,double Q=1.0,int m=3,int n=3){
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
		// Media aritmetica
		if(tipo_filtro==0)
			salida(i,j)=mediaAritmetica(vecindad);
		// Media geometrica
		if(tipo_filtro==1)
			salida(i,j)=mediaGeometrica(vecindad);
		// Media armonica
		if(tipo_filtro==2)
			salida(i,j)=mediaArmonica(vecindad);
		// Media contra-armonica
		if(tipo_filtro==3)
			salida(i,j)=mediaContraArmonica(vecindad,Q);
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
	// Valor de Q
	double Q=1.2;
	
	// Aplica los diferentes filtros
	// Media geometrica
	CImg<double> filtrada_geometrica=filtradoMedias(sangre_i_ruido,1,Q,3,3);
	
	// Media contra-armonica
	CImg<double> filtrada_contra_armonica=filtradoMedias(sangre_i_ruido,3,Q,3,3);
	
	// Media aritmetica
	CImg<double> filtrada_aritmetica=filtradoMedias(sangre_i_ruido,0,Q,3,3);
	
	// Media armonica
	CImg<double> filtrada_armonica=filtradoMedias(sangre_i_ruido,2,Q,3,3);
	
	// Resultados en color
	CImg<double> geometrica_color=reemplazarIntensidad(sangre,filtrada_geometrica);
	CImg<double> contra_armonica_color=reemplazarIntensidad(sangre,filtrada_contra_armonica);
	CImg<double> aritmetica_color=reemplazarIntensidad(sangre,filtrada_aritmetica);
	CImg<double> armonica_color=reemplazarIntensidad(sangre,filtrada_armonica);
	
	// Visualiza los resultados
	(sangre,sangre_ruido,geometrica_color,contra_armonica_color,aritmetica_color,armonica_color).display("Original - Ruido - Geometrica - Contra-armonica - Aritmetica - Armonica");
	
	// Calcula los ECMs de las imagenes
	cout<<"\n\n";
	cout<<"ECM Imagen con ruido:      "<<sangre.MSE(sangre_ruido)<<"\n";
	cout<<"ECM Media geometrica:      "<<sangre.MSE(geometrica_color)<<"\n";
	cout<<"ECM Media contra-armonica: "<<sangre.MSE(contra_armonica_color)<<"\n";
	cout<<"ECM Media aritmetica:      "<<sangre.MSE(aritmetica_color)<<"\n";
	cout<<"ECM Media armonica:        "<<sangre.MSE(armonica_color)<<"\n";
	
	// El ruido impulsivo no se puede filtrar facilmente, o se filtra la sal o
	// la pimienta. Ademas, algunos filtros pueden eliminar el ruido gaussiano
	// y una parte del impulsivo (sal)
	// Ruido gaussiano varianza=20; impulsivo varianza=20
	
	// ECM Imagen con ruido:       1695.89
	// ECM Media geometrica:        902.81
	// ECM Media contra-armonica:   799.006
	// ECM Media aritmetica:        668.786
	// ECM Media armonica:         1013.58
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
