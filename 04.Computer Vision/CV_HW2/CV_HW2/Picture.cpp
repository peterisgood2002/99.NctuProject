/******************************************
FILE
    Picture.cpp
PURPOSE
    儲存Image class用來進行以後處理用

NOTE
	

AUTHOR
    9757553 顏志晟
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
  傳入：圖檔檔名 光線位置
*/
/*得到string之Unicode表示法*/
WCHAR* getUnicode( const string str ) {
	int length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);//得到Unicode下的長度
	WCHAR* str1 = new WCHAR[ length + 1 ];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str1, length);//轉成Unicode

	return str1;
}
Picture::Picture(const string imgfn ){
	fname = imgfn;
	dir = imgfn.substr(0 ,imgfn.find_last_of("/") +1);
	WCHAR* fn = getUnicode( imgfn );
	image = new Bitmap( fn );


}
/*Copy Constuctor*/
Picture::Picture( const Picture& pic ) {
	image = pic.image->Clone(0 ,0 ,pic.image->GetWidth() ,pic.image->GetHeight() ,pic.image->GetPixelFormat() );
	fname = pic.fname;
	dir = pic.dir;
}
/*Destructor*/
Picture::~Picture() {
	delete image;
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
void 
Picture::drawPicture(const string fn) {
	WCHAR* fname = getUnicode(fn);
	CLSID clsid;
	GetEncoderClsid(L"image/jpeg", &clsid);//取得codec
	Status s = image->Save(fname , &clsid);
	if ( s != 0 ) cerr << s;
}
/*Get function
  --------->x
  |
  |
  |
  y
*/
Color
Picture::getColor(int x ,int y ) const {
	Color c;

	if ( x >= getWidth() || y >= getHeight() || x < 0 || y < 0  ) throw "index超過圖的高度或寬度 請檢查x y座標";
	image->GetPixel( x ,y ,&c );
	
	return c;
}
/*
	  取得一個(x ,y)周圍的資訊
	  傳入：座標 以及要取多少
	  傳出：一個vector
	*/
WColor*
Picture::getWindowPixels(int x ,int y ,int wheight ,int wwidth) const {
	WColor* wc = new WColor();
	int hw = wwidth / 2;
	int hh = wheight / 2;

	for ( int i = y - hh; i <= y + hh; i++ ) {
		for ( int j = x - hw; j <= x + hw; j++ ) {
			try {
				Color tmp = getColor( j ,i );
				wc->push_back(tmp);
			}
			catch ( char* str ) {
				Color d = Color::Black;
				wc->push_back( d );

			}
		}
	}

	return wc;
}
/*
  取得一行的資料
  傳入：要取的行號為何 windows size多大
  傳出：一個有windows size個vector之color的array
*/
vector<WColor*>* 
Picture::getOneLineColors( int l ,int wwidth ,int wheight) const {
	if ( l < getHeight() ) {
		vector<WColor*>* lc = new vector<WColor*>();
		for ( int i = 0; i < getWidth(); i++ ) {
			WColor* wc = getWindowPixels(i ,l ,wwidth ,wheight);
			//將( i , l )附近的pixel取出來
			
			lc->push_back( wc );
		}
		return lc;
	}
	else throw "你想取的行數超過圖檔的高度 請確認沒有取錯";
}

/*
  Set data member
*/
void 
Picture::setColor(int x ,int y ,const Color& c ) {
	image->SetPixel( x , y , c );
}