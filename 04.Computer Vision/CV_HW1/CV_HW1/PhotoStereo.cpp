/******************************************
FILE
    PhotoStereo.cpp
PURPOSE
    �w�qPhotometric Stereo�������禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "PhotoStereo.h"
#include "matlab.h"
#include <iostream>
#include "math.h"


using std::cout;


/*
  �إߤ@��Normal Map
  �ǤJ�G�n�إ�Normal Map������img
  �ǥX�GHeightmap
*/
Vector3*
*buildNormalMap( const vector<Picture>& imgs ) {
	/*�o��j�p�ۦP��2-D Normal map*/
	Picture tmp = imgs[0];
	int row = tmp.getHeight() ,col = tmp.getWidth();
	Normal* *map = create3DArray( row ,col );

	cout << "�p��Normal Map\n";
	/*��Ҧ����I�i��Photometric Stereo*/
	int i = 0;
	for ( int y = 0; y < row; y++ ) 
		for ( int x = 0; x < col ; x++ ) {
			printf("\r�ثe�����G%2d %%",  100 * (i++) / row / col ); 
			/*printf("(%d ,%d)\n" ,x , y);*/
			map[y][x] = calNormal( x ,y ,imgs );
		}
	cout << "\n�p�⵲��\n";
			
	return map;
}



/*
  ��(x ,y)�i��Photometric stereo��X��normalize��normal
  �ǤJ�G( x ,y ) �٦�����
  �ǥX�G��normalize��normal
*/
Normal calNormal( int x ,int y ,const vector<Picture>& imgs ) {
	unsigned size = imgs.size();
	/*��X�Ҧ�img��color �ò���I(x,y)��intensity �H�β���Lighr Coordinate matrix*/
	Color* ca = new Color [ size ];
	Light* l = new Light [ size ];
	for (unsigned i = 0; i < size; i++ )  {
		Picture p = imgs[i];
		ca[i] = p.getColor(x, y);
		l[i] = p.getLight();
	}
	double* I = new double[ size ];//�Ϫ�intensity
	double* *lc = create2DArray(size ,3 );//�ϥL�N�����u��m
	for ( unsigned i = 0; i < size ;i++ ) {
		double R = ca[i].GetR();
		double G = ca[i].GetG();
		double B = ca[i].GetB();
		I[i] = (ca[i].GetR() + ca[i].GetG() + ca[i].GetB() ) /3;//�Npixel���C���ন�Ƕ�
		lc[i][0] = l[i].x;
		lc[i][1] = l[i].y;
		lc[i][2] = l[i].z;
	}
	
	/*�إ�Diagonal matrix*/
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

	/*�i��Photometric�DNormal-----I = S * N */
	Normal normal;
	//���N��ƥHMATLAB Matrix�覡��� Matlab�OCollumn major
	mxArray *intensity = NULL ,*S = NULL ,*D = NULL ,*N = NULL ;
	intensity = mlfDoubleMatrix( size, 1, I, NULL) ;
	S = mlfDoubleMatrix( size, 3 ,_2DDouble2M1D(lc,size ,3 ), NULL);	
	D = mlfDoubleMatrix( size, size ,_2DDouble2M1D(d ,size ,size), NULL);
	S = mlfMtimes( D, S );// S' = D * S
	double* rank = mxGetPr( mlfRank(S ,NULL) );
	
	if ( *rank == size || *rank == 3) {//�S��inverse�ҥH�S��k�DPsuedo inverse
		intensity = mlfMtimes( D, intensity );//I' = D * I
		//��S' * N = I'
		mxArray* ST = mlfCtranspose ( S );//ST
		S = mlfMtimes( ST ,S );//ST*S
		intensity = mlfMtimes(ST ,intensity );//ST*I
		S = mlfInv(S);//(ST*S)-1
		N = mlfMtimes( S ,intensity );//(ST*S)-1* ST*I
		//���XNormal
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
	
	/*�M�����Τ��O����*/
	delete l;
	delete ca;
	mxDestroyArray(intensity);
	mxDestroyArray(S);
	mxDestroyArray(D);
	mxDestroyArray(N);

	return normal;
}
/*
  �NNormal normalize
  �ǤJ�G�@�ө|��Normalize��normal row col
  �Ǧ^�G����normal�öǦ^Normalize�����G
*/
Normal* 
*normalizeN( Normal* * map ,int row ,int col) {
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) {
			Normal n = map[i][j];
			double length = sqrt (n.x * n.x + n.y * n.y + n.z * n.z );
			if ( length != 0 ) {//�H����H0
				map[i][j].x /= -length;
				map[i][j].y /= -length;
				map[i][j].z /= length;
			}
		}

	
	return map;
}
/*
  ���dF ��M��k 
  ���3D�Ŷ��� ����(x ,y ,f(x,y) )���
  normal = ( -pf(x) ,-pf(y) ,1 )
  �]���ڭ̥i�H�z�L�Nnormal���H�L��z���Y�i�o��@��3D�Ŷ������ܵe�Ϲ�
  �ǤJ�GNormal map
  �Ǧ^�GHeight map
*/
//Check Height map���ȬO�_���T
bool FIsPass(Normal* *map ,int row ,int col);
Normal* 
*findF( Normal* * map ,int row ,int col) {

	for ( int  i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ ) {
			if ( map[i][j].z != 0 ) {//����H0
				map[i][j].x /= map[i][j].z;
				map[i][j].y /= map[i][j].z;
				map[i][j].z /= map[i][j].z;
			}
		}
	//Check Height map�p��O�_���T
	//if ( !FIsPass(map ,row ,col) ) throw "�藍�_!!�p����~ �i���]�ONormal�p��X�{���D\n";

	return map;
}
/*Check Height map���ȬO�_���T
  �Y�S�����D���~ �h�Ǧ^true
*/
bool FIsPass(Normal* *map ,int i ,int j ) {
	if ( map[i][j].z != 0  ) {//0���normal���s�b �N���γB�z�F 
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
  �إ�Height Map
  �ǤJ�GNormal Map
  �ǥXHeight Map
*/
double* *buildCrossHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//�إߤQ�r�Ϊ�Height map
double* *buildRBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//�إߥ��U��Height map
double* *buildRTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//�إߥ��W��Height map
double* *buildLBHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//�إߥk�U��Height map
double* *buildLTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col);//�إߥk�W��Height map

double* 
*buildHeightMap( Normal* *nmap ,int row ,int col ) {
	double* *hmap = create2DArray( row ,col );
	
	/*�ھ�Normal map�إ�Height map*/
	//for ( int i = 0; i < row; i++ ) {
	//	if ( i == 0 ) {//�Ĥ@�� �[����
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
	//	else {//��L�� �[�W��
	//		for ( int j = 0; j < col; j++ ) {
	//			if ( nmap[i][j].z != 0 ) {
	//				if ( FIsPass(nmap , i , j ) ){
	//					hmap[i][j] = hmap[i-1][j] + nmap[i][j].y;
	//				}
	//				else hmap[i][j] = hmap[i-1][j];

	//				//���F���Ƥư����ʧ@
	//				/*if ( i != 0 ) {
	//					hmap[i][j] = (hmap[i][j] + hmap[i][j-1]) / 2;
	//				}*/
	//			}
	//			
	//		}
	//	}

	//}

	for ( int i = 0; i < col; i++ ) {
		if ( i == 0 ) {//�Ĥ@�� �[����
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
		else {//��L�� �[�W��
			for ( int j = 0; j < row; j++ ) {
				if ( nmap[j][i].z != 0 ) {
					if ( FIsPass(nmap , j , i ) ){
						hmap[j][i] = hmap[j][i-1] + nmap[j][i].x;
					}
					else hmap[j][i] = hmap[j][i-1];

					//���F���Ƥư����ʧ@
					/*if ( i != 0 ) {
						hmap[j][i] = (hmap[j][i] + hmap[j-1][i]) / 2;
					}*/
				}
				
			}
		}

	}
	/*�q�����V����n�� for present*/
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
  ���N(r ,c)���n��
*/
double* 
*buildCrossHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) { 
	//��k��
	for ( int i = c + 1; i < col; i++ ) 
		if ( nmap[r][i].z != 0 )
			if ( FIsPass(nmap , r , i ) ) 
				hmap[r][i] = hmap[r][i-1] + nmap[r][i].x;
			else hmap[r][i] = hmap[r][i-1];
	//�索��
	for ( int i = c - 1; i >= 0; i-- ) 
		if ( nmap[r][i].z != 0 )
			if ( FIsPass(nmap , r , i ) ) 
				hmap[r][i] = hmap[r][i+1] + nmap[r][i].x;
			else hmap[r][i] = hmap[r][i+1];
	//��W��
	for ( int i = r + 1; i < row; i++ )
		if ( nmap[i][c].z != 0 )
			if ( FIsPass(nmap ,i ,c ) ) 
				hmap[i][c] = hmap[i-1][c] + nmap[i][c].y;
			else hmap[i][c] = hmap[i-1][c];
	//��U��
	for ( int i = r - 1; i >= 0; i-- )
		if ( nmap[i][c].z != 0 )
			if ( FIsPass(nmap ,i ,c ) ) 
				hmap[i][c] = hmap[i+1][c] + nmap[i][c].y;
			else hmap[i][c] = hmap[i+1][c];

	return hmap;
}
/*
  �إߥ��U��Height map
  �ǤJ�Gnormal map�H�αq���̶}�l�n �n�����
  �ǥX�GHeight map
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

				//���F���Ƥư����ʧ@
				hmap[i][j] = (hmap[i][j] + hmap[i][j-1]) / 2;
			}
			
		}
	}

	return hmap;

}
//�إߥ��W��Height map
double* 
*buildRTHeightMap( double* * hmap ,Normal* *nmap ,int r ,int c ,int row ,int col) {
	for ( int i = r - 1; i >= 0; i-- )
		for ( int j = c + 1; j < col; j++ )
			if ( nmap[i][j].z != 0 ) {
				if ( FIsPass(nmap , i , j ) ) 
					if ( FIsPass(nmap , i , j ) )
						hmap[i][j] = hmap[i+1][j] + nmap[i][j].y;
					else hmap[i][j] = hmap[i+1][j];

				//���F���Ƥư����ʧ@
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

				//���F���Ƥư����ʧ@
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

				//���F���Ƥư����ʧ@
				hmap[i][j] = (hmap[i][j] + hmap[i][j+1]) / 2;
			}
	
	return hmap;
}

/*
  �NHeight Map�i��Smooth
  �ǤJ�GHight map
  �ǥX�Gsmooth ��Height map
*/
double correlation3( double* *const hmap ,const double filter[5][5],int x ,int y );//�i��3 * 3��Correlation
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
	
	double* * nhmap = create2DArray( row ,col );//�s��height map
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
  �i��Correlation
  ----------->x
  |
  |
  |
  |
  y
  �ǤJ�Gheight map filter �nfilter����m
  �ǥX�Gcoorelation�����G

*/
double 
correlation3( double* *const hmap ,const double filter[5][5] ,int x ,int y ) {
	double total = 0;
	for ( int i = y - 2; i < y + 2; i++ ) 
		for ( int j = x - 2; j <= x + 2; j++ ) { 
			int fi = i - y + 2;//filter��index
			int fj = j - x + 2;
			
			//cout << fi <<" "<<fj <<" "<<hmap[i][j]<<" " <<filter[fi][fj]<<"\n";
			total += hmap[i][j] * filter[fi][fj];
			
		}


	return total;
}