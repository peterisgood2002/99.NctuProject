/******************************************
FILE
    calFMatrix.cpp
PURPOSE
    定義如何計算fundamental matrix相關資訊

NOTE
	1.透過下面公式計算F
      p'：image 2 的某點
      p：image 1 的某點
      p' 垂直於T * p'
      因此在沒有內部參數的情況下
      0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
      在有內部參數的情況下
      0 = p'  (K' E K)  p = p' F  p
    2.正規化correspondent point可以取得更加的值
      距離不會超過sqrt(2) 也就是
      scale = sqrt(2) / Avgdis
          [     scale           0     -avg*scale ]
	   S =[       0           scale   -avg*scale ]
	      [       0             0		   1     ]

AUTHOR
    9757553 顏志晟
*******************************************/
#include "calFMatrix.h"
#include <cmath>
#include <iostream>
#include "ArrayManipulation.h"
#include "Picture.h"
#include "matlab.h"

using std::cout;
/*
  取得相對應點集合中中平均值 
  傳入：所有的點對應集合
  傳出：一個CorPoint儲存各對應之平均值
*/
CorPoint* 
getAverage( const vector<CorPoint>& cp ) {
	CorPoint* avg = new CorPoint();

	//計算total
	for ( size_t i = 0; i <  cp.size(); i++ ) {
		const CorPoint tmp = cp[i];
		avg->pl.x += tmp.pl.x;
		avg->pl.y += tmp.pl.y;
		avg->pr.x += tmp.pr.x;
		avg->pr.y += tmp.pr.y;
	}

	//計算平均
	int length = (int)cp.size();
	avg->pl.x /= length;
	avg->pl.y /= length;
	avg->pr.x /= length;
	avg->pr.y /= length;
	avg->pl.z = 1;
	avg->pr.z = 1;

	return avg;

}
/*
 正規化correspondent point好進行運算
 距離不會超過sqrt(2)也就是
 scale = 影像高度或寬度
       [     scale           0     -avg*scale ]
	S =[       0           scale   -avg*scale ]
	   [       0             0		   1 ]
 傳入：點座標 相對應點所有的平均值 以及圖檔的高度寬度
 傳出：normalize過後的點座標 以及Normalize過後的矩陣 nl nr
*/
vector<CorPoint> normalizeCP( const vector<CorPoint>& cp  ,double imgw ,double imgh ,double* * nl ,double* * nr ,CorPoint* avg) {
	if ( avg == NULL ) avg = getAverage(cp);
	if ( nl == NULL ) nl = create2DArray(3 ,3);
	if ( nr == NULL ) nr = create2DArray(3 ,3);
	//double disl = 0 ,disr = 0;

	//for ( size_t i = 0; i < cp.size(); i++ ) {
	//	CorPoint tmp = cp[i];
	//	double dx = tmp.pl.x - avg->pl.x;
	//	double dy = tmp.pl.y - avg->pl.y;
	//	double p = pow( dx ,2 ) + pow( dy , 2);
	//	disl += sqrt(p);
	//	dx = tmp.pr.x - avg->pr.x;
	//	dy = tmp.pr.y - avg->pr.y;
	//	p = pow( dx ,2 ) + pow( dy , 2);
	//	disr += sqrt(p);
	//}
	//int length = (int) cp.size();
	////double sl = ( sqrt(2.0) / disl ) * length;
	////double sr = ( sqrt(2.0) / disr ) * length;
	//double sl = 341.7;
	//double sr = 341.7;
	//紀錄nl和nr
	nl[1][1] = 1/imgh; nl[0][0] = 1/imgw;
	nl[0][2] = - 1/imgw * avg->pl.x;
	nl[1][2] = - 1/imgh * avg->pl.y;
	nl[2][2] = 1;
	nr[1][1] = 1/imgh; nr[0][0] = 1/imgw;
	nr[0][2] = - 1/imgw * avg->pr.x;
	nr[1][2] = - 1/imgh * avg->pr.y;
	nr[2][2] = 1;

	//normalie cp
	vector<CorPoint> ncp;
	for ( size_t i = 0; i < cp.size(); i++ ) {
		CorPoint tmp =  cp[i];
		tmp.pl.x = nl[0][0] * tmp.pl.x + nl[0][2];
		tmp.pl.y = nl[1][1] * tmp.pl.y + nl[1][2];
		tmp.pr.x = nl[0][0] * tmp.pr.x + nl[0][2];
		tmp.pr.y = nl[1][1] * tmp.pr.y + nl[1][2];
		
		ncp.push_back(tmp);
	}
	
	return ncp;
}
/*
  1.透過下面公式計算F
    p'：image R 的某點
    p：image L 的某點
    p' 垂直於T * p'
    因此在沒有內部參數的情況下
    0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
    在有內部參數的情況下
    0 = p'  (K' E K)  p = p' F  p
  傳入：correspondent point
  傳出：Fundamental matrix
*/
double* calMinumX(double* * A ,int row ,int col);
double* 
*calF(const vector<CorPoint>& cp ) {
	
	/*進行初始化相關設定*/
	int r = (int) cp.size();
	double* * A = create2DArray(  r, 3 * 3 );
	for ( int i = 0; i < r; i++ ) {
		CorPoint p = cp[i];
		A[i][0] = p.pr.x * p.pl.x;
		A[i][1] = p.pr.x * p.pl.y;
		A[i][2] = p.pr.x * p.pl.z;
		A[i][3] = p.pr.y * p.pl.x;
		A[i][4] = p.pr.y * p.pl.y;
		A[i][5] = p.pr.y * p.pl.z;
		A[i][6] = p.pr.z * p.pl.x;
		A[i][7] = p.pr.z * p.pl.y;
		A[i][8] = p.pr.z * p.pl.z;
	}

	double* x = calMinumX(A ,r ,3 * 3 );
	/*計算 SVD讓他rank = 2*/
	double* * f = _M1DDouble22D(x ,3 ,3);
	mxArray* F = mlfDoubleMatrix(3 ,3 ,_2DDouble2M1D(f ,3 ,3 ) ,NULL);
	mxArray* S = NULL ,*V = NULL;
	mxArray* U = mlfSvd(&S ,&V ,F ,NULL);//F = U S V'
	mxArray* M = NULL ,*N =NULL;
	mlfSize(mlfVarargout(&M,&N,NULL),S,NULL); //取得S的大小[m n ] =size(S)
	int n =  (int)mxGetPr(N)[0];
	double* value = new double[ n * n ];
	for ( int i = 0; i < n * n; i++ ) {
		if ( i == 0 ) value[i] = 1;
		else if ( i == n + 1 ) value[i] = 1;
		else value[i] = 0;
	}
	mxArray* MA = mlfDoubleMatrix(3 ,3 ,value ,NULL);
	S = mlfMtimes(S ,MA );//rank(F) = 2
	F = mlfMtimes( U  ,mlfMtimes(S,mlfCtranspose(V)) );//F = U S V'
	
	//取得f
	x = mxGetPr(F);
	f = _M1DDouble22D(x ,3 ,3);
	//長度調整為1
	double norm = 0;
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			norm+=f[i][j] * f[i][j];	
		}
		
	}
	norm = sqrt( norm );
	for ( int i = 0; i < 3;i++ ) {
		for ( int j = 0; j < 3; j++ )
			f[i][j] /= norm;
	}
	dispose(A ,r , 3 * 3);

	/*釋放MATLAB array*/
	mxDestroyArray(MA);
	mxDestroyArray(S);
	mxDestroyArray(U);
	mxDestroyArray(V);
	mxDestroyArray(F);
	mxDestroyArray(M);
	mxDestroyArray(N);
	
	return f;
}
/*
 計算K' F K
 傳入：左邊的normalize matrix Fundamental matrix 右邊的normalize
 傳出：算出的值
*/
double* 
*calKFK( double* *nr , double* * f , double* *nl ,int row ,int col) {
	mxArray* L = mlfDoubleMatrix( row ,col ,_2DDouble2M1D(nr,row ,col) ,NULL);
	mxArray* F = mlfDoubleMatrix( row ,col ,_2DDouble2M1D(f ,row ,col) ,NULL);
	mxArray* R = mlfDoubleMatrix( row ,col ,_2DDouble2M1D(nl ,row ,col) ,NULL);
	L = mlfCtranspose( L );
	F = mlfMtimes(L ,mlfMtimes( F ,R ) );
	double* r = mxGetPr(F);
	double* * result = _M1DDouble22D( r, row ,col );

	//長度調整為1
	double norm = 0;
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 3; j++ ) 
			norm+=f[i][j] *f[i][j];	
	}
	norm = sqrt( norm );
	for ( int i = 0; i < 3;i++ ) {
		for ( int j = 0; j < 3; j++ )
			f[i][j] /= norm;
	}
	/*釋放MATLAB array*/
	mxDestroyArray(L);
	mxDestroyArray(F);
	mxDestroyArray(R);

	return result;

}
/*
  根據fundamental matrix將T解出 透過對應點解出方向
  公式為：
    sum(T * Xi') * (F Xi) < 0 ==> T =  -T
  傳入：fundamental matrix
  傳出：translation vector
*/
double* 
calT(double* * f ,const vector<CorPoint>& cp,int row ,int col) {
	double *t = calMinumX( f ,row ,col );

	mxArray* T = mlfDoubleMatrix(3 ,1 ,t ,NULL );
	
	//計算方向性
	mxArray* F = mlfDoubleMatrix(3 ,3 ,_2DDouble2M1D(f ,3 ,3 ) ,NULL );
	double total = 0;
	for ( size_t i = 0; i < cp.size(); i++ ) {
		CorPoint p = cp[i];
		double x[3] = { p.pr.x ,p.pr.y , p.pr.z };
		mxArray* X = mlfDoubleMatrix(3 ,1 ,x ,NULL );

		X = mlfCross(T,X,NULL);

		mxArray* A = mlfMtimes( F ,X );

		

		X = mlfTimes( X , A );//兩個3 * 1 array取相乘 以方便做dot product
		double* k = mxGetPr(X);
		for ( int i = 0; i < 3; i++ ) {
			//cout<<k[i]<<" ";
			total += k[i];
		}

		mxDestroyArray(X);
		mxDestroyArray(A);
	}

	mxDestroyArray(T);
	mxDestroyArray(F);

	
	if ( total < 0 ) 
		for ( int i = 0; i < 3;i++)
			t[i] = - t[i];
	
	return t;
}
/*
  根據fundamental matrix和T來解 R
  公式：
    R = W = [ E1 * T + E2 * T        E2 * T + E3 * E1      E3 * T + E1 * E2 ]
	有noise ==> || R - W|| 最小
  傳入：fundamental matrix translation vector
  傳出：R
*/
double* crossProduct(const double* a ,const double* b ,int size );
double* 
*calR( double* * f ,const double* t ,bool noise ) {
	double* * W = create2DArray(3 ,3 );
	double a[3] = { 0 } ,b[3] = { 0 };
	for ( int i = 0; i < 3; i++ ) {
		int nexti = (i+1) % 3;
		int nnexti = (i+2) % 3;
		a[0] = f[0][i]; a[1] = f[1][i] ; a[2] = f[2][i];//Ei
		double* r = crossProduct(a ,t ,3);//Ei * T

		a[0] = f[0][nexti]; a[1] = f[1][nexti]; a[2] = f[2][nexti];
		b[0] = f[0][nnexti]; b[1] = f[1][nnexti]; b[2] = f[2][nnexti];
		double* w = crossProduct(a ,b ,3 );
		//normalize 因為旋轉矩陣每一列長度為1
		

		W[0][i] = r[0] + w[0];
		W[1][i] = r[1] + w[1];
		W[2][i] = r[2] + w[2];
		double norm = 0;
		for ( int j = 0; j < 3; j++ )
			norm += W[j][i] * W[j][i];
		norm = sqrt(norm);
		for ( int j = 0; j < 3; j++ )
			W[j][i] /= norm;
		

		delete r;
		delete w;
	}

	return W;
}
double* 
crossProduct(const double* a ,const double* b ,int size ) {
	double* r = new double[size];

	for ( int i = 0; i < size; i++ ) 
		r[i] = a[ (i+1)%size ] * b[ (i+2)%size ] + b[ (i+1)%size ] * a[ (i+2)%size ]; 

	return r;
	
}
/*
 計算Ax = 0的解
 傳入：A
 傳出：x
*/
double* 
calMinumX(double* * A ,int row ,int col) {
	/*計算x*/
	mxArray* MA = mlfDoubleMatrix(row, col, _2DDouble2M1D(A ,row ,col ), NULL );
	mxArray* MAT = mlfCtranspose( MA );
	mxArray* S = mlfMtimes(MAT ,MA );//S = AT * A
	//解AT * A最小 也就是解eigen value最小
	mxArray* V = NULL;
	mxArray* P = mlfEig( &V ,S ,NULL ,NULL);//傳入的是eigen value 回傳的是eigenvector
	//找eigen value最小發生點
	mxArray* DIM = mlfSize( NULL ,V ,NULL );
	int d = (int)mxGetPr(DIM)[0];
	double* value = mxGetPr(V);
	double min = System::Double::MaxValue;
	int index = 0;
	for ( int i = 0; i < d * d; i += d )  {
		if ( value[i] < min ) {
			min = value[i];
			index = i % d;
		}
		
		i++;
	}
	//找出eigenvector
	double*  e= new double [d];
	for ( int i = 0; i < d; i++ ) {
		if ( index != i ) e[i] = 0;
		else e[i] = 1;
	}
	V = mlfDoubleMatrix(d, 1, e, NULL );
	mxArray* X = mlfMtimes(P ,V );
	double* x = mxGetPr(X);

	/*釋放MATLAB array*/
	mxDestroyArray(MA);
	mxDestroyArray(MAT);
	mxDestroyArray(S);
	mxDestroyArray(P);
	mxDestroyArray(V);
	mxDestroyArray(DIM);
	delete e;

	return x;

}

/*
  根據fundamental matrix 在圖上畫出一條Epipolar line
  傳入：圖 檔名 fundamental matrix 3 * 3對應點 以及這一張圖是左圖還是右圖 左圖為0 右圖為1
*/
void 
drawEpiLine(const Picture& pic ,const string fname ,double* * f ,vector<CorPoint>& cp ,bool isL) {
	Picture img = pic;
	mxArray* F = mlfDoubleMatrix(3, 3, _2DDouble2M1D(f ,3 ,3 ), NULL );

	for ( size_t i = 0; i < cp.size(); i++ ) {
		printf("找尋第%d的epipolar line" , i);
		CorPoint p = cp[i];
		double point[3]= { 0 };
		double n[3] = { 0 };
		mxArray* P = NULL;
		mxArray* N = NULL;
		if ( isL ) {//這一張圖是左圖 0 = Pl F Pr則Pr已知 要求所有的Pl
			point[0] = p.pr.x;
			point[1] = p.pr.y;
			point[2] = p.pr.z;
			mxArray* P = mlfDoubleMatrix(3 ,1 ,point ,NULL );
			mxArray* N = mlfMtimes(F ,P );

			n[0] = *(mxGetPr(N)+0); 
			n[1] = *(mxGetPr(N)+1); 
			n[2] = *(mxGetPr(N)+2) ;
			
		}
		else {
			point[0] = p.pl.x;
			point[1] = p.pl.y;
			point[2] = p.pl.z;
			mxArray* P = mlfDoubleMatrix(1 ,3 ,point ,NULL );
			mxArray* N = mlfMtimes(P ,F );
			n[0] = *(mxGetPr(N)+0); 
			n[1] = *(mxGetPr(N)+1); 
			n[2] = *(mxGetPr(N)+2);

		}

		for ( int x = 0; x < pic.getWidth(); x++ ) {
			//cout << x <<" "<<y <<"\n";
			int yhigh = (int) ( ( 0.001 - n[2] -n[0] * x) / n[1] );//ax + by + c <=0.05
			int ylow  = (int) ( ( -0.001 - n[2] -n[0] * x) / n[1]);//ax + by + c >= 0.05
			if ( ylow >= 0 ) {
				if ( ylow <= yhigh )
					for ( int y = ylow; y < yhigh && y < pic.getHeight(); y++ )
						img.setColor(x , y ,Color::Red );
				else 
					for ( int y = yhigh; y < ylow && y < pic.getHeight(); y++ )
						img.setColor(x , y ,Color::Red );
			}
		}

		mxDestroyArray(P);
		mxDestroyArray(N);
		cout<<"找尋結束\n";
	}
	
	mxDestroyArray(F);
    img.drawPicture(fname);

}
