#include <vector>

// Trunca los valores entre 0 y 255 para normalizar
unsigned char truncar(float val){
	unsigned char res=0;
	
	if(val>255)
		res=255;
    else
		if(val<0)
			res=0;
		else
			res=floor(val);
	
	return res;
}

// Transformacion lineal
unsigned char lineal(unsigned int r,unsigned int ini,float a,float c){
	float s=0;
	
	// Verifica si la pendiente es positiva o negativa
	if(a<0) // Negativa
		s=255+a*(r-ini)+c;
	else // Positiva
		s=a*(r-ini)+c;
	
	return truncar(s);
}

// Transformacion logaritmica
unsigned char logaritmica(unsigned int r,float c=1.0){
	float s=0;
	
	s=c*log(1+r);
	
	return truncar(s);
}

// Transformacion potencia
unsigned char potencia(unsigned int r,float c=1.0,float gamma=1.0){
	float s=0;
	
	s=c*pow(r,gamma);
	
	return truncar(s);
}

// Cadenas con los parametros para las funciones
// Estructura: inicio final pendiente offset
struct parametros{
	unsigned int inicio;
	unsigned int final;
	
	float pendiente;
	int offset;
};

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacion(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float pendiente=nombre.pendiente;
	int offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=lineal(i,inicio,pendiente,offset);
		
		// Agrega el valor al vector LUT
		LUT.push_back(valor);
	}
}

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

// Crea el vector con la LUT, tambien sirve para hacer la LUT por tramos
void crearTransformacionPOT(parametros nombre,vector<unsigned char> &LUT){
	// Valores para la LUT
	unsigned int inicio=nombre.inicio;
	unsigned int final=nombre.final;
	float gamma=nombre.gamma;
	float offset=nombre.offset;
	
	unsigned char valor=0;
	
	for(unsigned int i=inicio;i<final;i++){
		// Calcula la LUT para el punto actual
		valor=potencia(i,offset,gamma);
		
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

// Devuelve una imagen que representa el mapeo de la LUT
CImg<unsigned char> imagenMapeo(vector<unsigned char> LUT){
	unsigned int ancho=LUT.size()+1;
	
	CImg<unsigned char> mapeo(ancho,ancho,1,1,0);
	
	for(unsigned int i=0;i<ancho;i++){
		// Crea la curva de la LUT
		mapeo(i,ancho-LUT[i]-1)=255;
	}
	
	// Devuelve la imagen
	return mapeo;
}

// Calcula y grafica el mapeo del vector LUT y la imagen resultante
void graficas(CImg<unsigned char> img,vector<unsigned char> LUT){
	// Aplica LUT a la imagen
	CImg<unsigned char> salida=aplicarLUT(img,LUT);
	
	// Calcula la curva del mapeo LUT
	CImg<unsigned char> mapeo=imagenMapeo(LUT);
	
	// Muestra en pantalla los resultados
	CImgList<unsigned char> lista(img,mapeo,salida);
	lista.display();
}

