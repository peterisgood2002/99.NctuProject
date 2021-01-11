/******************************************
FILE
    IMGLoader.cpp
PURPOSE
    讀取Image

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#include "IMGLoader.h"
#include <cmath>

IMGLoader::IMGLoader( const string& fname ) {
	init();
	setRGB( fname );
}
/*Copy Constructor*/
IMGLoader::IMGLoader(const IMGLoader &loader) {
	width = loader.width;
	height = loader.height;

	hardCopy( loader);
}
/**Overloading the assignment operator*/
IMGLoader& 
IMGLoader::operator= (const IMGLoader& loader ) {
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
IMGLoader::~IMGLoader() {
	delete R;
	delete G;
	delete B;
}
/*Public Member Function*/
const float IMGLoader::NOTANUMBER = -1;
Vector 
IMGLoader::transformRGB2XYZ( int x ,int y) const {
	double color[3] = { 0 };
	color[0] = getRed( x ,y );
	color[1] = getGreen( x ,y );
	color[2] = getBlue( x ,y );
	
	Vector v( 3 ,color );
	return rgb2xyz->transform( v );
}
double 
IMGLoader::getX( int  x ,int y) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	Vector xyz = transformRGB2XYZ( x ,y );
	return xyz[0];
}
double 
IMGLoader::getY( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	Vector xyz = transformRGB2XYZ( x ,y );
	return xyz[1];
}
double 
IMGLoader::getZ( int  x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	Vector xyz = transformRGB2XYZ( x ,y );
	return xyz[1];
}
double 
IMGLoader::getBright( int x ,int y ) const {

	if ( !testDimension(x ,y) )
		return NOTANUMBER;

	double l = getLuminance( x ,y );

	if ( l > 0 )
		return log( getLuminance(x ,y ) );
	else return 0;
}
double 
IMGLoader::getLuminance( int x ,int y ) const {

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
IMGLoader::getAvgL() const {

	double avg = 0;
	for ( int y = 0; y < height; y++ ) 
		for ( int x = 0; x < width; x++ ) {
			avg += getBright( x ,y );
		}

	avg /= ( width * height );
	
	return exp(avg);

}

/*Private Mameber*/
void 
IMGLoader::init() {
	width = 0;
	height = 0;
	R = G = B = NULL;
	rgb2xyz = NULL;
}

#using <System.Drawing.dll>
#using <mscorlib.dll>

using namespace System::Drawing;
using namespace System;

static String^
str2String( const string& ss)
{
	if (ss.empty())
		return gcnew System::String("");

	IntPtr ptr(static_cast<System::IntPtr>(static_cast<void*> (const_cast<char*>(ss.c_str()))));

	String^ ret(System::Runtime::InteropServices::Marshal::PtrToStringAnsi(ptr));

	return ret;
}
void 
IMGLoader::setRGB( const string& fname ) {

	try {
		String^ str = str2String( fname );
		Bitmap image( str);
		
		width = image.Width;
		height = image.Height;
	
		R = create2DArray( height ,width );
		G = create2DArray( height ,width );
		B = create2DArray( height ,width );

		for ( int i = 0; i < height; i++ ) {
			for ( int j = 0; j < width; j++ ) {
				Color c = image.GetPixel(j ,i);
				
				R[i][j] = c.R;
				G[i][j] = c.G;
				B[i][j] = c.B;
				
			}
		}
		
	}catch ( System::ArgumentException^ ) {
	}

}
//進行Hard Copy用
void 
IMGLoader::hardCopy( const IMGLoader& v ) {
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
