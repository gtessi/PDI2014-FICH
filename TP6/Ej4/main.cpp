#include <iostream>
#include <complex>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Item 3
// Filtro rechaza-banda ideal
CImgList<double> filtroRBanIdeal(CImg<double> img,double frec_corte,double W){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro rechaza-banda ideal
	cimg_forXY(H[0],i,j){
		
		D(i,j)=sqrt(pow(i-M2,2)+pow(j-N2,2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		if(D(i,j)>=(frec_corte-W/2) && D(i,j)<=(frec_corte+W/2))
			H[0](i,j)=0.0;
		else
			H[0](i,j)=1.0;
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro pasa-banda ideal
CImgList<double> filtroPBanIdeal(CImg<double> img,double frec_corte,double W){
	CImgList<double> H=filtroRBanIdeal(img,frec_corte,W);
	
	// Calcula el filtro pasa-banda ideal
	H[0]=1-H[0];
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro rechaza-banda Butterworth
CImgList<double> filtroRBanButterworth(CImg<double> img,double frec_corte,double W,int orden){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro rechaza-banda Butterworth
	cimg_forXY(H[0],i,j){
		
		D(i,j)=sqrt(pow(i-M2,2)+pow(j-N2,2));
		
		double num=D(i,j)*W;
		double den=pow(D(i,j),2)-pow(frec_corte,2);
		
		double factor=pow(num/den,2*orden);
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1/(1+factor);
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro pasa-banda Butterworth
CImgList<double> filtroPBanButterworth(CImg<double> img,double frec_corte,double W,int orden){
	CImgList<double> H=filtroRBanButterworth(img,frec_corte,W,orden);
	
	// Calcula el filtro pasa-banda Butterworth
	H[0]=1-H[0];
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro rechaza-banda gaussiano
CImgList<double> filtroRBanGaussiano(CImg<double> img,double frec_corte,double W){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro rechaza-banda gaussiano
	cimg_forXY(H[0],i,j){
		
		D(i,j)=sqrt(pow(i-M2,2)+pow(j-N2,2));
		
		double num=pow(D(i,j),2)-pow(frec_corte,2);
		double den=D(i,j)*W;
		
		double factor=pow(num/den,2);
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1-exp(-0.5*factor);
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro pasa-banda gaussiano
CImgList<double> filtroPBanGaussiano(CImg<double> img,double frec_corte,double W){
	CImgList<double> H=filtroRBanGaussiano(img,frec_corte,W);
	
	// Calcula el filtro pasa-banda gaussiano
	H[0]=1-H[0];
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Item 4
// Filtro notch rechaza-banda ideal
CImgList<double> filtroNotchRBanIdeal(CImg<double> img,double frec_corte,int u0,int v0){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D1(M,N,1,1);
	CImg<double> D2(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro notch rechaza-banda ideal
	cimg_forXY(H[0],i,j){
		
		D1(i,j)=sqrt(pow(i-M2-u0,2)+pow(j-N2-v0,2));
		D2(i,j)=sqrt(pow(i-M2+u0,2)+pow(j-N2+v0,2));
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		if(D1(i,j)<=frec_corte || D2(i,j)<=frec_corte)
			H[0](i,j)=0.0;
		else
			H[0](i,j)=1.0;
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro notch pasa-banda ideal
CImgList<double> filtroNotchPBanIdeal(CImg<double> img,double frec_corte,int u0,int v0){
	CImgList<double> H=filtroNotchRBanIdeal(img,frec_corte,u0,v0);
	
	// Calcula el filtro notch pasa-banda ideal
	H[0]=1-H[0];
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro notch rechaza-banda Butterworth
CImgList<double> filtroNotchRBanButterworth(CImg<double> img,double frec_corte,int u0,int v0,int orden){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D1(M,N,1,1);
	CImg<double> D2(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro notch rechaza-banda Butterworth
	cimg_forXY(H[0],i,j){
		
		D1(i,j)=sqrt(pow(i-M2-u0,2)+pow(j-N2-v0,2));
		D2(i,j)=sqrt(pow(i-M2+u0,2)+pow(j-N2+v0,2));
		
		double num=pow(frec_corte,2);
		double den=D1(i,j)*D2(i,j);
		
		double factor=pow(num/den,orden);
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1/(1+factor);
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro notch pasa-banda Butterworth
CImgList<double> filtroNotchPBanButterworth(CImg<double> img,double frec_corte,int u0,int v0,int orden){
	CImgList<double> H=filtroNotchRBanButterworth(img,frec_corte,u0,v0,orden);
	
	// Calcula el filtro notch pasa-banda Butterworth
	H[0]=1-H[0];
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro notch rechaza-banda gaussiano
CImgList<double> filtroNotchRBanGaussiano(CImg<double> img,double frec_corte,int u0,int v0){
	// Tamaño del filtro
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Mitades
	double M2=M/2;
	double N2=N/2;
	
	// Distancia euclidea
	CImg<double> D1(M,N,1,1);
	CImg<double> D2(M,N,1,1);
	
	CImgList<double> H(2,M,N,1,1,0);
	
	// Crea el filtro notch rechaza-banda gaussiano
	cimg_forXY(H[0],i,j){
		
		double num=D1(i,j)*D2(i,j);
		double den=pow(frec_corte,2);
		
		double factor=num/den;
		
		// Definicion de la magnitud de respuesta en frecuencia de H en funcion
		// de D
		H[0](i,j)=1-exp(-0.5*factor);
	}
	
	// Desplaza el filtro
	H[0].shift(M2,N2,0,0,2);
	
	// Devuelve el filtro en el dominio frecuencial
	return H;
}

// Filtro notch pasa-banda gaussiano
CImgList<double> filtroNotchPBanGaussiano(CImg<double> img,double frec_corte,int u0,int v0){
	CImgList<double> H=filtroNotchRBanGaussiano(img,frec_corte,u0,v0);
	
	// Calcula el filtro notch pasa-banda gaussiano
	H[0]=1-H[0];
	
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

// Suma filtros
CImgList<double> sumaFiltros(CImgList<double> H1,CImgList<double> H2){
	CImgList<double> H_sum(H1);
	
	cimg_forXY(H_sum[0],i,j){
		// Suma las componentes de los filtros
		H_sum[0](i,j)=H1[0](i,j) && H2[0](i,j); // Parte real
		H_sum[1](i,j)=H1[1](i,j) && H2[1](i,j); // Parte imaginaria
	}
	
	// Devuelve el filtro
	return H_sum;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","img_degradada.tif","Image file\n");
	CImg<double> img_degradada(filename1);
	
	img_degradada.channel(0);
	
	// Item 1
	// Calcula la TDF de la imagen
	//CImgList<double> img_degradada_tdf=img_degradada.get_FFT();
	
	// Visualiza las imagenes
	(img_degradada,magn_tdf(img_degradada,true,"../gray.pal"),magn_tdf(img_degradada,false,"../gray.pal")).display("Original - TDF centrada - TDF sin centrar");
	
	// En la imagen original hay un ruido en franjas diagonales. En la TDF de
	// la imagen aparecen 4 picos en los "centros" de cada cuadrante, que se
	// condice con el patron de ruido presente en la imagen
	
	// Item 2
	// Los centros de los picos se calculan con el espectro centrado, asi se
	// puede aplicar el filtro notch
	// Centro en (58;38) y el conjugado (199;219)
	double x0=199, y0=219;
	
	double factor1=x0-img_degradada.width()/2;
	double factor2=y0-img_degradada.height()/2;
	
	// Frecuencia de corte
	double D0=sqrt(pow(factor1,2)+pow(factor2,2));
	
	// Ancho de banda
	double W=10;
	
	// Item 5
	// Carga la imagen en filename
	const char* filename2 = cimg_option("-i","img.tif","Image file\n");
	CImg<double> img(filename2);
	
	img.channel(0);
	
	// Crea el filtro rechaza-banda ideal
	CImgList<double> H_RBan=filtroRBanIdeal(img_degradada,D0,W);
	
	// Crea el filtro notch rechaza-banda ideal
	CImgList<double> H_NRBan=filtroNotchRBanIdeal(img_degradada,10,factor1,factor2);
	
	// Aplica los diferentes filtros a la imagen
	CImg<double> rechaza_banda=filtradoFrec(img_degradada,H_RBan);
	CImg<double> rechaza_banda_notch=filtradoFrec(img_degradada,H_NRBan);
	
	// Visualiza los resultados
	(img_degradada,rechaza_banda,rechaza_banda_notch).display("Original - Filtrada rechaza-banda ideal - Filtrada notch rechaza-banda ideal");
	
	// Calcula los ECMs de las imagenes
	cout<<"\n\n";
	cout<<"ECM Imagen con ruido:           "<<img.MSE(img_degradada)<<"\n";
	cout<<"ECM Rechaza-banda ideal:        "<<img.MSE(rechaza_banda)<<"\n";
	cout<<"ECM Notch rechaza-banda ideal:  "<<img.MSE(rechaza_banda_notch)<<"\n";
	
	// Cualitativamente la imagen mejor mucho, pero cuantitativamente (ECM)
	// todavia hay una gran diferencia entre los valores de los pixels
	
	// ECM Imagen con ruido:            3522.13
	// ECM Rechaza-banda ideal:         1572.98
	// ECM Notch rechaza-banda ideal:   1546.28
	
	// Item 6
	// Crea el filtro pasa-banda ideal
	CImgList<double> H_PBan=filtroPBanIdeal(img_degradada,D0,W);
	
	// Crea el filtro notch pasa-banda ideal
	CImgList<double> H_NPBan=filtroNotchPBanIdeal(img_degradada,10,factor1,factor2);
	
	// Aplica los diferentes filtros a la imagen
	CImg<double> pasa_banda=filtradoFrec(img_degradada,H_PBan);
	CImg<double> pasa_banda_notch=filtradoFrec(img_degradada,H_NPBan);
	
	// Visualiza los resultados
	(img_degradada,pasa_banda,pasa_banda_notch).display("Original - Filtrada pasa-banda ideal - Filtrada notch pasa-banda ideal");
	
	// Item 7
	// noisy_moon
	const char* filename3 = cimg_option("-i","noisy_moon.jpg","Image file\n");
	CImg<double> noisy_moon(filename3);
	
	// Visualiza las imagenes
	(noisy_moon,magn_tdf(noisy_moon,true,"../gray.pal"),magn_tdf(noisy_moon,false,"../gray.pal")).display("Original - TDF centrada - TDF sin centrar");
	
	// Centroides
	// 1: (251;167)
	// 2: (167;251)
	x0=251, y0=167;
	
	factor1=x0-noisy_moon.width()/2;
	factor2=y0-noisy_moon.height()/2;
	
	// Frecuencia de corte
	D0=sqrt(pow(factor1,2)+pow(factor2,2));
	
	// Ancho de banda
	W=15;
	
	// Crea el filtro rechaza-banda gaussiano
	H_NRBan=filtroRBanGaussiano(noisy_moon,D0,W);
	
	// Aplica el filtro a la imagen
	rechaza_banda=filtradoFrec(noisy_moon,H_NRBan);
	
	// Visualiza los resultados
	(noisy_moon,magn_tdf(noisy_moon,true,"../gray.pal"),rechaza_banda,magn_tdf(rechaza_banda,true,"../gray.pal")).display("Original - TDF - Filtrada rechaza-banda gaussiano - TDF");
	
	// Crea el filtro pasa-banda gaussiano
	H_PBan=filtroPBanGaussiano(noisy_moon,D0,W);
	
	// Aplica el filtro a la imagen
	pasa_banda=filtradoFrec(noisy_moon,H_PBan);
	
	// Visualiza los resultados
	(noisy_moon,pasa_banda).display("Original - Filtrada pasa-banda gaussiano");
	
	// HeadCT_degradada
	const char* filename4 = cimg_option("-i","HeadCT_degradada.tif","Image file\n");
	CImg<double> HeadCT_degradada(filename4);
	
	// Visualiza las imagenes
	(HeadCT_degradada,magn_tdf(HeadCT_degradada,true,"../gray.pal"),magn_tdf(HeadCT_degradada,false,"../gray.pal")).display("Original - TDF centrada - TDF sin centrar");
	
	// Centroides
	// 1: (296;296)
	x0=296, y0=296;
	
	factor1=x0-HeadCT_degradada.width()/2;
	factor2=y0-HeadCT_degradada.height()/2;
	
	// Crea el filtro notch rechaza-banda ideal
	CImgList<double> H_NRBan1=filtroNotchRBanIdeal(HeadCT_degradada,5,factor1,factor2);
	
	// 2: (266;256)
	x0=266, y0=256;
	
	factor1=x0-HeadCT_degradada.width()/2;
	factor2=y0-HeadCT_degradada.height()/2;
	
	// Crea el filtro notch rechaza-banda ideal
	CImgList<double> H_NRBan2=filtroNotchRBanIdeal(HeadCT_degradada,5,factor1,factor2);
	
	// 3: (256;276)
	x0=256, y0=276;
	
	factor1=x0-HeadCT_degradada.width()/2;
	factor2=y0-HeadCT_degradada.height()/2;
	
	// Crea el filtro notch rechaza-banda ideal
	CImgList<double> H_NRBan3=filtroNotchRBanIdeal(HeadCT_degradada,5,factor1,factor2);
	
	// Suma los filtros
	H_NRBan=sumaFiltros(H_NRBan1,H_NRBan2);
	H_NRBan=sumaFiltros(H_NRBan,H_NRBan3);

	// Aplica el filtro a la imagen
	rechaza_banda=filtradoFrec(HeadCT_degradada,H_NRBan);
	
	// Visualiza los resultados
	(HeadCT_degradada,magn_tdf(HeadCT_degradada,true,"../gray.pal"),rechaza_banda,magn_tdf(rechaza_banda,true,"../gray.pal")).display("Original - TDF - Filtrada notch rechaza-banda ideal - TDF");
	
	// Crea el filtro notch pasa-banda ideal
	H_NPBan=H_NRBan;
	H_NPBan[0]=1-H_NRBan[0];
	
	// Aplica el filtro a la imagen
	pasa_banda=filtradoFrec(HeadCT_degradada,H_NPBan);
	
	// Visualiza los resultados
	(HeadCT_degradada,pasa_banda).display("Original - Filtrada notch pasa-banda ideal");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
