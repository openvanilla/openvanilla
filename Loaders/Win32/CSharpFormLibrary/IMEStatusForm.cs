using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
    public enum WindowsMessage : int
    {
        WM_NULL =           0x0000,
        WM_CREATE =         0x0001,
        WM_DESTROY =        0x0002,
        WM_MOVE =           0x0003,
        WM_SIZE =           0x0005,
        WM_ACTIVATE =       0x0006,
        WM_SETFOCUS =       0x0007,
        WM_KILLFOCUS =      0x0008,
        WM_ENABLE =         0x000A,
        WM_SETREDRAW =      0x000B,
        WM_SETTEXT =        0x000C,
        WM_GETTEXT =        0x000D,
        WM_GETTEXTLENGTH =  0x000E,
        WM_PAINT =          0x000F,
        WM_CLOSE =          0x0010,

        WM_ERASEBKGND =     0x0014,

        WM_SHOWWINDOW =     0x0018,

        WM_SETCURSOR =      0x0020,
        WM_MOUSEACTIVATE =  0x0021,

        WM_WINDOWPOSCHANGING =  0x0046,
        WM_WINDOWPOSCHANGED =   0x0047,

        WM_NCCREATE =       0x0081,
        WM_NCDESTROY =      0x0082,
        WM_NCCALCSIZE =     0x0083,
        WM_NCHITTEST =      0x0084,
        WM_NCPAINT =        0x0085,
        WM_NCACTIVATE =     0x0086,

        WM_MENUSELECT =     0x011F,

        WM_ENTERIDLE =          0x0121,
        WM_MENURBUTTONUP =      0x0122,
        WM_MENUDRAG =           0x0123,
        WM_MENUGETOBJECT =      0x0124,
        WM_UNINITMENUPOPUP =    0x0125,
        WM_MENUCOMMAND =        0x0126,
        WM_CHANGEUISTATE =      0x0127,
        WM_UPDATEUISTATE =      0x0128,

        WM_MOUSEMOVE =      0x0200,
        WM_LBUTTONDOWN =    0x0201,
        WM_LBUTTONUP =      0x0202,
        WM_LBUTTONDBLCLK =  0x0203,
        WM_RBUTTONDOWN =    0x0204,
        WM_RBUTTONUP =      0x0205,
        WM_RBUTTONDBLCLK =  0x0206,

        WM_EXITMENULOOP =   0x0212,

        WM_CAPTURECHANGED = 0x0215,

        WM_IME_SETCONTEXT = 0x0281,
        WM_IME_NOTIFY =     0x0282,

        WM_MOUSEHOVER =     0x02A1,
        WM_MOUSELEAVE =     0x02A3
    };

    public class IMEButton : System.Windows.Forms.Button
    {
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;
        private const int WM_SETFOCUS = 0x0007;
        private const int WM_KILLFOCUS = 0x0008;
        private const int WM_LBUTTONDOWN = 0x0201;
        private const int WM_LBUTTONUP = 0x0202;
        private const int WM_LBUTTONDBLCLK = 0x0203;
        private const int WM_RBUTTONDOWN = 0x0204;
        private const int WM_RBUTTONUP = 0x0205;
        private const int WM_RBUTTONDBLCLK = 0x0206;

        private static int msgCounter = 0;
        private UInt64 m_AppHWnd;

        public void DoPressEffect()
        {
            Color tempForeColor = this.ForeColor;
            this.ForeColor = this.BackColor;
            this.BackColor = tempForeColor;
        }

        public IMEButton() : base()
        {
            //this.SetStyle(ControlStyles.StandardClick, false);
            //this.SetStyle(ControlStyles.EnableNotifyMessage, false);
            //this.SetStyle(ControlStyles.UserMouse, false);
        }

        public UInt64 AppHWnd
        {
            get { return m_AppHWnd; }
            set { m_AppHWnd = value; }
        }

        protected override void OnMouseUp(MouseEventArgs mevent)
        {
            base.OnMouseUp(mevent);
            DoPressEffect();
        }

        protected override void WndProc(ref Message m)
        {
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Button[" + msgCounter + "]: Msg->\t" +
                Enum.GetName(typeof(WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");

            //if (m.Msg == WM_MOUSEACTIVATE)
            if (m.Msg == WM_LBUTTONDOWN)
            {
                System.Diagnostics.Debug.WriteLine("==================");
                System.Diagnostics.Debug.WriteLine("WM_MOUSEACTIVATE");
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: this.Handle->\t0x" + this.Handle.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: Parent.Handle->\t0x" + this.Parent.Handle.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: App.Handle->\t0x" + m_AppHWnd.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: m.HWnd->\t0x" + m.HWnd.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = this.Handle;
                //m.WParam = new IntPtr((long)m_AppHWnd);
                //m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));

                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                //m.Result = (IntPtr)MA_NOACTIVATE;

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: Result->\t0x" + m.Result.ToString("X"));

                DoPressEffect();
            } else            
                base.WndProc(ref m);
        }
    }

	/// <summary>
	/// Summary description for IMEStatusForm.
	/// </summary>
	public class IMEStatusForm : System.Windows.Forms.Form
	{
		private IMEButton button1;
		private IMEButton button2;
		private IMEButton button3;
		private IMEButton button6;
		private System.Windows.Forms.ContextMenu contextMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;

		int currentMenu = 0;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_ACTIVATE=1;		
		private const int MA_ACTIVATEANDEAT = 2;
		private const int MA_NOACTIVATE=0x0003;		
		private const int MA_NOACTIVATEANDEAT = 0x0004;

        private int msgCounter = 0;
        private UInt64 m_AppHWnd;

		public IMEStatusForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			this.SetStyle(ControlStyles.StandardClick,false);
			this.SetStyle(ControlStyles.EnableNotifyMessage,false);
			this.SetStyle(ControlStyles.UserMouse,false);
			//
			// TODO: Add any constructor code after InitializeComponent call
			//            
		}

		protected override void WndProc(ref Message m) 
		{
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Form[" + msgCounter + "]: Msg->\t" +
                Enum.GetName(typeof(WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");

            if (m.Msg == WM_MOUSEACTIVATE)// && m.Result==(IntPtr) MA_ACTIVATE) 
            {
                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;				

                m.Result = (IntPtr)MA_NOACTIVATE;
            }
            else if (m.Msg == (Int32)WindowsMessage.WM_NCACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_NCACTIVATE! ==");
                /*
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));
                m.Result = new IntPtr(1);
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
                */
            }
            else if (m.Msg == (Int32)WindowsMessage.WM_ACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_ACTIVATE! ==");
                ShowNoActive();
                /*
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam (before)->\t0x" + m.LParam.ToString("X"));
                m.LParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam (after)->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));
                m.Result = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
                */
            }
            else
                base.WndProc(ref m);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.button1 = new IMEButton();
            this.button2 = new IMEButton();
            this.contextMenu1 = new System.Windows.Forms.ContextMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.button3 = new IMEButton();
            this.button6 = new IMEButton();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.Desktop;
            //this.button1.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.button1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.button1.Location = new System.Drawing.Point(4, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(80, 21);
            this.button1.TabIndex = 0;
            this.button1.Text = "某輸入法";
            //this.button1.UseVisualStyleBackColor = false;
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.SystemColors.Control;
            //this.button2.CausesValidation = false;
            this.button2.ContextMenu = this.contextMenu1;
            //this.button2.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.button2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.button2.Location = new System.Drawing.Point(84, 4);
            this.button2.Name = "button2";
            //this.button2.Size = new System.Drawing.Size(40, 21);
            this.button2.TabIndex = 1;
            this.button2.Text = "button2";
            //this.button2.AutoSize = true;
            //this.button2.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            //this.button2.UseVisualStyleBackColor = false;
            this.button2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button2_MouseDown);
            this.button2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button2_MouseUp);
            // 
            // contextMenu1
            // 
            this.contextMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem1,
            this.menuItem2,
            this.menuItem3});
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 0;
            this.menuItem1.Text = "";
            // 
            // menuItem2
            // 
            this.menuItem2.Index = 1;
            this.menuItem2.Text = "test2";
            // 
            // menuItem3
            // 
            this.menuItem3.Index = 2;
            this.menuItem3.Text = "test3";
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.SystemColors.Desktop;
            //this.button3.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.button3.ForeColor = System.Drawing.SystemColors.HighlightText;
            this.button3.Location = new System.Drawing.Point(124, 4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(40, 21);
            this.button3.TabIndex = 2;
            this.button3.Text = "button3";
            //this.button3.UseVisualStyleBackColor = false;
            // 
            // button6
            // 
            this.button6.BackColor = System.Drawing.SystemColors.Desktop;
            //this.button6.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.button6.ForeColor = System.Drawing.SystemColors.HighlightText;
            this.button6.Location = new System.Drawing.Point(164, 4);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(40, 21);
            this.button6.TabIndex = 5;
            this.button6.Text = "設定";
            //this.button6.UseVisualStyleBackColor = false;
            this.button6.Visible = false;
            // 
            // IMEStatusForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(249, 38);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "IMEStatusForm";
            this.ShowInTaskbar = false;
            this.Text = "IMEStatusForm";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.ResumeLayout(false);

		}
		#endregion

        public void SetAppHWnd(UInt64 HWND)
        {
            m_AppHWnd = HWND;
            System.Diagnostics.Debug.WriteLine("AppHWnd set: " + m_AppHWnd);
            this.button2.AppHWnd = m_AppHWnd;
            System.Diagnostics.Debug.WriteLine("button2.AppHWnd: " + this.button2.AppHWnd);
        }

		public void ShowNoActive()
		{
			UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}

        public void HideNoActive()
		{
			UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}

        public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}

		public void SetChiEng(bool isChinese)
		{
			if(isChinese)
			{
				this.button3.Text = "中";
			}else
				this.button3.Text = "英";
		}
		public void SetModString(string inputs)
		{
			if(inputs == null) return;
            
			if(inputs.Length >10)
			{
				this.button2.Width = (inputs.Length)*10;
			}
			else
			{
				this.button2.Width = (inputs.Length)*2*10;
			}
            
            this.button3.Left = this.button2.Right;
			this.button6.Left = this.button3.Right;
            
            this.Width = this.button1.Width+ this.button2.Width 
				+ this.button3.Width +/*this.button6.Width*/ + 8;
			//inputs = inputs.Length + inputs;
            
			this.button2.Text = inputs;
			//button2.ContextMenu.Show(button2, new System.Drawing.Point(0, button2.Height));			
		}

		public void SetMenuModString(string inputs)
		{
			if(inputs == null) return;

			if(currentMenu == 0)
				this.menuItem1.Text = inputs;
			if(currentMenu == 1)
				this.menuItem2.Text = inputs;
			if(currentMenu == 2)
				this.menuItem3.Text = inputs;			
			currentMenu++;			
		}

		private Point mouseOffset;
		private bool isMouseDown = false;

		private void IMEStatusForm_MouseDown(object sender,System.Windows.Forms.MouseEventArgs e)
		{
			int xOffset;
			int yOffset;
			if (e.Button == MouseButtons.Left)
			{
				xOffset = -e.X - SystemInformation.FrameBorderSize.Width;
				yOffset = -e.Y;
				mouseOffset = new Point(xOffset, yOffset);
				isMouseDown = true;
			}
		}
		private void IMEStatusForm_MouseMove(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (isMouseDown)
			{				
				Point mousePos = Control.MousePosition;
				mousePos.Offset(mouseOffset.X, mouseOffset.Y);
				Location = mousePos;
			}
		}

		private void IMEStatusForm_MouseUp(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				isMouseDown = false;
			}
		}

        private void button2_MouseDown(object sender, MouseEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Mouse Down!");
            //this.button2.ContextMenu.Show(button2, new System.Drawing.Point(0, (-2)*(button2.Height)));
            //System.Diagnostics.Debug.WriteLine("Context Menu Shown!");
        }

        private void button2_MouseUp(object sender, MouseEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Mouse Up!");
            this.button2.ContextMenu.Show(button2, new System.Drawing.Point(0, button2.Bottom));
            System.Diagnostics.Debug.WriteLine("Context Menu Shown!");
        }
	}

    /*
    public class MessageFilter : IMessageFilter
    {
        public bool PreFilterMessage(ref Message objMessage)
        {
            if (objMessage.Msg == Convert.ToInt32(WindowsMessage.WM_MOUSEACTIVATE))
                return true;
            return false;
        }
    }
    */
}
