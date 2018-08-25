#include <vector>

using namespace cimg_library;
using namespace std;

// -----------------------------------------------------------------------
// Binariza una imagen
CImg<bool> binarizar(CImg<unsigned char> img,unsigned int umbral=127){
	CImg<bool> salida(img.width(),img.height());
	
	// Aplica un umbral en gris=127 (default)
	cimg_forXY(img,i,j){
		if(img(i,j)>=umbral)
			// Blanco
			salida(i,j)=true;
		else
			// Negro
			salida(i,j)=false;
	}
	
	// Devuelve la imagen
	return salida;
}

// Toma una imagen del blister e identifica si esta completo o no
void chequearBlister(CImg<unsigned char> img){
	// Coordenadas
	// X = 55, 102, 153, 203, 252
	// Y = 55, 100
	
	// Vectores con coordenadas para cada pastilla
	vector<unsigned int> x, y;
	
	// X
	x.push_back(55);
	x.push_back(102);
	x.push_back(153);
	x.push_back(203);
	x.push_back(252);
	
	// Y
	y.push_back(55);
	y.push_back(100);
	
	// Binariza la imagen
	CImg<bool> binaria=binarizar(img,127);
	
	// Busca si faltan pastillas en el blister y muestra la posicion de cada una
	int c=0;
	
	for(unsigned int i=0;i<x.size();i++)
		for(unsigned int j=0;j<y.size();j++)
			// No hay pastilla
			if(!binaria(x[i],y[j])){
				// Cuenta la cantidad de pastillas faltantes
				c++;
				
				// Muestra las coordenadas de la pastilla faltante
				cout<<"Falta pastilla en la posicion x="<<x[i]<<", y="<<y[j]<<endl;
			}
	
	// Muestra en consola la cantidad de pastillas faltantes
	cout<<"\n\nTotal de pastillas faltantes: "<<c<<endl;
	
	// Muestra la imagen en pantalla
	//img.display();
	
	cout<<"\n****************************************************\n\n";
}
// -----------------------------------------------------------------------

void item3(int argc, char** argv){
	// Item 3
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","blister_completo.jpg","Image file\n");
	CImg<unsigned char> blister_completo(filename1);
	
	const char* filename2 = cimg_option("-i","blister_incompleto.jpg","Image file\n");
	CImg<unsigned char> blister_incompleto(filename2);
	
	// Verifica si el blister esta completo
	chequearBlister(blister_completo);
	chequearBlister(blister_incompleto);
}

// -----------------------------------------------------------------------
