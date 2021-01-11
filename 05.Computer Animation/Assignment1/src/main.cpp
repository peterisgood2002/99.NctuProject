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
	 *shift+mouseL or mouseR�G�磌��i�����
	  (1)x�b�����t�q�O�ثe�O�ݩ�y�b����
	  (2)y�b���t�q�ثe�O�ݩ�x�b����
     *shift+mouseR�Gx�b�����t�q�O�ثe�O�ݩ�y�b����
	 *G�G�ϥέ��O
	 *R�G�����}�l
	 *T�G���]�t��
	 *HOME END�G�]�w�ϥΪ̪��O�V�ۤW�U��V(y)
	 *LEFT RIGHT�G�]�w�ϥΪ̤O�V�ۥ��k��V(x)
	 *UP DOWN�G�]�w�ϥΪ̤O�V�۫e���V(z)
	 *Enter�G�[�JSpring
	 *�p�G�b�������ɭԷ|�ন�ƹ��i�H���ͤO

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