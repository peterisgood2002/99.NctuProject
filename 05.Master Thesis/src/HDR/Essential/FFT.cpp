/******************************************
FILE
    FFT.cpp
PURPOSE
    �i�榳����Fourier Transformation�������B��
	���{���N�|�]�A1-D FFT 2-D FFT �H��1-D iFFT 2-D iFFT�������禡
	���M���w�֪��g�X�FRadix-2 ���t��k 
	
	���]��size�ܤj�� �B��ɶ��w�C �]���M�w�ϥ�FFTW��source code

	


NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "FFT.h"
#include "fftw3.h"
#include <cassert>
#include <iostream>


/*
 *Given an array ,this function will perform 1-D Fast Fourier Transformation
  this algorithm will include the inverse Fast Fourier Transformation for convenience
 *Parameter�Gan data array and does this function perform iFFT
 *Return�GFFT array
          if iFFT is performed ,it'll return the real value of iFFT * M
*/
CNumberArray 
FFT::FFT_1D( const CNumberArray&  f ,Mode m ) {
	int size = f.getSize();
	fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size );

	for ( int i = 0; i < size; i++ ) {
		in[i][0] = f[i].getReal();
		in[i][1] = f[i].getImagery();
		
	}
	
	
	fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size );

	
	fftw_plan p = fftw_plan_dft_1d(size ,in ,out ,m, FFTW_ESTIMATE );
	
	fftw_execute(p);
	
	CNumberArray result( size );
	for ( int i = 0; i < size; i++ ) 
		result[i].setData( out[i][0] ,out[i][1] );
	

	fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

	return result;
}
/*Perform a Radix-2 FFT algorithm
  Parameter�Git'll treat the matrix as a 1-D element 
             which perform Radix-2 FFT according to the row element
			 plz ensure the row size is 2^n or it'll abort the program
  Return�Gthe result of FFT
*/
//static CNumberArray Radix_2_FFT_1D( const CNumberMatrix& d  ,Mode m);
/*Perform the inverse Fourier transformation of the array*/
CNumberArray 
FFT::iFFT_1D( const CNumberArray&  F ) {
	CNumberArray f = FFT_1D( F ,INVERSE );

	int size = f.getSize();
	for ( int i = 0; i < size; i++ )
		f[i] /= size;

	return f;
}
/*Given an 2D array ,this function will perform 2-D Fast Fourier Transformation
  this algorithm will include the inverse Fast Fourier Transformation for convenience
 *Parameter�Gan 2D array and does this function perform iFFT
 *Return�GFFT array
           if iFFT is performed ,it'll return the real value of iFFT * M
 */
CNumberMatrix 
FFT::FFT_2D( const CNumberMatrix& f ,Mode m ) {
	int row = f.getRowSize();
	int col = f.getColSize();

	fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (row * col) );
	
	int index = 0;
	for ( int i = 0; i < row ; i++ ) {
		for ( int j = 0; j < col; j++ ) {

			in[index][0] = f[i][j].getReal();
			in[index][1] = f[i][j].getImagery();
		
			index++;
		}

	}
	
	
	
	fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (row * col) );
	fftw_plan p = fftw_plan_dft_2d( row ,col ,in ,out ,m, FFTW_ESTIMATE );
	fftw_execute(p);
	

	
	CNumberMatrix r( row ,col );
	index = 0;
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) {
			r[i][j].setData( out[index][0] ,out[index][1] );
			index++;
		}
		
	
	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);

	return r;
}
CNumberMatrix 
FFT::iFFT_2D( const CNumberMatrix& F  ) {
	CNumberMatrix r = FFT_2D( F ,INVERSE );

	int row = r.getRowSize();
	int col = r.getColSize();
	int size = row * col;

	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			r[i][j] /= size;

	return r;
}


/*===========================================
��F�n�h�ɶ��g�n���{���X �n���˱o�R��
���O�S��k �B��t�פӺC�F
�u�n�d�۰����� �s!!�ڪG�M���j�|�g�{��
============================================*/
//
///*
// *given an array ,this function will perform 1-D Fast Fourier Transformation
//  this algorithm will include the inverse Fast Fourier Transformation for convenience
// *Parameter�Gan data array and does this function perform iFFT
// *Return�GFFT array
//*/
///*�ϥ�CLR���F��*/
//#using <mscorlib.dll>
//using System::Math;
//enum Z2  { Even = 0 ,Odd };
///*�ھ�odd or even ��Xd array�����_��F�ΰ���F �٦��@�w�n���w�쩳�O�nFFT�٬OiFFT
// *Parameter�G
//  odd or even �H��d array and does this function perform iFFT
//*/
//int testNPower2( int n );//It'll test n is power of 2
//CNumberArray  FT ( const CNumberArray&  f ,FFT::Mode minus );
//CNumberArray 
//FFT::FFT_1D( const CNumberArray&  f ,Mode minus ) {
//	/*
//	 Implement Note�G
//	        As mentioned in Cooley�VTukey FFT algorithm, we can separate the N into N1 * N2.
//			We can see the inner sum is just a FT of size N2.
//			As I derivate from the equation ,I can see the following result
//			0     0
//			1     2---(1)
//			2     4
//			3  ---------
//			4     1
//			5     3---(2)
//			6     5
//
//			N1 = row N2 = col
//
//			So i rearange the array into the matrix and perform Radix-2 algorithm depending on row
//	 Reference�G
//	      http://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm#cite_note-DuhamelVe90-6
//
//
//	*/
//	int size = f.getSize();
//	int N2 = testNPower2( size ); 
//	int N1 = size / N2;
//
//	if ( N1 > 1 ) {
//		CNumberMatrix fm(N1 ,N2);
//	
//		for ( int n1 = 0; n1 < N1; n1++ ) {
//			CNumberArray d( N2 );
//
//			for ( int n2 = 0; n2 < N2; n2++ ) 
//				d.setData( n2 , f[ n2 * N1 + n1 ] );
//			
//			CNumberArray r = FT( d ,minus );
//
//			/*
//			 ���WTwiddle factor
//
//			 e^ j(-2pi*n1* k2/N 
//			*/
//			for ( int k2 = 0; k2 < N2; k2++ ) {
//				double theta = minus * 2 * Math::PI * n1 * k2 / size;
//				ComplexNumber TF ( Math::Cos(theta) ,Math::Sin(theta) );
//				r[k2] *= TF;
//			}
//
//			fm.setRow( n1 ,  r );
//
//		}
//	
//		//std::cout << fm <<"\n";
//		CNumberArray F = Radix_2_FFT_1D( fm ,minus );
//
//		return F;
//	}
//	else return FT( f ,minus );
//	
//}
//CNumberArray FT ( const CNumberArray&  f ,FFT::Mode minus ) {
//	
//	/*
//	  F(u) = sum(  f(x) * Wn^ux )         Wn = e^(j2pi/n) = cosx + j sinx   x = j2pi/n
//	*/
//	int size = f.getSize();
//	if ( size == 1 ) 
//		return f;
//	CNumberArray F( size );
//
//	for ( int u = 0; u < size; u++ ) {
//
//		for ( int x = 0; x < size; x++ ) {
//			double theta = minus * 2 * Math::PI * u * x / size;
//			ComplexNumber Wnku ( Math::Cos(theta) ,Math::Sin(theta) );
//			F[u] += f[x] *  Wnku;
//		}
//	}
//
//	return F;
//
//}
///*Perform a Radix-2 FFT algorithm
//  Parameter�Git'll treat the matrix as a 1-D element 
//             which perform Radix-2 FFT according to the row element
//			 plz ensure the row size is 2^n or it'll abort the program
//  Return�Gthe result of FFT
//*/
//CNumberArray
//FFT::Radix_2_FFT_1D( const CNumberMatrix& f  ,Mode minus ) {
//	/*�Բӱ��ɹL�{ �Ьݻ��j�����{��*/
//	int s = f.getRowSize();
//	assert( testNPower2(s) == 1 );
//	
//	int n = int ( Math::Log( s ,2 ) );
//	/*�� iteration��Fourier Transformation*/
//	/*
//	 3.�z�Lhttp://www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html
//	   �ڭ̥i�H�N��ƥ��\��
//	   0   4 || 2   6    |||  1   5 || 3   7
//	   �H��Kiteration�p��
//	   �ھڥi�Ȫ��[�� �ڭ̥i�H�o�{
//	   (n2 n1 n0 ) --->(n0 n2 n1 ) --->(n0 n1 n2)  
//	   ==>
//	      �ڭ̦p�G�n��n bit�i�歫�� �ڭ̥����n���V�k���� n-2���H�� �b���@���̫���bit���洫
//	*/
//	CNumberMatrix F = f;//Rearange�����G
//	for ( int i = 0; i < s; i++ ) {
//		int ni = i;
//		if ( s > 2 ) {
//			//��Xi�۹�����location�b����
//			int n2lsb = int( Math::Pow(2 , n-2 ) - 1 ) & ni;//���Xn-2��LSB
//			n2lsb <<= 2;//�V�������
//			ni >>= ( n - 2 );
//			//�̫���bit�洫
//			if ( ni == 1 )
//				ni++;
//			else if ( ni == 2 )
//				ni--;
//			//�X�b�@�_
//			ni |= n2lsb;
//			
//		}
//		//��F��
//		F[i] = f[ni];
//		
//	}
//
//	//std::cout << F;
//	//�}�l�i��Fourier Transformation
//
//	for ( int ctr = 0; ctr <= n; ctr++ ) {//�@��n���n�i��
//		int g = int ( Math::Pow( 2 ,ctr ) );
//		for ( int i = 0; i < s; i+=g ) {
//			int pair = g / 2;
//			int u = 0;
//			for ( int j = i; j < i + pair; j++ ) {
//				CNumberArray Fe = F[j];
//				CNumberArray Fo = F[j+pair];
//				double theta = minus * 2 * Math::PI * (u++) / g;
//				ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//
//				F[j] = Fe + W2ku * Fo;
//				
//				F[j+pair] = Fe - W2ku * Fo;
//
//				//std::cout<< F[j] << "\n" << F[j+pair] << W2ku<<"\n";
//				
//			}
//			
//			
//		}
//		
//	}
//
//	//std::cout << F << "\n";
//	CNumberArray result( f.getColSize() * f.getRowSize() );
//
//	int index = 0;
//	for ( int i = 0; i < f.getRowSize(); i++ ) {
//		for ( int j = 0; j < f.getColSize(); j++ ) 
//			result[index++] = F[i][j];
//	}
//	return result;
//
//
//}
///*
// *It'll test n is power of 2
//  If it is ,it'll return 1
//  If it is not ,it'll return the factor
//  for instance , 6 is not a power of 2 it'll return 3
// *Parameter�G
//    n to be test
// *Return�G
//    the indicate result
//*/
//int testNPower2( int n ) {
//	while ( n % 2 == 0 ) 
//		n /= 2;
//	
//
//	return n;
//}
////���j���� �]������t�׸��C �ҥH�ثe�S�b�ϥ� �p�G�n�λݭn�i��ק� �]�����prototype����L�F
//CNumberArray FourierTransform(Z2 oore, const CNumberArray& d ,FFT::Mode minus );
//CNumberArray
//recursive_FFT_1D( const CNumberArray& d ,FFT::Mode minus) {
//	/*
//	  1.�ھ�http://www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html �ҥ�
//	    �쥻��Fourier transformation
//		    
//			F(u) = sum(  f(x) * Wn^ux )         Wn = e^(j2pi/n) = cosx + j sinx   x = j2pi/n
//	    
//		�ڭ̱N���_�ƩM���ƪ��ۥ[ �]�N�O��
//		       
//            F(u) = sum( f(2m) * Wn^ux) + sum ( f(2m+1) *Wn^ux )   n = 2m
//			    ^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^
//				      n���_��               n������
//	    
//			
//			F(u) = sum( f(2m) * Wn^u2m) + sum [ f(2m+1) *Wn^u(2m+1) ]
//			  
//			     = sum( f(2m) * Wn^u2m) + Wn * sum [ f(2m+1) *Wn^u(2m) ]
//	               ^^^^^^^^^^^^^^^^^^^^        ^^^^^^^^^^^^^^^^^^^^^^^^^
//				 =        Fe(u)         + Wn *         Fo(u)
//
//	  2.�䤤
//	     
//		   Wn^u2m = e^(j2pi*u2m/n) = e^(j2pi*u2m/n) = e^(j2pi*um/n) = Wm^ um
//
//	       
//		===>F(u) = sum( f(2m) * Wm^um)  + Wn * sum [ f(2m+1) *Wm^um ]    n = 2m
//		   
//		   �䤤�]��
//		   Wm^(u+m) = e^[(j2pi/m)*(u+m)] = e^[(j2pi/m)*u] = Wm^u
//		   W2m^(u+m) = e^[(j2pi/2m)*(u+m)] = e^[(j2pi/2m)*u] *e^[(j2pi/m) ] = -W2m^u
//
//	    ===>F(u+ m) = Fe(u) - Wn * Fo(u)
//
//		���y�ܻ� �u�n�D�X�@�b��F1�MF2 �N�i�H�D�X�@�䪺F �D�X�@�䪺F �N�i�H�D�X�t�@�䪺F
//	  3.�Ъ`�N
//	       F0(u) = sum( f(2m) * Wm^um)  �p�G�ڭ̧�f(2m)�令g(m)
//	    �ڭ̥i�H�o�{F0(u)���N�Og��Fourier transform
//	
//	  4.�Ҥl�G
//	   
//
//	    0   1   2   3   4   5   6   7
//
//		0   2   4   6  ||| 1   3   5   7
//
//		0   4 || 2   6    |||  1   5 || 3   7
//	
//	*/
//	int s = d.getSize();
//	if ( s == 1 ) {
//		return const_cast< CNumberArray& > (d);
//	}
//	
//	if ( s == 2 ) {
//		//�ھڤ��� �L�N�O�L����
//		double theta = minus * 2 * Math::PI * 0 * 0 / s / 2;
//		ComplexNumber Wku ( Math::Cos(theta) ,Math::Sin(theta) );
//		ComplexNumber F0 = d[0] * Wku;
//		theta = minus * 2 * Math::PI * 0 / 2 / s;
//		ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//		ComplexNumber F1 = d[1] * Wku;
//		CNumberArray r(s);
//		//�p��r[0]
//		r[0] = F0 + W2ku * F1;
//		//�p��r[1]
//		r[1] = F0 - W2ku * F1;
//		
//		return r;
//	}
//	
//	
//	int size = s;
//	if ( s % 2 != 0 ) //���ջ~�ˬd �p�G�j�p���O �h���[�@�N�n �o�˴N���ƤF
//		size = s + 1;
//
//	CNumberArray f = d;//�n�i�h�⪺f
//	if ( size == s + 1 )//���ջ~�ˬd �O�o�]��Fourier�����Ʃ� �]�����Y�[�b���ڴN�O�F
//		f[s] = d[0];
//
//	//std::cout<<f;
//	/*��l��*/
//	CNumberArray FT(size );
//	
//	//��XF0�� �L�N�|�h��X���Ʀ檺Fourier transformation���G
//	CNumberArray Fe = FourierTransform(Even ,f ,minus);
//	//��XF1���L�N�|�h��X�Ʀ檺Fourier transformation���G
//	CNumberArray Fo = FourierTransform(Odd ,f ,minus );
//
//	/*�u�n�D�X�@�b��F0�MF1 �t�@�b�۵M�Ӹ�*/
//	double theta = 0;
//	for ( int u = 0; u < size/2; u++ ) {//���b
//		theta = minus * 2 * Math::PI * u / size;
//		ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//		FT[u] = Fe[u] + W2ku * Fo[u];
//	}
//	for ( int u = 0; u < size/2; u++ ) {//�k�b
//		theta = minus * 2 * Math::PI * u / size;
//		ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//		FT[u] = Fe[u] - W2ku * Fo[u];
//	}
//
//	return FT;
//}
//
///*Perform the inverse Fourier transformation of the array*/
//CNumberArray
//FFT::iFFT_1D( const CNumberArray& F ) {
//	CNumberArray iF = FFT_1D(F ,INVERSE );
//
//	int size = iF.getSize();
//	for ( int i = 0; i < size; i++ ) 
//		iF[i] /= size;
//		
//	return iF;
//}
///*�ھ�odd or even ��Xd array�����_��F�ΰ���F �٦��@�w�n���w�쩳�O�nFFT�٬OiFFT
// *Parameter�G
//  odd or even �H��d array
//*/
//CNumberArray FourierTransform(Z2 oore, const CNumberArray& d ,FFT::Mode minus ) {
//	int size = d.getSize();
//	assert( size % 2 == 0 );
//
//	CNumberArray f (size/2);
//
//	int index = 0;
//	for ( int i = oore; i < size; i+=2 )
//		f[index++] = d[i];
//
//	CNumberArray F = FFT::FFT_1D( f ,minus );
//	
//
//	return F;
//
//	
//}
///*Given an 2D array ,this function will perform 2-D Fast Fourier Transformation
//  this algorithm will include the inverse Fast Fourier Transformation for convenience
// *Parameter�Gan 2D array and does this function perform iFFT
// *Return�GFFT array
// */
//CNumberMatrix
//FFT::FFT_2D( const CNumberMatrix&  f ,Mode minus ) {
//	
//	/*
//	 �Ьݽҥ� �ҥ������F���2D FFT�u�O�N1D����M�Ӥw
//
//	 --------------->x
//	 |
//	 |
//	 |
//	 v
//	 y
//	*/
//	
//	/*�ջ~���� ��dimension��2�����ƺ�_�Ӥ~���|�걼
//	  �Ъ`�N ��col�Ө��o����·� �Ҧp�G
//	  3 * 2�Ө� ��Y�@col��FT�L�|���X�h �ڭ̤����H�K �屼 �]���o�˵��G�|�����D
//	  �ҥH�~�n�b�o����@��X��
//	*/
//	/*�ջ~���� �ثe�u�n����o�˴N�n �]����Row�Ө�push_back���S�ƻ�t��ɭ�1D FFT�|�N�۰ʱN�F��[�W�h �ڭ̥u�n�T�O�L�����G������X�Y�i*/
//	CNumberMatrix F ( f.getRowSize() ,f.getColSize() );
//
//	for ( int y = 0; y < f.getRowSize(); y++ ) {//��C�@��row���@�M
//		//std::cout<<"\r����F��"<<y<<"��row";
//		F.setRow( y , FFT_1D( f[y] ,minus ) );
//	}
//	
//	//std::cout << F;
//	for ( int x = 0; x < F.getColSize(); x++ ) {//�b��F�C�@��col���@�M
//		//std::cout<<"\r����F��"<<x<<"��col";
//		
//		CNumberArray fx = F.getCol(x);
//		
//		CNumberArray Fx = FFT_1D( fx ,minus );
//
//		//������^�h
//		F.setCol( x ,Fx );
//
//	}
//
//	return F;
//	
//}
//CNumberMatrix
//FFT::iFFT_2D( const CNumberMatrix& F ) {
//	CNumberMatrix iF = FFT_2D(F ,FFT::FORWARD );
//
//	int row = iF.getRowSize();
//	int col = iF.getColSize();
//	for ( int i = 0; i < row; i++ ) {
//		for ( int j = 0; j < col; j++ )
//			iF[i][j] /= row * col;
//	}
//
//	return iF;
//}
//
