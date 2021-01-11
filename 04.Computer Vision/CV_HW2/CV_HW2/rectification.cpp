/******************************************
FILE
    retificationh.cpp
PURPOSE
    定義如何將圖形搬正的函式庫

NOTE
    1.
	 欲轉   z  ^   z'
	         /   \
       |    /     \
       |   /       \
       |  /         \
       --------------\epipole

	   根據這樣 我們可以知道 Z' = RZ + T
       Z x Z' = Z x (RZ+T) = Z x T
	   cos = Z * Z' / |Z||Z'| 
	2. e = [ex ey ez] Fe = 0
	   d = [-ey ex 0] = [dx dy dz]
			[ dx^2 + (1-dx^2) * cos		dxdy(1-cos				dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]
AUTHOR
    9757553 顏志晟
*******************************************/

#include <cmath>
#include <iostream>
#include "ArrayManipulation.h"
#include "rectification.h"
#include "matlab.h"

using std::cout;

/*
 取得旋轉的矩陣
 axis = d
 	        [ dx^2 + (1-dx^2) * cos		dxdy(1-cos	)			dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]
 傳入：旋轉軸 旋轉角度值
 傳出：旋轉矩陣
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
  根據一個向量找出他與x軸夾角並求出Rotation matrix 2
  傳入：向量
  傳出：rotation matrix
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
 將image epipole line轉正
     欲轉   z  ^   z'
	   x     /   \
       |    /     \
       |   /       \
       |  /         \
       --------------\epipole

	   根據這樣 我們可以知道 Z' = RZ + T
 或是我們先得到垂直於平面的向量 在內積得到x 然後以垂直於平面的向量轉到x軸 
 轉正矩陣
	   e = [ex ey ez] Fe = 0
	   d = [-ey ex 0] = [dx dy dz]
			[ dx^2 + (1-dx^2) * cos		dxdy(1-cos				dysin  ]
	   R =  [ dxdy(1-cos)				dx^2 + ( 1-dy^2)cos		-dxsin ]
			[ -dysin					dxsin					cos	   ]

	   newe = R e
	         [ cosx -sinx 0 ]
		Rx = [ sinx  cosx 0 ]     x = - tan
			 [  0     0   1 ]
 傳入：
     影像 
	 fundamental matrix 
	 translation vector
	 rotation matrix
 傳出：轉正的影像
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
	
	/*左圖轉正*/
	cout<<"左圖轉正\n";
	/*取得epipole e 也就是 ef = 0f 之單位長度*/
	cout<<"建立轉換矩陣H\n";
	mxArray* El = getH1H2(f ,t ,true );
	mxArray* Elinv = mlfInv(El);
	//mxArray* Elinv = mlfCtranspose(El);
	double* *elinv = _M1DDouble22D( mxGetPr(Elinv) ,3 ,3);
	cout<<"轉換矩陣建立結束\n";

	/*轉換*/
	/*cout<<"轉換左圖中\n";
	Picture img = transformImg( pics[0] ,elinv );
	p.push_back(img);
	img.drawPicture("./L.jpg");
	cout << "轉換結束\n";*/
	
	/*右圖轉正*/
	cout << "右圖轉正\n";
	cout<<"建立轉換矩陣H\n";
	mxArray* Er = getH1H2(f ,t , false);
	//mxArray* R = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(r ,3 ,3) ,NULL);
	mxArray* Erinv = mlfInv(Er);
	//mxArray* Erinv = mlfCtranspose(Er);
	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	//取得F~ 以便求T
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
	  解 ((T * H)-1)t F H-1 = B = [ 0 0 -1 ]
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
	cout<<"轉換矩陣建立結束\n";

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
 取得epipole 長度為單位長
 右圖的公式為 F * e = 0
 左圖為e' * F = 0
 傳入：fundamental matrix 以及是要找左圖還是右圖的epipole
 傳出：epipole
*/
double* normalize(const double* a , int size);
double* getEpipole(double* * f ,bool isL) {

	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	if ( !isL ) F = mlfCtranspose(F);//如果是要找右圖Epipole

	mxArray* E = mlfNull(F ,NULL);
	double* e = mxGetPr(E);
	e =  normalize(e , 3);

	//mxArray* V = NULL;
	//mxArray* P = mlfEig( &V ,F ,NULL ,NULL);//傳入的是eigen value 回傳的是eigenvector
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
	////找出eigenvector
	//double*  e= new double [d];
	//for ( int i = 0; i < d; i++ ) {
	//	if ( index != i ) e[i] = 0;
	//	else e[i] = 1;
	//}
	//V = mlfDoubleMatrix(d, 1, e, NULL );
	//mxArray* E = mlfMtimes(P ,V );
	//e = mxGetPr(E);

	////單位長
	//e = normalize(e , d);
	///*double norm = 0;
	//for ( int i = 0; i < d; i++ )
	//	norm +=e[i] * e[i];
	//norm = sqrt( norm );
	//for ( int i = 0; i < d; i++ )
	//	e[i] /= norm;*/
	///*找出d*/
	////刪除記憶體
	//mxDestroyArray(P);
	//mxDestroyArray(V);
	//mxDestroyArray(E);
	//mxDestroyArray(DIM);

	return e;
}
/*
  給定兩個向量 取得cos
  傳入：兩個向量
  傳出：cos
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
  根據是右邊或是左邊 丟進一個vector 計算相對應的vector
  傳入：一個向量 fundamental matrix 以及是在左邊還是右邊
  傳出：另一個相對應的向量
*/
double* calTheOtherCP2CameraCoord( double* v ,double* * f ,bool isL ) {
	mxArray* F = mlfDoubleMatrix( 3 ,3 ,_2DDouble2M1D(f ,3 ,3) ,NULL);
	/*  R*F* L = 0 */
	mxArray* A = NULL;
	if ( !isL ) {//已知R 求A L = 0
		mxArray* R = mlfDoubleMatrix(1 ,3 ,v ,NULL );	
		
		A = mlfMtimes(R ,F );//解ker(A)
		mxDestroyArray(R);
	}
	else {//已知L求 R A = 0
		mxArray* L = mlfDoubleMatrix(3 ,1 ,v ,NULL );
		mxArray* A = mlfMtimes(F ,L );
		A = mlfCtranspose(A);//解 ker (At)
		
		mxDestroyArray(L);
	}

	//找單位向量
	mxArray* kerA = mlfNull(A ,NULL );//找null space
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
 取得H矩陣
 傳入： matrix 以及他是左邊還是右邊
 傳出：H matrix 
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
	//double Cos = getCos(z ,x  ,3 );//兩個z和z'軸夾角cos值
	//double Tan = atan( (d[1] * e[0] - d[0] * e[1]) / e[2] );
	double theta = (3.141615926 / 2)  - atan( (d[1] * e[0] - d[0] * e[1]) / e[2] );
	double* * r1 = getRotation3By3(d ,theta );//我們將轉成正面 
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

	/*取得epipole轉過去後的新epipole 以算出下一個矩陣*/
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
 將vector正規化
 傳入：一個要normalize的vector
 傳出：normalize的vector
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
  給定圖和Hinv進行轉換
  轉換公式為
  新圖的點p' 其對應舊圖的為p = H-1 p'
  傳入：要轉換的picture以及轉換矩陣
  傳出：rectify後的圖

*/
Picture 
transformImg(const Picture& p ,double* * hinv) {
	Picture img = p;

	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ )
			cout << hinv[i][j]<<" ";
		cout<<"\n";
	}

	//從產生圖當中到推回去算
	int total = img.getHeight()*img.getWidth();
	for ( int y = 0; y < img.getHeight(); y++ ) {
		for ( int x = 0; x < img.getWidth(); x++ ) {
			int done = img.getWidth() * y + x + 1;
			printf("\r已轉換了 %d %%" , 100 * done / total );
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