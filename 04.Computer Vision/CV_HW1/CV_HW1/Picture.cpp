/******************************************
FILE
    Image.cpp
PURPOSE
    �x�sImage class�ΨӶi��H��B�z��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "Picture.h"
#include <iostream>

using std::cerr;

/*Static member function*/
void 
Picture::initGDIPlus() {
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
/*Constructor*/
/*
  �ǤJ�G�����ɦW ���u��m
*/
/*�o��string��Unicode��ܪk*/
WCHAR* getUnicode( const string str ) {
	int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);//�o��Unicode�U������
	WCHAR* str1 = new WCHAR[ length + 1 ];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str1, length);//�নUnicode

	return str1;
}
Picture::Picture(const string imgfn ,const Light& lc){
	lightc = lc;
	
	fname = imgfn;
	dir = imgfn.substr(0 ,imgfn.find_last_of("/") +1);
	WCHAR* fn = getUnicode( imgfn );
	image = new Bitmap( fn );


}
/*Destructor*/
Picture::~Picture() {
	
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
void 
Picture::drawPicture(const string fn) {
	WCHAR* fname = getUnicode(fn);
	CLSID clsid;
	GetEncoderClsid(L"image/jpeg", &clsid);//���ocodec
	Status s = image->Save(fname , &clsid);
	if ( s != 0 ) cerr << s;
}
/*Get function*/
Color
Picture::getColor(int x ,int y ) const {
	Color c;

	if ( x >= getWidth() || y >= getHeight() ) throw "index�W�L�Ϫ����שμe�� ���ˬdx y�y��";
	image->GetPixel( x ,y ,&c );
	
	return c;
}

