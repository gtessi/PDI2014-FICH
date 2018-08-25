// Funciones

// Pasa de Real/Imaginario a Magnitud/Fase
CImgList<double> RItoMF(CImgList<double> RI){
	CImgList<double> MF(RI);
	
	// Calcula la magnitud y la fase
	cimg_forXY(RI[0],i,j){
		// Magnitud
		double magnitud=sqrt(pow(RI[0](i,j),2)+pow(RI[1](i,j),2));
		
		// Fase
		double fase=atan2(RI[1](i,j),RI[0](i,j));
		
		// Guarda los valores
		MF[0](i,j)=magnitud;
		MF[1](i,j)=fase;
	}
	
	// Devuelve la magnitud y la fase
	return MF;
}

// Pasa de Magnitud/Fase a Real/Imaginario
CImgList<double> MFtoRI(CImgList<double> MF){
	CImgList<double> RI(MF);
	
	// Constante imaginaria I=sqrt(-1):
	complex<double> I(0.0,1.0);
	
	// Calcula la parte real e imaginaria
	cimg_forXY(MF[0],i,j){
		// Parte real
		RI[0](i,j)=real(MF[0](i,j)*exp(I*MF[1](i,j)));
		
		// Parte imaginaria
		RI[1](i,j)=imag(MF[0](i,j)*exp(I*MF[1](i,j)));
	}
	
	// Devuelve la parte real e imaginaria
	return RI;
}
