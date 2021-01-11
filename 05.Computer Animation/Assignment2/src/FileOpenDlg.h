/***************************************************************************
   NAME
     FileOpenDlg.h
   PURPOSE
     �}���ɮרϥ�
   NOTES

   AUTHOR
     
   HISTORY
     Zhi-Chebg Yan - Match 3, 2010: Modified.
***************************************************************************/

// For openfile dialog

#pragma once

#include <Commdlg.h>
#include <tchar.h>

class CFileOpenDlg
{
public:
	CFileOpenDlg(bool bOpenFile = true);
	~CFileOpenDlg(void);

	OPENFILENAME m_ofn;
	_TCHAR m_pPathName[512];
	_TCHAR m_pFileTitle[512];
	_TCHAR m_pFileExt[512];
	LPCTSTR m_pFilter;

	BOOL DoModal(HWND hParent=NULL);
protected:
	bool m_bOpenFile;//�O���O�}�ɥ�

};
