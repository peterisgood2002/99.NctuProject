/******************************************
FILE
    EXRLoader
PURPOSE
    宣告讀入EXR格式的class

NOTE
	都是因為High dynamic Range之file format沒有統一才讓我那麼麻煩
	

AUTHOR
    9757553 顏志晟
*******************************************/

#include <iostream>
#include <cmath>
#include <exception>
#include <assert.h>

#include "EXRLoader.h"
#include "../Essential/Matrix.h"
//#include "toFloat.h"

#include <ImfTestFile.h>

using std::exception;
using Imf::Rgba;

const float EXRLoader::DEFAULT = 0;
/*Constructor*/
EXRLoader::EXRLoader( const string& fname ) {
	init();
	if ( Imf::isOpenExrFile( fname.c_str() ) )
		readImage(fname);
	else throw exception("the file is the format of the OpenExr");
}
/*Copy Constructor*/
EXRLoader::EXRLoader( const EXRLoader& loader) {
	width = loader.width;
	height = loader.height;

	hardCopy( loader);
}

/**Overloading the assignment operator*/
EXRLoader& 
EXRLoader::operator= (const EXRLoader& loader ) {
	if (this == &loader ) return *this;

	width = loader.width;
	height = loader.height;

	dispose( R ,height ,width );
	dispose( G ,height ,width );
	dispose( B ,height ,width );
	delete rgb2xyz;

	hardCopy(loader);

	return *this;
}
EXRLoader::~EXRLoader() {
	if ( R != NULL )
		dispose( R ,height ,width );
	if ( G != NULL )
		dispose( G ,height ,width );
	if ( B != NULL )
		dispose( B ,height ,width );
	/*if ( headinfo != NULL )
		delete headinfo;*/

	delete rgb2xyz;

	
}
void 
EXRLoader::init() {

	width = 0; 
	height = 0;

	R = G = B = NULL;

	rgb2xyz = new RGB2XYZ();

	//headinfo = NULL;
}

/*
 讀入HDR 影像 RGBE的影像格式
 傳入：HDR影像的file name
 傳出：讀取是否成功
 Exception：若讀入檔案失敗 將會傳出Exception
*/
bool 
EXRLoader::readImage(const string& fname) {
	
	try {

		RgbaInputFile input( fname.c_str() );
		
		if ( !input.isComplete() )
			throw exception( " the file is not written completely\n" );

		readHeader(input);
		readPixel( input );
		
		return true;
	}catch ( exception e ) {
		std::cerr << e.what();

		return false;
	}
	
}
void 
EXRLoader::readHeader( const RgbaInputFile& input ) {
	
	Imath::Box2i dw = input.header().dataWindow();
    width  = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;

	

}
/*
讀取Pixel資料
請注意一件事情 因為OpenEXR的general intergace中FrameBuffer部分有點問題 所以改用直接讀RGBA的方式 沒辦法中的辦法
*/
void 
EXRLoader::readPixel( RgbaInputFile& input ) {

	assert( height > 0 && width > 0 );

	/*allocate RGB資料*/
	R = create2DArray( height ,width );
	G = create2DArray( height ,width );
	B = create2DArray( height ,width );

	
	
	Rgba* pixels = new Rgba[ width * height ];
	
	input.setFrameBuffer( pixels ,1 ,width );

	Imath::Box2i dw = input.dataWindow();
	input.readPixels( dw.min.y ,dw.max.y );

	/*可能會有連結錯誤 請參考以下網址
	  http://hebbut.net/Public.Offerings/OpenEXR.html
	 */
	int i = 0;
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			R[y][x] = pixels[i].r;
			G[y][x] = pixels[i].g;
			B[y][x] = pixels[i].b;

			i++;
		}
	}

	delete [] pixels;
}
/*
Virtual Function
*/
/*轉換color space*/
Vector 
EXRLoader::transformRGB2XYZ( int x ,int y) const{
	double color[3] = { 0 };
	color[0] = getRed( x ,y );
	color[1] = getGreen( x ,y );
	color[2] = getBlue( x ,y );
	
	Vector v( 3 ,color );
	return rgb2xyz->transform( v );

}
const float EXRLoader::NOTANUMBER = -1;
double 
EXRLoader::getX( int  x ,int y) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	Vector xyz = transformRGB2XYZ( x ,y );
	return xyz[0];
}
double 
EXRLoader::getY( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	
	Vector xyz = transformRGB2XYZ( x ,y );

	return xyz[1];
}
double 
EXRLoader::getZ( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	Vector xyz = transformRGB2XYZ( x ,y );
	
	return xyz[2];
}
double 
EXRLoader::getBright( int x ,int y ) const {
	
	if ( !testDimension(x ,y) )
		return NOTANUMBER;
	double l = getLuminance( x ,y );

	if ( l > 0 )
		return log( getLuminance(x ,y ) );
	else return 0;
}
double 
EXRLoader::getLuminance( int x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	/*先用這個 要不然太慢了 不知道要算到甚麼時候*/
	double l = 0.2125 * getRed( x , y) + 
               0.7154 * getGreen( x ,y ) + 
               0.0721 * getBlue( x ,y );

	if ( l > 0 )
		return l;
	else return 0;

	//return getY( x, y );
}
double 
EXRLoader::getAvgL() const {
	
	double avg = 0;
	for ( int y = 0; y < height; y++ ) 
		for ( int x = 0; x < width; x++ ) {
			avg += getBright( x ,y );
		}

	avg /= ( width * height );
	
	return exp(avg);
}

//進行Hard Copy用
void 
EXRLoader::hardCopy( const EXRLoader& v ) {

	R = create2DArray( height ,width );
	G = create2DArray( height ,width );
	B = create2DArray( height ,width );

	for ( int i = 0; i < height; i++ ) {
		for ( int j = 0; j < width; j++ ) {
			R[i][j] = v.R[i][j];
			G[i][j] = v.G[i][j];
			B[i][j] = v.B[i][j];
		}
	}
	
	rgb2xyz = new RGB2XYZ( *v.rgb2xyz );
	
}