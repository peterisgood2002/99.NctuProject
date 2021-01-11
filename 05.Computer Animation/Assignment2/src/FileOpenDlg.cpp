#include "stdafx.h"
#include "FileOpenDlg.h"

#pragma comment (lib, "Comdlg32.lib")

CFileOpenDlg::CFileOpenDlg(bool bOpenFile):m_bOpenFile(bOpenFile)
{
	ZeroMemory(&m_ofn, sizeof(m_ofn));//Fills a block of memory with zeros.
	ZeroMemory(m_pPathName, sizeof(_TCHAR)*512);
	ZeroMemory(m_pFileTitle, sizeof(_TCHAR)*512);
	ZeroMemory(m_pFileExt, sizeof(_TCHAR)*512);
	m_pFilter = NULL;
}

CFileOpenDlg::~CFileOpenDlg(void)
{
}

BOOL CFileOpenDlg::DoModal(HWND hParent)
{
	/*http://msdn.microsoft.com/en-us/library/ms646839%28VS.85%29.aspx*/
	/*the system uses the specified template to create a dialog box 
	       that is a child of the default Explorer-style dialog box*/
	m_ofn.Flags = OFN_EXPLORER;

	if (m_bOpenFile)//Specifies that the user can type only names of existing files in the File Name entry field.
		m_ofn.Flags |= OFN_PATHMUSTEXIST;
	else//Causes the Save As dialog box to generate a message box if the selected file already exists
		m_ofn.Flags |= OFN_OVERWRITEPROMPT;

	ZeroMemory(m_pFileTitle, sizeof(_TCHAR)*512);

	m_ofn.lStructSize=sizeof(m_ofn);
	m_ofn.hwndOwner = hParent;
	m_ofn.lpstrFile=m_pPathName;
	m_ofn.nMaxFile = 512;
	m_ofn.lpstrFileTitle=m_pFileTitle;
	m_ofn.nMaxFileTitle = 512;
	m_ofn.lpstrDefExt = m_pFileExt;
	m_ofn.nFileExtension = 512;
	m_ofn.lpstrFilter = m_pFilter;

	if (m_bOpenFile)
		return GetOpenFileName(&m_ofn);
	else
		return GetSaveFileName(&m_ofn);

}