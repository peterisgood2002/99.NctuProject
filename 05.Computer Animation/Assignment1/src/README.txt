本程式主要希望產生一個Particle的動畫
本程式建構在MFC的架構中 
*MFC有以下幾個重點請注意
MFC provides two important classes
1.CWinApp provides the application level functionalities
  (1)The CWinApp class has an important over-ridable function  InitInstance which handles the Window creations
  (2)The next important one is a data member, m_pMainWnd (of CWinApp) which holds the pointer to the window
2.CFrameWnd provides the functionalities related to GUI
2.2 CWnd provides the base functionality of all window classes
  (1)A CWnd object is distinct from a Windows window
     *A CWnd object is created or destroyed by the CWnd constructor and destructor.
     *The Windows window is a data structure internal to Windows 
      that is created by a Create member function and destroyed by the CWnd virtual destructor. 
  (2)The CWnd class and the message-map mechanism hide the WndProc  function
     *Incoming Windows notification messages are automatically routed through the message map 
      to the proper OnMessage CWnd member functions.
  (3)The CWnd class also lets you create a Windows child window for your application
     *call the constructor CWnd to construct the CWnd object
     *call the Create  member function to create the child window and attach it to the CWnd  object. 
  (4)further classes are derived from CWnd to provide specific window types. Many of these classes, 
     including CFrameWnd, CMDIFrameWnd, CMDIChildWnd, CView, and CDialog, are designed for further derivation. 
==>
All these classes are derived from CCmdTarget which in turn is derived from CObject.
CCmdTarget is created with the capability to handle windows messages, which is referred as Message Maps
3.Message maps
  (1)Message Maps are the way by which MFC handles the Application messages
  (2)Any class which is derived from CCmdTarget is a candidate for handling messages
  (3)In Win32 SDK , programmer is supposed to handle all messages posted to the message loop
     The application he wrote should have a function to handle the messages
  (4)A class can have only one message map.
4.This leaves us with the discussion about CClientDC. 
  (1)There is a concept in windows programming, called Device Context in windows. 
  (2)This is used in conjunction with the outputs. 
  (3)This device context (DC) can direct the outputs to the screen or to the printer. 可以將output導到螢幕上
  (4)The root class for device contexts is CDC. 
     The classes like CClientDC, CPaintDC, CMetaDC are all derived from this CDC class. 
  (5)The applications have to write their formatted outputs to the device context 
     and the DC will take care of writing it to the screen or to the printer. 
5.Menu programming
  MFC provides two ways of programming menus. 
  (1)the resource option 
  (2)the dynamic menu option
6.If "/subsystem:console" is given, the program will have a console window, 
         and the program starts from "main" function. 
  If "/subsystem:windows" is given, the program will not have a console window, 
         and the program starts from "WinMain" function, although "WinMain" is hidden for an MFC application.
 
7.rendering context
  (1)You can think of a rendering context as a port through which all OpenGL commands must pass
  (2)OpenGL requires an active rendering context (RC). 
     Without an active RC, OpenGL commands do nothing.
  (3)Before you can create an RC and make it active, you must have a device context (DC) with a valid pixel format selected
     When you choose a pixel format, you specify the number of color bits per pixel desired
     If the user's hardware supports 16, 24, or 32 bpps, a palette is not required. 
     If the user's hardware supports only 8 bpps, you do need a palette
  (4)A device context contains information for GDI, while a rendering context contains information for OpenGL
  (5)To create a rendering context, you use the wglCreateContext function
  (6)A program can have several rendering contexts
     Before we can draw using GDI, or render using OpenGL, 
     we need to specify the device context or rendering context we are using
     The wglMakeCurrent function is used to set the current active rendering context
8.Dialog Box
  (1)modal dialogs once invoked will not allow the users to access the parent window
  (2)modeless dialogs will allow the user to work with the parent window
  (3)Modeless Dialog Box handling also is done using CDialog class in MFC.
  (4)CDialog：
     *CWnd derived class
     *The initialization of the dialog is done using an over-ridable function OnInitDialog
     *The dialogs are closed by calling the function EndDialog.
     
*請注意以下幾個特別的檔案
1.MainFrm.h：此檔案中的class MainFrm繼承了CFrameWnd是用來控制UI用的檔案
2.Particle.h：此檔案中的class Particle繼承了CWinApp 提供一個很重要的application進入點
			  有以下兩個部份相當重要
  (1)InitInstance() which handles the Window creations
  (2)data member m_pMainWnd (of CWinApp) which holds the pointer to the window
3.SimProps.h：此檔案中的class CSimProps繼承了CDialog 為用來顯示IDD_SIMPROP之Dialog用
4.TimeProps.h：此檔案中的class CTimeProps繼承了CDialog 為用來顯示IDD_SIMTIMING之Dialog用
5.VertMass.h：此檔案中的class CTimeProps繼承了CDialog 為用來顯示IDD_VERTEXMASS之Dialog用
  
*Reference
 http://msdn.microsoft.com/en-us/library/ms970745.aspx
 http://www.codersource.net/mfc/mfc-tutorials/mfc-tutorial-part-1.aspx