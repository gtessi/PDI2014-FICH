
using namespace cimg_library;
using namespace std;

// -----------------------------------------------------------------------
// Calcula el plano de bit n de la imagen
CImg<bool> obtenerPlano(CImg<int> img,unsigned int n){
	CImg<bool> plano(img.width(),img.height());
	plano.fill(0);
	
	// Bit significativo
	unsigned int bitSignificativo=pow(2,n);
	
	// Aplica un AND entre la imagen y el bit significativo
	plano=img & bitSignificativo;
	
	// Devuelve el plano de bit n
	return plano;
}

// Esconde una imagen en otra dada una region dada, en el plano del bit menos
// significativo
CImg<int> esconder(CImg<int> target,CImg<unsigned char> obj,unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1){
	CImg<int> salida(target);
	
	// Supone que la region esta dentro de los limites de la imagen
	if(x0<0)
		x0=0;
	if(y0<0)
		y0=0;
	if(x1>(unsigned int)target.width())
		x1=target.width();
	if(y1>(unsigned int)target.height())
		y1=target.height();
	
	// Binariza la imagen
	CImg<bool> imagen_bin=binarizar(obj);
	
	// Define el plano de bit
	CImg<bool> plano(target.width(),target.height());
	plano.fill(true);
	
	// Pone la imagen a esconder en la region especificada
	cimg_forXY(imagen_bin,i,j){
		plano(i+x0,j+y0)=imagen_bin(i,j);
	}
	salida=plano;
	
	// Acumula los 7 planos restantes
	for(int i=1;i<8;i++){
		// Calcula el plano de bits
		plano=obtenerPlano(target,i);
		
		// Suma el plano de bit
		salida+=plano*pow(2,i);
	}
	
	// Devuelve la imagen modificada
	return salida;
}

// Recuperar la imagen escondida
CImg<bool> recuperar(CImg<int> target,unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1){
	// Supone que la region esta dentro de los limites de la imagen
	if(x0<0)
		x0=0;
	if(y0<0)
		y0=0;
	if(x1>(unsigned int)target.width())
		x1=target.width();
	if(y1>(unsigned int)target.height())
		y1=target.height();
	
	CImg<bool> salida(x1-x0,y1-y0);
	
	// Recupera el plano de bits menos significativo
	CImg<bool> plano=obtenerPlano(target,0);
	
	// Recupera la imagen escondida a partir de la region dada
	for(unsigned int i=x0;i<x1;i++)
		for(unsigned int j=y0;j<y1;j++)
			salida(i-x0,j-y0)=plano(i,j);
	
	// Devuelve la imagen modificada
	return salida;
}
// -----------------------------------------------------------------------

void item4(int argc, char** argv){
	// Item 4
	// Carga la imagen en filename
	const char* filename1 = cimg_option("-i","cameraman.tif","Image file\n");
	CImg<int> imagen(filename1);
	
	const char* filename2 = cimg_option("-i","star64.bmp","Image file\n");
	CImg<unsigned char> a_esconder(filename2);
	
	// Region donde se va a esconder la imagen
	unsigned int x0=180, y0=16, x1=a_esconder.width()+x0, y1=a_esconder.height()+y0;
	
	cout<<x0<<", "<<y0<<", "<<x1<<", "<<y1<<"\n";
	
	// Esconde la imagen en la principal
	CImg<int> salida=esconder(imagen,a_esconder,x0,y0,x1,y1);
	
	// Muestra la imagen en pantalla
	salida.display();
	
	// Recupera la imagen escondida
	CImg<bool> plano0=recuperar(salida,x0,y0,x1,y1);
	
	// Muestra la imagen escondida
	plano0.display();
}

// -----------------------------------------------------------------------
