/***************************************************************************
   NAME
     main.h
   PURPOSE
     此程式碼為一個bootstrap負責產生整個程式
   NOTES
	 http://msdn.microsoft.com/zh-tw/library/cc451557%28VS.71%29.aspx
	 http://msdn.microsoft.com/en-us/library/ms633559%28VS.85%29.aspx
	 http://msdn.microsoft.com/en-us/library/ms683199%28VS.85%29.aspx

	 操作說明：
	 (1)本程式在讀檔的時候會請你讀兩次檔案 一次是ASF檔一次是AMC檔
	 (2)P: 動畫是否播放

	 請注意 本程式因為沒有任何的Constrain等等 所以這表示了必須要自己設定
	 因此本程式 設定了一個單純的動作 也就是不管讀入怎樣的Motion Data
	 本程式讓右手部分去摸頭

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
	 換句話說就是本程式
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