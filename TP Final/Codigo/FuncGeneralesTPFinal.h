
template<typename T>
void medDesvio(CImg<T> Histo,double &med,double &des, double porcentaje){
	int ancho = Histo.width(),i=0,menos=0,mas=0, posmenos=0, posmas=0;
	T pix=0,media=0,desvio=0,max=Histo.max();
	/// buscamos el maximo
	for(;i<ancho;i++){
		if(Histo(i,0)==max)
			break;}
	
	posmenos = posmas = mas = menos = i;
	posmenos --; menos --; posmas ++; mas ++;
	
	/// control para casos particulares
	if(posmenos<0)
		posmenos=ancho;
	posmas = posmas%ancho;
	
	/// calculamos el rango desde el maximo haciendolo circular
	while((Histo(posmenos,0)>max*porcentaje || Histo(posmenos,0)<Histo(posmenos+1,0))){
		posmenos --; menos --;
		if(posmenos<0)
			posmenos=ancho; }
	while((Histo(posmas,0)>max*porcentaje || Histo(posmas,0)<Histo(posmas-1,0))){
		posmas ++; mas ++;
		posmas = posmas%ancho;}

	med = (mas + menos) / 2.0;
	des= (mas - menos)/2.0;
	
	cout<<"medes "<<med<<", "<<des<<endl;
	
}

template<typename T>
void umbralHSI(CImg<T> &hsi,T mas, T menos){
	cimg_forXY(hsi,x,y){
		if(!(hsi(x,y,0)>mas && hsi(x,y,0)<menos)){
			hsi(x,y,0)=0; hsi(x,y,1)=0; hsi(x,y,2)=0;}
	}
}

template<typename T>
void umbralHSINOT(CImg<T> &hsi,T mas, T menos){
	cimg_forXY(hsi,x,y){
		if(!(hsi(x,y,0)<mas || hsi(x,y,0)>menos)){
			hsi(x,y,0)=0; hsi(x,y,1)=0; hsi(x,y,2)=0;}
	}
}

template<typename T>
void umbralHSICircular(CImg<T> &hsi,T mas, T menos){
	T mayor, menor;
	if(mas<0 || menos>360.0){
		if(mas<0){
			mayor = 360.0 + mas;
			menor = menos;
			cimg_forXY(hsi,x,y){
				if(!(hsi(x,y)>mayor || hsi(x,y) < menor)){
					hsi(x,y,0)=0; hsi(x,y,1)=0; hsi(x,y,2)=0;}
		}}else{
			mayor = mas;
			menor = menos - 360.0;
			cimg_forXY(hsi,x,y){
				if(!(hsi(x,y)>mayor || hsi(x,y) < menor)){
					hsi(x,y,0)=0; hsi(x,y,1)=0; hsi(x,y,2)=0;}
		}}}
	else  {umbralHSI(hsi,mas,menos);}
}


template<typename T>
void umbralMedDes1C(CImg<T> &ima,T mas, T menos){
	cimg_forXY(ima,x,y){
		if((ima(x,y)<mas || ima(x,y)>menos)){
			ima(x,y)=0;}
}}


/// *******************************************************
///             BINARISAMOS UNA IMAGEN
/// *******************************************************
template<typename T>
void binarise(CImg<T> &img){
	if(img.spectrum()==3){
		cimg_forXY(img,x,y){
			if(img(x,y,0)!=0 || img(x,y,1)!=0 || img(x,y,2)!=0)
			{img(x,y,0) = img(x,y,1) = img(x,y,2) = 1;}}
	}else{
		cimg_forXY(img,x,y){
			if(img(x,y)!=0)
				img(x,y) = 1;}
	}
	img.crop(0,0,0,0,img.width()-1,img.height()-1,0,0,0);
}
 


/// *******************************************************
///            GRISES A PARTIR DEL PROMEDIO RGB
/// *******************************************************

template<typename T>
void grisProm(CImg<T> &img){
	CImg<T> aux(img);
	img.crop(0,0,0,1,img.width()-1,img.height()-1,0,1,0);
	cimg_forXY(img,x,y){
		img(x,y) = (aux(x,y,0)+aux(x,y,1)+aux(x,y,2))/3.0;}
}

template<typename T>
CImg<T> get_grisProm(CImg<T> img){
	CImg<T> aux(img.get_crop(0,0,0,1,img.width()-1,img.height()-1,0,1,0));
	cimg_forXY(img,x,y){
		aux(x,y) = (img(x,y,0)+img(x,y,1)+img(x,y,2))/3.0;}
	return aux;
}

/// *******************************************************
///           Producto mask * IMG
/// *******************************************************
template<typename T>
CImg<T> maskPorImg(CImg<T> img,CImg<T> mask){
	if(img.width() != mask.width() || img.height() != mask.height())
		return CImg<T>();
	CImg<T> r(img);
	int spectrum = r.spectrum();
	cimg_forXY(img,x,y){
		for(int i=0;i<spectrum;i++) r(x,y,i) = img(x,y,i) * mask(x,y); }
	return r;
}


/// *******************************************************
///           Invertir img binari
/// *******************************************************
template<typename T>
void inverBimg(CImg<T> &b){
	cimg_forXY(b,x,y){b(x,y) = T(int(b(x,y) + 1)%2);}}

template<typename T>
CImg<T> get_inverBimg(CImg<T> &b){
	CImg<T> r(b);
	cimg_forXY(b,x,y){r(x,y) = T(int(b(x,y) + 1)%2);} return r;}

/// *******************************************************
///           ELIMINAR BORDES
/// *******************************************************
template<typename T>
void delBordes(CImg<T> &img){
	bool b = true;
	cimg_forXY(img,x,y){
		if(b && img(x,y)==1 && img(x+1,y)==1){
			img(x,y)=0;
			b=false;}
		if(!b && img(x+1,y)==0){
			img(x,y)=0;
			b=true;}
		if(img(x,y)==1 && img(x+1,y)==0 && img(x-1,y)==0) img(x,y)=0;
			}
	int xsize = img.width(), ysize = img.height();
	b=true;
	for(int i=0;i<xsize;i++){
	for(int j=0;j<ysize;j++){
		if(b && img(i,j)==1 && img(i,j+1)==1){
			img(i,j)=0;
			b=false;}
		if(!b && img(i,j+1)==0){
			img(i,j)=0;
			b=true;}
		if(img(i,j)==1 && img(i,j+1)==0 && img(i,j-1)==0) img(i,j)=0;
	}}
}


/// *******************************************************
///             PRODUCTO BINARIO
/// *******************************************************
template<typename T>
CImg<T> productoBin(CImg<T> bin,CImg<T> img){
	
	if(bin.width() != img.width() || bin.height()!=img.height()){
		cout<<"*********************** MULTIPLICACION INVALIDA *************************"<<endl;
		return CImg<T>();}
	int spectrum = img.spectrum();
	CImg<T> resu(img);
	cimg_forXY(img,x,y){
		for(int i=0;i<spectrum;i++)
			resu(x,y) = bin(x,y)*img(x,y); }
	return resu;
}
template<typename T>
CImg<T> productoBinInvertido(CImg<T> bin,CImg<T> img){
	if(bin.width() != img.width() || bin.height()!=img.height()){
		cout<<"*********************** MULTIPLICACION INVALIDA *************************"<<endl;
		return CImg<T>();}
	int spectrum = img.spectrum();
	CImg<T> resu(img);
	inverBimg(bin);
	cimg_forXY(img,x,y){
		for(int i=0;i<spectrum;i++)
			resu(x,y) = bin(x,y)*img(x,y); }
	return resu;
}




template<typename T>
CImg<T> get_canchaBinarisada(CImg<T> img){

	/// imagen HSI
	CImg<T> hsi = img.get_RGBtoHSI();

	//hsi.display();

	CImg<double> H = hsi.get_channel(0);
	CImg<double> HH = H.get_histogram(360,0,359);
	double Hmed,Hdes, porcentaje=0.06;

	/// Calculamos la media y el desvio del color que mas aparece (verde de la cancah)
	medDesvio(HH,Hmed,Hdes,porcentaje);
	
	/// Calculamos el umbral en HSI
	umbralHSI(hsi,Hmed-Hdes,Hmed+Hdes);

	/// Inicializamos la imagen para eliminar los tonos
	CImg<T> cancha = hsi.get_HSItoRGB();
	delTonos(cancha,15);
	
	/// Transformamos la imagen ya segmentada a escala de grises y binario
	grisProm(cancha);
	binarise(cancha);
	
	/// Aplicamos los erodes y dilates para sacarnos el ruido de la tribuna 
	/// y quedarnos solamente con la cancha y sus jugadores
	cancha.dilate(5);
	cancha.erode(30);
	cancha.dilate(70);
	cancha.erode(50);
	return cancha;
}

template<typename T>
CImg<T> nuevaUmbralizacion(CImg<T> img, double porcentaje = 0.05){
	double Hmed,Hdes;
	/// vamos a segmentar la cancha con un metodo raro pero complementario
	CImg<T> hsii = img.get_RGBtoHSI();
	cimg_forXY(hsii,x,y){
		hsii(x,y,0) = hsii(x,y,0)+100.0 *hsii(x,y,1)+100.0 *hsii(x,y,2); }
	hsii = hsii.get_channel(0);
	/// normalizamos y calculamos su histograma
	hsii.normalize(0,255);
	CImg<double> HhsiP = hsii.get_histogram(255,1,255);
	/// Calculamos la media y el desvio del color que mas aparece 
	/// (verde de la cancha expresado en escala de grises)
	medDesvio(HhsiP,Hmed,Hdes,porcentaje);
	//HhsiP.display_graph("histograma nuevaUmbralizacion");
	/// Calculamos el umbral en la escala de grises
	umbralMedDes1C(hsii,Hmed-Hdes,Hmed+Hdes);
	binarise(hsii);
	return hsii;
}


template<typename T>
CImgList<T> get_Equipos(CImg<T> canchaBinaria, CImg<T> img, double HArbitros = 60.0){
	double Hmed,Hdes, porcentaje;
	
	/// obtenemos la umbralizacion 
	CImg<double> hsii = nuevaUmbralizacion(img);
	hsii.dilate(3);
	
	CImg<double> JugadoresBin = productoBin(canchaBinaria,hsii);
	JugadoresBin = XOR(JugadoresBin, canchaBinaria);
	
	//JugadoresBin.display("jugadores bin");
	
	/// hacermo un erode para que detecte bien a los jugadores
	/// hacemos el producto con la imagen y la mascara de jugadores
	CImg<double> jugadoresColor = maskPorImg(img,JugadoresBin);
	
	
	/// Eliminamos los arbitros (los que mas joden son los amarillos)
	jugadoresColor.RGBtoHSI(); double uma = 15.0;
	umbralHSINOT(jugadoresColor,HArbitros - uma,HArbitros + uma );
	jugadoresColor.HSItoRGB();
	
	/// Segmentamos al primer equipo con la nueva umbralizacion
	CImg<double> equipo1 = nuevaUmbralizacion(jugadoresColor,0.2), equipo2(equipo1);
	equipo1.erode(2);
	equipo1.dilate(5);
	
	/// Segmentamos al segundo equipo eliminando al primer equipo y 
	/// usando nuevaente la nueva umbralizacion
	inverBimg(equipo2);
	equipo2 = nuevaUmbralizacion(maskPorImg(jugadoresColor,equipo2),0.2);
	
	/// engordamos los juegadores
	equipo2.erode(2);
	equipo2.dilate(5);
	
	
	return CImgList<T>(maskPorImg(jugadoresColor,equipo1),maskPorImg(jugadoresColor,equipo2));
	
	/*
	
	/// obtenemos el hsi de los jugadores para segmentar los equipos
	CImg<double> Jhsi = jugadoresColor.get_RGBtoHSI();

	/// Separamos los diferentes canales de la imagen y segmentar al equipo1
	CImg<double> jh = Jhsi.get_crop(0,0,0,0,Jhsi.width()-1,Jhsi.height()-1,0,0,0),
		js = Jhsi.get_crop(0,0,0,1,Jhsi.width()-1,Jhsi.height()-1,0,1,0),
		ji = Jhsi.get_crop(0,0,0,2,Jhsi.width()-1,Jhsi.height()-1,0,2,0);//.get_histogram(360,0,359);
	
	/// calculamos el histograma del canal H para segmentar al equipo1
	CImg<double> jhh = jh.get_histogram(360,1,358), jhhh(jhh);
	
	/// Calculamos la media y el desvio
	porcentaje = 0.35;
	medDesvio(jhh,Hmed,Hdes,porcentaje);
	
	
	CImg<double> equipo1(Jhsi),equipo2(Jhsi);
	
	/// umbralizamos al equipo 1
	umbralHSICircular(equipo1,Hmed-Hdes,Hmed+Hdes);
	
	/// Procesamos al equipo 1
	grisProm(equipo1);  binarise(equipo1);
	//equipo1.display("equipo1 1");
	//equipo1.erode(2);
	equipo1.dilate(5);
	//equipo1.display("equipo1 1");
	/// Producto binario umbralizado para obtener al equipo2
	equipo2 = productoBinInvertido(equipo1,Jhsi);
	
	//equipo2.display("equipo 2");
	
	/// Separamos los diferentes canales de la imagen sin el equipo1
	CImg<double> jh2 = equipo2.get_crop(0,0,0,0,equipo2.width()-1,equipo2.height()-1,0,0,0),
		js2 = equipo2.get_crop(0,0,0,1,equipo2.width()-1,equipo2.height()-1,0,1,0),
		ji2 = equipo2.get_crop(0,0,0,2,equipo2.width()-1,equipo2.height()-1,0,2,0);
	
	/// calculamos el histograma del canal H para segmentar al equipo2
	CImg<double> jhh2 = jh2.get_histogram(360,1,358);
	
	/// Calculamos la media y el desvio
	porcentaje = 0.3;
	medDesvio(jhh2,Hmed,Hdes,porcentaje);
	
	//jhh2.display_graph();
	
	/// umbralizamos al equipo2
	umbralHSICircular(equipo2,Hmed-Hdes,Hmed+Hdes);
	
	/// Procesamos al equipo 1
	grisProm(equipo2);  binarise(equipo2);
	equipo2.erode(2);
	equipo2.dilate(7);
	
	
	/// Enmascaramos a los jugadores de ambos equipos
	CImg<double> ColorE1 = maskPorImg(img,equipo1),
		ColorE2 = maskPorImg(img,equipo2);
	
	return CImgList<T>(ColorE1,ColorE2);*/
}




template<typename T>
void get_limites(CImg<T> Hori, T &ro1, T &ro2){
	/// hacemos la transformada de hough
	CImg<T> H = THough(Hori);
	int mid = Hori.width()/2, alto = Hori.height();
	double val1, val2;
	
	/// inicializamos los valores maximos
	if(H(mid,0) <H(mid,1)){
		val1 = H(mid,1); ro1 = 1;
		val2 = H(mid,0); ro2 = 0;
	}else{
		val2 = H(mid,1); ro2 = 1; 
		val1 = H(mid,0); ro1 = 0;
	}
	/// buscamos los valores maximos en el medio de la imagen (teta = 0)
	for(int i=2;i<alto;i++){
		if(H(mid,i) > val1){
			val2 = val1;     ro2=ro1; 
			val1 = H(mid,i); ro1=i;}
		if(H(mid,i) < val1 && H(mid,i) > val2){
			ro2=i ; val2=H(mid,i);}}
	/// en las posiciones maximas de los ro ponemos valor 1
	H.fill(0);
	H(mid,ro1) = 1;
	H(mid,ro2) = 1;
	
	ro1 = ro2 = 0;
	/// hacemos la antitransformada para tener las dos rectas horizontales
	H = THough(H,true);
	
	/// buscamos los valores de ro, posiciones en y de la imagen
	for(int i=0;i<alto;i++){
		if(H(mid, i) == 255.0 && !ro1)
			ro1 = i;
		if(H(mid, i) == 255.0 && ro1)
			ro2 = i;
	}
}

template<typename T>
void get_emesYequis(CImg<T> &Vert,T &m1,T &m2, T &x1, T &x2){
	
	CImg<T> TH = THough(Vert), lineas(TH);
	lineas.fill(0);
	double max = TH.max(), tita1,tita2;
	int ro=0, ancho = TH.width();
	/// Buscamos el maximo
	cimg_forXY(TH,x,y){
		if(max == TH(x,y)){
			tita1 = x; ro=y;break;}}
	lineas(tita1,ro) = 1;
	/// borramos el maximo y sus valores cercanos
	umbraMaximoZona(TH,max,tita1,ro,0.20);
	/// calculamos el valor de tita en grados
	tita1 = (tita1 - ancho/2) *90.0/double(ancho/2);
	
	/// buscamos el x1 para la linea con m1
	lineas = THough(lineas,true); x1 = 0; int i=0;
	while(!x1){
		if(lineas(i,0) == 255)
			x1=i;
		i++;}
	lineas.fill(0);
	
	/// Buscamos el nuevo maximo
	max = TH.max();
	cimg_forXY(TH,x,y){
		if(max == TH(x,y)){
			tita2 = x;ro=y;break;}}
	lineas(tita2,ro) = 1;
	/// obtenemos las dos lineas haciendo la antitransformada
	lineas = THough(lineas,true); x2 = 0;
	
	/// inicializamos las variables y buscamos las lineas en 
	i=0;
	while(!x2){
		if(lineas(i,0) == 255)
			x2=i;
		i++;}
	
	/// calculamos el tita en grados
	tita2= (tita2 - ancho/2) *90.0/double(ancho/2);
	
	/// calculamos los m utilizando los titas 
	double dy = sin(tita1 * M_PI / 180.0 ), dx = cos(tita1 * M_PI / 180.0  );
	m1 = dy / dx ;
	dy = sin(tita2 * M_PI / 180.0 ); dx = cos(tita2 * M_PI / 180.0  );
	m2 = dy / dx ;
}


template<typename T>
void drawLineOffsideJugador(CImg<T> &img, T m1, T m2, T x1, T x2, T y0, T y1, int px, int py, T color[] = {255,255,255}){
	
	T nx1 =  x1 - py/m1,
		nx2 = x2 - py/m2, m;
	
	/// Inicializamos las variables para la interpolacion lineal
	T a1 = (nx2 - px)/(nx2 - nx1), a2 = (px-nx1)/(nx2 - nx1);
	
	/// inicializamos las variables 
	T nx00 =  x1 - y0/m1,
		nx01 = x2 - y0/m2,
		nx10 =  x1 - y1/m1,
		nx11 = x2 - y1/m2;
	/// hacemos la interpolacion lineal
	T 	xx0 = a1*nx00 + a2*nx01,
		xx1 = a1*nx10 + a2*nx11;
	
	/// Dibujamos la recta de offside del jugador
	img.draw_line(xx0+1,y0,xx1+1,y1,color,1);
	img.draw_line(xx0,y0,xx1,y1,color,1);
	img.draw_line(xx0-1,y0,xx1-1,y1,color,1);
}

template<typename T>
void ultimoJugador(CImg<T> equipo, int &x, int &y, bool derecha=false){
	int ancho = equipo.width(), alto = equipo.height();
	cout<<ancho<<", "<<alto<<endl;
	if(derecha){
		for(x=ancho-2;x>=0;x--){
			for(y=0;y<alto;y++){
				if(equipo(x,y) != 0)
					return;
	}}}else{
		for(x=0;x<ancho;x++){
			for(y=0;y<alto;y++){
				if(equipo(x,y) != 0)
					return;
	}}}
}


template<typename T>
bool offside(CImg<T> lineas, T *const color_def, bool derecha=false){
	int alto = lineas.height()/2, ancho= lineas.width();
	if(derecha){
		for(int i=ancho-1;i>0;i++){
			if( lineas(i,alto,0)!= 0 || lineas(i,alto,1)!= 0 || lineas(i,alto,2)!= 0){
				if(color_def[0] == lineas(i,alto,0) && color_def[1] == lineas(i,alto,1) && color_def[2] == lineas(i,alto,2))
					return false;
				else 
					return true;
		}}
	}else{
		for(int i=0;i<ancho;i++){
			if( lineas(i,alto,0)!= 0 || lineas(i,alto,1)!= 0 || lineas(i,alto,2)!= 0){
				if(color_def[0] == lineas(i,alto,0) && color_def[1] == lineas(i,alto,1) && color_def[2] == lineas(i,alto,2))
					return false;
				else 
					return true; 
		}}
	}
	return false;
}


///***************************************************************************************************
///*****************************                 MARCOS                 ******************************
///***************************************************************************************************




CImg<double> mascara_elpise(CImg<double> elipse, int x0, int y0, double r1_a, double r1_b, double r2, double angulo){
	unsigned char color[]={0,0,0};
	CImg<double> img_inferior(elipse.get_crop(0,y0,elipse.width()-1,elipse.height()-1));
	CImg<double> img_superior(elipse.get_crop(0,0,elipse.width()-1,y0));
	
	//Dibujo la misma elipse con r1_a pero de color negro.
	img_inferior.draw_ellipse(x0,0,r1_a,r2,angulo,color,1,1);
	
	//Dibujo la misma elipse con r2_a pero de color negro.
	img_superior.draw_ellipse(x0,y0,r1_b,r2,angulo,color,1,1);
//	(img_inferior,img_superior).display("");
	
	
	int ancho=img_inferior.width();
	int alto=img_inferior.height()+img_inferior.height();
	
	//Concateno las dos imagenes, img_superior e img_inferior:
	CImg<double> resultado(ancho,alto,1,1);
	resultado.fill(0);
	
	
	CImg<double> result(img_superior.width(), img_superior.height()+img_inferior.height()-1,1,1);
	for(int i=0; i<img_superior.width(); i++){
		for(int j=0; j<img_superior.height(); j++){
			result(i,j,0)=img_superior(i,j);					
		}
	}
	
	for(int i=0; i<img_inferior.width(); i++){
		for(int j=0; j<img_inferior.height(); j++){
			result(i,j+y0,0)=img_inferior(i,j);					
		}
	}
	return result;
}


void clic_mouse(CImg<double> img, int &semilla_x, int &semilla_y){
	//Evento del Mouse
	CImgDisplay ventana(img, "Seleccionar Posición de la pelota");
	while(!ventana.is_closed()){
//		cargo las coordenadas del primer click del mouse
		ventana.wait();
		if(ventana.button() == 1){
			semilla_x = ventana.mouse_x();
			semilla_y = ventana.mouse_y();
			cout<<"Posicion seleccionada: "<<"["<<semilla_x<<","<<semilla_y<<"]"<<endl;
			cout<<"Puede cerrar la vantana"<<endl<<endl;
		}		
	}	
}

CImg<double> tirolibre(CImg<double> img, CImg<double> EquipoDef){
	CImg<double> elipse(img);
	elipse.channel(0);
	elipse.fill(0);
	
	unsigned char color_elipse[]={1,1,1};
	int x0=760; int y0=200;
	
	clic_mouse(img,x0,y0);	
	
	double r1_a=75; double r1_b=76; double r2=162;
	double angulo=90;
	elipse.draw_ellipse(x0,y0,r1_a,r2,angulo,color_elipse);
	elipse.draw_ellipse(x0,y0,r1_b,r2,angulo,color_elipse);
	
	elipse=mascara_elpise(elipse,x0,y0,r1_a,r1_b,r2,angulo);
	
	
	//MASCARA DEFENSIVOS
	//Equipos[0].display("");//Equipo en Ataque
	//Equipos[1].display("");//Equipo en Defensa
	
	CImg<double> equipoINV = maskPorImg(EquipoDef,elipse);
	//equipoINV.display("equipoINV");
	
	//Verifico si dentro de la elipse existe algun jugador en DEFENSA
	int contar_pixel=0;
	cimg_forXY(equipoINV,i,j){
		if(equipoINV(i,j)!= 0){
			contar_pixel++;}
	}
	
	if(contar_pixel>5){
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-------EXISTE INVASION EN EL AREA DEL TIRO LIBRE------"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}else{
		cout<<"------------------------------------------------------"<<endl;
		cout<<"-- /NO/ EXISTE INVASION EN EL AREA DEL TIRO LIBRE-----"<<endl;
		cout<<"------------------------------------------------------"<<endl;
	}
	
	
	
	
	// Aplico el circulo sobre la imagen
	CImg<double> concirculoHSI(img.get_RGBtoHSI());
	
	//Aplico el Circulo sobre la imagen
	cimg_forXY(concirculoHSI,i,j){
		if(elipse(i,j)!=0){
			concirculoHSI(i,j,2)=0.7*concirculoHSI(i,j,2);
	}}
	
	return concirculoHSI.get_HSItoRGB();
	
}
