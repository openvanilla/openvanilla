using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
	/// <summary>
	/// Summary description for IMEStatusForm.
	/// </summary>
	public class IMEStatusForm : System.Windows.Forms.Form
	{
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private int msgCounter = 0;
        private UInt64 m_AppHWnd;

		private int currentMenu = 0;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private IMEButton button1;
        private IMEButton button2;
        private IMEButton button3;
        private IMEButton button6;
        private System.Windows.Forms.ContextMenu contextMenu1;
        private System.Windows.Forms.MenuItem menuItem1;

		public IMEStatusForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			this.SetStyle(ControlStyles.StandardClick, false);
			this.SetStyle(ControlStyles.EnableNotifyMessage, false);
			this.SetStyle(ControlStyles.UserMouse, false);
		
            //Application.AddMessageFilter(new MessageFilter());
        }

        #region IME-specific overridden methods
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                //System.Diagnostics.Debug.WriteLine(
                //    "Form: CreateParams.ExStyle (before) =" + cp.ExStyle);
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                //System.Diagnostics.Debug.WriteLine(
                //    "Form: CreateParams.ExStyle (after) =" + cp.ExStyle);
                return cp;
            }
        }

		protected override void WndProc(ref Message m) 
		{
            /*
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Form[" + msgCounter + "]: (before base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
            */

            base.WndProc(ref m);

            /*
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Form[" + msgCounter + "]: (after base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
             */

            /*
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_PARENTNOTIFY)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_PARENTNOTIFY! ==");
            }
            else*/
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                //System.Diagnostics.Debug.WriteLine("== Damn WM_MOUSEACTIVATE! ==");

                //System.Diagnostics.Debug.WriteLine(
                //    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));

                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;

                //System.Diagnostics.Debug.WriteLine(
                //    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
                
            }
            /*
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_NCACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_NCACTIVATE! ==");

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
            }
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_ACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_ACTIVATE! ==");
                //ShowNoActive();
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
            }
             */
        }
        #endregion

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
			this.button1 = new CSharpFormLibrary.IMEButton();
			this.button2 = new CSharpFormLibrary.IMEButton();
			this.contextMenu1 = new System.Windows.Forms.ContextMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.button3 = new CSharpFormLibrary.IMEButton();
			this.button6 = new CSharpFormLibrary.IMEButton();
			this.SuspendLayout();
			// 
			// button1
			// 
			// TODO: Code generation for 'this.button1.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button1.BackColor = System.Drawing.Color.Gray;
			this.button1.Dock = System.Windows.Forms.DockStyle.Left;
			this.button1.ForeColor = System.Drawing.Color.WhiteSmoke;
			this.button1.Location = new System.Drawing.Point(0, 0);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(96, 48);
			this.button1.TabIndex = 0;
			this.button1.Text = "某輸入法";
			// 
			// button2
			// 
			// TODO: Code generation for 'this.button2.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button2.BackColor = System.Drawing.Color.Gray;
			this.button2.ContextMenu = this.contextMenu1;
			this.button2.Dock = System.Windows.Forms.DockStyle.Left;
			this.button2.ForeColor = System.Drawing.Color.White;
			this.button2.Location = new System.Drawing.Point(96, 0);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(48, 48);
			this.button2.TabIndex = 1;
			this.button2.Text = "button2";
			this.button2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button2_MouseUp);
			this.button2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button2_MouseDown);
			// 
			// contextMenu1
			// 
			this.contextMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.menuItem1});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.Text = "";
			this.menuItem1.Click += new System.EventHandler(this.menuItem_Click);
			// 
			// button3
			// 
			// TODO: Code generation for 'this.button3.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button3.BackColor = System.Drawing.Color.Gray;
			this.button3.Dock = System.Windows.Forms.DockStyle.Left;
			this.button3.ForeColor = System.Drawing.Color.White;
			this.button3.Location = new System.Drawing.Point(144, 0);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(48, 48);
			this.button3.TabIndex = 2;
			this.button3.Text = "button3";
			// 
			// button6
			// 
			// TODO: Code generation for 'this.button6.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button6.BackColor = System.Drawing.Color.Gray;
			this.button6.Dock = System.Windows.Forms.DockStyle.Fill;
			this.button6.ForeColor = System.Drawing.Color.White;
			this.button6.Location = new System.Drawing.Point(192, 0);
			this.button6.Name = "button6";
			this.button6.Size = new System.Drawing.Size(64, 48);
			this.button6.TabIndex = 5;
			this.button6.Text = "設定";
			// 
			// IMEStatusForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.BackColor = System.Drawing.Color.Gray;
			this.ClientSize = new System.Drawing.Size(256, 48);
			this.Controls.Add(this.button6);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.ForeColor = System.Drawing.Color.Yellow;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.Name = "IMEStatusForm";
			this.ShowInTaskbar = false;
			this.Text = "IMEStatusForm";
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
			this.ResumeLayout(false);

		}
		#endregion

        #region public methods
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
			if(isChinese)   this.button3.Text = "中";
			else            this.button3.Text = "英";
		}

		public void SetModString(string inputs)
		{
			if(inputs == null) return;
            
			if(inputs.Length >10)
				this.button2.Width = (inputs.Length)*10;
			else
				this.button2.Width = (inputs.Length)*2*10;
            
            this.button3.Left = this.button2.Right;
			this.button6.Left = this.button3.Right;
            
            this.Width = this.button1.Width+ this.button2.Width 
				+ this.button3.Width +/*this.button6.Width*/ + 8;
			//inputs = inputs.Length + inputs;
            
			this.button2.Text = inputs;
			//button2.ContextMenu.Show(button2, new System.Drawing.Point(0, button2.Height));			
		}

        public void ClearMenuModString()
        {
            this.contextMenu1.MenuItems.Clear();
        }

        public void SetMenuModString(string inputs)
		{
			if(inputs == null) return;

            if (currentMenu == 0)
                this.menuItem1.Text = inputs;
            else
                this.contextMenu1.MenuItems.Add(
                    inputs, new EventHandler(menuItem_Click));

			currentMenu++;
        }

        public int GetSelectedModuleIndex()
        {
            return m_selectedModuleIndex;
        }

        #endregion

        #region private memebers

        private Point mouseOffset;
		private bool isMouseDown = false;
        private int m_selectedModuleIndex = 0;

        private void menuItem_Click(object sender, EventArgs e)
        {   
            /*
            System.Diagnostics.Debug.WriteLine(
                "Menu[xxx]: Changes to module[" +
                ((MenuItem)sender).Index + "]=" + ((MenuItem)sender).Text);
             */

            m_selectedModuleIndex = ((MenuItem)sender).Index;

            //System.Diagnostics.Debug.WriteLine("Menu[xxx]: Send message to handle " +
            //    m_AppHWnd.ToString("X"));
            int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                3);
            //System.Diagnostics.Debug.WriteLine("Menu[xxx]: Message ret=" + ret);
            //System.Diagnostics.Debug.WriteLine("Menu[xxx]: GetLastError=" + UtilFuncs.GetLastError());
        }

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
        #endregion
    }

    #region Deprecated MessageFilter implementation
    /*
    public class MessageFilter : IMessageFilter
    {
        public bool PreFilterMessage(ref Message objMessage)
        {
            System.Diagnostics.Debug.WriteLine("PreFilterMessage");
            if (objMessage.Msg == Convert.ToInt32(UtilFuncs.WindowsMessages.WM_MOUSEACTIVATE))
            {
                System.Diagnostics.Debug.WriteLine("WM_MOUSEACTIVATE");
                return true;
            }
            return false;
        }
    }
    */
    #endregion
}
