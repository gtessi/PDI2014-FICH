
/// *******************************************************
///            METODOS DE ORDENAMIENTO
/// *******************************************************

template <typename T>
void merge(T *v, int i, int m, int f) {
	T *aux = new T[m-i+1];
	for(int j=i; j<=m; j++)
		aux[j-i] = v[j];
	
	int c1=0, c2=m+1;
	for(int j=i; j<=f; j++) {
		if(aux[c1] < v[c2]) {
			v[j] = aux[c1++];
			if(c1==m-i+1)
				for(int k=c2; k<=f; k++)
					v[++j] = v[k];
		}
		else {
			v[j] = v[c2++];
			if(c2==f+1)
				for(int k=c1; k<=m-i; k++)
					v[++j] = aux[k];
	}}	
}

template <typename T>
void mergesort(T *v, int i, int f) {
	if(i!=f) {
		int m = (i+f)/2;
		mergesort(v, i, m);
		mergesort(v, m+1, f);
		merge(v, i, m, f);
	}
}

/// *******************************************************
///                FUNCION DE VECINDAD
/// *******************************************************

template <typename T>
void vecindad(CImg<T> &img, T *&v, int x, int y, int m,int n){
	
	v = new T[n*m];
	int step_x = m /2.0;
	int step_y = n /2.0;
	
	for(int i=x-step_x;i<x+step_x+1;i++){
		for(int j=y-step_y;j<y+step_y+1;j++){
			v[(i - (x-step_x) )*n  + j - (y-step_y) ] = img(i,j);
		}
	}
}

template <typename T>
void vecindadRGB(CImg<T> &img, T **&v, int x, int y, int m,int n){
	
	delete[] v[2];
	
	v = new T *[n*m];
	for(int i=0;i<n*m;i++)
		v[i]= new T[3];
	
	int step_x = m /2.0;
	int step_y = n /2.0;
	for(int i=x-step_x;i<x+step_x+1;i++){
		for(int j=y-step_y;j<y+step_y+1;j++){
			v[(i - (x-step_x) )*n  + j - (y-step_y) ][0] = img(i,j,0);
			v[(i - (x-step_x) )*n  + j - (y-step_y) ][1] = img(i,j,1);
			v[(i - (x-step_x) )*n  + j - (y-step_y) ][2] = img(i,j,2);
		}
	}
}

template <typename T>
void vecindad(CImg<T> &img, T *&v, int x, int y){
	v= new T[9];
	v[0] = img(x-1,y-1);v[1] = img(x,y-1);v[2] = img(x+1,y-1);
	v[3] = img(x-1,y);v[4] = img(x,y);v[5] = img(x+1,y);
	v[6] = img(x-1,y+1);v[7] = img(x,y+1);v[8] = img(x+1,y+1);
}

/// *******************************************************
///                DISTANCIA RGB
/// *******************************************************

template <typename T>
T distanciaRGB(T **&v, T *&dist, int m, int n){
	
	double aux=0;
	for(int i=0;i<n*m;i++){
		for(int j=0;j<n*m;j++){
			aux+=pow(v[i][0]-v[j][0],2); aux+=pow(v[i][1]-v[j][1],2); aux+=pow(v[i][2]-v[j][2],2);}
		dist[i] = aux;
		aux=0;
	}
	return minimo(dist, m*n);
}


/// **************************************************************************************************************
/// **************************  FILTROS DE RESTAURACION  *********************************************************
/// **************************************************************************************************************


/// *******************************************************
///                FILTRO MEDIANA
/// *******************************************************

template <typename T>
void mediana(CImg<T> &img){
	CImg<T> img2(img),imgAux(img.get_channel(0));
	T *v = new T[9];
	int ancho = img.width(), alto = img.height();
	
	if (img.spectrum() == 3){
		for(int z=0;z<3;z++){
			imgAux= img.get_channel(z);
			for(int x=1;x<ancho-1;x++){
				for(int y=1;y<alto-1;y++){
					vecindad(imgAux, v, x,y);
					mergesort(v,0,8);
					img2(x,y) = v[4];
	}}}
	}else{
		for(int x=1;x<ancho-1;x++){
			for(int y=1;y<alto-1;y++){
				vecindad(img, v, x,y);
				mergesort(v,0,8);
				img2(x,y) = v[4];
	}}}
	img=img2;
}


template <typename T>
CImg<T> get_mediana(CImg<T> &img){
	CImg<T> aux(img),imgAux(img.get_channel(0));
	T *v = new T[9];
	int ancho = img.width(), alto = img.height();
	
	if (img.spectrum() == 3){
		for(int z=0;z<3;z++){
			imgAux= img.get_channel(z);
			for(int x=1;x<ancho-1;x++){
				for(int y=1;y<alto-1;y++){
					vecindad(img, v, x,y);
					mergesort(v,0,8);
					aux(x,y,z) = v[4];
	}}}
	}else{
		for(int x=1;x<ancho-1;x++){
			for(int y=1;y<alto-1;y++){
				vecindad(img, v, x,y);
				mergesort(v,0,8);
				aux(x,y) = v[4];
	}}}
	return aux;
}


/// *******************************************************
///                FILTRO MEDIA ARITMETICA
/// *******************************************************

template <typename T>
void mediaAritmetica(CImg<T> &img, int size){	
	CImg<double> mask(size,size,1,1,1);
	double factor = 1.0 / double(size);
	mask*= factor;
	img.convolve(mask);
}
	

/// *******************************************************
///                FILTRO MEDIA GEOMETRICA
/// *******************************************************

template <typename T>
void mediaGeometrica(CImg<T> &img, int size){	
	T aux=1, *v = new T[1];
	CImg<T> img2(img), imgAux(img.get_channel(0));
	int ancho = img.width(), alto = img.height();
	if (img.spectrum() == 3){
		for(int z=0;z<3;z++){
			imgAux= img.get_channel(z);
			for(int x=1;x<ancho-1;x++){
				for(int y=1;y<alto-1;y++){
					vecindad(imgAux,v,x,y,size,size);
					for(int i=0;i<size*size;i++)
						aux*=v[i];
					img2(x,y,z) = pow(aux,1.0/double(size*size));
					aux=0;
	}}}}
	
	else{
		for(int x=1;x<ancho-1;x++){
			for(int y=1;y<alto-1;y++){
			vecindad(img,v,x,y,size,size);
				for(int i=0;i<size*size;i++)
					aux*=v[i];
				img2(x,y) = pow(aux,1.0/double(size*size));
				aux=1;
	}}}
	img=img2;
}

/// *******************************************************
///                FILTRO DEL PUNTO MEDIO
/// *******************************************************

template <typename T>
void puntoMedio(CImg<T> &img){
	
	T *v = new T[9];
	int ancho = img.width(), alto = img.height();
	CImg<T> img2(img),imgAux(img.get_channel(0));
	
	if (img.spectrum() == 3){
		for(int z=0;z<3;z++){
			imgAux= img.get_channel(z);
			for(int x=1;x<ancho-1;x++){
				for(int y=1;y<alto-1;y++){
					vecindad(imgAux, v, x,y);
					mergesort(v,0,8);
					img2(x,y,z) = (v[0]+v[8])/2.0;
	}}}
	}else{
		for(int x=1;x<ancho-1;x++){
			for(int y=1;y<alto-1;y++){
				vecindad(img, v, x,y);
				mergesort(v,0,8);
				img2(x,y) = (v[0]+v[8])/2.0;
	}}}
	img=img2;
}


/// *******************************************************
///             FILTRO DE MEDIA ALPHA RECORTADO
/// *******************************************************

template <typename T>
void mediaAlpha(CImg<T> &img, int d){
	T *v = new T[9], aux = 0;
	int ancho = img.width(), alto = img.height();
	CImg<T> img2(img),imgAux(img.get_channel(0));
	if (img.spectrum() == 3){
		for(int z=0;z<3;z++){
			imgAux=img.get_channel(z);
			for(int x=1;x<ancho-1;x++){
				for(int y=1;y<alto-1;y++){
					vecindad(imgAux, v, x,y);
					mergesort(v,0,8);
					for (int i=d/2;i<9-d/2;i++)
						aux+=(v[i]/(9-d));
					img2(x,y,z) = aux;
					aux=0;
	}}}
	}else{
		for(int x=1;x<ancho-1;x++){
			for(int y=1;y<alto-1;y++){
				vecindad(img, v, x,y);
				mergesort(v,0,8);
				for (int i=d/2;i<9-d/2;i++)
					aux+=(v[i]/(9-d));
				img2(x,y) = aux;
				aux=0;
	}}}
	img=img2;
}


/// *******************************************************
///             FILTRO PASA BANDA
/// *******************************************************

template <typename T>
void filtroPasaBanda(CImg<T> &filtro, int r, int grosor){
	
	uc blanco[] = {1,0,0};
	uc negro[] = {0,0,0};
	int ancho = filtro.width(), alto = filtro.height();
	filtro.fill(0);
	filtro.draw_circle(ancho/2,alto/2,r+grosor/2,blanco,1);
	filtro.draw_circle(ancho/2,alto/2,r-grosor/2,negro,1);
}

/// *******************************************************
///             FILTRO RECHAZA BANDA
/// *******************************************************

template <typename T>
void filtroRechazaBanda(CImg<T> &filtro, int r, int grosor){
	CImg<T> aux(filtro);
	filtroPasaBanda(aux,r,grosor);
	filtro= 1 - aux;
}

/// *******************************************************
///             FILTRO PASA CRUZ
/// *******************************************************

template <typename T>
void filtroPasaCruz(CImg<T> &filtro, int x, int y, int grosor){
	uc blanco[] = {1,0,0};
	uc negro[] = {0,0,0};
	int ancho = filtro.width(), alto = filtro.height();
	filtro.fill(0);
	for(int i= -grosor/2;i<grosor/2+1;i++){
		filtro.draw_line(x+i,0,x+i,alto,blanco,1);
		filtro.draw_line(0,y+i,ancho,y+i,blanco,1);
		
		filtro.draw_line(ancho-(x+i),0,ancho-(x+i),alto,blanco,1);
		filtro.draw_line(0,alto-(y+i),ancho,alto-(y+i),blanco,1);
	}
}

/// *******************************************************
///             FILTRO RECHAZA CRUZ
/// *******************************************************

template <typename T>
void filtroRechazaCruz(CImg<T> &filtro, int x, int y, int grosor){
	CImg<T> aux(filtro);
	filtroPasaCruz(aux,x,y,grosor);
	filtro = 1 - aux;
}

/// *******************************************************
///             FILTRO PASA NOTCH
/// *******************************************************

template <typename T>
void filtroPasaNotch(CImg<T> &filtro, int x, int y, int r){
	uc blanco[] = {1,0,0};
	int ancho = filtro.width(), alto = filtro.height();
	filtro.fill(0);
	filtro.draw_circle(x,y,r,blanco,1);
	filtro.draw_circle(ancho-x,alto-y,r,blanco,1);
}

/// *******************************************************
///             FILTRO RECHAZA NOTCH
/// *******************************************************

template <typename T>
void filtroRechazaNotch(CImg<T> &filtro, int x, int y, int r){
	CImg<T> aux(filtro);
	filtroPasaNotch(aux,x,y,r);
	filtro = 1 - aux;
	
}


/// *******************************************************
///                DIVISION ENTRE LISTAS
/// *******************************************************

template <typename T>
CImg<T> divisionListas(CImgList<T> G, CImgList<T> H){
	
	CImgList<T> F(G);
	double e=0.001;
	cimg_forXY(G[0],x,y){
		
		if((H[1](x,y)*H[1](x,y) + H[0](x,y)*H[0](x,y))>e){
			/// PARA HACER LA DIVISION HAY QUE CAMBIAR LOS SIGNOS DEL DIVIDENDO (EL DE ARRIBA)
			F[0](x,y) = (G[0](x,y)*H[0](x,y) + G[1](x,y)*H[1](x,y)) / (H[1](x,y)*H[1](x,y) + H[0](x,y)*H[0](x,y));
			F[1](x,y) = (G[1](x,y)*H[0](x,y) - G[0](x,y)*H[1](x,y)) / (H[1](x,y)*H[1](x,y) + H[0](x,y)*H[0](x,y));
		}
	}
	return (F.get_FFT(true)[0]).get_normalize(0,255);
	
}

/// *******************************************************
///           MFILTRO DE DESPLAZAMIENTO CON A, B
/// *******************************************************


template <typename T>
void filtroDespla(CImgList <T> &H, double a, double b){
	
	std::complex<double> j(.0,1.0);
	H[0].fill(0);H[1].fill(0);
	
	unsigned int centro_x = H[0].width()/2;
	unsigned int centro_y = H[0].height()/2;
	std::complex<double> I(0.,1.);
	cimg_forXY(H[0],x,y){
		double u=x-centro_x;
		double v=y-centro_y;
		double coef =M_PI*(u*a +v*b);
		if(fabs(coef)> 0.00005){
			H[0](x,y)=((1.0/coef)* sin(coef)*real(exp(-I*coef)));
			H[1](x,y)=((1.0/coef)* sin(coef)*imag(exp(-I*coef)));
		}
		
	}
	H[0].shift(centro_x,centro_y,0,0,2);
	H[1].shift(centro_x,centro_y,0,0,2);
}

/// *******************************************************
///           FILTRO DE DISTANCIA EN COLOR RGB
/// *******************************************************

template <typename T>
void filtroDistRGB(CImg<T> &img, int m){
	
	if(img.spectrum()!=3)
		return;
	
	CImg<double> vecinos(m,m,1,3,0);
	
	int alto=img.height(), ancho= img.width();
	
	/// Definimos la matriz de vecindad y el vector de las distancias
	double **v, *dist, disMin = 0;
	dist = new double [m*m];
	v = new double *[m];
	for(int i=0;i<m;i++){
		v[i]=new double[m]; 
	}
	
	/// LIMPIAMOS LA IMAGEN
	int pos=0;
	for(int x=m/2;x<ancho-m/2;x++){
		for(int y=m/2;y<alto-m/2;y++){
			vecindadRGB(img,v,x,y,m,m);
			disMin=distanciaRGB(v,dist,m,m);
			pos=findPos(dist,disMin,m*m);
			img(x,y,0) = v[pos][0];
			img(x,y,1) = v[pos][1];
			img(x,y,2) = v[pos][2];
}}}



/// *******************************************************
///                MEDIDAS OBJETIVAS DE RESTAURACION
/// *******************************************************

template <typename T>
double eCuadratico(CImg<T> &img, CImg<T> &img2){
	double e=0;
	if (img.width() != img2.width() || img.height() != img2.height() || img.spectrum() != img2.spectrum())
		return -1;
	if(img.spectrum() == 3){
		cimg_forXYZ(img,x,y,z){
			e+=(pow(img(x,y,z) - img2(x,y,z),2));
		}
	}else{
		cimg_forXY(img,x,y){
			e+=(pow(img(x,y) - img2(x,y),2));
		}
	}
	
	e = e / double(img2.width() * img2.height());
	
	return e;
}





