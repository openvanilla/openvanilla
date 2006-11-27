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

        //private int msgCounter = 0;
        private UInt64 m_AppHWnd;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private IMEButton button2;
        private IMEButton button3;
        private IMEButton button6;
        private System.Windows.Forms.ContextMenu contextMenu1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
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
			this.button2 = new CSharpFormLibrary.IMEButton();
			this.contextMenu1 = new System.Windows.Forms.ContextMenu();
			this.button3 = new CSharpFormLibrary.IMEButton();
			this.button6 = new CSharpFormLibrary.IMEButton();
			this.label1 = new System.Windows.Forms.Label();
			this.panel1 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// button2
			// 
			// TODO: Code generation for 'this.button2.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button2.BackColor = System.Drawing.Color.Gray;
			this.button2.ContextMenu = this.contextMenu1;
			this.button2.Dock = System.Windows.Forms.DockStyle.Right;
			this.button2.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button2.ForeColor = System.Drawing.Color.White;
			this.button2.Location = new System.Drawing.Point(300, 0);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(48, 32);
			this.button2.TabIndex = 1;
			this.button2.Text = "button2";
			this.button2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button2_MouseUp);
			// 
			// button3
			// 
			// TODO: Code generation for 'this.button3.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button3.BackColor = System.Drawing.Color.Gray;
			this.button3.Dock = System.Windows.Forms.DockStyle.Right;
			this.button3.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button3.ForeColor = System.Drawing.Color.White;
			this.button3.Location = new System.Drawing.Point(348, 0);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(48, 32);
			this.button3.TabIndex = 2;
			this.button3.Text = "button3";
			// 
			// button6
			// 
			// TODO: Code generation for 'this.button6.AppHWnd' failed because of Exception 'Invalid Primitive Type: System.UInt64. Only CLS compliant primitive types can be used. Consider using CodeObjectCreateExpression.'.
			this.button6.BackColor = System.Drawing.Color.Gray;
			this.button6.Dock = System.Windows.Forms.DockStyle.Right;
			this.button6.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button6.ForeColor = System.Drawing.Color.White;
			this.button6.Location = new System.Drawing.Point(396, 0);
			this.button6.Name = "button6";
			this.button6.Size = new System.Drawing.Size(60, 32);
			this.button6.TabIndex = 5;
			this.button6.Text = "設定";
			// 
			// label1
			// 
			this.label1.Cursor = System.Windows.Forms.Cursors.Hand;
			this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label1.Font = new System.Drawing.Font("PMingLiU", 16.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(224)), ((System.Byte)(224)), ((System.Byte)(224)));
			this.label1.Location = new System.Drawing.Point(0, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 32);
			this.label1.TabIndex = 6;
			this.label1.Text = "囧";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.label1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
			this.label1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
			this.label1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.label1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(64, 32);
			this.panel1.TabIndex = 7;
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.button2);
			this.panel2.Controls.Add(this.button3);
			this.panel2.Controls.Add(this.button6);
			this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel2.Location = new System.Drawing.Point(64, 0);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(456, 32);
			this.panel2.TabIndex = 8;
			// 
			// IMEStatusForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.BackColor = System.Drawing.Color.Gray;
			this.ClientSize = new System.Drawing.Size(520, 32);
			this.Controls.Add(this.panel2);
			this.Controls.Add(this.panel1);
			this.ForeColor = System.Drawing.Color.Yellow;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.Name = "IMEStatusForm";
			this.ShowInTaskbar = false;
			this.Text = "IMEStatusForm";
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
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
            
            this.Width = this.label1.Width+ this.button2.Width 
				+ this.button3.Width +this.button6.Width ;
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

            System.Diagnostics.Debug.WriteLine("status: set menu=\t" + inputs);
            this.contextMenu1.MenuItems.Add(
                inputs, new EventHandler(menuItem_Click));

            System.Diagnostics.Debug.WriteLine(
                "status: menu size=\t" + this.contextMenu1.MenuItems.Count);
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

        private void button2_MouseUp(object sender, MouseEventArgs e)
        {            
            this.button2.ContextMenu.Show(button2, new System.Drawing.Point(0, button2.Bottom));
            System.Diagnostics.Debug.WriteLine("status: context menu shown.");
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
