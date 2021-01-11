/******************************************
FILE
    DPAlgo.h
PURPOSE
    進行Dynamic programming algorithm的一些函式
	因為Ohta and Kanade要算很久 所以用助教給的方法
	使用助教給的方式 
	C( x ,y ) = 
			C(x-1 ,y-1) + c(x ,y)
	min		C(x-1 ,y ) + Occlusion
			C(x ,y-1 ) + Occlusion

	Occlusion設為定值

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <cmath>
#include "DPAlgo.h"
#include "ArrayManipulation.h"
#include <iostream>

using std::cout;


/*
 從Color中找尋最接近的點
 傳入：左圖和右圖的epipolar line之color值以及window size
 傳出：相對應的位置
*/
double* *
dpAlgorithm(const vector<WColor*>& l , const vector<WColor*>& r ) {
	//將Color轉成灰階並 用double array儲存起來
	cout<<"轉灰階值中\n";
	int llength = (int) l.size();
	int rlength = (int) r.size();
	vector<double*> leftc;
	vector<double*> rightc;

	int wsize = (int) l[0]->size();
	for ( int i = 0; i < llength; i++ ) {
		WColor* tmp = l[i];
		
		double* pc = new double [wsize];
		for ( int j = 0; j < wsize; j++ ) {
			Color tmpc = (*tmp)[j];
			pc[j] = ( tmpc.GetR() + tmpc.GetG() + tmpc.GetB() ) / 3;
		}

		leftc.push_back(pc);
		
	}

	for ( int i = 0; i < rlength; i++ ) {
		WColor* tmp = r[i];

		double* pc = new double [wsize];
		for ( int j = 0; j < wsize; j++ ) {
			Color tmpc = (*tmp)[j];
			pc[j] = ( tmpc.GetR() + tmpc.GetG() + tmpc.GetB() ) / 3;
		}

		rightc.push_back(pc);
	
	}
	cout <<"轉換完畢\n";

	cout<<"計算Dynamic Programming\n";
	double* * result = dpAlgorithm(leftc ,rightc ,wsize);
	cout<<"計算Dynamic Programming完畢\n";
	
	for ( int i = 0; i < llength; i++ ) 
		delete leftc[i];
	for ( int i = 0; i < rlength; i++ )
		delete rightc[i];
	

	return result;
}
/*
 透過DP algorithm結果 找出相對應的資料
 傳入：DP algo結果 
      要以row為準 找相對應資料還是col d = ROW表以row為準 d = COL表以col為準
 傳出：以d為準的相對應結果
*/
int* getCorrespondence(double* * dpresult ,int row ,int col ,Direction d) {
	//初始化資料
	int* cor;
	int size = 0;
	int index = 0;//用來進行對應的
	if ( d == ROW ) {
		size = row;
		index = row - 1;
	}
	else if ( d == COL ) {
		size = col;
		index = col - 1;
	}
	cor = new int[ size ];
	for ( int i = 0; i < size; i++ ) 
		cor[i] = (int) NOTANUMBER;

	
	//從最左邊開始做對應 找出所有D的狀況 如果沒有D則assign左邊的D給右邊
	int r = row-1 , c = col-1;
	int i = size - 1;
	bool ignore = false;
	while( i >= 0  && r >= 0 && c >= 0) {
		/*如果是\ 則先assign再走*/
		//找到對應
		if ( dpresult[r][c] == D ) {
			if ( d == ROW ) {
				i = r; index = c;
			}
			else if ( d == COL ) {
				i = c; index = r;
			}
					
			//加入對應
				
			cor[i] = index;
		}
			
		//printf("assign：目前在(%d ,%d) 而剛剛修改了cor[%d] = %d\n" ,c ,r ,i ,index);
		//cout<<"--------------\n";
		
		/*修改位置到下一筆去*/
		//cout<<"go：";
		if ( dpresult[r][c] == UP ) {
			//cout<< "往|\n";
			r--;
		}
		else if ( dpresult[r][c] == LEFT ) {
			//cout << "往<--\n";
			c--;
		}
		else if ( dpresult[r][c] == D ) {
			//cout<<"往\\\n";
			r--; c--;
		}
		else break;
		//printf("修改了目前位置在(%d ,%d)\n" ,c , r );

		
		
		
	}

	/*for ( int i = 0; i < size; i++ )
		cout <<cor[i]<<" ";*/
	return cor;
}

/*
  負責計算cost用的function
  1.paper中計算的方式為假設有a1......ak b1.....bl的interval需要比較
    先求所有的平均M 和再求SSB
    M = 1/2 ( 1/k * Sum(ai) + 1/l * Sum(bi) )
    SSB = 1/2 ( 1/k * Sum( (ai - m )^2 ) + 1/l * Sum( (bi - m )^2 )
  2.兩個值取cos 再變成sin
    若值相等則是0
    傳入：兩筆資料 wsize為多少
    傳出：cost
*/
double 
costFunc(const double* l ,const double* r ,int wsize) {
	/*
	//Paper方法
	int lsize = (int) l.size();
	int rsize = (int) r.size();
	//求平均
	double mean = 0;
	double total = 0;
	for ( int i = 0; i < lsize; i++ )
		total += l[i];
	mean += total / lsize;
	total = 0;
	for ( int i = 0; i < rsize; i++ )
		total += r[i];
	mean += total / rsize;

	mean /= 2;

	//求variance
	double var = 0;
	total = 0;
	for ( int i = 0; i < lsize; i++ )
		total += pow ( (l[i] - mean ) ,2 );
	var += total / lsize;
	total = 0;
	for ( int i = 0; i < rsize; i++ )
		total +=pow ( (r[i] - mean) , 2 );
	var+= total / rsize;
	var /= 2;

	return var * sqrt( (double)lsize * lsize + rsize * rsize);
	*/
	if ( wsize != 1) {
		double cos = 0;
		for ( int i = 0; i < wsize; i++ ) 
			cos += l[i] * r[i];
		
		double lnorm = 0 , rnorm = 0;
		for ( int i = 0; i < wsize; i++ ) {
			lnorm +=  l[i] * l[i];
			rnorm +=  r[i] * r[i];
			//cout << l[i] <<" " <<r[i] <<"\n";
		}

		cos /= sqrt(lnorm) * sqrt(rnorm);

		//cout << 1 -cos<<" ";
		//刪除記憶體
		return  1 - cos;
	}
	else {
		return abs(l[0] - r[0]);
	}
	
}
/*
 Dynamic programming主要實作處
 傳入：兩個陣列 以及window size
 傳出：Dynamic programming結果
*/
double* *
dpAlgorithm(const vector<double*>& left ,const vector<double*>& right ,int wsize) {
	int lsize = (int) left.size();
	int rsize = (int) right.size();
	/*初始化相關資料*/
	double* *direction = create2DArray(lsize ,rsize ,NOTANUMBER);
	double* *cost = create2DArray(lsize ,rsize , NOTANUMBER);
	
	cost[0][0] = 0; direction[0][0] = D;
	double occlusion = 0.001;
	/*計算Dynamic programming*/
	for ( int i = 1; i < lsize; i++ ) {
		cost[i][0] = i * occlusion;
		direction[i][0] = UP;
	}
	for ( int i = 1; i < lsize; i++ ) {
		cost[0][i] = i * occlusion;
		direction[0][i] = LEFT;
	}
	for ( int i = 1; i < lsize; i++ ) {
		for ( int j = 1; j < rsize; j++ ) {
			
			double min1 = cost[i-1][j-1] + costFunc( left[i] , right[j] ,wsize);
			double min2 = cost[i-1][j] + occlusion;
			double min3 = cost[i][j-1] + occlusion;

			cost[i][j] = min( min1 ,min(min2 ,min3) );

			//指派方向
			if ( cost[i][j] == min1 ) direction[i][j] = D;
			else if ( cost[i][j] == min2 ) direction[i][j] = UP;
			else if ( cost[i][j] == min3 ) direction [i][j] = LEFT;
			/*printf("計算(%d ,%d) left = %f right = %f\n" ,k ,l , left[k] ,right[l] );
			cout<<"cost：\n";
			printData(cost ,lsize ,rsize);
			cout<<"Direction：\n";
			printData(direction ,lsize , rsize);
			cout<<"==============\n";*/
			

		}
	}

	/*cout<<"cost：\n";
	printData(cost ,lsize ,rsize);
	cout<<"Direction：\n";
	printData(direction ,lsize , rsize);
	cout<<"==============\n";*/
	
	cout<<"Direction：\n";
	printData(direction ,lsize , rsize);
	cout<<"==============\n";
	/*釋出記憶體*/
	for ( int i = 0; i < lsize; i++ )
		delete cost[i];
	delete cost;

	return direction;
}
void printData( double* * const data ,int r ,int c) {
	for ( int i = 0; i < 8; i++ ) {
		for ( int j = 0; j < 8; j++ ) {
			if ( data[i][j] == UP)
				cout << "|\t";
			else if ( data[i][j] == LEFT )
				cout <<"<--\t";
			else if ( data[i][j] == D)
				cout <<"\\\t";
			else 
				cout <<data[i][j] <<" ";
		}
		cout <<"\n";
	}
}
/*測試用*/
double* 
*dpAlgorithm(double* l ,int llength ,double* r ,int rlength ) {
	vector<double*> leftc;
	vector<double*> rightc;

	for ( int i = 0; i < llength; i++ ) {
		double* pc= &l[i];
	
		leftc.push_back(pc);
	}

	for ( int i = 0; i < rlength; i++ ) {
		double* pc= &r[i];

		rightc.push_back(pc);
	}
	cout <<"轉換完畢\n";
	for ( int i = 0; i < llength; i++ ) {
		cout << leftc[i][0]<<" ";
	}

	for ( int i = 0; i < rlength; i++ ) {
		cout << rightc[i][0] <<" ";
	}

	cout<<"計算Dynamic Programming\n";
	double* * result = dpAlgorithm(leftc ,rightc ,1);
	cout<<"計算Dynamic Programming完畢\n";
	
	/*for ( int i = 0; i < llength; i++ ) 
		delete leftc[i];
	for ( int i = 0; i < rlength; i++ )
		delete rightc[i];*/
	

	return result;
}