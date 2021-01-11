/******************************************
FILE
    HDRLoader.cpp
PURPOSE
    �w�qŪ�JHDR�榡��member function

NOTE
	

AUTHOR
    9757553 �C����
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
//��l�Ƭ������
void 
HDRLoader::init() {
	//header�]�w
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
 Ū�JHDR �v��
 �ǤJ�GHDR�v����file name
 �ǥX�GŪ���O�_���\
 Exception�G�YŪ�J�ɮץ��� �N�|�ǥXException
*/
bool 
HDRLoader::readImage( const string& fname )  {
	cout<<"HDRLoader:Ū�J�G"<< fname << "��\n";

	ifstream input( fname.c_str() ,ifstream::binary );//�Hbinary�覡Ū�J

	input.exceptions( ifstream::failbit | ifstream::badbit );//�u���bfail bit���p�U�~�|��X���~�T��

	try {

		//Ū��Header��T
		
		readHeader( input );
		//Ū��resolution string
		int x = 0 , y = 0 ,ix = 0 ,iy = 0;//�W�[����V
		readResolution( input ,x ,y ,ix ,iy);
		//Ū��scanline record
		readScanline(input ,x ,y ,ix ,iy );

		cout <<"==============\n";
		return true;

	}
	catch ( exception e ) {
		cerr << e.what() <<"\n";

		return false;
	}

}

//Ū��HDR file��Header���� �åB�]�w������T
void 
HDRLoader::readHeader( ifstream& input) {

	const int len = 100;
	char ch[len] = "";
	input.getline( ch ,len );
	string header = ch;
	/*header = ch;
	input >> header;*/
	if ( header != "#?RADIANCE" )
		throw exception("�D�зǪ�HDR �ɮ׮榡" );

	while ( header != "" )  {//the end of the header is indicated by an empty line.
		//input >> header; 
		input.getline( ch ,len );
		header = ch;

		size_t equalp = header.find("=");
		if ( equalp == string::npos ) continue;//�S���=��ܳo���Ovariable assignment

		//��� =  �i���ܼƪ�assign
		if ( header.find("FORMAT") != string::npos) {
			//��M�O�_���h��FORMAT ���঳�h�� 
			if ( headinfo.format != "" )
				throw exception("At most one FORMAT line is allowed\n" );
				
			string f  = header.substr(equalp+1);

			if ( f != "32-bit_rle_rgbe" && f != "32-bit_rle_xyze" )
				throw exception( "it must be assigned a value of either \"32- bit_rle_rgbe\" or \"32-bit_rle_xyze\"\n" );

			headinfo.format = f;
		}
		else if ( header.find("EXPOSURE") != string::npos ) {
			//�w�g���Q����C�@��pixel�W������ �ҥH�����N�n���^��
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
			 �ΨӶi��RGB--> XYZ�ഫ��
			*/
			string f  = header.substr(equalp+1);
			
			//���X��T
			//�Ъ`�N �o��|��Memory Leakage ���ӭn�Q�ѨM��
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
 Ū��Resolution�j�p �åB�M�wX�b��Y�b���V
 �ɮפ���X Y coordinate��

 y
 ^
 |
 |
 |
 |
 |
 ------------------>x
 �Ӯھ��ɮ׮榡������
 -Y +X ����ܬ��q�v�����W���}�l�B�z
 �ڭ̥����b�U�����ˤl �H��K�ǲ�
   --------------->x
   |
   |
   |
   |
   v
	  
   y


 �ǤJ�G�B�z��J�ɮ����O X�b�PY�b�W�[���V
 �ǥX�G���禡�|�ק�ix iy�ܼƥH��K���w���V
*/
void 
HDRLoader::readResolution( std::ifstream& input ,int& x ,int& y ,int& ix ,int& iy) {

	char signy ='+' ,str ='X' ,signx = '+';
	input >> signy >> str >> height;
	input >> signx >> str >> width;

	//��l��1D�}�C
	C1 = create2DArray(height ,width );
	C2 = create2DArray(height ,width );
	C3 = create2DArray(height ,width );
	E = create2DArray(height ,width );

	//�]�wx y�b�_�l��m�H�Ψ��V
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
	
	input.ignore(1);//����Ÿ����n
	
}
/*
  Ū��Pixel���
  �ǤJ�G�B�z��J�ɮ����O X�b�PY�b�_�l��m�H�μW�[���V
*/
const int MIN = 8;//
const int MAX = 0x7FFF;
//�ھ�channel�Mvalue�]�wpixel��
void 
HDRLoader::readScanline( ifstream& input ,int x ,int y ,int ix ,int iy) {
	
	cout<<"HDRLoader:Ū��Pixel��Ƥ� �еy��\n";
	
	if ( headinfo.format.find("rle" ) == string::npos && ( width < MIN || width > MAX ) ) {
		//Uncompressed mode
		/*
		 �p�G�Orun length encoding�L�|�bformat�|��rle���r��
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
		  �Ҥl�G

		      WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW

			  encode�G12W1B12W3B24W1B14W 
		2.adaptive run length encoding
		  count���t�ȥX�{
		  count < 0�� ��ܭn����output
		  �Ҧp�G
		      200 200 200 190 189 180 180 180
			  encode�G3 200 -2 190 189 3 180
		*/
		/*
		  �ھڻ������
		  the four scanline components (three primaries and exponent) are separated 
		  for better compression using adaptive run-length encoding
		  A run is indicated by a byte with its highorder bit set, corresponding to a count with excess 128. 
		  A nonrun is indicated with a byte less than 128.
		*/
		
		for ( y; y >= 0 && y < height; y+=iy ) {
			//���oan unnormalized pixel�`�@��4 byte
			
			input.ignore(4);
			
			//���}Ū��pixel��
			for ( int channel = 0; channel < 4; channel++ ) {
				int tmpx = x;
				while ( tmpx < width && tmpx >= 0) {

				
					int c = input.get();
					
					
					if ( c > 128 ) {
						//A run is indicated by a byte with its highorder bit set, corresponding to a count with excess 128.
						int runnum = (int) c & 127;//��logical and�|�����
						float value = (float) input.get();
						for ( int j = 0; j < runnum; j++ ) {
							
							setPixel( y , tmpx  ,channel ,value );
							
							tmpx += ix;
						}
					}else {
						//���Orun ������i�h�Y�i
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

	cout << "Ū������\n";
}

//�ھ�channel�Mvalue�]�wpixel��
void 
HDRLoader::setPixel(int i ,int j ,int channel ,float value) {
	switch( channel ) {//�]�wchannel
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
 ���o(x ,y )��RGBE���

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
  �ھ�Color space���@�gpeper luminance��XYZ��Y��
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
//�]�w���
void 
HDRLoader::scaleLuminanceData(int x ,int y ,float scale ) {
	if ( !testDimension(x ,y) )
		return;

	if ( headinfo.format.find("xyze")  != string::npos ) {//CIE XYZ Y��Luminance
		C2[y][x] *= scale;
	}
	else {//CIE RGB 
		//1.���নLAB�]�w���b��^�� �]�w��
		double lab[3] = { 0 };
		
		//lab[0] = getL(x ,y ) * scale;
		lab[0] = scale;
		lab[1] = getA(x ,y ) * scale;
		lab[2] = getB(x ,y ) * scale;
		bool issucess = false;
		Vector rgb = headinfo.rgb2lab->inverseTransform( Vector(3 ,lab ) ,issucess);
		
		if ( issucess == false ) {
			cerr << "HDRLoader::setLuminanceData() �ഫ���� �Ъ`�N�ƻ�Ƴ��S���o��\n";
			return;
		}

		if ( (y==215 && x ==  140) || (y==56 && x ==  134) || (y==255 && x ==  57)) {
	
			printf("(%d ,%d )\n" ,x ,y );
			printf("old R = %lf G = %lf B = %lf L = %lf\n" ,getRed(x ,y) ,getGreen(x ,y) ,getBlue(x,y) ,getL(x ,y ));

		}
		/*
	         �ھ�Greg Ward��Real Pixel�o�@�g�峹
	         �ڭ̵o�{��ƪ��榡�M�u�����B�I�ƬO�H�U�C���Y�e�{
	         0.3 = 0.6 * (1/2) 

	             M        E
	            0.6      127
	         0.6 = 153/256
	             M        E
	            153      127

	         �ҥH
	         Cf = C / 256 * (1/2)^(e-128)
        */
		for ( int i = 0; i < 3; i++ ) 
			rgb[i] *= headinfo.exposure * headinfo.colorcorr;
		
		//���T�ӼƷ��̤j��
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
  �ھ�Channel�H�έn����T����
  �ǤJ�G�n��(x ,y )���@��Channel�H�άO�H���@��ColorSpace c
		channel���T�إi��ʤ��O�O1 2 3
        c�u���T�إi��� �@�جOrgb�@�جOxyz �@�جOlab
  �Ǧ^�G��channel���� �p�G�S�����o���\�N�|�^��NOTANUMBER
*/
double 
HDRLoader::getChannel( int x ,int y ,int c ,const string colorspace  ) const{
	if ( c >= 3 || c < 0) {
		cerr <<"�A�n��channel>3 ���~\n";
		return NOTANUMBER;
	}
	//�}�l
	/*
	  �ھ�Greg Ward��Real Pixel�o�@�g�峹
	  �ڭ̵o�{��ƪ��榡�M�u�����B�I�ƬO�H�U�C���Y�e�{
	  0.3 = 0.6 * (1/2) 

	      M        E
	     0.6      127
	  0.6 = 153/256
	      M        E
	     153      127

	  �ҥH
	  Cf = C / 256 * (1/2)^(e-128)
	*/
	if ( headinfo.format.find(colorspace)  != string::npos ) {
		//�p�G�n��ColorSpace�M�ثe���@�� �]�����Y�u��RGB�MXYZ��� �ҥH�N���|��LAB�Ҽ{ ������X�Y�i
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

		//�p�GColor space�M�ثe�n�����@�� �h�ǥX���P��color space
		double* color = new double[3];

		double channel = 0;
		//cerr << (headinfo.format.find(XYZ)  != string::npos && colorspace.find(RGB) != string::npos)<<"\n";
		if ( headinfo.format.find(XYZ)  != string::npos && colorspace.find(RGB) != string::npos ) {
			//��ܬO�qXYZ ->RGB
			color[0] = getX(x ,y ); color[1] = getY(x ,y); color[2] = getZ(x ,y);
			Vector color1(3 , color );//�쥻��

			bool trans = false;
			Vector color2 = headinfo.rgb2lab ->inverseTransform( color1 ,trans ,RGB2XYZ::type);
			if ( trans == false ) return NOTANUMBER;
			channel = color2[c];
		}
		else if ( headinfo.format.find(RGB)  != string::npos && colorspace.find(XYZ) != string::npos ){
			//��ܱqRGB -> XYZ
			color[0] = getRed(x ,y ); color[1] = getGreen(x ,y); color[2] = getBlue(x ,y);
			Vector color1(3 , color );//�쥻��
			
			
			Vector color2 = headinfo.rgb2lab ->transform( color1 , RGB2XYZ::type );
		
			channel = color2[c];
		}

		//cerr << color2;


		delete [] color;
		return channel;
	}

}
/*
  ���oLAB����
  �ǤJ�G�n��(x ,y )�ഫ�᪺LAB�������@�ӳ���
  �Ǧ^�G�ӭ�
*/
double 
HDRLoader::getLAB(int x ,int y ,int c) const{
	double color[3] = {0};
	Vector color2;
	if ( headinfo.format.find(RGB)  != string::npos ){
			//RGB -> LAB
		//���o��RGB��
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
//�i��Hard Copy��
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
 �x�s�Ϥ�
*/
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

/*�o��string��Unicode��ܪk*/
WCHAR* getUnicode( const string str );
/*�e�ϽT�w���g�i�h��*/
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
	GetEncoderClsid(L"image/jpeg", &clsid);//���ocodec
	Status s = output->Save(fname , &clsid);
	if ( s != 0 ) cerr << s;

	delete fname;
	delete output;

}
/*�o��string��Unicode��ܪk*/
WCHAR* getUnicode( const string str ) {
	int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);//�o��Unicode�U������
	WCHAR* str1 = new WCHAR[ length + 1 ];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str1, length);//�নUnicode

	return str1;
}
/*�e�ϽT�w���g�i�h��*/
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

