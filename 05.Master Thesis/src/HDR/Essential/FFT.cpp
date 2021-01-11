/******************************************
FILE
    FFT.cpp
PURPOSE
    進行有關於Fourier Transformation的相關運算
	此程式將會包括1-D FFT 2-D FFT 以及1-D iFFT 2-D iFFT的相關函式
	雖然很歡樂的寫出了Radix-2 的演算法 
	
	但因為size很大時 運算時間緩慢 因此決定使用FFTW的source code

	


NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "FFT.h"
#include "fftw3.h"
#include <cassert>
#include <iostream>


/*
 *Given an array ,this function will perform 1-D Fast Fourier Transformation
  this algorithm will include the inverse Fast Fourier Transformation for convenience
 *Parameter：an data array and does this function perform iFFT
 *Return：FFT array
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
  Parameter：it'll treat the matrix as a 1-D element 
             which perform Radix-2 FFT according to the row element
			 plz ensure the row size is 2^n or it'll abort the program
  Return：the result of FFT
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
 *Parameter：an 2D array and does this function perform iFFT
 *Return：FFT array
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
花了好多時間寫好的程式碼 好不捨得刪喔
但是沒辦法 運算速度太慢了
只好留著做紀念 哀!!我果然不大會寫程式
============================================*/
//
///*
// *given an array ,this function will perform 1-D Fast Fourier Transformation
//  this algorithm will include the inverse Fast Fourier Transformation for convenience
// *Parameter：an data array and does this function perform iFFT
// *Return：FFT array
//*/
///*使用CLR的東西*/
//#using <mscorlib.dll>
//using System::Math;
//enum Z2  { Even = 0 ,Odd };
///*根據odd or even 找出d array中的奇數F或偶數F 還有一定要給定到底是要FFT還是iFFT
// *Parameter：
//  odd or even 以及d array and does this function perform iFFT
//*/
//int testNPower2( int n );//It'll test n is power of 2
//CNumberArray  FT ( const CNumberArray&  f ,FFT::Mode minus );
//CNumberArray 
//FFT::FFT_1D( const CNumberArray&  f ,Mode minus ) {
//	/*
//	 Implement Note：
//	        As mentioned in Cooley–Tukey FFT algorithm, we can separate the N into N1 * N2.
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
//	 Reference：
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
//			 乘上Twiddle factor
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
//  Parameter：it'll treat the matrix as a 1-D element 
//             which perform Radix-2 FFT according to the row element
//			 plz ensure the row size is 2^n or it'll abort the program
//  Return：the result of FFT
//*/
//CNumberArray
//FFT::Radix_2_FFT_1D( const CNumberMatrix& f  ,Mode minus ) {
//	/*詳細推導過程 請看遞迴版的程式*/
//	int s = f.getRowSize();
//	assert( testNPower2(s) == 1 );
//	
//	int n = int ( Math::Log( s ,2 ) );
//	/*做 iteration之Fourier Transformation*/
//	/*
//	 3.透過http://www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html
//	   我們可以將資料先擺成
//	   0   4 || 2   6    |||  1   5 || 3   7
//	   以方便iteration計算
//	   根據可怕的觀察 我們可以發現
//	   (n2 n1 n0 ) --->(n0 n2 n1 ) --->(n0 n1 n2)  
//	   ==>
//	      我們如果要對n bit進行重排 我們必須要先向右旋轉 n-2次以後 在做一次最後兩個bit的交換
//	*/
//	CNumberMatrix F = f;//Rearange的結果
//	for ( int i = 0; i < s; i++ ) {
//		int ni = i;
//		if ( s > 2 ) {
//			//找出i相對應的location在哪裡
//			int n2lsb = int( Math::Pow(2 , n-2 ) - 1 ) & ni;//取出n-2個LSB
//			n2lsb <<= 2;//向左移兩格
//			ni >>= ( n - 2 );
//			//最後兩個bit交換
//			if ( ni == 1 )
//				ni++;
//			else if ( ni == 2 )
//				ni--;
//			//合在一起
//			ni |= n2lsb;
//			
//		}
//		//放東西
//		F[i] = f[ni];
//		
//	}
//
//	//std::cout << F;
//	//開始進行Fourier Transformation
//
//	for ( int ctr = 0; ctr <= n; ctr++ ) {//共有n次要進行
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
// *Parameter：
//    n to be test
// *Return：
//    the indicate result
//*/
//int testNPower2( int n ) {
//	while ( n % 2 == 0 ) 
//		n /= 2;
//	
//
//	return n;
//}
////遞迴版的 因為執行速度較慢 所以目前沒在使用 如果要用需要進行修改 因為整個prototype有改過了
//CNumberArray FourierTransform(Z2 oore, const CNumberArray& d ,FFT::Mode minus );
//CNumberArray
//recursive_FFT_1D( const CNumberArray& d ,FFT::Mode minus) {
//	/*
//	  1.根據http://www.cmlab.csie.ntu.edu.tw/cml/dsp/training/coding/transform/fft.html 所示
//	    原本的Fourier transformation
//		    
//			F(u) = sum(  f(x) * Wn^ux )         Wn = e^(j2pi/n) = cosx + j sinx   x = j2pi/n
//	    
//		我們將其拆成奇數和偶數的相加 也就是說
//		       
//            F(u) = sum( f(2m) * Wn^ux) + sum ( f(2m+1) *Wn^ux )   n = 2m
//			    ^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^
//				      n為奇數               n為偶數
//	    
//			
//			F(u) = sum( f(2m) * Wn^u2m) + sum [ f(2m+1) *Wn^u(2m+1) ]
//			  
//			     = sum( f(2m) * Wn^u2m) + Wn * sum [ f(2m+1) *Wn^u(2m) ]
//	               ^^^^^^^^^^^^^^^^^^^^        ^^^^^^^^^^^^^^^^^^^^^^^^^
//				 =        Fe(u)         + Wn *         Fo(u)
//
//	  2.其中
//	     
//		   Wn^u2m = e^(j2pi*u2m/n) = e^(j2pi*u2m/n) = e^(j2pi*um/n) = Wm^ um
//
//	       
//		===>F(u) = sum( f(2m) * Wm^um)  + Wn * sum [ f(2m+1) *Wm^um ]    n = 2m
//		   
//		   其中因為
//		   Wm^(u+m) = e^[(j2pi/m)*(u+m)] = e^[(j2pi/m)*u] = Wm^u
//		   W2m^(u+m) = e^[(j2pi/2m)*(u+m)] = e^[(j2pi/2m)*u] *e^[(j2pi/m) ] = -W2m^u
//
//	    ===>F(u+ m) = Fe(u) - Wn * Fo(u)
//
//		換句話說 只要求出一半的F1和F2 就可以求出一邊的F 求出一邊的F 就可以求出另一邊的F
//	  3.請注意
//	       F0(u) = sum( f(2m) * Wm^um)  如果我們把f(2m)改成g(m)
//	    我們可以發現F0(u)其實就是g的Fourier transform
//	
//	  4.例子：
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
//		//根據公式 他就是他本身
//		double theta = minus * 2 * Math::PI * 0 * 0 / s / 2;
//		ComplexNumber Wku ( Math::Cos(theta) ,Math::Sin(theta) );
//		ComplexNumber F0 = d[0] * Wku;
//		theta = minus * 2 * Math::PI * 0 / 2 / s;
//		ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//		ComplexNumber F1 = d[1] * Wku;
//		CNumberArray r(s);
//		//計算r[0]
//		r[0] = F0 + W2ku * F1;
//		//計算r[1]
//		r[1] = F0 - W2ku * F1;
//		
//		return r;
//	}
//	
//	
//	int size = s;
//	if ( s % 2 != 0 ) //做試誤檢查 如果大小不是 則讓加一就好 這樣就偶數了
//		size = s + 1;
//
//	CNumberArray f = d;//要進去算的f
//	if ( size == s + 1 )//做試誤檢查 記得因為Fourier的重複性 因此把頭加在尾巴就是了
//		f[s] = d[0];
//
//	//std::cout<<f;
//	/*初始化*/
//	CNumberArray FT(size );
//	
//	//算出F0的 他將會去算出偶數行的Fourier transformation結果
//	CNumberArray Fe = FourierTransform(Even ,f ,minus);
//	//算出F1的他將會去算出數行的Fourier transformation結果
//	CNumberArray Fo = FourierTransform(Odd ,f ,minus );
//
//	/*只要求出一半的F0和F1 另一半自然而解*/
//	double theta = 0;
//	for ( int u = 0; u < size/2; u++ ) {//左半
//		theta = minus * 2 * Math::PI * u / size;
//		ComplexNumber W2ku ( Math::Cos(theta) ,Math::Sin(theta) );
//		FT[u] = Fe[u] + W2ku * Fo[u];
//	}
//	for ( int u = 0; u < size/2; u++ ) {//右半
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
///*根據odd or even 找出d array中的奇數F或偶數F 還有一定要給定到底是要FFT還是iFFT
// *Parameter：
//  odd or even 以及d array
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
// *Parameter：an 2D array and does this function perform iFFT
// *Return：FFT array
// */
//CNumberMatrix
//FFT::FFT_2D( const CNumberMatrix&  f ,Mode minus ) {
//	
//	/*
//	 請看課本 課本說明了其實2D FFT只是將1D做兩遍而已
//
//	 --------------->x
//	 |
//	 |
//	 |
//	 v
//	 y
//	*/
//	
//	/*試誤測驗 讓dimension為2的倍數算起來才不會爛掉
//	  請注意 對col而言這比較麻煩 例如：
//	  3 * 2而言 對某一col做FT他會長出去 我們不能隨便 砍掉 因為這樣結果會有問題
//	  所以才要在這邊長一行出來
//	*/
//	/*試誤測驗 目前只要做到這樣就好 因為對Row而言push_back其實沒甚麼差到時候1D FFT會將自動將東西加上去 我們只要確保他的結果完全輸出即可*/
//	CNumberMatrix F ( f.getRowSize() ,f.getColSize() );
//
//	for ( int y = 0; y < f.getRowSize(); y++ ) {//對每一個row做一遍
//		//std::cout<<"\r做到了第"<<y<<"個row";
//		F.setRow( y , FFT_1D( f[y] ,minus ) );
//	}
//	
//	//std::cout << F;
//	for ( int x = 0; x < F.getColSize(); x++ ) {//在對F每一個col做一遍
//		//std::cout<<"\r做到了第"<<x<<"個col";
//		
//		CNumberArray fx = F.getCol(x);
//		
//		CNumberArray Fx = FFT_1D( fx ,minus );
//
//		//哈哈放回去
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
