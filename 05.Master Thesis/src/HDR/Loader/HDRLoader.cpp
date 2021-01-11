/******************************************
FILE
    HDRLoader.cpp
PURPOSE
    定義讀入HDR格式的member function

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <stdlib.h>
#include <cmath>

#include "../ColorSpace/RGB2LAB.h"
#include "HDRLoader.h"


using std::cerr;
using std::cout;
using std::ifstream;
using std::string;
using std::exception;

const float HDRLoader::DEFAULT = 1;
const float HDRLoader::NOTANUMBER = -1;
const string HDRLoader::RGB = "rgb";
const string HDRLoader::XYZ = "xyz";

/*Constuctor*/
HDRLoader::HDRLoader(){
	init();
};
HDRLoader::HDRLoader( const string& fname ) {
	init();
	if ( fname.find(".hdr") != fname.npos )
		readImage(fname);
	else throw "the file extension is not \".hdr\" ";

}
//the "Big Three"
/*Copy Constructor*/
HDRLoader::HDRLoader( const HDRLoader& loader) {
	width = loader.width;
	height = loader.height;

	hardCopy( loader);
}

/**Overloading the assignment operator*/
HDRLoader& 
HDRLoader::operator= (const HDRLoader& loader ) {
	if (this == &loader ) return *this;

	width = loader.width;
	height = loader.height;

	dispose( C1 ,height ,width );
	dispose( C2 ,height ,width );
	dispose( C3 ,height ,width );
	dispose( E ,height ,width );

	hardCopy(loader);

	return *this;
}
/*Destructor*/
HDRLoader::~HDRLoader() {
	if ( headinfo.rgb2lab != NULL )
		delete headinfo.rgb2lab;
	if ( C1 != NULL )
		dispose( C1 ,height ,width );
	if ( C2 != NULL )
		dispose( C2 ,height ,width );
	if ( C3 != NULL )
		dispose( C3 ,height ,width );
	if ( E != NULL )
		dispose( E ,height ,width );
}
//初始化相關資料
void 
HDRLoader::init() {
	//header設定
	headinfo.format = "";
	headinfo.exposure = DEFAULT;
	headinfo.colorcorr = DEFAULT;
	headinfo.pixelratio = DEFAULT;//no PIXASPECT assignment appears, the ratio is assumed to be 1.
	headinfo.rgb2lab = new RGB2LAB();
	//
	width = 0;
	height = 0;
	//
	//pixels = NULL;
	
}
/*
 讀入HDR 影像
 傳入：HDR影像的file name
 傳出：讀取是否成功
 Exception：若讀入檔案失敗 將會傳出Exception
*/
bool 
HDRLoader::readImage( const string& fname )  {
	cout<<"HDRLoader:讀入："<< fname << "中\n";

	ifstream input( fname.c_str() ,ifstream::binary );//以binary方式讀入

	input.exceptions( ifstream::failbit | ifstream::badbit );//只有在fail bit狀況下才會丟出錯誤訊息

	try {

		//讀取Header資訊
		
		readHeader( input );
		//讀取resolution string
		int x = 0 , y = 0 ,ix = 0 ,iy = 0;//增加的方向
		readResolution( input ,x ,y ,ix ,iy);
		//讀取scanline record
		readScanline(input ,x ,y ,ix ,iy );

		cout <<"==============\n";
		return true;

	}
	catch ( exception e ) {
		cerr << e.what() <<"\n";

		return false;
	}

}

//讀取HDR file的Header部分 並且設定相關資訊
void 
HDRLoader::readHeader( ifstream& input) {

	const int len = 100;
	char ch[len] = "";
	input.getline( ch ,len );
	string header = ch;
	/*header = ch;
	input >> header;*/
	if ( header != "#?RADIANCE" )
		throw exception("非標準的HDR 檔案格式" );

	while ( header != "" )  {//the end of the header is indicated by an empty line.
		//input >> header; 
		input.getline( ch ,len );
		header = ch;

		size_t equalp = header.find("=");
		if ( equalp == string::npos ) continue;//沒找到=表示這不是variable assignment

		//找到 =  進行變數的assign
		if ( header.find("FORMAT") != string::npos) {
			//找尋是否有多的FORMAT 不能有多的 
			if ( headinfo.format != "" )
				throw exception("At most one FORMAT line is allowed\n" );
				
			string f  = header.substr(equalp+1);

			if ( f != "32-bit_rle_rgbe" && f != "32-bit_rle_xyze" )
				throw exception( "it must be assigned a value of either \"32- bit_rle_rgbe\" or \"32-bit_rle_xyze\"\n" );

			headinfo.format = f;
		}
		else if ( header.find("EXPOSURE") != string::npos ) {
			//已經先被乘到每一個pixel上面的值 所以等等就要除回來
			//must be cumulative
			string f  = header.substr(equalp+1);
			
			headinfo.exposure *= (float) atof( f.c_str() );

		}
		else if ( header.find("COLORCORR") != string::npos ) {
			/*
			  Similar to the EXPOSURE variable except given in three parts for the three primaries.
			  the value should have a brightness of unity, so that it does not affect the actual brightness of pixels
			*/
			string f  = header.substr(equalp+1);
			
			headinfo.colorcorr *= (float) atof( f.c_str() );

		}
		else if ( header.find("PIXASPECT") != string::npos ) {
			//a decimal fraction of the height of each pixel to its width
			string f  = header.substr(equalp+1);

			headinfo.pixelratio *= (float) atof( f.c_str() );
		}
		else if ( header.find("PRIMARIES") != string::npos ) {
			/*
			 The CIE (x,y) chromaticity coordinates of the three (RGB) primaries and 
			 the white point used to standardize the picture's color system.
			 用來進行RGB--> XYZ轉換用
			*/
			string f  = header.substr(equalp+1);
			
			//取出資訊
			//請注意 這邊會有Memory Leakage 應該要被解決之
			float c[3][3] = { 0 };
			
			float w[3] = { 0 };
			

			sscanf( const_cast<char *> ( f.c_str() ) ,"%lf %lf %lf %lf %lf %lf %lf %lf\n" ,
							&c[0][0] ,&c[1][0] ,&c[0][1] ,&c[1][1] ,&c[0][2] ,&c[1][2] ,&w[0] ,&w[1]);

			for ( int i = 0; i < 3; i++ )
				c[2][i] = 1 - c[0][i] - c[1][i];

			w[2] = 1 - w[0] - w[1];

			ColorSpaceTransformation* tmpcs = headinfo.rgb2lab;

			float* *d = create2DArray( 3 ,3 );
			for ( int i = 0; i < 3; i++ )
				for ( int j = 0; j < 3; j++ )
					d[i][j] = c[i][j];

			headinfo.rgb2lab = new RGB2LAB(d ,w);
			dispose( d ,3 ,3 );
			delete tmpcs;	
			
			

		}
	}
	
}
/*
 讀取Resolution大小 並且決定X軸及Y軸走向
 檔案中的X Y coordinate為

 y
 ^
 |
 |
 |
 |
 |
 ------------------>x
 而根據檔案格式說明書
 -Y +X 此表示為從影像左上角開始處理
 我們必須在下面的樣子 以方便傳統
   --------------->x
   |
   |
   |
   |
   v
	  
   y


 傳入：處理輸入檔案類別 X軸與Y軸增加走向
 傳出：此函式會修改ix iy變數以方便指定走向
*/
void 
HDRLoader::readResolution( std::ifstream& input ,int& x ,int& y ,int& ix ,int& iy) {

	char signy ='+' ,str ='X' ,signx = '+';
	input >> signy >> str >> height;
	input >> signx >> str >> width;

	//初始化1D陣列
	C1 = create2DArray(height ,width );
	C2 = create2DArray(height ,width );
	C3 = create2DArray(height ,width );
	E = create2DArray(height ,width );

	//設定x y軸起始位置以及走向
	if ( signy =='-' ) {
		iy = 1; y = 0;
	}else {
		iy = -1; y = height;
	}
	
	if ( signx == '+' ) {
		ix = 1; x = 0;
	}else {
		ix = -1; x = width;
	}
	
	input.ignore(1);//換行符號不要
	
}
/*
  讀取Pixel資料
  傳入：處理輸入檔案類別 X軸與Y軸起始位置以及增加走向
*/
const int MIN = 8;//
const int MAX = 0x7FFF;
//根據channel和value設定pixel值
void 
HDRLoader::readScanline( ifstream& input ,int x ,int y ,int ix ,int iy) {
	
	cout<<"HDRLoader:讀取Pixel資料中 請稍候\n";
	
	if ( headinfo.format.find("rle" ) == string::npos && ( width < MIN || width > MAX ) ) {
		//Uncompressed mode
		/*
		 如果是run length encoding他會在format會有rle的字樣
		 Each scanline is represented by M pixels with 4 bytes per pixel,
		*/
		char rgbe[4];
		for ( y; y >= 0 && y <= height; y+=iy ) {

			for ( x; x >= 0 && x <= width; x+=ix ) {
				input.read(rgbe ,4);

				C1[y][x] = float( rgbe[0] );
				C2[y][x] = float( rgbe[1] );
				C3[y][x] = float( rgbe[2] );
				E[y][x] = float( rgbe[3] );
				
			}
		}

	}else {
		//adaptive run length encoding
		/*
		1.run length encoding
		  The record begins with an unnormalized pixel having two bytes equal to 2, 
		  followed by the upper byte and the lower byte of the scanline length 
		  (which must be less than 32768).
		  例子：

		      WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW

			  encode：12W1B12W3B24W1B14W 
		2.adaptive run length encoding
		  count有負值出現
		  count < 0時 表示要直接output
		  例如：
		      200 200 200 190 189 180 180 180
			  encode：3 200 -2 190 189 3 180
		*/
		/*
		  根據說明文件
		  the four scanline components (three primaries and exponent) are separated 
		  for better compression using adaptive run-length encoding
		  A run is indicated by a byte with its highorder bit set, corresponding to a count with excess 128. 
		  A nonrun is indicated with a byte less than 128.
		*/
		
		for ( y; y >= 0 && y < height; y+=iy ) {
			//取得an unnormalized pixel總共有4 byte
			
			input.ignore(4);
			
			//分開讀取pixel值
			for ( int channel = 0; channel < 4; channel++ ) {
				int tmpx = x;
				while ( tmpx < width && tmpx >= 0) {

				
					int c = input.get();
					
					
					if ( c > 128 ) {
						//A run is indicated by a byte with its highorder bit set, corresponding to a count with excess 128.
						int runnum = (int) c & 127;//做logical and會比較快
						float value = (float) input.get();
						for ( int j = 0; j < runnum; j++ ) {
							
							setPixel( y , tmpx  ,channel ,value );
							
							tmpx += ix;
						}
					}else {
						//不是run 直接丟進去即可
						int dumpnum = int(c);
						for ( int j = 0; j < dumpnum; j++ ) {
							
							float value = (float) input.get();

							setPixel( y , tmpx  ,channel ,value );

							//RealColor& pixel =pixels->at(y)->at(tmpx);
							
							//setPixel( pixel ,channel ,value );
							

							tmpx += ix;
						}
					}
				}

			}
			
		}

	}

	cout << "讀取完畢\n";
}

//根據channel和value設定pixel值
void 
HDRLoader::setPixel(int i ,int j ,int channel ,float value) {
	switch( channel ) {//設定channel
		case 0:
			C1[i][j] = value;
			break;
		case 1:
			C2[i][j] = value;
			break;
		case 2:
			C3[i][j] = value;
			break;
		case 3:
			E[i][j] = value;
			break;
	}
}

/*
 取得(x ,y )的RGBE資料

  --------------->x
  |
  |
  |
  |
  v
  
  y
*/
double 
HDRLoader::getRed( int  x ,int y) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;
	
	
	return getChannel(x ,y ,RED,RGB);

}
double 
HDRLoader::getGreen( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getChannel(x ,y ,GREEN,RGB);
	
}
double 
HDRLoader::getBlue( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getChannel(x ,y ,BLUE,RGB);
}

double 
HDRLoader::getX( int  x ,int y) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getChannel( x ,y ,X ,XYZ );

}
double 
HDRLoader::getY( int  x ,int y ) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getChannel(x ,y ,Y ,XYZ );
}
double 
HDRLoader::getZ( int  x ,int y ) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getChannel(x ,y ,Z ,XYZ );
}
double 
HDRLoader::getL( int x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getLAB(x ,y ,L);
}
double 
HDRLoader::getA( int x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getLAB(x ,y ,A);
}
double 
HDRLoader::getB( int x ,int y ) const {
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	return getLAB(x ,y ,B);
}
double 
HDRLoader::getBright( int x ,int y ) const {
	try {
		testDimension(x ,y);
	}catch (exception e) {
		cerr << e.what() <<"\n";
		return NOTANUMBER;
	}

	/*
	If the FORMAT string indicates XYZ data, 
    then the units of the Y primary are already lumens/steradian/sq.meter, 
	so the above conversion is unnecessary.
    */
	return log( getLuminance(x ,y ) );
	
}
/*
  根據Color space那一篇peper luminance為XYZ的Y值
*/
double 
HDRLoader::getLuminance( int x ,int y ) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	//return getY(x ,y);
	double l = 0.2125 * getRed( x , y) + 
               0.7154 * getGreen( x ,y ) + 
               0.0721 * getBlue( x ,y );
	
	if ( l > 0 )
		return l;
	else return 0;
}
double 
HDRLoader::getAvgL() const {

	double avg = 0;
	for ( int y = 0; y < height; y++ ) 
		for ( int x = 0; x < width; x++ )
			avg += getBright( x ,y );

	avg /= ( width * height );
	
	return exp(avg);
}
//設定資料
void 
HDRLoader::scaleLuminanceData(int x ,int y ,float scale ) {
	if ( !testDimension(x ,y) )
		return;

	if ( headinfo.format.find("xyze")  != string::npos ) {//CIE XYZ Y為Luminance
		C2[y][x] *= scale;
	}
	else {//CIE RGB 
		//1.先轉成LAB設定完在轉回來 設定值
		double lab[3] = { 0 };
		
		//lab[0] = getL(x ,y ) * scale;
		lab[0] = scale;
		lab[1] = getA(x ,y ) * scale;
		lab[2] = getB(x ,y ) * scale;
		bool issucess = false;
		Vector rgb = headinfo.rgb2lab->inverseTransform( Vector(3 ,lab ) ,issucess);
		
		if ( issucess == false ) {
			cerr << "HDRLoader::setLuminanceData() 轉換失敗 請注意甚麼事都沒有發生\n";
			return;
		}

		if ( (y==215 && x ==  140) || (y==56 && x ==  134) || (y==255 && x ==  57)) {
	
			printf("(%d ,%d )\n" ,x ,y );
			printf("old R = %lf G = %lf B = %lf L = %lf\n" ,getRed(x ,y) ,getGreen(x ,y) ,getBlue(x,y) ,getL(x ,y ));

		}
		/*
	         根據Greg Ward的Real Pixel這一篇文章
	         我們發現資料的格式和真正的浮點數是以下列關係呈現
	         0.3 = 0.6 * (1/2) 

	             M        E
	            0.6      127
	         0.6 = 153/256
	             M        E
	            153      127

	         所以
	         Cf = C / 256 * (1/2)^(e-128)
        */
		for ( int i = 0; i < 3; i++ ) 
			rgb[i] *= headinfo.exposure * headinfo.colorcorr;
		
		//找到三個數當中最大的
		double max = 0;
		if ( rgb[0] > rgb[1] ) 
			max = rgb[0];
		else max = rgb[1];
		if ( rgb[2] > max )
			max = rgb[2];

		int exp = 0;
		float significand = (float) frexp( max ,&exp);
		E[y][x] = (exp + 128) * headinfo.exposure * headinfo.colorcorr;
		C1[y][x] = rgb[0] / max * ( significand * 256 );
		C2[y][x] = rgb[1] / max * ( significand * 256 );
		C3[y][x] = rgb[2] / max * ( significand * 256 );

		/*if ( (y==215 && x ==  140) || (y==56 && x ==  134) || (y==255 && x ==  57)) {
			
			printf("(%d ,%d )\n" ,x ,y );
			printf("new R = %lf G = %lf B = %lf L = %lf\n" ,getRed(x ,y) ,getGreen(x ,y) ,getBlue(x,y) ,getL(x ,y ));

		}*/
		
		
	}
}

/*
  根據Channel以及要的資訊取值
  傳入：要取(x ,y )哪一個Channel以及是以哪一種ColorSpace c
		channel有三種可能性分別是1 2 3
        c只有三種可能性 一種是rgb一種是xyz 一種是lab
  傳回：該channel的值 如果沒有取得成功將會回傳NOTANUMBER
*/
double 
HDRLoader::getChannel( int x ,int y ,int c ,const string colorspace  ) const{
	if ( c >= 3 || c < 0) {
		cerr <<"你要的channel>3 錯誤\n";
		return NOTANUMBER;
	}
	//開始
	/*
	  根據Greg Ward的Real Pixel這一篇文章
	  我們發現資料的格式和真正的浮點數是以下列關係呈現
	  0.3 = 0.6 * (1/2) 

	      M        E
	     0.6      127
	  0.6 = 153/256
	      M        E
	     153      127

	  所以
	  Cf = C / 256 * (1/2)^(e-128)
	*/
	if ( headinfo.format.find(colorspace)  != string::npos ) {
		//如果要的ColorSpace和目前的一樣 因為檔頭只有RGB和XYZ兩種 所以就不會有LAB考慮 直接輸出即可
		double channel = 0;
		switch ( c ) {
			case RED:
				channel = C1[y][x] / ( headinfo.exposure * headinfo.colorcorr );
				break;
			case GREEN:
				channel = C2[y][x] / ( headinfo.exposure * headinfo.colorcorr );
				break;
			case BLUE:
				channel = C3[y][x] / ( headinfo.exposure * headinfo.colorcorr );
				break;
		}

		double exp = E[y][x] / headinfo.exposure ;

		return ( channel+0.5 ) * ldexp(1.0/256.0 ,exp - 128 );
		

	}else {

		//如果Color space和目前要的不一樣 則傳出不同的color space
		double* color = new double[3];

		double channel = 0;
		//cerr << (headinfo.format.find(XYZ)  != string::npos && colorspace.find(RGB) != string::npos)<<"\n";
		if ( headinfo.format.find(XYZ)  != string::npos && colorspace.find(RGB) != string::npos ) {
			//表示是從XYZ ->RGB
			color[0] = getX(x ,y ); color[1] = getY(x ,y); color[2] = getZ(x ,y);
			Vector color1(3 , color );//原本的

			bool trans = false;
			Vector color2 = headinfo.rgb2lab ->inverseTransform( color1 ,trans ,RGB2XYZ::type);
			if ( trans == false ) return NOTANUMBER;
			channel = color2[c];
		}
		else if ( headinfo.format.find(RGB)  != string::npos && colorspace.find(XYZ) != string::npos ){
			//表示從RGB -> XYZ
			color[0] = getRed(x ,y ); color[1] = getGreen(x ,y); color[2] = getBlue(x ,y);
			Vector color1(3 , color );//原本的
			
			
			Vector color2 = headinfo.rgb2lab ->transform( color1 , RGB2XYZ::type );
		
			channel = color2[c];
		}

		//cerr << color2;


		delete [] color;
		return channel;
	}

}
/*
  取得LAB的值
  傳入：要取(x ,y )轉換後的LAB中的哪一個部分
  傳回：該值
*/
double 
HDRLoader::getLAB(int x ,int y ,int c) const{
	double color[3] = {0};
	Vector color2;
	if ( headinfo.format.find(RGB)  != string::npos ){
			//RGB -> LAB
		//先得到RGB值
		color[0] = getRed(x ,y); color[1] = getGreen(x ,y ); color[2] = getBlue(x ,y);
		Vector color1(3 ,color );
	
		color2 = headinfo.rgb2lab ->transform( color1 );
	}
	else if ( headinfo.format.find(XYZ)  != string::npos ){
		//XYZ -> LAB
		RGB2LAB* xyz2lab = ( RGB2LAB* )headinfo.rgb2lab;

		color[0] = getX(x ,y); color[1] = getY(x ,y ); color[2] = getZ(x ,y);
		Vector color1(3 ,color );

		color2 = xyz2lab->XYZ2LAB( color1 ); 
		delete xyz2lab;
	}

	return color2[c];
}
//進行Hard Copy用
void 
HDRLoader::hardCopy( const HDRLoader& v ) {
	C1 = create2DArray( height ,width );
	C2 = create2DArray( height ,width );
	C3 = create2DArray( height ,width );
	E = create2DArray( height ,width );

	for ( int i = 0; i < height; i++ ) {
		for ( int j = 0; j < width; j++ ) {
			C1[i][j] = v.C1[i][j];
			C2[i][j] = v.C2[i][j];
			C3[i][j] = v.C3[i][j];
			E[i][j] = v.E[i][j];
		}
	}
	headinfo.format = v.headinfo.format;
	headinfo.exposure = v.headinfo.exposure;
	headinfo.colorcorr = v.headinfo.colorcorr;
	headinfo.pixelratio = v.headinfo.pixelratio;
	

	Matrix* m = v.headinfo.rgb2lab -> getTransformMatrix();
	headinfo.rgb2lab = new RGB2LAB( *m );

	delete m ;
	
}

/*
 儲存圖片
*/
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

/*得到string之Unicode表示法*/
WCHAR* getUnicode( const string str );
/*畫圖確定有寫進去用*/
int GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid);
void 
HDRLoader::saveImage(const string& fn) {
	//Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* output = new Bitmap( width ,height );
	cout << "output image\n";

	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int R = (int) (getRed(x ,y) * 255 );
			
			int G = (int) (getGreen(x ,y) * 255 );
			int B = (int) (getBlue(x ,y) * 255 );

			Color c( R ,G ,B );
			output->SetPixel(x ,y ,c );
		}
	}

	WCHAR* fname = getUnicode(fn);
	CLSID clsid;
	GetEncoderClsid(L"image/jpeg", &clsid);//取得codec
	Status s = output->Save(fname , &clsid);
	if ( s != 0 ) cerr << s;

	delete fname;
	delete output;

}
/*得到string之Unicode表示法*/
WCHAR* getUnicode( const string str ) {
	int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);//得到Unicode下的長度
	WCHAR* str1 = new WCHAR[ length + 1 ];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str1, length);//轉成Unicode

	return str1;
}
/*畫圖確定有寫進去用*/
int   
GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid)  
{  
    UINT     num   =   0;                     //   number   of   image   encoders  
    UINT     size   =   0;                   //   size   of   the   image   encoder   array   in   bytes  

    ImageCodecInfo*   pImageCodecInfo   =   NULL;  

    GetImageEncodersSize(&num,   &size);  
    if(size   ==   0)  
          return   -1;     //   Failure  

    pImageCodecInfo   =   (ImageCodecInfo*)(malloc(size));  
    if(pImageCodecInfo   ==   NULL)  
          return   -1;     //   Failure  

    GetImageEncoders(num,   size,   pImageCodecInfo);  

    for(UINT   j   =   0;   j   <   num;   ++j)  
    {  
          if(   wcscmp(pImageCodecInfo[j].MimeType,   format)   ==   0   )  
          {  
                *pClsid   =   pImageCodecInfo[j].Clsid;  
                free(pImageCodecInfo);  
                return   j;     //   Success  
          }          
    }  

    free(pImageCodecInfo);  
    return   -1;     //   Failure  
}   

