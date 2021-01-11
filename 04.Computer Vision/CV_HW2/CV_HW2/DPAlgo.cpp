/******************************************
FILE
    DPAlgo.h
PURPOSE
    �i��Dynamic programming algorithm���@�Ǩ禡
	�]��Ohta and Kanade�n��ܤ[ �ҥH�ΧU�е�����k
	�ϥΧU�е����覡 
	C( x ,y ) = 
			C(x-1 ,y-1) + c(x ,y)
	min		C(x-1 ,y ) + Occlusion
			C(x ,y-1 ) + Occlusion

	Occlusion�]���w��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <cmath>
#include "DPAlgo.h"
#include "ArrayManipulation.h"
#include <iostream>

using std::cout;


/*
 �qColor����M�̱����I
 �ǤJ�G���ϩM�k�Ϫ�epipolar line��color�ȥH��window size
 �ǥX�G�۹�������m
*/
double* *
dpAlgorithm(const vector<WColor*>& l , const vector<WColor*>& r ) {
	//�NColor�ন�Ƕ��� ��double array�x�s�_��
	cout<<"��Ƕ��Ȥ�\n";
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
	cout <<"�ഫ����\n";

	cout<<"�p��Dynamic Programming\n";
	double* * result = dpAlgorithm(leftc ,rightc ,wsize);
	cout<<"�p��Dynamic Programming����\n";
	
	for ( int i = 0; i < llength; i++ ) 
		delete leftc[i];
	for ( int i = 0; i < rlength; i++ )
		delete rightc[i];
	

	return result;
}
/*
 �z�LDP algorithm���G ��X�۹��������
 �ǤJ�GDP algo���G 
      �n�Hrow���� ��۹�������٬Ocol d = ROW��Hrow���� d = COL��Hcol����
 �ǥX�G�Hd���Ǫ��۹������G
*/
int* getCorrespondence(double* * dpresult ,int row ,int col ,Direction d) {
	//��l�Ƹ��
	int* cor;
	int size = 0;
	int index = 0;//�ΨӶi�������
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

	
	//�q�̥���}�l������ ��X�Ҧ�D�����p �p�G�S��D�hassign���䪺D���k��
	int r = row-1 , c = col-1;
	int i = size - 1;
	bool ignore = false;
	while( i >= 0  && r >= 0 && c >= 0) {
		/*�p�G�O\ �h��assign�A��*/
		//������
		if ( dpresult[r][c] == D ) {
			if ( d == ROW ) {
				i = r; index = c;
			}
			else if ( d == COL ) {
				i = c; index = r;
			}
					
			//�[�J����
				
			cor[i] = index;
		}
			
		//printf("assign�G�ثe�b(%d ,%d) �ӭ��ק�Fcor[%d] = %d\n" ,c ,r ,i ,index);
		//cout<<"--------------\n";
		
		/*�ק��m��U�@���h*/
		//cout<<"go�G";
		if ( dpresult[r][c] == UP ) {
			//cout<< "��|\n";
			r--;
		}
		else if ( dpresult[r][c] == LEFT ) {
			//cout << "��<--\n";
			c--;
		}
		else if ( dpresult[r][c] == D ) {
			//cout<<"��\\\n";
			r--; c--;
		}
		else break;
		//printf("�ק�F�ثe��m�b(%d ,%d)\n" ,c , r );

		
		
		
	}

	/*for ( int i = 0; i < size; i++ )
		cout <<cor[i]<<" ";*/
	return cor;
}

/*
  �t�d�p��cost�Ϊ�function
  1.paper���p�⪺�覡�����]��a1......ak b1.....bl��interval�ݭn���
    ���D�Ҧ�������M �M�A�DSSB
    M = 1/2 ( 1/k * Sum(ai) + 1/l * Sum(bi) )
    SSB = 1/2 ( 1/k * Sum( (ai - m )^2 ) + 1/l * Sum( (bi - m )^2 )
  2.��ӭȨ�cos �A�ܦ�sin
    �Y�Ȭ۵��h�O0
    �ǤJ�G�ⵧ��� wsize���h��
    �ǥX�Gcost
*/
double 
costFunc(const double* l ,const double* r ,int wsize) {
	/*
	//Paper��k
	int lsize = (int) l.size();
	int rsize = (int) r.size();
	//�D����
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

	//�Dvariance
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
		//�R���O����
		return  1 - cos;
	}
	else {
		return abs(l[0] - r[0]);
	}
	
}
/*
 Dynamic programming�D�n��@�B
 �ǤJ�G��Ӱ}�C �H��window size
 �ǥX�GDynamic programming���G
*/
double* *
dpAlgorithm(const vector<double*>& left ,const vector<double*>& right ,int wsize) {
	int lsize = (int) left.size();
	int rsize = (int) right.size();
	/*��l�Ƭ������*/
	double* *direction = create2DArray(lsize ,rsize ,NOTANUMBER);
	double* *cost = create2DArray(lsize ,rsize , NOTANUMBER);
	
	cost[0][0] = 0; direction[0][0] = D;
	double occlusion = 0.001;
	/*�p��Dynamic programming*/
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

			//������V
			if ( cost[i][j] == min1 ) direction[i][j] = D;
			else if ( cost[i][j] == min2 ) direction[i][j] = UP;
			else if ( cost[i][j] == min3 ) direction [i][j] = LEFT;
			/*printf("�p��(%d ,%d) left = %f right = %f\n" ,k ,l , left[k] ,right[l] );
			cout<<"cost�G\n";
			printData(cost ,lsize ,rsize);
			cout<<"Direction�G\n";
			printData(direction ,lsize , rsize);
			cout<<"==============\n";*/
			

		}
	}

	/*cout<<"cost�G\n";
	printData(cost ,lsize ,rsize);
	cout<<"Direction�G\n";
	printData(direction ,lsize , rsize);
	cout<<"==============\n";*/
	
	cout<<"Direction�G\n";
	printData(direction ,lsize , rsize);
	cout<<"==============\n";
	/*���X�O����*/
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
/*���ե�*/
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
	cout <<"�ഫ����\n";
	for ( int i = 0; i < llength; i++ ) {
		cout << leftc[i][0]<<" ";
	}

	for ( int i = 0; i < rlength; i++ ) {
		cout << rightc[i][0] <<" ";
	}

	cout<<"�p��Dynamic Programming\n";
	double* * result = dpAlgorithm(leftc ,rightc ,1);
	cout<<"�p��Dynamic Programming����\n";
	
	/*for ( int i = 0; i < llength; i++ ) 
		delete leftc[i];
	for ( int i = 0; i < rlength; i++ )
		delete rightc[i];*/
	

	return result;
}