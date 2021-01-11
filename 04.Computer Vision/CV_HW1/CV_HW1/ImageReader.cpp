/******************************************
FILE
    ImageReader.cpp
PURPOSE
    讀入Image的相關程式
	從資料夾中讀取Image檔案出來 以便使用

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include <iostream>
#include <fstream>
#include <exception>
#include "ImageReader.h"

/*使用CLR的東西*/
#using <mscorlib.dll>
using namespace System;
using System::IO::Directory;

using std::cout;
using std::cin;
using std::ifstream;

/*
  將C++/CLI的]String轉成ISO-C++之string
  傳入：C++/CLI定義的字串物件
  傳出：ISO-C++定義的string物件
*/
string 
S2str(String^ str) {
	int i = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
	char* t = (char*) i;
	
	string s(t);
	return s;
}

/*
  讀取Image
  傳出一個Picture陣列
*/
vector<Picture>
readIMG() {
	/*讀取directory中的file資料*/
	String^ dname = "./images/teapot/";
	cout<<"請輸入影像資料夾所在：\n";
	dname = System::Console::ReadLine();
	if ( !Directory::Exists(dname) ) {
		cout<<"資料夾不存在\n";
		exit(-1);
	}
	array<String^>^ files = Directory::GetFiles(dname);
	/*directory內容讀取完畢*/
	
	/*讀取光線位置*/
	string dir = S2str( dname );
	dir += "light.txt";
	string lp = readLightFile( dir );//light的位置

	/*將file之圖檔讀出*/
	vector<Picture> imgs;
	for ( int i = 0; i < files->Length; i++ ) {
		//得到file相關資訊 包括附檔名 等等 用來找尋Light位置用 並且確保是圖檔用
		String^ fname = files[i]->Substring( files[i]->LastIndexOf('/') );//絕對位置
		String^ fe = fname ->Substring( fname->LastIndexOf('.') );//副檔名
		string fp = S2str( fname ->Substring( 1 ,fname->LastIndexOf('.')-1 ) );//檔名

		if ( fe == ".bmp" || fe == ".jpeg" || fe == ".jpg") {//處理圖檔的檔案
			//讀取光線位置
			Light l = readLP( lp ,fp );
			string imgfn =  S2str(files[i]);//img絕對位置
			cout << "試圖讀取圖檔："<< imgfn <<"\n";
			Picture pic(imgfn ,l);
			imgs.push_back(pic);
			
		}
	}
	
	cout << "讀取檔案完畢無誤\n";
	
	return imgs;

}

/*
  讀取Light座標
  傳入：讀取fname檔案
  傳出：將檔案存成string
*/
string 
readLightFile(const string fname) {
	
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
  讀取光線位置
  傳入：存有光線座標的字串 圖的名字
  傳出：光線位置
*/
/*
  讀取光線位置
  傳入：存有光線座標的字串 圖的名字
  傳出：光線位置
*/
Light 
readLP(const string lp ,const string imgn ) {
	
	try {
		//pic1: (pic1: (0,0,150)) .......
		size_t pos = lp.find(imgn);
		string str = lp.substr( pos );
		
		/*產生light位置*/
		Light l;
		//分別產生pic1: (0,0,150)
		double t[3];
		for ( int i = 0; i < 3; i++ ) {
			if ( i == 0 )
				pos = str.find('(');
			else pos= str.find(",");
			size_t epos = str.find("," ,pos+1);
			if ( i == 2 )
				epos =str.find(")" ,pos+1);
			string str1 = str.substr(pos+1 ,epos-pos-1 );
			t[i] = atoi( str1.c_str() );
			str = str.substr(epos);
		}

		l.x = t[0];
		l.y = t[1];
		l.z = t[2];
		return l;
	}catch ( std::exception& e ) {
		cout << "light檔案內沒有" << imgn <<"\n";
		exit(-1);
	}
	
}