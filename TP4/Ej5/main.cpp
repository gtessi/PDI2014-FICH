#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Aplica una mascara sobre la imagen
CImg<float> aplicarMascara(CImg<float> img,CImg<bool> mascara){
	CImg<float> salida(img);
	
	// Aplica la mascara en todos los canales
	cimg_forXYC(img,i,j,c){
		salida(i,j,c)*=mascara(i,j,c);
	}
	
	// Devuelve la imagen enmascarada
	return salida;
}

// Genera la mascara a partir de una imagen, del centroide de la esfera RGB y
// el radio a considerar
CImg<bool> generarMascaraRGB(CImg<float> img,float centroide[],float radio){
	CImg<bool> mascara(img);
	mascara.fill(0);
	
	// Variables
	float comp_r, comp_g, comp_b, distancia;
	
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
CImg<bool> generarMascaraHS(CImg<float> img_hsi,float sub_H[],float sub_S[]){
	CImg<bool> mascara(img_hsi);
	mascara.fill(0);
	
	// Obtiene los espacios H y S
	CImg<float> H=img_hsi.get_channel(0);
	CImg<float> S=img_hsi.get_channel(1);
	
	// Normaliza los espacios
	H.normalize(0,359);
	S.normalize(0,255);
	
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


int main(int argc, char** argv) {
	// Item 1
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","futbol.jpg","Image file\n");
	CImg<float> futbol(filename1);
	
	// Recorte el area de interes
	CImg<float> recorte=futbol.get_crop(130,105,205,230);
	
	// Calcula el valor promedio de cada canal del recorte
	float r_medio=recorte.get_channel(0).mean();
	float g_medio=recorte.get_channel(1).mean();
	float b_medio=recorte.get_channel(2).mean();
	
	// Imprime las componentes promedio en consola
	cout<<"\n\nPromedios - R: "<<r_medio<<", G: "<<g_medio<<", B: "<<b_medio<<endl<<endl;
	
	// Valores medios
	// R=44.6105
	// G=80.5298
	// B=150.361
	
	// Muestra los histogramas de cada canal para determinar el radio de la
	// esfera
	recorte.get_channel(0).get_histogram(256,0,255).display_graph(0,3);
	recorte.get_channel(1).get_histogram(256,0,255).display_graph(0,3);
	recorte.get_channel(2).get_histogram(256,0,255).display_graph(0,3);
	
	// Pico mas alto
	// R=41
	// G=83
	// B=179
	
	// Color a segmentar determinado a partir del valor promedio de cada canal
	float color_segmentar[]={r_medio,g_medio,b_medio};
	
	// Radio de la esfera determinado a partir de cada histograma
	float radio=60.0;
	
	// Genera la mascara RGB
	CImg<bool> mascara=generarMascaraRGB(futbol,color_segmentar,radio);
	
	// Aplica la mascara a la imagen
	CImg<float> resultado_prom=aplicarMascara(futbol,mascara);
	
	// Color a segmentar determinado a partir del valor maximo de cada canal
	color_segmentar[0]=41.0;
	color_segmentar[1]=83.0;
	color_segmentar[2]=179.0;
	
	// Genera la mascara RGB
	mascara=generarMascaraRGB(futbol,color_segmentar,radio);
	
	// Aplica la mascara a la imagen
	CImg<float> resultado_max=aplicarMascara(futbol,mascara);
	
	// Visualiza los resultados
	(futbol,resultado_prom,resultado_max).display("Original - Segmentada Promedio - Segmentada Maximo");
	
	cout<<"\n\n*********************************************************\n\n";
	
	// Item 2
	// Convierte la imagen de RGB a HSI
	CImg<float> futbol_hsi=futbol.get_RGBtoHSI();
	
	// Muestra los histogramas de H y S para detectar el subespacio
	futbol_hsi.get_channel(0).get_histogram(360,0,359).display_graph(0,3);
	futbol_hsi.get_channel(1).get_histogram(256,0,1).display_graph(0,3);
	
	// Define el subespacio HS
	// H=[215;225]
	// S=[130;160]
	float sub_H[]={210.0,225.0};
	float sub_S[]={90.0,255.0};
	
	// Genera la mascara HS
	mascara=generarMascaraHS(futbol_hsi,sub_H,sub_S);
	
	// Aplica la mascara a la imagen
	CImg<float> resultado_hsi=aplicarMascara(futbol,mascara);
	
	// Visualiza los resultados
	(futbol,resultado_hsi).display("Original - Segmentada HSI");
	
	// Item 3
	// La mascara RGB es mas fiable para filtrar colores, siendo mucho mas
	// sencilla de implementar. Sin embargo, la mascara HSI proporciona un
	// rango mayor y mas preciso de colores ya que se basa en el rango del Hue
	// en lugar del valor combinado de pixeles RGB.
	
	(futbol,resultado_prom,resultado_max,resultado_hsi).display("Original - Segmentada Promedio - Segmentada Maximo - Segmentada HSI");
	
	// Item 4
	// Carga la imagen en filename
	const char* filename2 = cimg_option("-i","rostro0.png","Image file\n");
	CImg<float> rostro0(filename2);
	
	CImg<float> rostro0_hsi=rostro0.get_RGBtoHSI();
	
	(rostro0_hsi.get_channel(0).normalize(0,359),rostro0_hsi.get_channel(1).normalize(0,255)).display();
	
	// Define el subespacio HS
	sub_H[0]=0.0; sub_H[1]=100.0;
	sub_S[0]=0.0; sub_S[1]=50.0;
	
	// Genera la mascara HS
	mascara=generarMascaraHS(rostro0_hsi,sub_H,sub_S);
	
	// Aplica la mascara a la imagen
	resultado_hsi=aplicarMascara(rostro0,mascara);
	
	// Visualiza los resultados
	(rostro0,resultado_hsi).display("Original - Segmentada HSI");
	
	// Carga la imagen en filename
	const char* filename3 = cimg_option("-i","rostro2.png","Image file\n");
	CImg<float> rostro2(filename3);
	
	CImg<float> rostro2_hsi=rostro2.get_RGBtoHSI();
	
	(rostro2_hsi.get_channel(0).normalize(0,359),rostro2_hsi.get_channel(1).normalize(0,255)).display();
	
	// Define el subespacio HS
	sub_H[0]=10.0; sub_H[1]=55.0;
	sub_S[0]=40.0; sub_S[1]=80.0;
	
	// Genera la mascara HS
	mascara=generarMascaraHS(rostro2_hsi,sub_H,sub_S);
	
	// Aplica la mascara a la imagen
	resultado_hsi=aplicarMascara(rostro2,mascara);
	
	// Visualiza los resultados
	(rostro2,resultado_hsi).display("Original - Segmentada HSI");
	
	// Carga la imagen en filename
	const char* filename4 = cimg_option("-i","rostro10.png","Image file\n");
	CImg<float> rostro10(filename4);
	
	CImg<float> rostro10_hsi=rostro10.get_RGBtoHSI();
	
	(rostro10_hsi.get_channel(0).normalize(0,359),rostro10_hsi.get_channel(1).normalize(0,255)).display();
	
	// Define el subespacio HS
	sub_H[0]=20.0; sub_H[1]=80.0;
	sub_S[0]=25.0; sub_S[1]=90.0;
	
	// Genera la mascara HS
	mascara=generarMascaraHS(rostro10_hsi,sub_H,sub_S);
	
	// Aplica la mascara a la imagen
	resultado_hsi=aplicarMascara(rostro10,mascara);
	
	// Visualiza los resultados
	(rostro10,resultado_hsi).display("Original - Segmentada HSI");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
