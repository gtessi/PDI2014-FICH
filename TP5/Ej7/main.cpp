#include <iostream>
#include <complex>
#include "CImg.h"
#include "../PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Crea una mascara a partir de un patron
CImg<double> crearMascara(unsigned int n,double patron[]){
	CImg<double> mascara(n,n);
	
	// Construye la mascara a partir del patron
	cimg_forXY(mascara,i,j){
		mascara(i,j)=patron[i+j*n];
	}
	
	// Devuelve la mascara
	return mascara;
}

// Aplica el filtro pasa-altos a una imagen
CImg<double> pasaAltos(CImg<double> img,CImg<double> mascara){
	CImg<double> salida;
	
	// Convolucion entre la imagen y la mascara
	salida=img.get_convolve(mascara);
	
	// Normaliza
	salida.normalize(0,255);
	
	// Devuelve la imagen filtrada
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","camaleon.tif","Image file\n");
	CImg<double> camaleon(filename);
	
	// Patron para crear la mascara (suma 0)
	double patron[]={
		0.0, -1.0, 0.0,
		-1.0, 4.0, -1.0,
		0.0, -1.0, 0.0
	};
	
	// Crea la mascara para el filtro pasa-altos
	CImg<double> mascara=crearMascara(3,patron);
	
	// Item 1
	// Parametros para los filtros
	double A=1.0;
	double a=A-1.0;
	double b=1.0;
	
	// Para el filtrado frecuencial, hay que normalizar los tamaños de la
	// imagen y del filtro, rellenando con ceros (zero-padding)
	// El ancho total es ancho_img+ancho_mascara-1
	// El alto total es alto_img+alto_mascara-1
	unsigned int ancho_total=camaleon.width()+mascara.width()-1;
	unsigned int alto_total=camaleon.height()+mascara.height()-1;
	
	// Redimensiona las imagenes para poder hacer la multiplicacion en
	// frecuencias
	CImg<double> camaleon_resize=camaleon.get_resize(ancho_total,alto_total,-100,-100,0);
	CImg<double> mascara_resize=mascara.get_resize(ancho_total,alto_total,-100,-100,0);
	
	// Calcula la TDF de la imagen y de la mascara
	CImgList<double> camaleon_tdf=camaleon_resize.get_FFT();
	CImgList<double> HPA=mascara_resize.get_FFT();
	
	// Espacios de dibujo para los filtros en el dominio frecuencial
	CImgList<double> HAP=HPA;
	CImgList<double> HEAF=HPA;
	
	// Calcula los filtros en el dominio frecuencial
	cimg_forXY(mascara_resize,i,j){
		// Filtro de alta potencia
		HAP[0](i,j)=(A-1.0)+HPA[0](i,j); // Parte real
		HAP[1](i,j)=(A-1.0)+HPA[1](i,j); // Parte imaginaria
		
		// Filtro de enfasis de alta frecuencia
		HEAF[0](i,j)=a+b*HPA[0](i,j); // Parte real
		HEAF[1](i,j)=a+b*HPA[1](i,j); // Parte imaginaria
	}
	
	// Item 2
	// Espacios de dibujo para las imagenes filtradas en el dominio frecuencial
	CImgList<double> filtradaAP_frec_tdf=HPA;
	CImgList<double> filtradaEAF_frec_tdf=HPA;
	
	// Realiza el filtrado de la imagen en el dominio frecuencial
	cimg_forXY(camaleon_resize,i,j){
		// Valor complejo de la TDF de la imagen original
		complex<double> factor_camaleon(camaleon_tdf[0](i,j),camaleon_tdf[1](i,j));
		
		// Filtrado de alta potencia
		// Valor complejo de la TDF del filtro
		complex<double> factor_HAP(HAP[0](i,j),HAP[1](i,j));
		
		// Realiza el producto entre los numeros complejos
		complex<double> producto=factor_camaleon*factor_HAP;
		
		// Guarda la parte real e imaginaria de la imagen filtrada
		filtradaAP_frec_tdf[0](i,j)=real(producto); // Parte real
		filtradaAP_frec_tdf[1](i,j)=imag(producto); // Parte imaginaria
		
		// Filtrado de enfasis de alta frecuencia
		// Valor complejo de la TDF del filtro
		complex<double> factor_HEAF(HEAF[0](i,j),HEAF[1](i,j));
		
		// Realiza el producto entre los numeros complejos
		producto=factor_camaleon*factor_HEAF;
		
		// Guarda la parte real e imaginaria de la imagen filtrada
		filtradaEAF_frec_tdf[0](i,j)=real(producto); // Parte real
		filtradaEAF_frec_tdf[1](i,j)=imag(producto); // Parte imaginaria
	}
	
	// Calcula la TDF inversa y toma solo la parte real
	CImg<double> filtradaAP_frec=filtradaAP_frec_tdf.get_FFT(true)[0];
	CImg<double> filtradaEAF_frec=filtradaEAF_frec_tdf.get_FFT(true)[0];
	
	// Calcula el grosor de los bordes para el recorte
	unsigned int deltaX=floor(mascara.width()/2.0);
	unsigned int deltaY=floor(mascara.height()/2.0);
	
	// Recorta los bordes que sobran
	filtradaAP_frec.crop(deltaX,deltaY,filtradaAP_frec.width()-deltaX-1,filtradaAP_frec.height()-deltaY-1);
	filtradaEAF_frec.crop(deltaX,deltaY,filtradaEAF_frec.width()-deltaX-1,filtradaEAF_frec.height()-deltaY-1);
	
	// Aplica el filtro pasa-altos a la imagen en el espacio
	CImg<double> filtradaPA_esp=pasaAltos(camaleon,mascara);
	
	// Visualiza la imagen original y su TDF
	(camaleon,magn_tdf(camaleon,true,"../gray.pal")).display("Imagen original - TDF");
	
	// Visualiza la imagen filtrada con alta potencia y su TDF
	(filtradaAP_frec,magn_tdf(filtradaAP_frec,true,"../gray.pal")).display("Imagen filtrada con alta potencia - TDF");
	
	// Visualiza la imagen filtrada con enfasis de alta frecuencia y su TDF
	(filtradaEAF_frec,magn_tdf(filtradaEAF_frec,true,"../gray.pal")).display("Imagen filtrada con enfasis de alta frecuencia - TDF");
	
	// Visualiza la imagen filtrada en el dominio espacial y su TDF
	(filtradaPA_esp,magn_tdf(filtradaPA_esp,true,"../gray.pal")).display("Imagen filtrada en el dominio espacial - TDF");
	
	// Item 3
	// Parametros para el filtro de alta potencia
	A=2.0;
	
	// Calcula la imagen filtrada con alta potencia en el dominio espacial
	// f_ap = (A-1)*f + f_pa
	CImg<double> filtradaAP2_esp=(A-1.0)*camaleon+filtradaPA_esp;
	
	// Espacio de dibujo para el filtro en el dominio frecuencial
	CImgList<double> HAP_2=HPA;
	
	// Espacio de dibujo para la imagen filtrada en el dominio frecuencial
	CImgList<double> filtradaAP2_frec_tdf=HPA;
	
	// Calcula el filtro de alta potencia y realiza el filtrado en el dominio
	// frecuencial
	cimg_forXY(camaleon_resize,i,j){
		// Filtro de alta potencia
		HAP_2[0](i,j)=(A-1.0)+HPA[0](i,j); // Parte real
		HAP_2[1](i,j)=(A-1.0)+HPA[1](i,j); // Parte imaginaria
		
		// Valor complejo de la TDF de la imagen original
		complex<double> factor_camaleon(camaleon_tdf[0](i,j),camaleon_tdf[1](i,j));
		
		// Filtrado de alta potencia
		// Valor complejo de la TDF del filtro
		complex<double> factor_HAP_2(HAP_2[0](i,j),HAP_2[1](i,j));
		
		// Realiza el producto entre los numeros complejos
		complex<double> producto=factor_camaleon*factor_HAP_2;
		
		// Guarda la parte real e imaginaria de la imagen filtrada
		filtradaAP2_frec_tdf[0](i,j)=real(producto); // Parte real
		filtradaAP2_frec_tdf[1](i,j)=imag(producto); // Parte imaginaria
	}
	
	// Calcula la TDF inversa y toma solo la parte real
	CImg<double> filtradaAP2_frec=filtradaAP2_frec_tdf.get_FFT(true)[0];
	
	// Recorta los bordes que sobran
	filtradaAP2_frec.crop(deltaX,deltaY,filtradaAP2_frec.width()-deltaX-1,filtradaAP2_frec.height()-deltaY-1);
	
	// Visualiza la imagen filtrada en el dominio espacial y en el frecuencial
	(filtradaAP2_esp,filtradaAP2_frec).display("Filtrado de alta potencia espacial - Filtrado de alta potencia frecuencial");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
