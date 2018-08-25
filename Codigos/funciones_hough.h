struct Pixel {
	int x;
	int y;
	CImg<double>* img;
	
	Pixel() : x(0), y(0), img(NULL) {};
	Pixel(const Pixel& p) : x(p.x), y(p.y), img(p.img) {};
	Pixel(int x, int y, CImg<double>* img) : x(x), y(y), img(img) {};
	
	bool operator<( const Pixel& p ) const {
		return this->value() < p.value();
	}
	double value() const {
		return img->operator()(x,y);
	}
	bool AdentroImagen( ) {
		return x>=0 && y>=0 && x<img->width() && y<img->height();
	}
};

//enfoca el objeto en la imagen
void enfocarImagen(CImg<double> img,double lim){
	int xmin=img.width(), xmax=0, ymin=img.height(), ymax=0;
	
	for(unsigned int j=0;j<img.height();j++)
		for(unsigned int i=0;i<img.width();i++){
			if(img(i,j,0,0)+img(i,j,0,1)+img(i,j,0,2)<lim*3){
				if(xmin>i) xmin=i;
				if(xmax<i) xmax=i;
				if(ymin>j) ymin=j;
				if(ymax<j) ymax=j;
			}
		}
	
	img.crop(xmin,ymin,xmax,ymax);
}

/*******TRANSFORMADA DE HOUGH*************/

CImg<double> TransformadaHough(CImg<bool> img){
	const int M=img.width(), N=img.height();
	
	CImg<double> iHough(M,N,1,1,0.0);
	
	double maxRho=sqrt(double(pow(N-1,2)+pow(M-1,2)));
	double stepRho=2.*maxRho/(N-1);
	double stepTheta=M_PI/(M-1)
	double rho;
	double theta;
	int r;
	
	cimg_forXY(img,y,x){
		if(img(y,x)){
			for(int t=0;t<M;t++){
				theta=t*stepTheta-M_PI/2;
				rho=x*cos(theta)+y*sin(theta);
				r=floor((rho+maxRho)/stepRho+.5);
				iHough(t,r)+=1;
			}
		}
	}
	
	return iHough; 
}

CImg<double> InversaHough(CImg<double> img, int nLineas){
	const unsigned M=img.width(), N=img.height();
	
	CImg<double> iHough(M,N,1,1,0.0);
	
	double maxRho=sqrt(double(pow(N-1,2)+pow(M-1,2)));
	double stepRho=2.*maxRho/(N-1);
	double stepTheta=M_PI/(M-1);
	double rho;
	double theta;
	const double blanco[1]={255.f};
	double x0, x1, y0, y1;
	
	for(int nl=0;nl<nLineas;nl++){
		Pixel p=Maximo(img);
		int t=p.x, r=p.y;
		theta=t*stepTheta-M_PI/2;
		rho=r*stepRho-maxRho;
		
		if(theta>-M_PI/2 && theta<M_PI/2){
			y0=0;
			y1=M-1;
			x0=rho/cos(theta);
			x1=rho/cos(theta)-(M-1)*tan(theta);
		}
		else{
			x0=0;
			x1=N-1;
			y0=rho/sin(theta);
			y1=rho/sin(theta)-(N-1)/tan(theta);
		}
		
		iHough.draw_line(y0,x0,y1,x1,blanco);
		img(t,r)=0;
	}
	
	return iHough; 
}


CImg<double> TransformadaHoughCirculos(CImg<bool> img, int r){
	int M=img.width(), N=img.height();
	
	CImg<double> iHough(M,N,1,1,0.0);
	
	cimg_forXY(img,i,j){
		if(img(i,j)){
			for(int a=0;a<M;++a){
				double det=r*r-(i-a)*(i-a);
				if (det>0){
					double b=round(j-sqrt(det));
					if(b>0 && b<N)
						iHough(a,b)++;
				}
			}
		}
	}
	
	return iHough;
}

/**********************************************************************/
/** Deteccion de bordes **/
/*********************************************************************/
void segmentacionSobel(CImg<double> &imgGris){
	CImg<double> img1(imgGris),img2(imgGris);
	CImg<double> mascara1(3,3,1,1);
	CImg<double> mascara2(3,3,1,1);
	
	mascara1.fill(-1,-2,-1,0,0,0,1,2,1);
	mascara2.fill(-1,0,1,-2,0,2,-1,0,1);
	
	img1=imgGris.get_convolve(mascara1);
	img2=imgGris.get_convolve(mascara2);
	imgGris=img1.get_abs()+img2.get_abs();
}

//SI img<umbral negro, sino blanco.
CImg<double> ImgBinaria(CImg<double> img,double umbral){
	cimg_for2XY(img,i,j){
		if(img(i,j)<umbral) img(i,j)=0;
		else img(i,j)=1;
	}
	return img;
}


/***/
//Busca mediante la transformada de hough el angulo mas probable en una imagen, para rotarla.
struct angulo { //struct que representa un angulo theta, cuyo "peso" en la imagen es de accu
	int accu;
	double theta;
	
	const bool operator<(const angulo &a) const{
		return (accu<a.accu);
	}
	
};

//busca el angulo mas probable en una imagen, el umbral lo usa para binarizar
//la imagen de bordes, para no considerar todos los bordes
double angulo_principal(CImg<double> img){
	int w=img.width(), h=img.height();
	
	CImg<double> bordes=img;
	segmentacionSobel(bordes); //calculo la imagen de bordes
	
	//lo siguiente calcula la media de intensidad de los puntos (descartando aquellos de intensidad inferior a 5)
	int contador=0;
	double media_borde=0;
	
	for(int i=0;i<w;++i)
		for(int j=0;j<h;++j){
			if(bordes(i,j)>5){ //fijo un pequeño umbral para no tener en cuenta todos los puntos
				media_borde+=bordes(i,j);
				contador++;
			}
		}
	
	media_borde/=double(contador);
	
	CImg<bool> bordesB=ImgBinaria(bordes,3*media_borde); 
	
	//  Y Binarizo la imagen de bordes...
//	bordesB.display("Bordes binarizada",0);
	
	CImg<double> hough=TransformadaHough(bordesB); //calculo la transformada de Hough
	
	//lo siguiente calcula el maximo acumulador de la transformada
	int max_acc=0,i_;
	for(int i=0;i<w;++i){
		for(int j=0;j<h;++j){
			if(max_acc<hough(i,j)){
				max_acc=hough(i,j);
				i_=i;
			}
		}
	}
	
	double pasoTheta=180/double(w-1);
	double angulo=i_*pasoTheta-90; // desplaza el eje a -90;90
	
	return angulo;
	
	/*
	//cout<<"Maximo acumulador: "<<max_acc<<endl;
	//lo siguiente calcula, para cada angulo de la transformada (eje theta), su peso, sumando todos los acumuladores que se pueda considerar que corresponden
	//a rectas en la imagen (se toma como criterio que su valor sea superior a la mitad del maximo acumulador)
	
	vector<angulo> angulos;
	double pasoTheta= 180/double(w-1);
	for(int i=0; i<w; ++i){
	angulo nuevo;
	nuevo.accu=0;
	nuevo.theta=i*pasoTheta - 90;
	for(int j=0; j<h; ++j){
	if(trans_hough(i,j)>max_acc*.5)
	nuevo.accu += trans_hough(i,j);
	}
	angulos.push_back(nuevo);
	}
	
	sort(angulos.begin(),angulos.end()); //ordeno el vector de angulos (segun su acumulador total)
	reverse(angulos.begin(),angulos.end()); //revierto el vector, para que los angulos de mayor acumulador queden adelante
	
	angulo ang=angulos.at(0);
	return ang.theta;
	*/
}