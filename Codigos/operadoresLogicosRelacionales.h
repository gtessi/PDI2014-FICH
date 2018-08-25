// Operaciones logicas
// AND
CImg<bool> andImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) and img2(i,j);
	}
	
	return salida;
}

// OR
CImg<bool> orImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) or img2(i,j);
	}
	
	return salida;
}

// XOR
CImg<bool> xorImg(CImg<bool> img1,CImg<bool> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) xor img2(i,j);
	}
	
	return salida;
}

// NOT
CImg<bool> notImg(CImg<bool> img){
	CImg<bool> salida(img.width(),img.height());
	
	cimg_forXY(img,i,j){
		salida(i,j)=!img(i,j);
		//salida(i,j)=not(img(i,j));
	}
	
	return salida;
}
//----------------------------------------------------------------------
// Operaciones relacionales
// Menor <
CImg<bool> menorImg(CImg<double> img1,CImg<double> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) < img2(i,j);
	}
	
	return salida;
}

// Menor o igual <=
CImg<bool> menorIgualImg(CImg<double> img1,CImg<double> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) <= img2(i,j);
	}
	
	return salida;
}

// Mayor >=
CImg<bool> mayorImg(CImg<double> img1,CImg<double> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) > img2(i,j);
	}
	
	return salida;
}

// Mayor o igual >=
CImg<bool> mayorIgualImg(CImg<double> img1,CImg<double> img2){
	assert(img1.is_sameXY(img2));
	
	CImg<bool> salida(img1.width(),img1.height());
	
	cimg_forXY(img1,i,j){
		salida(i,j)=img1(i,j) >= img2(i,j);
	}
	
	return salida;
}

