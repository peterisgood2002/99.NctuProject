/******************************************
FILE
    HDRVideo.h
PURPOSE
    this define the tone mapping of my thesis.
	I hope it'll work in order to graduate.

NOTE
	this is for HDR Video
	

AUTHOR
    9757553 �C����
*******************************************/
#include "HDRVideo.h"

#include <iostream>

using std::cout;

#using <System.Drawing.dll>
#using <mscorlib.dll>
using namespace System;
using namespace System::Drawing;
using System::IO::Directory;


/*
  �NC++/CLI��]String�নISO-C++��string
  �ǤJ�GC++/CLI�w�q���r�ꪫ��
  �ǥX�GISO-C++�w�q��string����
*/
static string 
S2str(String^ str) {
	int i = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
	char* t = (char*) i;
	
	string s(t);
	return s;

}
static String^
str2String( const string& ss)
{
	if (ss.empty())
		return gcnew System::String("");

	IntPtr ptr(static_cast<System::IntPtr>(static_cast<void*> (const_cast<char*>(ss.c_str()))));

	String^ ret(System::Runtime::InteropServices::Marshal::PtrToStringAnsi(ptr));

	return ret;
}


/*class�M�ݸ�T*/
HDRVideo::HDRVideo( const string& dname ) {
	dirname = dname;
}
/*
  ����Temporal �� scale �L�|�h����Tone mapping��Attention�MAdaptation����
  ��Temporal����k�D�n�O�z�L

    St = alpha * St + ( 1 - alpha ) * St-1
  �ӨM�w�v�� �ثealpha�ȷ|�O0.5
*/
#include <fstream>

using std::ofstream;
void 
HDRVideo::performWithTemporal( const string& dstdir ) {
	
	String^ dir = str2String( dirname );
	array<String^>^ files = Directory::GetFiles( dir );

	Matrix* St_1 = NULL;
	float alpha = 0.5;
	for ( int i = 0; i < files->Length; i++ ) {
		String^ file = files[i];
		
		if ( file->Contains( ".exr" ) || file->Contains( ".hdr" )){
			try {
				string str = S2str( file );
				//�P�_�O���O�̫᪺frame
				bool lastframe = false;
				if ( i == files->Length - 1 )
					lastframe = true;


				ThesisToneMapping t( str ,true ,lastframe );
				//Matrix* St = t.getAttentionMap();
				/*if ( St_1 == NULL ) 
					St_1 = St;
				else {
					St_1->scale( 1 - alpha );
					St->scale ( alpha );
					(*St_1) += (*St);
					delete St;
				}*/

				/*�i��B�z*/
				//t.perform( St_1 );
				t.perform( );
				
				
				/*if ( St_1 == NULL )
					St_1 = t.getAttentionMap();
				else {
					St_1->scale( 1 - alpha );
					Matrix* St = t.getAttentionMap();
					St->scale ( alpha );
					(*St_1) += (*St);
					
					delete St;*/

				//	/*int row = St_1->getRowCount();
				//	int col = St_1->getColCount();
				//	Matrix s( row ,col);
				//	for ( int i = 0; i < row;i++ ) {
				//		for ( int j = 0; j < col; j++ ) {
				//			double d = St_1->getData(i, j);
				//			if ( d >= 0 )
				//				s.setData(i ,j , 255);
				//			else s.setData(i ,j ,128);
				//		}
				//	}
				//	int firstindex = str.rfind( "\\" );
				//	int finalindex = str.rfind( "." );
				//	string dest = "./Test4/SaliencyMapAccu/";
				//	dest += str.substr( firstindex+1 ,finalindex - firstindex - 1 );
				//	dest +="s.jpg";
				//	t.saliencemap->displayMap(s ,dest);*/

				//	/*
				//	  �̷ӦѮv�����k ������At�ݬ�
				//	*/
				//	//delete St_1;
				//	//St_1 = t.getAttentionMap();
				//	
				//	
				//	//if ( i == 1 ) {
				//	//	/*Debug�� ��{�@�U��T�ݬ�*/
				//	//	int row = St_1->getRowCount();
				//	//	int col = St_1->getColCount();
				//	//	Matrix m( row ,col );

				//	//	for ( int j = 0; j < row; j++ ) {
				//	//		for ( int k = 0; k < col; k++ ) {
				//	//			double y = 127.5 * ( St_1->getData( j ,k )+1 );
				//	//			m.setData( j , k , y );
				//	//		}
				//	//	}
				//	//	t.saliencemap->displayMap( m ,"./saliencemap.jpg" );
				//	//}
				//}
				
				string dest = dstdir;
				int firstindex = str.rfind( "\\" );
				int finalindex = str.rfind( "." );

				dest += str.substr( firstindex+1 ,finalindex - firstindex - 1 );
				dest +=".jpg";



				t.saveResult( dest );

				cout << "===============\n";

			}catch (char* c ) {
				cout << c;
			}
		}
	}
	delete St_1;
}

/*
  Perform the tone mapping without Temporal effect
*/
void 
HDRVideo::performWithoutTemporal( const string& dstdir ) {

	String^ dir = str2String( dirname );
	array<String^>^ files = Directory::GetFiles( dir);

	for ( int i = 0; i < files->Length; i++ ) {
		String^ file = files[i];

		if ( file->Contains( ".exr" ) || file->Contains( ".hdr" )){
			try {
				string str = S2str( file );

				//�P�_�O���O�̫᪺frame
				bool lastframe = false;
				if ( i == file->Length - 1 )
					lastframe = true;

				ThesisToneMapping t( str ,true ,lastframe );
				t.perform();
				
				string dest = dstdir;
				int firstindex = str.rfind( "\\" );
				int finalindex = str.rfind( "." );

				dest += str.substr( firstindex+1 ,finalindex - firstindex - 1 );
				dest +=".jpg";



				t.saveResult( dest );

				cout << "===============\n";

			}catch (char* c ) {
				cout << c;
			}
		}
	}
}