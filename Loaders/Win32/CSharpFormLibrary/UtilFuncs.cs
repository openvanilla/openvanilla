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
        public const int SWP_HIDEWINDOW = 128; //0x0080
  
		public static void ShowWindowTopMost(IntPtr handle) 
		{ 
			SetWindowPos( handle, (IntPtr) HWND_TOPMOST, 0, 0, 0, 0, 
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW ); 
		}

        public static void HideWindowTopMost(IntPtr handle)
        {
            SetWindowPos(handle, (IntPtr)HWND_TOPMOST, 0, 0, 0, 0,
               SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_HIDEWINDOW);
        }
 
		public static void SetVisibleNoActivate(Form form, bool visible) 
		{
            
            if (visible)
                ShowWindowTopMost(form.Handle);
            form.Visible = visible;
            /*else
            {
                HideWindowTopMost(form.Handle);                
            }*/
		} 

		public const ulong HWND_BROADCAST = 0xFFFF;

        [DllImport("user32.dll")]
        public static extern int SendMessage(
            IntPtr hWnd,      // handle to destination window
            uint Msg,       // message
            uint wParam,  // first message parameter
            uint lParam   // second message parameter
            );

        [DllImport("kernel32.dll")]
        public static extern int GetLastError();

		//public static void GiveBackFocus(System.UInt64 hwnd)
		//{			
		//	long lngResult = SendMessage(hwnd, (int)WindowsMessage.WM_IME_SETCONTEXT, 1, 0); 
		//}

        public enum WindowsMessage : uint
        {
            WM_NULL = 0x0000,
            WM_CREATE = 0x0001,
            WM_DESTROY = 0x0002,            
            WM_MOVE = 0x0003,
            WM_SIZE = 0x0005,
            WM_ACTIVATE = 0x0006,
            WM_SETFOCUS = 0x0007,
            WM_KILLFOCUS = 0x0008,
            WM_ENABLE = 0x000A,
            WM_SETREDRAW = 0x000B,
            WM_SETTEXT = 0x000C,
            WM_GETTEXT = 0x000D,
            WM_GETTEXTLENGTH = 0x000E,
            WM_PAINT = 0x000F,
            WM_CLOSE = 0x0010,

            WM_ERASEBKGND = 0x0014,

            WM_SHOWWINDOW = 0x0018,

            WM_SETCURSOR = 0x0020,
            WM_MOUSEACTIVATE = 0x0021,

            WM_WINDOWPOSCHANGING = 0x0046,
            WM_WINDOWPOSCHANGED = 0x0047,

            WM_NCCREATE = 0x0081,
            WM_NCDESTROY = 0x0082,
            WM_NCCALCSIZE = 0x0083,
            WM_NCHITTEST = 0x0084,
            WM_NCPAINT = 0x0085,
            WM_NCACTIVATE = 0x0086,

            WM_MENUSELECT = 0x011F,

            WM_ENTERIDLE = 0x0121,
            WM_MENURBUTTONUP = 0x0122,
            WM_MENUDRAG = 0x0123,
            WM_MENUGETOBJECT = 0x0124,
            WM_UNINITMENUPOPUP = 0x0125,
            WM_MENUCOMMAND = 0x0126,
            WM_CHANGEUISTATE = 0x0127,
            WM_UPDATEUISTATE = 0x0128,

            WM_MOUSEMOVE = 0x0200,
            WM_LBUTTONDOWN = 0x0201,
            WM_LBUTTONUP = 0x0202,
            WM_LBUTTONDBLCLK = 0x0203,
            WM_RBUTTONDOWN = 0x0204,
            WM_RBUTTONUP = 0x0205,
            WM_RBUTTONDBLCLK = 0x0206,

            WM_PARENTNOTIFY = 0x0210,

            WM_EXITMENULOOP = 0x0212,

            WM_CAPTURECHANGED = 0x0215,

            WM_IME_SETCONTEXT = 0x0281,
            WM_IME_NOTIFY = 0x0282,

            WM_MOUSEHOVER = 0x02A1,
            WM_MOUSELEAVE = 0x02A3,
            WM_IME_RELOADCONFIG = 0x8000 + 601,
            WM_IME_STARTCOMPOSITION = 0x010D
            
        }
	}
}
