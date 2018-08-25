#include <vector>
#include <cassert>

using namespace cimg_library;
using namespace std;

// -----------------------------------------------------------------------
// Cadenas con los parametros para las funciones
// Estructura: inicio final gamma offset
struct parametros{
	unsigned int inicio;
	unsigned int final;
	
	float gamma;
	float offset;
};

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacionLOG(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=logaritmica(i,offset);
		
		// Agrega el valor al vector LUT
		LUT.push_back(valor);
	}
}

// Dada una imagen y un vector LUT, aplica la transformacion
CImg<unsigned char> aplicarLUT(CImg<unsigned char> img,vector<unsigned char> LUT){
	CImg<unsigned char> s(img.width(),img.height());
	
	// Aplica la LUT a toda la imagen
	cimg_forXY(img,i,j){
		// El vector LUT tiene un mapeo de valores en el rango 0-255 para cada pixel
		s(i,j)=LUT[img(i,j)];
	}
	
	// Devuelve la imagen modificada
	return s;
}

// Crea una mascara en la region (x0;y0)-(x1;y1)
CImg<bool> crearMascara(unsigned int ancho,unsigned int alto,unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1){
	CImg<bool> mascara(ancho,alto,1,1,0);
	mascara.fill(0);
	
	// Verifica que el punto (x0;y0) sea menor que (x1;y1)
	unsigned int temp=0;
	if(x0+y0>x1+y1){
		// x
		temp=x0;
		x0=x1;
		x1=temp;
		
		// y
		temp=y0;
		y0=y1;
		y1=temp;
	}
	
	// Pone en 1 los pixels dentro de la region
	for(unsigned int i=x0;i<x1;i++)
		for(unsigned int j=y0;j<y1;j++)
			mascara(i,j)=true;
	
	// Devuelve la mascara
	return mascara;
}

// Aplica la mascara a una imagen
CImg<unsigned char> aplicarMascara(CImg<unsigned char> img,CImg<bool> mascara){
	assert(img.is_sameXY(mascara));
	
	CImg<unsigned char> salida(img.width(),img.height());
	
	cimg_forXY(img,i,j){
		//salida(i,j)=img(i,j)*(unsigned char)mascara(i,j);
		salida(i,j)=img(i,j)*mascara(i,j);
	}
	
	// Devuelve la imagen modificada
	return salida;
}
// -----------------------------------------------------------------------

void item1(int argc, char** argv){
	// Item 1
	// Carga la imagen en filename
	const char* filename = cimg_option("-i","earth.bmp","Image file\n");
	CImg<unsigned char> earth(filename);
	
	CImg<unsigned char> salida;
	
	// Vector LUT
	vector<unsigned char> LUT;
	
	parametros log1;
	log1.inicio=0;
	log1.final=255;
	log1.gamma=0.0; // El gamma no se usa en logaritmo
	log1.offset=50.0;
	
	//	// Crea y aplica la transformacion a la imagen
	//	crearTransformacionLOG(log1,LUT);
	//	salida=aplicarLUT(earth,LUT);
	//	salida.display();
	//	
	//	LUT.clear();
	
	// Con la transformacion potencia no hay buenos resultados, pero se nota la
	// imagen oculta
	
	// La imagen oculta aparece en la region (437-final;0-274)
	
	// Con una mascara en dicha region, es posible mostrar la imagen oculta sin
	// afectar al resto
	
	// Crea la mascara en la region mencionada
	CImg<bool> mascara=crearMascara(earth.width(),earth.height(),437,0,earth.width(),274);
	
	// Aplica la mascara a la imagen
	CImg<unsigned char> enmascarada=aplicarMascara(earth,mascara);
	
	// Crea y aplica la transformacion a la imagen enmascarada
	crearTransformacionLOG(log1,LUT);
	salida=aplicarLUT(enmascarada,LUT);
	
	// Pega la imagen enmascarada sobre la original
	salida+=earth;
	
	// Muestra las imagenes en pantalla
	CImgList<unsigned char> lista(earth,salida);
	lista.display();
	
	lista.clear();
}

// -----------------------------------------------------------------------
