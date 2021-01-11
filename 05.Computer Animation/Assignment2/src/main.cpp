/***************************************************************************
   NAME
     main.h
   PURPOSE
     ���{���X���@��bootstrap�t�d���;�ӵ{��
   NOTES
	 http://msdn.microsoft.com/zh-tw/library/cc451557%28VS.71%29.aspx
	 http://msdn.microsoft.com/en-us/library/ms633559%28VS.85%29.aspx
	 http://msdn.microsoft.com/en-us/library/ms683199%28VS.85%29.aspx

	 �ާ@�����G
	 (1)���{���bŪ�ɪ��ɭԷ|�ЧAŪ�⦸�ɮ� �@���OASF�ɤ@���OAMC��
	 (2)P: �ʵe�O�_����

	 �Ъ`�N ���{���]���S������Constrain���� �ҥH�o��ܤF�����n�ۤv�]�w
	 �]�����{�� �]�w�F�@�ӳ�ª��ʧ@ �]�N�O����Ū�J��˪�Motion Data
	 ���{�����k�ⳡ���h�N�Y

   AUTHOR
     9757553 Zhi-Cheng Yan
   HISTORY
    
***************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <exception>

extern "C"
{
	int PASCAL WinMain(HINSTANCE inst,HINSTANCE dumb,LPSTR param,int show);
};

int
main(int argc ,char** argv) {
	char buf[256];
	int i;
	HINSTANCE inst;
	
	/*
	 If this parameter is NULL, 
	 GetModuleHandle returns a handle to the file used to create the calling process (.exe file).
	 ���y�ܻ��N�O���{��
	*/
	inst=(HINSTANCE)GetModuleHandle(NULL);
	
	buf[0]=0;
	for(i=1; i<argc; i++)
	{
		strcat(buf,argv[i]);
		strcat(buf," ");
	}
	try {
		return WinMain(inst,NULL,buf,SW_SHOWNORMAL);
	}catch ( std::exception e ) {

	}

}