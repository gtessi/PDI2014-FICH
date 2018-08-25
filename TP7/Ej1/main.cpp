#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

// Operadores de gradiente cruzado de Roberts
CImgList<double> operadorRoberts(){
	CImgList<double> G(2,2,2,1,1,0);
	
	// Gragiente en x (Gx)
	G[0](0,0)=-1;
	G[0](1,1)=1;
	
	// Gragiente en y (Gy)
	G[1](0,1)=-1;
	G[1](1,0)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Prewitt
CImgList<double> operadorPrewitt(){
	CImgList<double> G(4,3,3,1,1,0);
	
	// Gradiente horizontal
	G[0](0,0)=-1;
	G[0](0,1)=-1;
	G[0](0,2)=-1;
	G[0](2,0)=1;
	G[0](2,1)=1;
	G[0](2,2)=1;
	
	// Gradiente vertical
	G[1](0,0)=-1;
	G[1](1,0)=-1;
	G[1](2,0)=-1;
	G[1](0,2)=1;
	G[1](1,2)=1;
	G[1](2,2)=1;
	
	// Gradientes a 45º
	G[2](1,0)=-1;
	G[2](2,0)=-1;
	G[2](2,1)=-1;
	G[2](0,1)=1;
	G[2](0,2)=1;
	G[2](1,2)=1;
	
	G[3](1,0)=-1;
	G[3](0,0)=-1;
	G[3](0,1)=-1;
	G[3](1,2)=1;
	G[3](2,2)=1;
	G[3](2,1)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Sobel
CImgList<double> operadorSobel(){
	CImgList<double> G(4,3,3,1,1,0);
	
	// Gradiente horizontal
	G[0](0,0)=-1;
	G[0](0,1)=-2;
	G[0](0,2)=-1;
	G[0](2,0)=1;
	G[0](2,1)=2;
	G[0](2,2)=1;
	
	// Gradiente vertical
	G[1](0,0)=-1;
	G[1](1,0)=-2;
	G[1](2,0)=-1;
	G[1](0,2)=1;
	G[1](1,2)=2;
	G[1](2,2)=1;
	
	// Gradientes a 45º
	G[2](1,0)=-1;
	G[2](2,0)=-2;
	G[2](2,1)=-1;
	G[2](0,1)=1;
	G[2](0,2)=2;
	G[2](1,2)=1;
	
	G[3](1,0)=-1;
	G[3](0,0)=-2;
	G[3](0,1)=-1;
	G[3](1,2)=1;
	G[3](2,2)=2;
	G[3](2,1)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano 4
CImgList<double> operadorLaplaciano4(){
	CImgList<double> G(1,3,3,1,1,0);
	
	G[0](0,1)=-1;
	G[0](1,0)=-1;
	G[0](1,1)=4;
	G[0](1,2)=-1;
	G[0](2,1)=-1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano 8
CImgList<double> operadorLaplaciano8(){
	CImgList<double> G(1,3,3,1,1,-1);
	
	// Rellena todo con -1, centro en 8
	G[0](1,1)=8;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano del Gaussiano (LoG)
CImgList<double> operadorLoG(){
	CImgList<double> G(1,5,5,1,1,0);
	
	G[0](0,2)=-1;
	
	G[0](1,1)=-1;
	G[0](1,2)=-2;
	G[0](1,3)=-1;
	
	G[0](2,0)=-1;
	G[0](2,1)=-2;
	G[0](2,2)=16;
	G[0](2,3)=-2;
	G[0](2,4)=-1;
	
	G[0](2,1)=-1;
	G[0](2,2)=-2;
	G[0](2,3)=-1;
	
	G[0](3,2)=-1;
	
	// Devuelve el operador
	return G;
}

// Binariza una lista de imagenes a partir de un umbral
CImgList<bool> aplicarUmbralLista(CImgList<double> lista,double umbral){
	CImgList<bool> salida;
	
	// Recorre la lista de imagenes
	for(unsigned int n=0;n<lista.size();n++){
		CImg<bool> temp(lista[n].width(),lista[n].height(),lista[n].depth(),lista[n].spectrum(),false);
		
		// Recorre la imagen a binarizar
		cimg_forXY(lista[n],i,j){
			if(fabs(lista[n](i,j))>umbral)
				temp(i,j)=true;
		}
		
		// Guarda la imagen binarizada
		salida.push_back(temp);
	}
	
	// Devuelve la lista
	return salida;
}

// Calcula la derivada a partir de un operador
// Tipo de operador
// 0: Gradiente de Roberts
// 1: Gradiente de Prewitt
// 2: Gradiente de Sobel
// 3: Laplaciano de 4 vecinos
// 4: Laplaciano de 8 vecinos
// 5: Laplaciano del Gaussiano (LoG)
CImgList<double> derivada(CImg<double> img,unsigned int opcion){
	CImgList<double> salida;
	
	CImgList<double> operador;
		
	// Crea el operador segun la opcion elegida
	switch(opcion){
	// Gradiente de Roberts
	case 0:
		operador=operadorRoberts();
		break;
	// Gradiente de Prewitt
	case 1:
		operador=operadorPrewitt();
		break;
	// Gradiente de Sobel
	case 2:
		operador=operadorSobel();
		break;
	// Laplaciano de 4 vecinos
	case 3:
		operador=operadorLaplaciano4();
		break;
	// Laplaciano de 8 vecinos
	case 4:
		operador=operadorLaplaciano8();
		break;
	// Laplaciano del Gaussiano (LoG)
	case 5:
		operador=operadorLoG();
		break;
	}
	
	// Calcula la convolucion entre la imagen y los operadores
	for(unsigned int n=0;n<operador.size();n++)
		salida.push_back(img.get_convolve(operador[n]));
	
	// Devuelve la derivada de la imagen
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","estambul.tif","Image file\n");
	CImg<double> estambul(filename1);
	
	// Pasa de RGB a HSI
	CImg<double> estambul_hsi=estambul.get_RGBtoHSI();
	
	// Toma el canal de intensidad
	CImg<double> estambul_i=estambul_hsi.get_channel(2);
	
	// Umbral de binarizacion
	double umbral=0.2;
	
	// Item 1
	// Aplica la derivada segun el operador
	CImgList<double> estambul_roberts=derivada(estambul_i,0);
	
	// Binariza las derivadas
	CImgList<bool> estambul_roberts_bin=aplicarUmbralLista(estambul_roberts,umbral);
	
	// Visualiza los resultados
	(estambul_roberts,estambul_roberts_bin).display("Gradiente de Roberts");
	
	// Item 2
	// Aplica la derivada segun el operador
	CImgList<double> estambul_prewitt=derivada(estambul_i,1);
	CImgList<double> estambul_sobel=derivada(estambul_i,2);
	CImgList<double> estambul_laplaciano4=derivada(estambul_i,3);
	CImgList<double> estambul_laplaciano8=derivada(estambul_i,4);
	CImgList<double> estambul_LoG=derivada(estambul_i,5);
	
	// Binariza las derivadas
	CImgList<bool> estambul_prewitt_bin=aplicarUmbralLista(estambul_prewitt,umbral);
	CImgList<bool> estambul_sobel_bin=aplicarUmbralLista(estambul_sobel,umbral);
	CImgList<bool> estambul_laplaciano4_bin=aplicarUmbralLista(estambul_laplaciano4,umbral);
	CImgList<bool> estambul_laplaciano8_bin=aplicarUmbralLista(estambul_laplaciano8,umbral);
	CImgList<bool> estambul_LoG_bin=aplicarUmbralLista(estambul_LoG,umbral);
	
	// Visualiza los resultados
	(estambul_prewitt,estambul_prewitt_bin).display("Gradiente de Prewitt");
	(estambul_sobel,estambul_sobel_bin).display("Gradiente de Sobel");
	(estambul_laplaciano4,estambul_laplaciano4_bin).display("Laplaciano de 4 vecinos");
	(estambul_laplaciano8,estambul_laplaciano8_bin).display("Laplaciano de 8 vecinos");
	(estambul_LoG,estambul_LoG_bin).display("Laplaciano del Gaussiano");
	
	// Item 3
	// Carga la imagen en filename
	const char* filename2 = cimg_option("-i","mosquito.jpg","Image file\n");
	CImg<double> mosquito(filename2);
	
	// Desvio del ruido
	double desvio=10;
	
	// Agrega ruido a la imagen
	CImg<double> mosquito_ruido=mosquito.get_noise(desvio);
	
	// Pasa de RGB a HSI
	CImg<double> mosquito_ruido_hsi=mosquito_ruido.get_RGBtoHSI();
	
	// Toma el canal de intensidad
	CImg<double> mosquito_ruido_i=mosquito_ruido_hsi.get_channel(2);
	
	// Aplica la derivada segun el operador
	CImgList<double> mosquito_roberts=derivada(mosquito_ruido_i,0);
	CImgList<double> mosquito_prewitt=derivada(mosquito_ruido_i,1);
	CImgList<double> mosquito_sobel=derivada(mosquito_ruido_i,2);
	CImgList<double> mosquito_laplaciano4=derivada(mosquito_ruido_i,3);
	CImgList<double> mosquito_laplaciano8=derivada(mosquito_ruido_i,4);
	CImgList<double> mosquito_LoG=derivada(mosquito_ruido_i,5);
	
	// Binariza las derivadas
	CImgList<bool> mosquito_roberts_bin=aplicarUmbralLista(mosquito_roberts,umbral);
	CImgList<bool> mosquito_prewitt_bin=aplicarUmbralLista(mosquito_prewitt,umbral);
	CImgList<bool> mosquito_sobel_bin=aplicarUmbralLista(mosquito_sobel,umbral);
	CImgList<bool> mosquito_laplaciano4_bin=aplicarUmbralLista(mosquito_laplaciano4,umbral);
	CImgList<bool> mosquito_laplaciano8_bin=aplicarUmbralLista(mosquito_laplaciano8,umbral);
	CImgList<bool> mosquito_LoG_bin=aplicarUmbralLista(mosquito_LoG,umbral);
	
	// Visualiza los resultados
	(mosquito_roberts,mosquito_roberts_bin).display("Gradiente de Roberts");
	(mosquito_prewitt,mosquito_prewitt_bin).display("Gradiente de Prewitt");
	(mosquito_sobel,mosquito_sobel_bin).display("Gradiente de Sobel");
	(mosquito_laplaciano4,mosquito_laplaciano4_bin).display("Laplaciano de 4 vecinos");
	(mosquito_laplaciano8,mosquito_laplaciano8_bin).display("Laplaciano de 8 vecinos");
	(mosquito_LoG,mosquito_LoG_bin).display("Laplaciano del Gaussiano");
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
