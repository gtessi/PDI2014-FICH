


///********************************************************
///            DETECTOR DE BORDES ROBERTS
///********************************************************

template <typename T>
void Roberts(CImg<T> &img){
	CImg<T> Gx(3,3,1,1,0),Gy(3,3,1,1,0);
	Gx(1,1)=-1;Gx(2,2)=1;
	Gy(2,1)=1;Gy(1,2)=-1;
	CImg<T> imgGx = get_umbral255(img.get_convolve(Gx).normalize(0,255),128),
		imgGy = get_umbral255(img.get_convolve(Gy).normalize(0,255),128);
	
	img = get_umbral01((imgGx + imgGy)/2.0,0.5);
	
}





///********************************************************
///            DETECTOR DE BORDES PREWITT
///********************************************************

template <typename T>
void Prewitt(CImg<T> &img){
	CImg<T> Gx(3,3,1,1,0),Gy(3,3,1,1,0),Gxy(3,3,1,1,0),Gyx(3,3,1,1,0);
	/// Horizontal y vertical
	Gx(0,0)=-1;Gx(0,1)=-1;Gx(0,2)=-1;Gx(2,0)=1;Gx(2,1)=1;Gx(2,2)=1;
	Gy(0,0)=-1;Gy(1,0)=-1;Gy(2,0)=-1;Gy(0,2)=1;Gy(1,2)=1;Gy(2,2)=1;
	/// Diagonales
	Gxy(0,1)=-1;Gxy(0,2)=-1;Gxy(1,2)=-1;Gxy(1,0)=1;Gxy(2,0)=1;Gxy(2,1)=1;
	Gyx(0,0)=-1;Gyx(1,0)=-1;Gyx(0,1)=-1;Gyx(1,2)=1;Gyx(2,1)=1;Gyx(2,2)=1;
	
	CImg<T> imgGx = get_umbral255(img.get_convolve(Gx).normalize(0,255),128),
		imgGy = get_umbral255(img.get_convolve(Gy).normalize(0,255),128),
		imgGyx = get_umbral255(img.get_convolve(Gyx).normalize(0,255),128),
		imgGxy = get_umbral255(img.get_convolve(Gxy).normalize(0,255),128);
	
	img=get_umbral01((imgGx + imgGy + imgGyx + imgGxy)/4.0,0.25);
	
	
}

///********************************************************
///            DETECTOR DE BORDES SOBEL
///********************************************************

template <typename T>
void Sobel(CImg<T> &img){
	CImg<T> Gx(3,3,1,1,0),Gy(3,3,1,1,0),Gxy(3,3,1,1,0),Gyx(3,3,1,1,0);
	/// Horizontal y vertical
	Gx(0,0)=-1;Gx(0,1)=-2;Gx(0,2)=-1;Gx(2,0)=1;Gx(2,1)=2;Gx(2,2)=1;
	Gy(0,0)=-1;Gy(1,0)=-2;Gy(2,0)=-1;Gy(0,2)=1;Gy(1,2)=2;Gy(2,2)=1;
	/// Diagonales
	Gxy(0,1)=-1;Gxy(0,2)=-2;Gxy(1,2)=-1;Gxy(1,0)=1;Gxy(2,0)=2;Gxy(2,1)=1;
	Gyx(0,0)=-2;Gyx(1,0)=-1;Gyx(0,1)=-1;Gyx(1,2)=1;Gyx(2,1)=1;Gyx(2,2)=2;
	
	CImg<T> imgGx = img.get_convolve(Gx).normalize(0,255),
		imgGy = img.get_convolve(Gy).normalize(0,255),
		imgGyx = img.get_convolve(Gyx).normalize(0,255),
		imgGxy = img.get_convolve(Gxy).normalize(0,255);
	
	cimg_forXY(img,x,y){
		img(x,y)=sqrt(pow(imgGx(x,y),2)+pow(imgGy(x,y),2)+pow(imgGxy(x,y),2)+pow(imgGyx(x,y),2));
	}
	img.normalize(0,255);
}

/*
template <typename T>
void Sobel(CImg<T> &img){
	CImg<T> Gx(3,3,1,1,0),Gy(3,3,1,1,0),Gxy(3,3,1,1,0),Gyx(3,3,1,1,0);
	/// Horizontal y vertical
	Gx(0,0)=-1;Gx(0,1)=-2;Gx(0,2)=-1;Gx(2,0)=1;Gx(2,1)=2;Gx(2,2)=1;
	Gy(0,0)=-1;Gy(1,0)=-2;Gy(2,0)=-1;Gy(0,2)=1;Gy(1,2)=2;Gy(2,2)=1;
	/// Diagonales
	Gxy(0,1)=-1;Gxy(0,2)=-2;Gxy(1,2)=-1;Gxy(1,0)=1;Gxy(2,0)=2;Gxy(2,1)=1;
	Gyx(0,0)=-2;Gyx(1,0)=-1;Gyx(0,1)=-1;Gyx(1,2)=1;Gyx(2,1)=1;Gyx(2,2)=2;
	
	CImg<T> imgGx = get_umbral255(img.get_convolve(Gx).normalize(0,255),128),
		imgGy = get_umbral255(img.get_convolve(Gy).normalize(0,255),128),
		imgGyx = get_umbral255(img.get_convolve(Gyx).normalize(0,255),128),
		imgGxy = get_umbral255(img.get_convolve(Gxy).normalize(0,255),128);
	
	img=get_umbral01((imgGx + imgGy + imgGyx + imgGxy)/4.0,0.25);
	
}//*/


///********************************************************
///            DETECTOR DE BORDES LAPLACIANO
///********************************************************
template <typename T>
void Laplaciano4(CImg<T> &img){
	CImg<T> Mask(3,3,1,1,0);
	Mask(0,1)=-1;Mask(1,0)=-1;Mask(2,1)=-1;Mask(1,2)=-1;
	Mask(1,1)=4;
	img = get_umbral255(img.get_convolve(Mask).normalize(0,255),128);
}

template <typename T>
void Laplaciano8(CImg<T> &img){
	CImg<T> Mask(3,3,1,1,-1);
	Mask(1,1)=8;
	img = get_umbral255(img.get_convolve(Mask).normalize(0,255),128);	
}

///********************************************************
///            DETECTOR DE BORDES LOG
///********************************************************
template <typename T>
void LoG(CImg<T> &img, double sigma, int n){
	CImg<T> Mask(n,n,1,1,0);
	double r=0, s2 = pow(sigma,2), s4 = pow(sigma,4);
	cimg_forXY(Mask,x,y){
		r= pow((x-n/2),2) + pow((y-n/2),2);
		Mask(x,y) = -1 * (r - s2)/s4 * exp(-1* (r / (2*s2)));
	}
	img = get_umbral255(img.get_convolve(Mask).normalize(0,255),128);
	
	
}

///********************************************************
///       DETECTOR DE BORDES POR CRECIMIENTO DE REGIONES
///********************************************************
template <typename T>
void CreciRegiones(CImg<T> &img,T color,int x, int y, int umbral){
	
	if(x< 0 || x>img.width()-1 || y<0 || y >img.height()-1)
		return;
	
	if(fabs(img(x,y)-color)>umbral || img(x,y) == 300)
		return;
	T col = img(x,y);
	img(x,y) = 300;
	
	/// Horizontels y verticales
	CreciRegiones(img,col,x-1,y,umbral);
	CreciRegiones(img,col,x,y-1,umbral);
	CreciRegiones(img,col,x+1,y,umbral);
	CreciRegiones(img,col,x,y+1,umbral);
	
	/// Diagonales
	CreciRegiones(img,col,x-1,y-1,umbral);
	CreciRegiones(img,col,x+1,y+1,umbral);
	CreciRegiones(img,col,x+1,y-1,umbral);
	CreciRegiones(img,col,x-1,y+1,umbral);
}

///********************************************************
///          SEGMENTACION POR REGION DE COLOR
///********************************************************
template <typename T>
void SegColor(CImg<T> &img, T *Color, double r, int x, int y){
	
	if(x< 0 || x>img.width()-1 || y<0 || y >img.height()-1)
		return;
	if(img(x,y,0) == 300 && img(x,y,1) == 300 && img(x,y,2) == 300)
		return;
	
	T dist = sqrt(pow(img(x,y,0) - Color[0],2)+ pow(img(x,y,1) - Color[1],2) + pow(img(x,y,2) - Color[2],2));
	
	if(r<dist)
		return;
	
	/*
	Color[0] = img(x,y,0);
	Color[1] = img(x,y,1);
	Color[2] = img(x,y,2);//*/
	
	img(x,y,0) = 300;
	img(x,y,1) = 300;
	img(x,y,2) = 300;
	
	/// Horizontales y Verticales
	SegColor(img, Color, r, x+1, y);
	SegColor(img, Color, r, x, y+1);
	SegColor(img, Color, r, x-1, y);
	SegColor(img, Color, r, x, y-1);
	
	/// Diagonales
	SegColor(img, Color, r, x-1, y-1);
	SegColor(img, Color, r, x+1, y+1);
	SegColor(img, Color, r, x-1, y+1);
	SegColor(img, Color, r, x+1, y-1);
	
	
	
}

///********************************************************
///          VECINDAD POR CIMG
///********************************************************
template <typename T>
void vecindadCIMG(CImg<T> &img, CImg<T> &v, int x, int y, int n, int m){
	
	int x_stepmas = n/2,y_stepmas = m/2,
		x_stepmenos = n/2,y_stepmenos = m/2;
	/// modifico los stepmenos
	if(x-x_stepmenos < 0)
		x_stepmenos = x_stepmenos + (x-x_stepmenos);
	if(y-y_stepmenos < 0)
		y_stepmenos = y_stepmenos + (y-y_stepmenos);
	/// modifico los step mas
	if(x+x_stepmas > img.width()-1)
		x_stepmas = x_stepmas - (x+x_stepmas -(img.width() -1));
	if(y+y_stepmas > img.height()-1)
		y_stepmas = y_stepmas - (y+y_stepmas -(img.height() -1));
	v = CImg<T>(x_stepmas + x_stepmenos + 1,y_stepmas + y_stepmenos + 1,1,img.spectrum(),0);
	cimg_forXY(v,p,q){
		v(p,q) = img(p+(x-x_stepmenos),q+(y-y_stepmenos));}
}

///********************************************************
///          ETIQUETADO POR REGION, COLOR
///********************************************************
template <typename T>
int findv(vector<T> v, T val){
	int n = v.size();
	for(int i=0;i<n;i++){ if(val == v[i]) return i; }
	return -1;}

template <typename T>
void addRegion(vector< vector< T> > &ref, T val, T vecino){
	
	int n = ref.size(),i=0,j=0;
	for(;i<n;i++){
		if(findv(ref[i],val)+1)
			break;}
	for(;j<n;j++){
		if(findv(ref[j],vecino)+1)
			break;}
	if(i==j)
		return;
	if(i==n){
		ref[j].push_back(val);
	}else{
		int naux=ref[j].size();
		for(int l=0;l<naux;l++){
			ref[i].push_back(ref[j][l]);
		}
		ref.erase(ref.begin() + j);
	}
}

template <typename T>
CImg<T> SegRegion(CImg<T> &img, int mn, vector< vector<T> > &ref, T color[][3], int cantColor){
	
	if(img.spectrum() != 3)
		return CImg<T>();
	CImg<T> v;
	T region=2;
	bool flag = false;
	ref.clear();
	cimg_forXY(img,x,y){
		if(img(x,y)!=0){
			vecindadCIMG(img,v,x,y,mn,mn);
			v.sort(0);
			img(x,y)=region;
			int q=0,p=0;
			
			
			for(;v(p,q)==region && p<v.width();p++){
				for(;q<v.height();q++){
					if(v(p,q)==region)
						break;
				}
			}
			//cout<<x<<" "<<y<<" size "<<v.width()<<" "<<v.height()<<endl;
			//v.display();
			//cout<<"pq "<<p<<" "<<q<<endl;
			if(v(p,q)>1)
				addRegion(ref,region,v[1]);
			else
				ref.push_back(vector<T>(1,region));
			flag=true;
		}
		if(flag && img(x,y)==0){region++;flag=false;}
	}
	CImg<T> aux(img.width(),img.height(),1,1,0);
	
	for(int i=0;i<ref.size();i++){
		if(ref[i].size()<3){
			ref.erase(ref.begin() + i);
			i--;
		}
	}
	
	cimg_forXY(img,x,y){
		int n = ref.size(),i;
		if(img(x,y)>1){
			i=0;
			for(;i<n;i++){
				if(findv(ref[i],img(x,y))+1)
					break;}
			aux(x,y) = i+1;
			img(x,y,0) = color[i%cantColor][0];
			img(x,y,1) = color[i%cantColor][1];
			img(x,y,2) = color[i%cantColor][2];
		}
	}
	return aux;
}
//*/

///****************************************
/// Transformada Hough directa e inversa
///****************************************
CImg<double> THough(CImg<double> img, bool inverse=false) {
	
	CImg<double> iHough(img); iHough.fill(0.0);
	const unsigned M = img.width(),
		N = img.height();
	
	double max_rho = sqrt(float(pow(N-1,2)+pow(M-1,2))), //maximo valor posible de radio se da en la diagonal pcipal
		step_rho = 2.*max_rho/(N-1), //paso en eje rho (rho=[-max_rho , max_rho])
		step_theta = M_PI/(M-1),     //paso en eje theta (M_PI=pi) (theta=[-90,90])
		rho, theta;
	
	if (!inverse){ 
		int r;  // radio mapeado en los N pixeles
		cimg_forXY(img,y,x){
			if (img(y,x) > 0.5)
				for (int t=0; t<M; t++) { //calculo rho variando theta en todo el eje, con x e y fijo
					theta=t*step_theta-M_PI/2;  // mapea t en [0,M-1] a t en [-90,90]
					rho=x*cos(theta)+y*sin(theta); // calcula rho para cada theta
					r=floor((rho+max_rho)/step_rho+.5); // mapea r en [-max_rho , max_rho] a r en [0,N-1] el floor(r+.5) sirve para redondear
					iHough(t,r)+= 1;               // suma el acumulador     
				}
		} 
	}else{
		const double blanco[1] = {255.f};
		float x0, x1, y0, y1;
		cimg_forXY(img,t,r){
			if (img(t,r) > 0.5) {
				theta=t*step_theta-M_PI/2;   // mapea t en [0,M-1] a t en [-90,90]
				rho=r*step_rho-max_rho;      // mapea r en [0,N-1] a r en [-max_rho,max_rho]
				if (theta>-M_PI/2 && theta<M_PI/2){
					y0=0; y1=M-1;
					x0=rho/cos(theta);      // calcula y para y=0
					x1=rho/cos(theta)-(M-1)*tan(theta); // calcula y para y=M-1	  
				}else{
					x0=0; x1=N-1;
					y0=rho/sin(theta);      // calcula y para x=0
					y1=rho/sin(theta)-(N-1)/tan(theta); // calcula y para x=M-1
				}
				//cout<<endl<<"("<<t<<","<<r<<")->("<<theta<<","<<rho<<") "<<"("<<y0<<","<<x0<<")->("<<y1<<","<<x1<<")"<<endl;
				iHough.draw_line(y0,x0,y1,x1,blanco); // dibuja una línea de (0,y0) a (M-1,y1)
			}
		}
	}
	return iHough; 
}



template <typename T>
void THoughTitaRo(CImg<T> img, T &tita, T &ro, bool espacial=true){
	
	CImg<T> TH = THough(img);
	vector<int> titas,ros;
	double max = TH.max();
	int ancho = img.width(), alto=img.height();
	
	cimg_forXY(TH,x,y){
		if(max == TH(x,y)){
			titas.push_back(x); ros.push_back(y);}}
	
	for(int i=0;i<titas.size();i++){
		tita+=titas[i];
		ro+=ros[i];}
	
	tita/=double(titas.size());
	ro/=double(ros.size());
	
	if(!espacial){
		tita= (tita - ancho/2) *90.0/double(ancho/2);
		ro= (ro - alto/2) *(sqrt(2.0)*alto)/double(alto/2);}
}



