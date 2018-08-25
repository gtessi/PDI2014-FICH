#include <iostream>
#include "CImg.h"
#include "PDI_functions.h"

using namespace cimg_library;
using namespace std;

// Transformada de Hough a partir de un angulo y una magnitud
CImg<double> miHough(CImg<double> img,double theta,double rho,int theta_tol=0,int rho_tol=0,bool solo_maximo=false){
	// Calcula la transformada de Hough
	CImg<double> transformada=hough(img);
	
	// Columna en rango de Hough (suma 90 para hacer el rango [0..180])
	double columna_hough=(theta+90)/180*transformada.width();
	
	// Fila en rango de Hough (escala entre 0 y 1)
	double fila_hough=(rho+sqrt(2))*sqrt(2)/4*transformada.height();
	
	// Valor del maximo pico
	double max_pico=0;
	
	// Coordenada del maximo pico
	double x_max_pico=0;
	double y_max_pico=0;
	
	// Espacio de Hough con los picos maximos
	CImg<double> salida(transformada.width(),transformada.height(),1,1,0);
	
	// Recorre el espacio de Hough en el rango deseado
	for(int i=-rho_tol;i<=rho_tol;i++)
		for(int j=-theta_tol;j<=theta_tol;j++){
			// Control de limites
			if(columna_hough+i<transformada.width() && columna_hough+i>=0 && fila_hough+j<transformada.height() && fila_hough+j>=0){
				// Guarda la coordenada del pico
				x_max_pico=columna_hough+i;
				y_max_pico=fila_hough+j;
				
				// Solo toma el maximo pico o todos
				if(solo_maximo){
					if(transformada(columna_hough+i,fila_hough+j)>max_pico)
						max_pico=transformada(x_max_pico,y_max_pico);
				}
				else{
					salida(x_max_pico,y_max_pico)=transformada(x_max_pico,y_max_pico);
				}
			}
		}
	
	if(solo_maximo)
		// Solo toma el valor maximo
		salida(x_max_pico,y_max_pico)=transformada(x_max_pico,y_max_pico);
	else
		// Aplica un umbral
		salida.threshold(0.3*salida.mean()+0.7*salida.max());
	
	// Devuelve el resultado
	return salida;
}


int main(int argc, char** argv) {
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","letras1.tif","Image file\n");
	CImg<double> letras1(filename1);
	
	// Aplicar filtros para bordes y binarizar
	
	// Calcular la transformada de Hough
	
	// Buscar los maximos en un angulo en particular
	
	// Antitransformar los maximos
	
	
	return 0;
}
