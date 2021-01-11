/******************************************
FILE
    readData.cpp
PURPOSE
    �q�ɮפ�Ū���������

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "readData.h"
#include <fstream>
#include <iostream>

/*�ϥ�CLR���F��*/
#using <mscorlib.dll>
using namespace System;
using System::IO::Directory;


using std::ifstream;
using std::cout;

/*
  ���o��i�Ϫ��I���� �H��K��MF
  �ǤJ�Gcorrespondence ���ɮצ�m
  �ǥX�G�۹������I�y�ж��X
*/
string readFile2Str ( string fname );
vector<CorPoint> 
getCorrespondPoint( string cfile ) {
	string data =  readFile2Str( cfile );
	/*cout << data <<"\n";*/
	cout<<"�N���Ū�J��"<<cfile<<"\n";

	vector<CorPoint> cp;
	//( 62 , 124 )( 51 , 125 )
	while ( data != "" ) {
		CorPoint c;

		//���Ū�J
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

	cout<<"Ū�J��Ƶ���\n";
	return cp;
}
/*�N�ɮ�Ū��string*/
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
		cout << "Ū����"<< fname;
		exit(-1);
	}
}
/*
  Ū����Ƨ��U������
  �ǤJ�G��Ƨ��W��
  �ǥX�G���ɶ��X
*/
string S2str(String^ str);
String^ str2S( const string str );
vector<Picture>  getPicture( const string dname ) {
	printf("Ū��%s�������ɤ�\n" , dname.c_str() );
	String^ dn = str2S(dname);
	if ( !Directory::Exists(dn) ) {
		cout<<"��Ƨ����s�b\n";
		exit(-1);
	}
	array<String^>^ files = Directory::GetFiles(dn);

	//Ū��
	vector<Picture> img;
	for ( int i = 0; i < files->Length; i++ ) {
		String^ fe = files[i] ->Substring( files[i]->LastIndexOf('.') );//���ɦW
		if ( fe == ".bmp" || fe == ".jpeg" || fe == ".jpg" ) {//�B�z���ɪ��ɮ�
			//Ū�����u��m
			string imgfn =  S2str(files[i]);//img�����m
			cout << "�չ�Ū�����ɡG"<< imgfn <<"\n";
			Picture pic(imgfn );
			img.push_back(pic);
			
		}
	}
	cout <<"Ū������\n";
	return img;
}
/*�i��r��κA���ഫ*/
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