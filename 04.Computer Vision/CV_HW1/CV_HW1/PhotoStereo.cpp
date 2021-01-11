/******************************************
FILE
    PhotoStereo.cpp
PURPOSE
    定義Photometric Stereo的相關函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "PhotoStereo.h"
#include "matlab.h"
#include <iostream>
#include "math.h"


using std::cout;


/*
  建立一個Normal Map
  傳入：要建立Normal Map的相關img
  傳出：Heightmap
*/
Vector3*
*buildNormalMap( const vector<Picture>& imgs ) {
	/*得到大小相同的2-D Normal map*/
	Picture tmp = imgs[0];
	int row = tmp.getHeight() ,col = tmp.getWidth();
	Normal* *map = create3DArray( row ,col );

	cout << "計算Normal Map\n";
	/*對所有的點進行Photometric Stereo*/
	int i = 0;
	for ( int y = 0; y < row; y++ ) 
		for ( int x = 0; x < col ; x++ ) {
			printf("\r目前完成：%2d %%",  100 * (i++) / row / col ); 
			/*printf("(%d ,%d)\n" ,x , y);*/
			map[y][x] = calNormal( x ,y ,imgs );
		}
	cout << "\n計算結束\n";
			
	return map;
}



/*
  對(x ,y)進行Photometric stereo算出未normalize之normal
  傳入：( x ,y ) 還有圖檔
  傳出：未normalize之normal
*/
Normal calNormal( int x ,int y ,const vector<Picture>& imgs ) {
	unsigned size = imgs.size();
	/*抽出所有img的color 並產生I(x,y)的intensity 以及產生Lighr Coordinate matrix*/
	Color* ca = new Color [ size ];
	Light* l = new Light [ size ];
	for (unsigned i = 0; i < size; i++ )  {
		Picture p = imgs[i];
		ca[i] = p.getColor(x, y);
		l[i] = p.getLight();
	}
	double* I = new double[ size ];//圖的intensity
	double* *lc = create2DArray(size ,3 );//圖他代表的光線位置
	for ( unsigned i = 0; i < size ;i++ ) {
		double R = ca[i].GetR();
		double G = ca[i].GetG();
		double B = ca[i].GetB();
		I[i] = (ca[i].GetR() + ca[i].GetG() + ca[i].GetB() ) /3;//將pixel的顏色轉成灰階
		lc[i][0] = l[i].x;
		lc[i][1] = l[i].y;
		lc[i][2] = l[i].z;
	}
	
	/*建立Diagonal matrix*/
	double* *d = create2DArray( size ,size );
	for ( size_t i = 0; i < size; i++ ) 
		d[i][i] = I[i];

	/*Debug mode*/
	/*cout <<"\n";
	for ( int i = 0; i < size; i++ )
		cout << I[i] << " " ;
	cout <<"\n";
	for ( int i = 0; i < size; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			cout << lc[i][j] << " ";
		}
		cout <<"\n";
	}
	cout <<"\n";*/
	/*Debug end*/

	/*進行Photometric求Normal-----I = S * N */
	Normal normal;
	//先將資料以MATLAB Matrix方式表示 Matlab是Collumn major
	mxArray *intensity = NULL ,*S = NULL ,*D = NULL ,*N = NULL ;
	intensity = mlfDoubleMatrix( size, 1, I, NULL) ;
	S = mlfDoubleMatrix( size, 3 ,_2DDouble2M1D(lc,size ,3 ), NULL);	
	D = mlfDoubleMatrix( size, size ,_2DDouble2M1D(d ,size ,size), NULL);
	S = mlfMtimes( D, S );// S' = D * S
	double* rank = mxGetPr( mlfRank(S ,NULL) );
	
	if ( *rank == size || *rank == 3) {//沒有inverse所以沒辦法求Psuedo inverse
		intensity = mlfMtimes( D, intensity );//I' = D * I
		//解S' * N = I'
		mxArray* ST = mlfCtranspose ( S );//ST
		S = mlfMtimes( ST ,S );//ST*S
		intensity = mlfMtimes(ST ,intensity );//ST*I
		S = mlfInv(S);//(ST*S)-1
		N = mlfMtimes( S ,intensity );//(ST*S)-1* ST*I
		//取出Normal
		double *n = mxGetPr(N);
		normal.x = n[0];
		normal.y = n[1];
		normal.z = n[2];
	}
	else {
		normal.x = 0;
		normal.y = 0;
		normal.z = 0;
	}
	
	/*清除不用之記憶體*/
	delete l;
	delete ca;
	mxDestroyArray(intensity);
	mxDestroyArray(S);
	mxDestroyArray(D);
	mxDestroyArray(N);

	return normal;
}
/*
  將Normal normalize
  傳入：一個尚未Normalize的normal row col
  傳回：改變normal並傳回Normalize的結果
*/
Normal* 
*normalizeN( Normal* * map ,int row ,int col) {
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) {
			Normal n = map[i][j];
			double length = sqrt (n.x * n.x + n.y * n.y + n.z * n.z );
			if ( length != 0 ) {//以防止除以0
				map[i][j].x /= -length;
				map[i][j].y /= -length;
				map[i][j].z /= length;
			}
		}

	
	return map;
}
/*
  找到dF 找尋方法 
  對於3D空間中 平面(x ,y ,f(x,y) )其實
  normal = ( -pf(x) ,-pf(y) ,1 )
  因此我們可以透過將normal除以他的z項即可得到一個3D空間中的變畫圖圖
  傳入：Normal map
  傳回：Height map
*/
//Check Height map的值是否正確
bool FIsPass(Normal* *map ,int row ,int col);
Normal* 
*findF( Normal* * map ,int row ,int col) {

	for ( int  i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ ) {
			if ( map[i][j].z != 0 ) {//防止除以0
				map[i][j].x /= map[i][j].z;
				map[i][j].y /= map[i][j].z;
				map[i][j].z /= map[i][j].z;
			}
		}
	//Check Height map計算是否正確
	//if ( !FIsPass(map ,row ,col) ) throw "對不起!!計算錯誤 可能原因是Normal計算出現問題\n";

	return map;
}
/*Check Height map的值是否正確
  若沒有問題錯誤 則傳回true
*/
bool FIsPass(Normal* *map ,int i ,int j ) {
	if ( map[i][j].z != 0  ) {//0表示normal不存在 就不用處理了 
		/*cout << map[i][j].y << " " << map[i][j+1].y<<"\n";
		cout << map[i][j].x <<" " << map[i+1][j].x<<" " << "\n";*/
		double dfdy = map[i+1][j].x - map[i][j].x; 
		double dfdx = map[i][j+1].y - map[i][j].y;
		if ( abs( dfdy - dfdx ) > 1 ) 
			return false;
	}
			
	return true;
}
/*
  建立Height Map
  傳入：Normal Map
  傳出Height Map
*/
double* *buildCrossHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//建立十字形的Height map
double* *buildRBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//建立左下之Height map
double* *buildRTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//建立左上之Height map
double* *buildLBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//建立右下之Height map
double* *buildLTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//建立右上之Height map

double* 
*buildHeightMap( Normal* *nmap ,int row ,int col ) {
	double* *hmap = create2DArray( row ,col );
	
	/*根據Normal map建立Height map*/
	//for ( int i = 0; i < row; i++ ) {
	//	if ( i == 0 ) {//第一行 加左邊
	//		for ( int j = 0; j < col; j++ ) {
	//			if ( i == 0 && j == 0 ) hmap[i][j] = 0;
	//			else {
	//				if ( nmap[i][j].z != 0 ) {
	//					if ( FIsPass(nmap , i , j ) ){
	//						hmap[i][j] = hmap[i][j-1] + nmap[i][j].x;
	//					}
	//					else hmap[i][j] = hmap[i][j-1];
	//				}
	//			}
	//		}
	//	}
	//	else {//其他行 加上面
	//		for ( int j = 0; j < col; j++ ) {
	//			if ( nmap[i][j].z != 0 ) {
	//				if ( FIsPass(nmap , i , j ) ){
	//					hmap[i][j] = hmap[i-1][j] + nmap[i][j].y;
	//				}
	//				else hmap[i][j] = hmap[i-1][j];

	//				//為了平滑化做的動作
	//				/*if ( i != 0 ) {
	//					hmap[i][j] = (hmap[i][j] + hmap[i][j-1]) / 2;
	//				}*/
	//			}
	//			
	//		}
	//	}

	//}

	for ( int i = 0; i < col; i++ ) {
		if ( i == 0 ) {//第一行 加左邊
			for ( int j = 0; j < row; j++ ) {
				if ( i == 0 && j == 0 ) hmap[j][i] = 0;
				else {
					if ( nmap[j][i].z != 0 ) {
						if ( FIsPass(nmap , j , i ) ){
							hmap[j][i] = hmap[j-1][i] + nmap[j][i].y;
						}
						else hmap[j][i] = hmap[j-1][i];
					}
				}
			}
		}
		else {//其他行 加上面
			for ( int j = 0; j < row; j++ ) {
				if ( nmap[j][i].z != 0 ) {
					if ( FIsPass(nmap , j , i ) ){
						hmap[j][i] = hmap[j][i-1] + nmap[j][i].x;
					}
					else hmap[j][i] = hmap[j][i-1];

					//為了平滑化做的動作
					/*if ( i != 0 ) {
						hmap[j][i] = (hmap[j][i] + hmap[j-1][i]) / 2;
					}*/
				}
				
			}
		}

	}
	/*從中間向旁邊積分 for present*/
	/*int midr = row / 2;
	int midc = col / 2;
	hmap[midr][midc] = 0;
	hmap = buildCrossHeightMap(hmap ,nmap ,midr ,midc ,row ,col);
	hmap = buildRBHeightMap(hmap ,nmap ,midr ,midc ,row ,col);
	hmap = buildRTHeightMap(hmap ,nmap ,midr ,midc ,row ,col);
	hmap = buildLTHeightMap(hmap ,nmap ,midr ,midc ,row ,col);
	hmap = buildLBHeightMap(hmap ,nmap ,midr ,midc ,row ,col);*/

	
	return hmap;

}
/*
  先將(r ,c)做積分
*/
double* 
*buildCrossHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) { 
	//對右邊
	for ( int i = c + 1; i < col; i++ ) 
		if ( nmap[r][i].z != 0 )
			if ( FIsPass(nmap , r , i ) ) 
				hmap[r][i] = hmap[r][i-1] + nmap[r][i].x;
			else hmap[r][i] = hmap[r][i-1];
	//對左邊
	for ( int i = c - 1; i >= 0; i-- ) 
		if ( nmap[r][i].z != 0 )
			if ( FIsPass(nmap , r , i ) ) 
				hmap[r][i] = hmap[r][i+1] + nmap[r][i].x;
			else hmap[r][i] = hmap[r][i+1];
	//對上面
	for ( int i = r + 1; i < row; i++ )
		if ( nmap[i][c].z != 0 )
			if ( FIsPass(nmap ,i ,c ) ) 
				hmap[i][c] = hmap[i-1][c] + nmap[i][c].y;
			else hmap[i][c] = hmap[i-1][c];
	//對下面
	for ( int i = r - 1; i >= 0; i-- )
		if ( nmap[i][c].z != 0 )
			if ( FIsPass(nmap ,i ,c ) ) 
				hmap[i][c] = hmap[i+1][c] + nmap[i][c].y;
			else hmap[i][c] = hmap[i+1][c];

	return hmap;
}
/*
  建立左下之Height map
  傳入：normal map以及從哪裡開始積 積到哪裡
  傳出：Height map
*/
double* 
*buildRBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) {
	for ( int i = r + 1; i < row; i++ ) {
		for ( int j = c + 1; j < col; j++ ) {
			if ( nmap[i][j].z != 0 ) {
				if ( FIsPass(nmap , i , j ) ){
					hmap[i][j] = hmap[i-1][j] + nmap[i][j].y;
				}
				else hmap[i][j] = hmap[i-1][j];

				//為了平滑化做的動作
				hmap[i][j] = (hmap[i][j] + hmap[i][j-1]) / 2;
			}
			
		}
	}

	return hmap;

}
//建立左上之Height map
double* 
*buildRTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) {
	for ( int i = r - 1; i >= 0; i-- )
		for ( int j = c + 1; j < col; j++ )
			if ( nmap[i][j].z != 0 ) {
				if ( FIsPass(nmap , i , j ) ) 
					if ( FIsPass(nmap , i , j ) )
						hmap[i][j] = hmap[i+1][j] + nmap[i][j].y;
					else hmap[i][j] = hmap[i+1][j];

				//為了平滑化做的動作
				hmap[i][j] = (hmap[i][j] + hmap[i][j-1]) / 2;
			}
	
	return hmap;
}

double* 
*buildLBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) {
	for ( int i = r + 1; i < row; i++ ) {
		for ( int j = c - 1; j >= 0; j-- ) {
			if ( nmap[i][j].z != 0 ) {
				if ( FIsPass(nmap , i , j ) ){
					hmap[i][j] = hmap[i-1][j] + nmap[i][j].y;
				}
				else hmap[i][j] = hmap[i-1][j];

				//為了平滑化做的動作
				hmap[i][j] = (hmap[i][j] + hmap[i][j+1]) / 2;
				
			}
			
		}
	}

	return hmap;

}
double* 
*buildLTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) {
	for ( int i = r - 1; i >= 0; i-- )
		for ( int j = c - 1; j >= 0; j-- )
			if ( nmap[i][j].z != 0 ) {
				if ( FIsPass(nmap , i , j ) ) 
					if ( FIsPass(nmap , i , j ) )
						hmap[i][j] = hmap[i+1][j] + nmap[i][j].y;
					else hmap[i][j] = hmap[i+1][j];

				//為了平滑化做的動作
				hmap[i][j] = (hmap[i][j] + hmap[i][j+1]) / 2;
			}
	
	return hmap;
}

/*
  將Height Map進行Smooth
  傳入：Hight map
  傳出：smooth 的Height map
*/
double correlation3( double* *const hmap ,const double filter[5][5],int x ,int y );//進行3 * 3的Correlation
double* 
*smoothHMap( double* * hmap ,int row ,int col ) {

	double filter[5][5] = { {0 ,0 ,1 ,0 ,0} ,
	                        {0 ,1 ,1 ,1 ,0} ,
	                        {1 ,1 ,1 ,1 ,1} , 
							{0 ,1 ,1 ,1 ,0} ,
							{0 ,0 ,1 ,0 ,0 } };
	for ( int i = 0; i < 5; i++ ) 
		for ( int j=0; j < 5; j++ ) 
			filter[i][j] /=13;
	
	double* * nhmap = create2DArray( row ,col );//新的height map
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			if ( hmap[i][j] != 0 ) {
				nhmap[i][j] = correlation3(hmap ,filter ,j ,i );
			}
		}
	}
	return nhmap;
}
/*
  進行Correlation
  ----------->x
  |
  |
  |
  |
  y
  傳入：height map filter 要filter的位置
  傳出：coorelation的結果

*/
double 
correlation3( double* *const hmap ,const double filter[5][5] ,int x ,int y ) {
	double total = 0;
	for ( int i = y - 2; i < y + 2; i++ ) 
		for ( int j = x - 2; j <= x + 2; j++ ) { 
			int fi = i - y + 2;//filter的index
			int fj = j - x + 2;
			
			//cout << fi <<" "<<fj <<" "<<hmap[i][j]<<" " <<filter[fi][fj]<<"\n";
			total += hmap[i][j] * filter[fi][fj];
			
		}


	return total;
}