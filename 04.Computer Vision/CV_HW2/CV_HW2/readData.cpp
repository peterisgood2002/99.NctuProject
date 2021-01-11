/******************************************
FILE
    readData.cpp
PURPOSE
    從檔案中讀取相關資料

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "readData.h"
#include <fstream>
#include <iostream>

/*使用CLR的東西*/
#using <mscorlib.dll>
using namespace System;
using System::IO::Directory;


using std::ifstream;
using std::cout;

/*
  取得兩張圖的點對應 以方便找尋F
  傳入：correspondence 的檔案位置
  傳出：相對應的點座標集合
*/
string readFile2Str ( string fname );
vector<CorPoint> 
getCorrespondPoint( string cfile ) {
	string data =  readFile2Str( cfile );
	/*cout << data <<"\n";*/
	cout<<"將資料讀入中"<<cfile<<"\n";

	vector<CorPoint> cp;
	//( 62 , 124 )( 51 , 125 )
	while ( data != "" ) {
		CorPoint c;

		//資料讀入
		for ( int i = 0; i < 2; i++ ) {
			size_t ptr = data.find('(');
			size_t eptr = data.find(')');

			string p = data.substr(ptr+1 ,eptr - 1);//62 ,124
			size_t comma = p.find(',');
			string tmp =  p.substr( ptr ,comma);
			int x = atoi ( tmp.c_str() );
			tmp =  p.substr( comma+1 );
			int y = atoi ( tmp.c_str() );

			if ( i == 0 ) {
				c.pl.x = x;
				c.pl.y = y;
				c.pl.z = 1;
			}
			else {
				c.pr.x = x;
				c.pr.y = y;
				c.pr.z = 1;
			}

			data = data.substr( eptr+1 );

		}
		cp.push_back(c);
	}

	cout<<"讀入資料結束\n";
	return cp;
}
/*將檔案讀成string*/
string 
readFile2Str ( string fname ) {
	ifstream input( fname.c_str() );
	
	if ( input != NULL ) {
		string str;
		
		while ( !input.eof() ) {
			string tmp;
			input >> tmp;
			str += tmp;
		}
		//cout<< str;
		return str;
	}
	else {
		cout << "讀不到"<< fname;
		exit(-1);
	}
}
/*
  讀取資料夾下的圖檔
  傳入：資料夾名稱
  傳出：圖檔集合
*/
string S2str(String^ str);
String^ str2S( const string str );
vector<Picture>  getPicture( const string dname ) {
	printf("讀取%s內的圖檔中\n" , dname.c_str() );
	String^ dn = str2S(dname);
	if ( !Directory::Exists(dn) ) {
		cout<<"資料夾不存在\n";
		exit(-1);
	}
	array<String^>^ files = Directory::GetFiles(dn);

	//讀圖
	vector<Picture> img;
	for ( int i = 0; i < files->Length; i++ ) {
		String^ fe = files[i] ->Substring( files[i]->LastIndexOf('.') );//副檔名
		if ( fe == ".bmp" || fe == ".jpeg" || fe == ".jpg" ) {//處理圖檔的檔案
			//讀取光線位置
			string imgfn =  S2str(files[i]);//img絕對位置
			cout << "試圖讀取圖檔："<< imgfn <<"\n";
			Picture pic(imgfn );
			img.push_back(pic);
			
		}
	}
	cout <<"讀取完畢\n";
	return img;
}
/*進行字串形態的轉換*/
string 
S2str(String^ str) {
	int i = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
	char* t = (char*) i;
	
	string s(t);
	return s;
}
String^ 
str2S( const string str ) {
	System::IntPtr ptr( static_cast<System::IntPtr>( const_cast<char*>(str.c_str() ) )  );
	return System::Runtime::InteropServices::Marshal::PtrToStringAnsi( ptr );
	
}