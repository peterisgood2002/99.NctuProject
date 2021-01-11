/******************************************
FILE
    ImageReader.cpp
PURPOSE
    Ū�JImage�������{��
	�q��Ƨ���Ū��Image�ɮץX�� �H�K�ϥ�

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <iostream>
#include <fstream>
#include <exception>
#include "ImageReader.h"

/*�ϥ�CLR���F��*/
#using <mscorlib.dll>
using namespace System;
using System::IO::Directory;

using std::cout;
using std::cin;
using std::ifstream;

/*
  �NC++/CLI��]String�নISO-C++��string
  �ǤJ�GC++/CLI�w�q���r�ꪫ��
  �ǥX�GISO-C++�w�q��string����
*/
string 
S2str(String^ str) {
	int i = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
	char* t = (char*) i;
	
	string s(t);
	return s;
}

/*
  Ū��Image
  �ǥX�@��Picture�}�C
*/
vector<Picture>
readIMG() {
	/*Ū��directory����file���*/
	String^ dname = "./images/teapot/";
	cout<<"�п�J�v����Ƨ��Ҧb�G\n";
	dname = System::Console::ReadLine();
	if ( !Directory::Exists(dname) ) {
		cout<<"��Ƨ����s�b\n";
		exit(-1);
	}
	array<String^>^ files = Directory::GetFiles(dname);
	/*directory���eŪ������*/
	
	/*Ū�����u��m*/
	string dir = S2str( dname );
	dir += "light.txt";
	string lp = readLightFile( dir );//light����m

	/*�Nfile������Ū�X*/
	vector<Picture> imgs;
	for ( int i = 0; i < files->Length; i++ ) {
		//�o��file������T �]�A���ɦW ���� �Ψӧ�MLight��m�� �åB�T�O�O���ɥ�
		String^ fname = files[i]->Substring( files[i]->LastIndexOf('/') );//�����m
		String^ fe = fname ->Substring( fname->LastIndexOf('.') );//���ɦW
		string fp = S2str( fname ->Substring( 1 ,fname->LastIndexOf('.')-1 ) );//�ɦW

		if ( fe == ".bmp" || fe == ".jpeg" || fe == ".jpg") {//�B�z���ɪ��ɮ�
			//Ū�����u��m
			Light l = readLP( lp ,fp );
			string imgfn =  S2str(files[i]);//img�����m
			cout << "�չ�Ū�����ɡG"<< imgfn <<"\n";
			Picture pic(imgfn ,l);
			imgs.push_back(pic);
			
		}
	}
	
	cout << "Ū���ɮק����L�~\n";
	
	return imgs;

}

/*
  Ū��Light�y��
  �ǤJ�GŪ��fname�ɮ�
  �ǥX�G�N�ɮצs��string
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
		cout << "Ū����"<< fname;
		exit(-1);
	}

}

/*
  Ū�����u��m
  �ǤJ�G�s�����u�y�Ъ��r�� �Ϫ��W�r
  �ǥX�G���u��m
*/
/*
  Ū�����u��m
  �ǤJ�G�s�����u�y�Ъ��r�� �Ϫ��W�r
  �ǥX�G���u��m
*/
Light 
readLP(const string lp ,const string imgn ) {
	
	try {
		//pic1: (pic1: (0,0,150)) .......
		size_t pos = lp.find(imgn);
		string str = lp.substr( pos );
		
		/*����light��m*/
		Light l;
		//���O����pic1: (0,0,150)
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
		cout << "light�ɮפ��S��" << imgn <<"\n";
		exit(-1);
	}
	
}