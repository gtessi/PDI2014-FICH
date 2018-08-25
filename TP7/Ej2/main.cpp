#include <iostream>
#include "CImg.h"
#include "../PDI_functions.h"

// Constante PI
const double PI=3.14159265358979323846;

using namespace cimg_library;
using namespace std;

CImg<double> transformadaHough(CImg<double> img,bool inversa,double theta_deseado,double rho_deseado){
	CImg<double> iHough(img);
	iHough.fill(0.0);
	
	// Tamaño de la imagen
	unsigned int M=img.width();
	unsigned int N=img.height();
	
	// Maximo valor posible de rho (radio), que esta en la diagonal principal
	double max_rho=sqrt(float(pow(N-1,2)+pow(M-1,2)));
	
	// Pasos
	double paso_rho=2.*max_rho/(N-1); // Paso en eje rho (rho=[-max_rho;max_rho])
	double paso_theta=PI/(M-1); // Paso en eje theta (theta=[-90;90])
	
	// Variables
	double rho;
	double theta;
	
	// Verifica si es la transformada directa o inversa
	if(!inversa){
		// Directa
		// Radio mapeado en los N pixels
		int r;
		// Recorre los pixels de la imagen (ahora el eje vertical es x)
		cimg_forXY(img,y,x){
			// Opera solo si pasa un umbral de la imagen
			if(img(y,x)>0.5)
				// Calculo rho variando theta en todo el eje, con x e y fijos
				for(unsigned int t=0;t<M;t++){
					// Mapea t en [0;M-1] a t en [-90;90]
					theta=t*paso_theta-PI/2;
					// Calcula rho segun theta
					rho=x*cos(theta)+y*sin(theta);
					// Mapea el radio r en [-max_rho;max_rho] a r en [0;N-1]
					r=floor((rho+max_rho)/paso_rho+0.5);
					
					// Acumula solo si esta en el angulo y magnitud deseados
					if(fabs(theta-theta_deseado)<0.0001){// && fabs(rho-rho_deseado)<0.0001){
						// Aumenta el acumulador
						iHough(t,r)+=1;
					}
				}
		}
	}
	else{
		// Inversa
		// Define el vector de color blanco
		const double blanco[1]={255.f};
		
		// Variables para las coordenadas
		float x0;
		float x1;
		float y0;
		float y1;
		
		// Recorre los pixels de la transformada (ahora el eje vertical es theta)
		cimg_forXY(img,t,r){
			// Opera solo si pasa un umbral de los valores acumulados
			if(img(t,r)>0.5){
				// Mapea t en [0;M-1] a t en [-90;90]
				theta=t*paso_theta-PI/2;
				// Mapea el radio r en [0;N-1] a r en [-max_rho;max_rho]
				rho=r*paso_rho-max_rho;
				// Verifica que theta este entre [-90;90]
				if(theta>-PI/2 && theta<PI/2){
					// Calcula las coordenadas (x es el eje vertical)
					y0=0;
					y1=M-1;
					// Calcula x para y=0
					x0=rho/cos(theta);
					// Calcula x para y=M-1
					x1=rho/cos(theta)-(M-1)*tan(theta);	  
				}else{
					// Calcula las coordenadas (y es el eje horizontal)
					x0=0;
					x1=N-1;
					// Calcula y para x=0
					y0=rho/sin(theta);
					// calcula y para x=M-1
					y1=rho/sin(theta)-(N-1)/tan(theta);
				}
				
				// Dibuja una linea desde (0;y0) a (M-1;y1);
				iHough.draw_line(y0,x0,y1,x1,blanco);
			}
		}
	}
	
	// Devuelve la transformada
	return iHough;
}

//////// Transformada
//////CImg<double> transformadaHough(CImg<double> img,bool inversa,double theta,double rho,int rango_theta,int rango_rho){
//////	// Tamaño de la imagen
//////	unsigned int M=img.width();
//////	unsigned int N=img.height();
//////	
//////	// Calcula la transformada de Hough a la imagen
//////	CImg<double> img_hough=hough(img,false);
//////	
//////	// Calcula la columna correspondiente al angulo theta
//////	double columna=(theta+90)/180*M; // (le sumo 90 para hacer el rango [0..180])
//////	
//////	// Calcula la fila correspondiente a la magnitud rho
//////	double fila=(rho+sqrt(2))*sqrt(2)/4*N; // (las operaciones de adelante es para escalar entre 0 y 1)
//////	
//////	// Variables del maximo pico
//////	double x_max_pico=0;
//////	double y_max_pico=0;
//////	
//////	CImg<double> salida(M,N,1,1,0);
//////	
//////	for(int i=-rango_rho;i<=rango_rho;i++)
//////		for(int j=-rango_theta;i<=rango_theta;i++)
//////			// Controla que no se salga del rango de la imagen
//////			if(columna+i<M && columna+i>=0 && fila+j<N && fila+j>=0){
//////				// Guarda las coordenadas
//////				x_max_pico=columna+i;
//////				y_max_pico=fila+j;
//////				
//////				// Guarda el valor acumulado de la transformada del punto
//////				salida(x_max_pico,y_max_pico)=img_hough(x_max_pico,y_max_pico);
//////			}
//////	
//////	// Devuelve el valor acumulado
//////	return salida;
//////}

// Extrae los maximos picos de la transformada de Hough
CImg<double> maxPicos(CImg<double> hough,unsigned int picos=1){
	CImg<double> salida(hough);
	
	// 
	double max_umbral=0;
	
	// 
	for(unsigned int n=0;n<picos;n++){
		// Busca el pico maximo
		max_umbral=hough.max();
		
		// Elimina el pico
		hough.max()=0;
		
		cout<<"Max-umbral: "<<max_umbral<<endl;
	}
	
	// Limita el rango al ultimo maximo obtenido
	salida.threshold(max_umbral);
	
	salida.display();
	
	// Devuelve la transformada
	return salida;
}


int main(int argc, char** argv) {
////	// Item 1
////	CImg<unsigned char> img(500,500,1,1,0);
////	const unsigned char color[]={255,255,255};
////	
////	int x1,x2,y1,y2;
////	while(x1!=-1){
////		cout<<"Ingrese Coordenadas de la recta: "<<endl;
////		cin >> x1;
////		cin >> y1;
////		cin >> x2;
////		cin >> y2;
////		
////		img.draw_line(x1,y1,x2,y2,color);
////		(img,hough(img,false)).display();
////	}
	
	// Item 2
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","letras1.tif","Image file\n");
	CImg<double> letras1(filename1);
	
	const char* filename2 = cimg_option("-i","letras2.tif","Image file\n");
	CImg<double> letras2(filename2);
	
	const char* filename3 = cimg_option("-i","snowman.png","Image file\n");
	CImg<double> snowman(filename3);
	
	(letras1,hough(letras1,false),transformadaHough(letras1,false,60,0)).display();
	
	// el procedimiento es filtrar las imagenes con un pasa-altos, binarizar y
	// aplica hough
	
	// hay que aplicar tambien eso de los n maximos
	
	// *** hay problemas con hough de datos especificos
	
	
	
	
	cout<<"\n\n*********************************************************\n\n";
	
	return 0;
}
