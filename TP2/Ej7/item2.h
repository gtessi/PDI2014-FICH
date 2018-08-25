

using namespace cimg_library;
using namespace std;

// -----------------------------------------------------------------------
// Toma una imagen del mother e identifica el modelo
void identificarMother(CImg<unsigned char> img){
	// Define la region de interes donde esta el componente caracteristico
	unsigned int x0=125, y0=30, x1=150, y1=50;
	
	// Calcula el promedio de los grises en dicha region
	float promedio=0;
	int contador=0;
	
	for(unsigned int i=x0;i<x1;i++)
		for(unsigned int j=y0;j<y1;j++){
			promedio+=img(i,j);
			
			contador++;
		}
	
	promedio/=contador;
	
	// Promedios
	// X  -> 129
	// SE -> 63
	
	if(promedio>90)
		// Mother X
		cout<<"\n\nEl modelo de mother es: A7V600-X\n\n";
	else
		// Mother SE
		cout<<"\n\nEl modelo de mother es: A7V600-SE\n\n";
}
// -----------------------------------------------------------------------

void item2(int argc, char** argv){
	// Item 2
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","a7v600-X.gif","Image file\n");
	CImg<unsigned char> moboX(filename1);
	
	const char* filename2 = cimg_option("-i","a7v600-SE.gif","Image file\n");
	CImg<unsigned char> moboSE(filename2);
	
	// Imagenes con ruido
	const char* filename1ri = cimg_option("-i","a7v600-X(RImpulsivo).gif","Image file\n");
	CImg<unsigned char> moboXRI(filename1ri);
	
	const char* filename2ri = cimg_option("-i","a7v600-SE(RImpulsivo).gif","Image file\n");
	CImg<unsigned char> moboSERI(filename2ri);
	
	// Identifica el modelo del mother
	identificarMother(moboX);
	identificarMother(moboSE);
	
	// Imagenes con ruido impulsivo
	identificarMother(moboXRI);
	identificarMother(moboSERI);
	
	// Si hay mucho ruido, no es posible diferenciar las placas con este metodo
	// Otra opcion es usar mascaras y calcular el error cuadratico medio de la
	// region para obtener una medida del grado de parecido
}

// -----------------------------------------------------------------------
