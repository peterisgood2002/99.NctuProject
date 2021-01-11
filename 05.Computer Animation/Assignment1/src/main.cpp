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
	 *shift+mouseL or mouseR：對物體進行旋轉
	  (1)x軸的偏差量是目前是屬於y軸旋轉
	  (2)y軸偏差量目前是屬於x軸旋轉
     *shift+mouseR：x軸的偏差量是目前是屬於y軸旋轉
	 *G：使用重力
	 *R：模擬開始
	 *T：重設系統
	 *HOME END：設定使用者的力向著上下方向(y)
	 *LEFT RIGHT：設定使用者力向著左右方向(x)
	 *UP DOWN：設定使用者力向著前後方向(z)
	 *Enter：加入Spring
	 *如果在模擬的時候會轉成滑鼠可以產生力

   AUTHOR
     9757553 Zhi-Cheng Yan
   HISTORY
    
***************************************************************************/
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