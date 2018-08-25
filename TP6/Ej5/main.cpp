#include <iostream>
#include <complex>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Constante PI
const double PI=3.14159265358979323846;

// Define una constante para evitar casos cercanos a cero
const double epsilon=0.0001;

// Filtro de movimiento
CImgList<double> filtroMovimiento(CImg<double> img,double a,double b,double T=1.0){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Variable compleja
	complex<double> I(0,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro de movimiento
	cimg_forXY(H[0],i,j){
		double factor;
		
		// Controla el caso especial de u=0 y v=0
		if(i==0 && j==0)
			factor=1.0;
		else
			factor=PI*(double(i-M2)*a+double(j-N2)*b);
		
		// Calcula la constante y el seno
		double constante=T/factor;
		double seno=sin(factor);
		
		// Solo calcula el filtro si el seno es mayor que epsilon
		if(fabs(seno)>epsilon){
			// Calcula la exponencial compleja
			complex<double> exponencial=exp(-I*factor);
			
			// Calcula el valor del filtro
			complex<double> prod=constante*seno*exponencial;
			
			// Guarda las componentes
			H[0](i,j)=real(prod); // Parte real
			H[1](i,j)=imag(prod); // Parte imaginaria
		}
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	H[1].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Aplica un filtro en el dominio frecuencial
CImg<double> filtradoFrec(CImg<double> img,CImgList<double> H){
	CImg<double> filtrada;
	
	// Calcula la TDF de la imagen
	CImgList<double> img_tdf=img.get_FFT();
	
	// Realiza la multiplicacion en el dominio frecuencial
	CImgList<double> filtrada_tdf=img_tdf;
	
	cimg_forXY(img_tdf[0],i,j){
		// Valor complejo de la TDF de la imagen original
		complex<double> factor1(img_tdf[0](i,j),img_tdf[1](i,j));
		
		// Valor complejo de la TDF del filtro
		complex<double> factor2(H[0](i,j),H[1](i,j));
		
		// Realiza el producto entre los numeros complejos
		complex<double> producto=factor1*factor2;
		
		// Guarda la parte real e imaginaria de la imagen filtrada
		filtrada_tdf[0](i,j)=real(producto); // Parte real
		filtrada_tdf[1](i,j)=imag(producto); // Parte imaginaria
	}
	
	// Calcula la TDF inversa y toma solo la parte real
	filtrada=filtrada_tdf.get_FFT(true)[0];
	
	// Devuelve la imagen filtrada
	return filtrada;
}

// Aplica un filtro invertido en el dominio frecuencial
CImg<double> filtradoFrecInv(CImg<double> img,CImgList<double> H){
	CImg<double> filtrada;
	
	// Calcula la TDF de la imagen
	CImgList<double> img_tdf=img.get_FFT();
	
	// Realiza la division en el dominio frecuencial
	CImgList<double> filtrada_tdf=img_tdf;
	
	cimg_forXY(img_tdf[0],i,j){
		// Valor complejo de la TDF de la imagen original
		complex<double> factor1(img_tdf[0](i,j),img_tdf[1](i,j));
		
		// Valor complejo de la TDF del filtro
		complex<double> factor2(H[0](i,j),H[1](i,j));
		
		// Controla que no sean nulos los valores del filtro
		if(fabs(real(factor2))>epsilon){
			// Realiza la division entre los numeros complejos
			complex<double> producto=factor1/factor2;
			
			// Guarda la parte real e imaginaria de la imagen filtrada
			filtrada_tdf[0](i,j)=real(producto); // Parte real
			filtrada_tdf[1](i,j)=imag(producto); // Parte imaginaria
		}
	}
	
	// Calcula la TDF inversa y toma solo la parte real
	filtrada=filtrada_tdf.get_FFT(true)[0];
	
	// Devuelve la imagen filtrada
	return filtrada;
}

// Aplica un filtrado de Wiener (aproximado) a una imagen
CImg<double> filtradoWiener(CImg<double> img,CImgList<double> H,double K){
	// Calcula la imagen con el filtrado inverso
	CImg<double> salida=filtradoFrecInv(img,H);
	
	cimg_forXY(salida,i,j){
		// Magnitud al cuadrado
		double H_mag2=pow(H[0](i,j),2)+pow(H[1](i,j),2);
		
		// Calcula el factor |H|^2/(|H|^2+K)
		double factor;
		
		if(H_mag2>epsilon)
			factor=(H_mag2/(H_mag2+K));
		else
			factor=1.0;
		
		// Aplica el factor a la imagen filtrada
		salida(i,j)*=factor;
	}
	
	// Devuelve la imagen filtrada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","huang3_movida.tif","Image file\n");
	CImg<double> huang3_movida(filename1);
	
	const char* filename2 = cimg_option("-i","huang3.jpg","Image file\n");
	CImg<double> huang3(filename2);
	
	// Idea: aplicando un filtro de movimiento a la imagen limpia, ir variando
	// los parametros a, b y T hasta que la imagen filtrada y la movida sean
	// lo mas parecidas posibles. Luego, para probar que el filtro de
	// movimiento este bien definido, se lo aplica en forma inversa en el
	// dominio frecuencial.
	
	// Filtro de movimiento
	// Parametros para el filtro de movimiento
	// a y b inversos al desplazamiento
	double a=0.04;
	double b=0.0;
	double T=1.0;
	
	// Crea el filtro de movimiento
	CImgList<double> H_Mov=filtroMovimiento(huang3,a,b,T);
	
	// Aplica el filtro a la imagen
	CImg<double> huang3_filtrada=filtradoFrec(huang3,H_Mov);
	
	// Visualiza los resultados
	(huang3,huang3_filtrada,huang3_movida).display("Original - Filtrada movimiento - Movida");
	
	// Aplica el filtro inverso a la imagen
	CImg<double> huang3_movida_filtrada=filtradoFrecInv(huang3_movida,H_Mov);
	
	// Visualiza los resultados
	(huang3_movida,huang3_movida_filtrada,huang3).display("Movida - Filtrada movimiento - Original");
	
	// La restauracion obtenida mediante la inversa del filtro de movimiento es
	// visualmente aceptable, pero sin la imagen limpia es casi imposible
	// determinar los parametros del filtro de movimiento, y aun si se hiciese
	// los resultados no son optimos (tambien revisar los bordes).
	
	// Filtro de Wiener
	// Factor K
	double K=0.0;
	
	// *** sigue con problemas el de Wiener
	
	// Aplica el filtro a la imagen
	CImg<double> huang3_movida_wiener=filtradoWiener(huang3_movida,H_Mov,K);
	
	// Visualiza los resultados
	(huang3_movida,huang3_movida_wiener,huang3).display("Movida - Filtro de Wiener - Original");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
