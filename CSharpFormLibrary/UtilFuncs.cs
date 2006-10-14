using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
	/// <summary>
	/// Summary description for UtilFuncs.
	/// </summary>
	public class UtilFuncs 
	{ 
		[ DllImport( "user32.dll" ) ] 
		extern public static bool SetWindowPos( IntPtr hWnd, 
			IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, int uFlags );
 
		public const int HWND_TOPMOST = -1; // 0xffff 
		public const int SWP_NOSIZE = 1; // 0x0001 
		public const int SWP_NOMOVE = 2; // 0x0002 
		public const int SWP_NOACTIVATE = 16; // 0x0010 
		public const int SWP_SHOWWINDOW = 64; // 0x0040 
  
		public static void ShowWindowTopMost(IntPtr handle) 
		{ 
			SetWindowPos( handle, (IntPtr) HWND_TOPMOST, 0, 0, 0, 0, 
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW ); 
		}
 
		public static void SetVisibleNoActivate(Form form, bool visible) 
		{ 
			if ( visible ) 
				ShowWindowTopMost(form.Handle);
			form.Visible = visible;
		} 

		public const uint WM_IME_SETCONTEXT	=	0x281;
		public const ulong HWND_BROADCAST = 0xffff ;
		
			[DllImport("user32.dll")]
			public static extern int SendMessage(
				System.UInt64 hWnd,      // handle to destination window
				uint Msg,       // message
				long wParam,  // first message parameter
				long lParam   // second message parameter
				);

		public static void GiveBackFocus(System.UInt64 hwnd)
		{			
			//long lngResult = SendMessage(hwnd, WM_IME_SETCONTEXT,1L, 0L ); 
			long lngResult = SendMessage(hwnd, WM_IME_SETCONTEXT,1L, 0L ); 
		}
	}
}
