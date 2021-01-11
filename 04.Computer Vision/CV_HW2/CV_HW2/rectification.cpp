/******************************************
FILE
    retificationh.cpp
PURPOSE
    �w�q�p��N�ϧηh�����禡�w

NOTE
    1.
	 ����   z  ^   z'
	         /   \
       |    /     \
       |   /       \
       |  /         \
       --------------\epipole

	   �ھڳo�� �ڭ̥i�H���D Z' = RZ + T
       Z x Z' = Z x (RZ+T) = Z x T
	   cos = Z * Z' / |Z||Z'| 
	2. e = [ex ey ez] Fe = 0
	   d = [-ey ex 0] = [dx dy dz]
			[ dx^2 + (1-dx^2) * cos		dxdy(1-cos				dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]
AUTHOR
    9757553 �C����
*******************************************/

#include <cmath>
#include <iostream>
#include "ArrayManipulation.h"
#include "rectification.h"
#include "matlab.h"

using std::cout;

/*
 ���o���઺�x�}
 axis = d
 	        [ dx^2 + (1-dx^2) * cos		dxdy(1-cos	)			dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]
 �ǤJ�G����b ���ਤ�׭�
 �ǥX�G����x�}
*/
double* 
*getRotation3By3( const double* axis ,double theta ) {
	double* * r = create2DArray( 3 ,3 );
	/*double Sin = sqrt( 1 - cos(theta) * cos );*/
	
	r[0][0] = axis[0] * axis[0] + ( 1 - axis[0] * axis[0] ) * cos(theta);
	r[0][1] = axis[0] * axis[1] * ( 1 - cos(theta) );
	r[0][2] = axis[1] * sin(theta);
	r[1][0] = axis[0] * axis[1] * ( 1 - cos(theta) );
	r[1][1] = axis[1] * axis[1] + ( 1 - axis[1] * axis[1] ) * cos(theta);
	r[1][2] = -axis[0] * sin(theta);
	r[2][0] = - axis[1] * sin(theta);
	r[2][1] = axis[0] * sin(theta);
	r[2][2] = cos(theta);


	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ )
			cout << r[i][j]<<" ";
		cout<<"\n";
	}
	return r;
}
/*
  �ھڤ@�ӦV�q��X�L�Px�b�����èD�XRotation matrix 2
  �ǤJ�G�V�q
  �ǥX�Grotation matrix
*/
double*
*getRotation2(const double* v) {
	double theta = - atan( v[1] / v[0] );

	double* * r = create2DArray(3 ,3 );
	r[0][0] = cos(theta);
	r[0][1] = -sin(theta);
	r[1][0] = sin ( theta );
	r[1][1] = cos ( theta );
	r[2][2] = 1;

	return r;
}

/*
 �Nimage epipole line�ॿ
     ����   z  ^   z'
	   x     /   \
       |    /     \
       |   /       \
       |  /         \
       --------------\epipole

	   �ھڳo�� �ڭ̥i�H���D Z' = RZ + T
 �άO�ڭ̥��o�쫫���󥭭����V�q �b���n�o��x �M��H�����󥭭����V�q���x�b 
 �ॿ�x�}
	   e = [ex ey ez] Fe = 0
	   d = [-ey ex 0] = [dx dy dz]
			[ dx^2 + (1-dx^2) * cos		dxdy(1-cos				dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]

	   newe = R e
	         [ cosx -sinx 0 ]
		Rx = [ sinx  cosx 0 ]     x = - tan
			 [  0     0   1 ]
 �ǤJ�G
     �v�� 
	 fundamental matrix 
	 translation vector
	 rotation matrix
 �ǥX�G�ॿ���v��
*/
double* getEpipole(double* * f ,bool isL = true);
double getCos( const double* a ,const double* b ,int size); 
double* calTheOtherCP2CameraCoord( double* v ,double* * f ,bool isL );
double* crossProduct(const double* a ,const double* b ,int size );
mxArray* getH1H2(double* * f ,const double* t,bool isL );
Picture transformImg(const Picture& p ,double* * hinv);
vector<Picture> 
getRectifyImage( vector<Picture>& pics , double* * f ,const double* t ,double* * r ) {
	vector<Picture> p;
	
	/*�����ॿ*/
	cout<<"�����ॿ\n";
	/*���oepipole e �]�N�O ef = 0f ��������*/
	cout<<"�إ��ഫ�x�}H\n";
	mxArray* El = getH1H2(f ,t ,true );
	mxArray* Elinv = mlfInv(El);
	//mxArray* Elinv = mlfCtranspose(El);
	double* *elinv = _M1DDouble22D( mxGetPr(Elinv) ,3 ,3);
	cout<<"�ഫ�x�}�إߵ���\n";

	/*�ഫ*/
	/*cout<<"�ഫ���Ϥ�\n";
	Picture img = transformImg( pics[0] ,elinv );
	p.push_back(img);
	img.drawPicture("./L.jpg");
	cout << "�ഫ����\n";*/
	
	/*�k���ॿ*/
	cout << "�k���ॿ\n";
	cout<<"�إ��ഫ�x�}H\n";
	mxArray* Er = getH1H2(f ,t , false);
	//mxArray* R = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(r ,3 ,3) ,NULL);
	mxArray* Erinv = mlfInv(Er);
	//mxArray* Erinv = mlfCtranspose(Er);
	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	//���oF~ �H�K�DT
	mxArray* Fnew = mlfMtimes( mlfCtranspose(Erinv) , mlfMtimes(F ,Elinv ) );
	double* fnew = mxGetPr(Fnew);
	double* * fn = _M1DDouble22D(fnew ,3 ,3);
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ )
			cout<< fn[i][j]<<" ";
		cout<<"\n";
	}
	cout<<"========\n";
	/*
						   		  [ 0 0 0  ]
	  �� ((T * H)-1)t F H-1 = B = [ 0 0 -1 ]
								  [ 0 1 0  ]
		(T-1)t (H-1)t F H-1 = B
		(T-1)t      A       = B
		At (T-1) = Bt
		
	*/
	double* * y = create2DArray( 3 ,3 );
	//y[1][2] = -1; y[2][1] = 1;
	//mxArray* B = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(y ,3 ,3) ,NULL );
	//B = mlfMtimes( Fnew ,mlfCtranspose( B ) );//Fnew * B
	//Fnew = mlfMtimes(  Fnew ,mlfCtranspose(Fnew) );//Fnew * (Fnew)t
	//mxArray* Fninv = mlfInv(Fnew);

	//mxArray* T = mlfMtimes(Fninv ,B);//(Fnew * (Fnew)t)-1 * B
	//T = mlfCtranspose(T);
	//Erinv = mlfMtimes(Erinv ,T );
	y[0][0] = 1; y[1][1] = -fn[1][2]; y[1][2] = -fn[2][2];
	y[2][2] = fn[2][1];
	
	/*mxArray* T = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(y ,3 ,3) ,NULL );

	Er = mlfMtimes( T ,Er );
	double* *e = _M1DDouble22D( mxGetPr(Er) ,3 ,3);
	
	Erinv = mlfInv(Er);*/

	double* *erinv = _M1DDouble22D( mxGetPr(Erinv) ,3 ,3);
	//Erinv = mlfInv(Er);
	Picture img1 = transformImg( pics[1] ,erinv );
	img1.drawPicture("./R.jpg");
	p.push_back(img1);
	cout<<"�ഫ�x�}�إߵ���\n";

	mxDestroyArray(El);
	mxDestroyArray(F);
	mxDestroyArray(Elinv);
	mxDestroyArray(Er);
	mxDestroyArray(Erinv);
	mxDestroyArray(Fnew);

	dispose(elinv ,3 ,3);
	dispose(erinv ,3 ,3);
	delete y;
	//delete fn;

	return p;

}
/*
 ���oepipole ���׬�����
 �k�Ϫ������� F * e = 0
 ���Ϭ�e' * F = 0
 �ǤJ�Gfundamental matrix �H�άO�n�䥪���٬O�k�Ϫ�epipole
 �ǥX�Gepipole
*/
double* normalize(const double* a , int size);
double* getEpipole(double* * f ,bool isL) {

	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	if ( !isL ) F = mlfCtranspose(F);//�p�G�O�n��k��Epipole

	mxArray* E = mlfNull(F ,NULL);
	double* e = mxGetPr(E);
	e =  normalize(e , 3);

	//mxArray* V = NULL;
	//mxArray* P = mlfEig( &V ,F ,NULL ,NULL);//�ǤJ���Oeigen value �^�Ǫ��Oeigenvector
	//mxArray* DIM = mlfSize( NULL ,V ,NULL );
	//int d = (int)mxGetPr(DIM)[0];
	//double* value = mxGetPr(V);
	//double min = System::Double::MaxValue;
	//int index = 0;
	//for ( int i = 0; i < d * d; i += d )  {
	//	//cout << value[i];
	//	if ( value[i] == 0 ) index = i;
	//	i++;
	//}
	////��Xeigenvector
	//double*  e= new double [d];
	//for ( int i = 0; i < d; i++ ) {
	//	if ( index != i ) e[i] = 0;
	//	else e[i] = 1;
	//}
	//V = mlfDoubleMatrix(d, 1, e, NULL );
	//mxArray* E = mlfMtimes(P ,V );
	//e = mxGetPr(E);

	////����
	//e = normalize(e , d);
	///*double norm = 0;
	//for ( int i = 0; i < d; i++ )
	//	norm +=e[i] * e[i];
	//norm = sqrt( norm );
	//for ( int i = 0; i < d; i++ )
	//	e[i] /= norm;*/
	///*��Xd*/
	////�R���O����
	//mxDestroyArray(P);
	//mxDestroyArray(V);
	//mxDestroyArray(E);
	//mxDestroyArray(DIM);

	return e;
}
/*
  ���w��ӦV�q ���ocos
  �ǤJ�G��ӦV�q
  �ǥX�Gcos
*/
double getCos( const double* a ,const double* b ,int size) {
	double dot = 0;
	for ( int i = 0;i < size; i++ ) 
		dot += a[i] * b[i];

	double norma = 0 ,normb = 0;
	for ( int i = 0; i < size; i++ ) {
		norma += a[i];
		normb += b[i];
	}
	dot /= norma;
	dot /= normb;
	return dot;

}

/*
  �ھڬO�k��άO���� ��i�@��vector �p��۹�����vector
  �ǤJ�G�@�ӦV�q fundamental matrix �H�άO�b�����٬O�k��
  �ǥX�G�t�@�Ӭ۹������V�q
*/
double* calTheOtherCP2CameraCoord( double* v ,double* * f ,bool isL ) {
	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	/*  R*F* L = 0 */
	mxArray* A = NULL;
	if ( !isL ) {//�w��R �DA L = 0
		mxArray* R = mlfDoubleMatrix(1 ,3 ,v ,NULL );	
		
		A = mlfMtimes(R ,F );//��ker(A)
		mxDestroyArray(R);
	}
	else {//�w��L�D R A = 0
		mxArray* L = mlfDoubleMatrix(3 ,1 ,v ,NULL );
		mxArray* A = mlfMtimes(F ,L );
		A = mlfCtranspose(A);//�� ker (At)
		
		mxDestroyArray(L);
	}

	//����V�q
	mxArray* kerA = mlfNull(A ,NULL );//��null space
	double* rv = mxGetPr( kerA );
	

	double norm = 0;
	for ( int i = 0; i < 3; i++ )
		norm += rv[i] * rv[i];
	norm = sqrt(norm);

	for ( int i = 0; i < 3; i++ )
		rv[i] /= norm;

	
	mxDestroyArray(A);

	return rv;

}
/*
 ���oH�x�}
 �ǤJ�G matrix �H�ΥL�O�����٬O�k��
 �ǥX�GH matrix 
*/
mxArray* 
getH1H2(double* * f ,const double* t,bool isL ) {

	double* e = getEpipole(f ,isL);
	e = normalize(e , 3);
	//double* d = crossProduct(t ,e ,3);
	
	double* d = new double[3];
	d[0] = -e[1]; d[1] = e[0]; d[2] = 0;
	//double d[3] = { -e[1] ,e[0] ,0};
	d = normalize(d , 3);
	//double z[3] = { 0 ,0 ,1};
	//double* x = crossProduct(d ,z ,3);
	//double Cos = getCos(z ,x  ,3 );//���z�Mz'�b����cos��
	//double Tan = atan( (d[1] * e[0] - d[0] * e[1]) / e[2] );
	double theta = (3.141615926 / 2)  - atan( (d[1] * e[0] - d[0] * e[1]) / e[2] );
	double* * r1 = getRotation3By3(d ,theta );//�ڭ̱N�ন���� 
    mxArray* H2 = mlfDoubleMatrix(3 ,3 ,_2DDouble2M1D(r1 ,3 ,3) ,NULL);
	//double* exd = crossProduct(e ,d ,3);
	/*exd = normalize(exd , 3);

	double* * coordinate = create2DArray(3 ,3);
	for ( int i = 0; i < 3; i++ ) {
		coordinate[0][i] = e[i];
		coordinate[1][i] = d[i];
		coordinate[2][i] = exd[i];
	}
	
	cout<<"coordinate\n";
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ )
			cout <<coordinate[i][j] <<" ";
		cout<<"\n";
	}
	mxArray* H2 = mlfDoubleMatrix(3 ,3 ,_2DDouble2M1D(coordinate ,3 ,3) ,NULL);*/

	/*���oepipole��L�h�᪺�sepipole �H��X�U�@�ӯx�}*/
	mxArray* E0 = mlfDoubleMatrix( 3 ,1 ,e ,NULL);
	
	E0 = mlfMtimes(H2 ,E0 );// e' = R * e
	double* newe = mxGetPr(E0);
	
	r1 = getRotation2(newe);
	E0 = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(r1 ,3 ,3) ,NULL);
	H2 = mlfMtimes(E0 ,H2 );
	

	mxDestroyArray(E0);

	/*delete e;
	delete exd;
	delete d;*/

	return H2;
}
/*
 �Nvector���W��
 �ǤJ�G�@�ӭnnormalize��vector
 �ǥX�Gnormalize��vector
*/
double* normalize(const double* a ,int size) {
	double* d = new double[size];
	double norm = 0;
	for ( int i = 0; i < size; i++ )
		norm += a[i] * a[i];
	norm = sqrt(norm);

	for ( int i = 0; i < size; i++ )
		d[i] = a[i] / norm;

	return d;

}
/*
  ���w�ϩMHinv�i���ഫ
  �ഫ������
  �s�Ϫ��Ip' ������¹Ϫ���p = H-1 p'
  �ǤJ�G�n�ഫ��picture�H���ഫ�x�}
  �ǥX�Grectify�᪺��

*/
Picture 
transformImg(const Picture& p ,double* * hinv) {
	Picture img = p;

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ )
			cout << hinv[i][j]<<" ";
		cout<<"\n";
	}

	//�q���͹Ϸ�����^�h��
	int total = img.getHeight()*img.getWidth();
	for ( int y = 0; y < img.getHeight(); y++ ) {
		for ( int x = 0; x < img.getWidth(); x++ ) {
			int done = img.getWidth() * y + x + 1;
			printf("\r�w�ഫ�F %d %%" , 100 * done / total );
			double oldpt[3] = {0};
			for ( int i = 0; i < 3; i++ )
				oldpt[i] = hinv[i][0] * x + hinv[i][1] *  y + hinv[i][2] * 1;
			
			/*for ( int i = 0; i < 3; i++ )
				cout<< oldpt[i]<<" ";*/
			cout << "\t"<<oldpt[0] / oldpt[2] <<" " <<oldpt[1]/oldpt[2];
			try {
				//cout << x <<" "<<y<<"\n";
				Color c = p.getColor( (int) (oldpt[0]/oldpt[2]) ,(int) (oldpt[1]/oldpt[2]) );
				img.setColor( x ,y ,c );
			}catch (char* str ) {
				img.setColor(x ,y ,Color::Black);
			}

			//mxDestroyArray(P);
		}
	}

	return img;
}