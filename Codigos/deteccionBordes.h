// Operadores de gradiente cruzado de Roberts
CImgList<double> operadorRoberts(){
	CImgList<double> G(2,2,2,1,1,0);
	
	// Gragiente en x (Gx)
	G[0](0,0)=-1;
	G[0](1,1)=1;
	
	// Gragiente en y (Gy)
	G[1](0,1)=-1;
	G[1](1,0)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Prewitt
CImgList<double> operadorPrewitt(){
	CImgList<double> G(4,3,3,1,1,0);
	
	// Gradiente horizontal
	G[0](0,0)=-1;
	G[0](0,1)=-1;
	G[0](0,2)=-1;
	G[0](2,0)=1;
	G[0](2,1)=1;
	G[0](2,2)=1;
	
	// Gradiente vertical
	G[1](0,0)=-1;
	G[1](1,0)=-1;
	G[1](2,0)=-1;
	G[1](0,2)=1;
	G[1](1,2)=1;
	G[1](2,2)=1;
	
	// Gradientes a 45º
	G[2](1,0)=-1;
	G[2](2,0)=-1;
	G[2](2,1)=-1;
	G[2](0,1)=1;
	G[2](0,2)=1;
	G[2](1,2)=1;
	
	G[3](1,0)=-1;
	G[3](0,0)=-1;
	G[3](0,1)=-1;
	G[3](1,2)=1;
	G[3](2,2)=1;
	G[3](2,1)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Sobel
CImgList<double> operadorSobel(){
	CImgList<double> G(4,3,3,1,1,0);
	
	// Gradiente horizontal
	G[0](0,0)=-1;
	G[0](0,1)=-2;
	G[0](0,2)=-1;
	G[0](2,0)=1;
	G[0](2,1)=2;
	G[0](2,2)=1;
	
	// Gradiente vertical
	G[1](0,0)=-1;
	G[1](1,0)=-2;
	G[1](2,0)=-1;
	G[1](0,2)=1;
	G[1](1,2)=2;
	G[1](2,2)=1;
	
	// Gradientes a 45º
	G[2](1,0)=-1;
	G[2](2,0)=-2;
	G[2](2,1)=-1;
	G[2](0,1)=1;
	G[2](0,2)=2;
	G[2](1,2)=1;
	
	G[3](1,0)=-1;
	G[3](0,0)=-2;
	G[3](0,1)=-1;
	G[3](1,2)=1;
	G[3](2,2)=2;
	G[3](2,1)=1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano 4
CImgList<double> operadorLaplaciano4(){
	CImgList<double> G(1,3,3,1,1,0);
	
	G[0](0,1)=-1;
	G[0](1,0)=-1;
	G[0](1,1)=4;
	G[0](1,2)=-1;
	G[0](2,1)=-1;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano 8
CImgList<double> operadorLaplaciano8(){
	CImgList<double> G(1,3,3,1,1,-1);
	
	// Rellena todo con -1, centro en 8
	G[0](1,1)=8;
	
	// Devuelve el operador
	return G;
}

// Operadores de Laplaciano del Gaussiano (LoG)
CImgList<double> operadorLoG(){
	CImgList<double> G(1,5,5,1,1,0);
	
	G[0](0,2)=-1;
	
	G[0](1,1)=-1;
	G[0](1,2)=-2;
	G[0](1,3)=-1;
	
	G[0](2,0)=-1;
	G[0](2,1)=-2;
	G[0](2,2)=16;
	G[0](2,3)=-2;
	G[0](2,4)=-1;
	
	G[0](2,1)=-1;
	G[0](2,2)=-2;
	G[0](2,3)=-1;
	
	G[0](3,2)=-1;
	
	// Devuelve el operador
	return G;
}

// Binariza una lista de imagenes a partir de un umbral
CImgList<bool> aplicarUmbralLista(CImgList<double> lista,double umbral){
	CImgList<bool> salida;
	
	// Recorre la lista de imagenes
	for(unsigned int n=0;n<lista.size();n++){
		CImg<bool> temp(lista[n].width(),lista[n].height(),lista[n].depth(),lista[n].spectrum(),false);
		
		// Recorre la imagen a binarizar
		cimg_forXY(lista[n],i,j){
			if(fabs(lista[n](i,j))>umbral)
				temp(i,j)=true;
		}
		
		// Guarda la imagen binarizada
		salida.push_back(temp);
	}
	
	// Devuelve la lista
	return salida;
}

// Calcula la derivada a partir de un operador
// Tipo de operador
// 0: Gradiente de Roberts
// 1: Gradiente de Prewitt
// 2: Gradiente de Sobel
// 3: Laplaciano de 4 vecinos
// 4: Laplaciano de 8 vecinos
// 5: Laplaciano del Gaussiano (LoG)
CImgList<double> derivada(CImg<double> img,unsigned int opcion){
	CImgList<double> salida;
	
	CImgList<double> operador;
	
	// Crea el operador segun la opcion elegida
	switch(opcion){
	// Gradiente de Roberts
	case 0:
		operador=operadorRoberts();
		break;
	// Gradiente de Prewitt
	case 1:
		operador=operadorPrewitt();
		break;
	// Gradiente de Sobel
	case 2:
		operador=operadorSobel();
		break;
	// Laplaciano de 4 vecinos
	case 3:
		operador=operadorLaplaciano4();
		break;
	// Laplaciano de 8 vecinos
	case 4:
		operador=operadorLaplaciano8();
		break;
	// Laplaciano del Gaussiano (LoG)
	case 5:
		operador=operadorLoG();
		break;
	}
	
	// Calcula la convolucion entre la imagen y los operadores
	for(unsigned int n=0;n<operador.size();n++)
		salida.push_back(img.get_convolve(operador[n]));
	
	// Devuelve la derivada de la imagen
	return salida;
}

