#include <iostream>
#include <complex>
#include "CImg.h"

using namespace cimg_library;
using namespace std;


int main(int argc, char** argv) {
	
	unsigned int n=5;  //tamaño mascara
	CImg<double> img("clown.jpg"),mask(n,n,1,1);
	
	unsigned int A=img.width(), B=img.height();
	CImg<double> img1(A,B);
	
	// filtrado espacial
	mask.fill(1.0);
	mask=mask/(n*n);
	
	img1=img.get_convolve(mask);  //imagen suavizada
	
	(img,mask,img1).display();
	
	// filtrado frecuencial
	unsigned int nuevoW=A+n-1, nuevoH=B+n-1;
	
	CImg<double> img_new=img.resize(nuevoW,nuevoH,-100,-100,0);
	CImg<double> mask_new=mask.resize(nuevoW,nuevoH,-100,-100,0);
	
	CImgList<double> H=mask_new.get_FFT(), TDF_img=img_new.get_FFT();
	//CImg <double> rreal(nuevoW,nuevoH),rimag(nuevoW,nuevoH);
	
	
	CImgList<double> temp(2,img_new.width(),img_new.height(),img_new.depth(),img_new.spectrum(),0);
	
	cimg_forXY(img,i,j){
		complex<double> factor1(TDF_img[0](i,j), TDF_img[1](i,j));
		complex<double> factor2(H[0](i,j),H[1](i,j));
		complex<double> prod = factor1*factor2;
		temp[0](i,j)=real(prod);
		temp[1](i,j)=imag(prod);
////		temp[0](i,j)=prod.real();  // sobreescribo con el nuevo producto
////		temp[1](i,j)=prod.imag();
	}
	
	CImgList<double> itdf_final=temp.get_FFT(true);
	//itdf_final[0].shift((nuevoW/2)-(A/2),(nuevoH/2)-(B/2));
	
	(img,img1,itdf_final[0]).display();
	
	return 0;
}
