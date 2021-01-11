1.���{���O�ϥ�Win32 API�g�� ����
  WinMain() is windows equivalent of main() from DOS or UNIX. This is where your program starts execution.
2.�n���ͤ@��²�檺window���H�U�X�ӨB�J
  (1)Registering the Window Class
     *A Window Class stores information about a type of window, including it's Window Procedure which controls the window  
  (2)Creating the Window
  (3)The Message Loop
     *This is the heart of the whole program, 
      pretty much everything that your program does passes through this point of control.
     *GetMessage() gets a message from your application's message queue.
     *TranslateMessage() does some additional processing on keyboard events
     *DispatchMessage() sends the message out to the window that the message was sent to. 
      This could be our main window or it could be another one, or a control
  (4)the Window Procedure
     *If the message loop is the heart of the program, the window procedure is the brain
     *���X�ӰѼƽЪ`�N
      hWnd - this is the unique handle of the window.
      message - this is the message your window has received
      wParam and lParam - each message sent to your window can have a number of extra values,
  
Reference:
  (1)http://www.winprog.org/tutorial/simple_window.html