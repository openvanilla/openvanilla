using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;


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
        private static string m_baseDir =
            Environment.GetEnvironmentVariable("WINDIR") +
            System.IO.Path.DirectorySeparatorChar +
            "OpenVanilla";
        private static string m_moduleDir =
            m_baseDir +
            System.IO.Path.DirectorySeparatorChar +
            "Modules";
        private static string m_userDir =
            Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData) +
            System.IO.Path.DirectorySeparatorChar +
            "OpenVanilla";
        private static string m_confingPath =
            m_userDir +
            System.IO.Path.DirectorySeparatorChar +
            "config.xml";
        
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private IMEButton buttonMenu;
        private IMEButton buttonZhEn;
        private IMEButton buttonHantHans;
        private IMEButton buttonPref;
        private System.Windows.Forms.ContextMenu menuModule;
		private System.Windows.Forms.Panel panelHead;
		private System.Windows.Forms.Panel panelBody;

		public IMEStatusForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			this.SetStyle(ControlStyles.StandardClick, false);
			this.SetStyle(ControlStyles.EnableNotifyMessage, false);
			this.SetStyle(ControlStyles.UserMouse, false);

            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
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
            this.menuModule = new System.Windows.Forms.ContextMenu();
            this.panelHead = new System.Windows.Forms.Panel();
            this.panelBody = new System.Windows.Forms.Panel();
            this.buttonMenu = new CSharpFormLibrary.IMEButton();
            this.buttonHantHans = new CSharpFormLibrary.IMEButton();
            this.buttonZhEn = new CSharpFormLibrary.IMEButton();
            this.buttonPref = new CSharpFormLibrary.IMEButton();
            this.panelBody.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelHead
            // 
            this.panelHead.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panelHead.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.panelHead.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelHead.Location = new System.Drawing.Point(0, 0);
            this.panelHead.Name = "panelHead";
            this.panelHead.Size = new System.Drawing.Size(10, 40);
            this.panelHead.TabIndex = 1;
            this.panelHead.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.panelHead.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.panelHead.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            // 
            // panelBody
            // 
            this.panelBody.CausesValidation = false;
            this.panelBody.Controls.Add(this.buttonMenu);
            this.panelBody.Controls.Add(this.buttonHantHans);
            this.panelBody.Controls.Add(this.buttonZhEn);
            this.panelBody.Controls.Add(this.buttonPref);
            this.panelBody.Dock = System.Windows.Forms.DockStyle.Right;
            this.panelBody.Location = new System.Drawing.Point(10, 0);
            this.panelBody.Name = "panelBody";
            this.panelBody.Size = new System.Drawing.Size(170, 40);
            this.panelBody.TabIndex = 0;
            // 
            // buttonMenu
            // 
            this.buttonMenu.AppHWnd = ((ulong)(0ul));
            this.buttonMenu.AutoSize = true;
            this.buttonMenu.BackColor = Color.Transparent;
            this.buttonMenu.Dock = System.Windows.Forms.DockStyle.Left;
            this.buttonMenu.ContextMenu = this.menuModule;
            this.buttonMenu.Font = new System.Drawing.Font("PMingLiU", 12F);
            this.buttonMenu.Location = new System.Drawing.Point(0, 0);
            this.buttonMenu.Name = "buttonMenu";
            this.buttonMenu.Size = new System.Drawing.Size(65, 40);
            this.buttonMenu.TabIndex = 0;
            this.buttonMenu.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMenu_MouseUp);
            // 
            // buttonHantHans
            // 
            this.buttonHantHans.AppHWnd = ((ulong)(0ul));
            this.buttonHantHans.BackColor = Color.Transparent;
            this.buttonHantHans.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonHantHans.Font = new System.Drawing.Font("PMingLiU", 12F);
            this.buttonHantHans.Location = new System.Drawing.Point(65, 0);
            this.buttonHantHans.Name = "buttonHantHans";
            this.buttonHantHans.Size = new System.Drawing.Size(40, 40);
            this.buttonHantHans.TabIndex = 1;
            this.buttonHantHans.Visible = false;
            this.buttonHantHans.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonHantHans_MouseUp);
            // 
            // buttonZhEn
            // 
            this.buttonZhEn.AppHWnd = ((ulong)(0ul));
            this.buttonZhEn.BackColor = Color.Transparent;
            this.buttonZhEn.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonZhEn.Font = new System.Drawing.Font("PMingLiU", 12F);
            //<comment author='b6s'>
            //The localtion of buttonZhEn is overlapped with buttonHantHans temporarily.
            this.buttonZhEn.Location = new System.Drawing.Point(65, 0);
            //</comment>
            this.buttonZhEn.Name = "buttonZhEn";
            this.buttonZhEn.Size = new System.Drawing.Size(40, 40);
            this.buttonZhEn.TabIndex = 2;
            this.buttonZhEn.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonZhEn_MouseUp);
            // 
            // buttonPref
            // 
            this.buttonPref.AppHWnd = ((ulong)(0ul));
            this.buttonPref.BackColor = Color.Transparent;
            this.buttonPref.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonPref.Font = new System.Drawing.Font("PMingLiU", 12F);
            this.buttonPref.Location = new System.Drawing.Point(105, 0);
            this.buttonPref.Name = "buttonPref";
            this.buttonPref.Size = new System.Drawing.Size(65, 40);
            this.buttonPref.TabIndex = 3;
            this.buttonPref.Text = "設定";
            this.buttonPref.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPref_MouseUp);
            // 
            // IMEStatusForm
            // 
            this.ClientSize = new System.Drawing.Size(180, 40);
            this.ControlBox = false;
            this.Controls.Add(this.panelHead);
            this.Controls.Add(this.panelBody);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "IMEStatusForm";
            this.ShowInTaskbar = false;
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.panelBody.ResumeLayout(false);
            this.panelBody.PerformLayout();
            this.ResumeLayout(false);
		}
		#endregion

        #region public methods
        public void SetAppHWnd(UInt64 HWND)
        {
            m_AppHWnd = HWND;
            Debug.WriteLine("m_AppHWnd (status) =" + HWND);
            //System.Diagnostics.Debug.WriteLine("AppHWnd set: " + m_AppHWnd);
            //this.buttonMenu.AppHWnd = m_AppHWnd;
            //this.buttonZhEn.AppHWnd =
            //System.Diagnostics.Debug.WriteLine("buttonMenu.AppHWnd: " + this.buttonMenu.AppHWnd);
        }

		public void ShowNoActive()
		{
            if (!this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}

        public void HideNoActive()
		{
            if (this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}

        public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}

		public void SetChiEng(bool isChinese)
		{
			if(isChinese)   this.buttonZhEn.Text = "中";
			else            this.buttonZhEn.Text = "英";
		}

        public void SetSimpifiedOrTraditional(bool isTraditional)
        {
            if (isTraditional) this.buttonHantHans.Text = "繁";
            else this.buttonHantHans.Text = "簡";
        }

        public void SetModString(int index)
        {
            if (index > this.menuModule.MenuItems.Count) return;
            this.buttonMenu.Text = this.menuModule.MenuItems[index].Text;
            ResizeAll();
        }

        public void ClearMenuModString()
        {
            this.menuModule.MenuItems.Clear();
        }

        public void SetMenuModString(string inputs)
		{
			if(inputs == null) return;

            System.Diagnostics.Debug.WriteLine("\tstatus: set menu=\t" + inputs);
            this.menuModule.MenuItems.Add(
                inputs, new EventHandler(menuItem_Click));
            //System.Diagnostics.Debug.WriteLine("\tstatus: menu size=\t" + this.menuModule.MenuItems.Count);
            
            SetModString(0);
        }

        public void SetDir(string baseDir, string moduleDir , string userDir)
		{
            m_baseDir = baseDir;
            m_moduleDir = moduleDir;
            m_userDir = userDir;
            /*Debug.WriteLine(m_baseDir);
            Debug.WriteLine(m_moduleDir);
            Debug.WriteLine(m_userDir);*/
        }

        public int GetSelectedModuleIndex()
        {
            return m_selectedModuleIndex;
        }

        public void RotateModule()
        {
            m_selectedModuleIndex++;
            if (m_selectedModuleIndex == menuModule.MenuItems.Count)
                m_selectedModuleIndex = 0;
            this.buttonMenu.Text =
                menuModule.MenuItems[m_selectedModuleIndex].Text;
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

        private void buttonMenu_MouseUp(object sender, MouseEventArgs e)
        {            
            this.buttonMenu.ContextMenu.Show(buttonMenu, new System.Drawing.Point(0, buttonMenu.Bottom));
            //System.Diagnostics.Debug.WriteLine("status: context menu shown.");
        }

        private void buttonZhEn_MouseUp(object sender, MouseEventArgs e) //中英
        {
            Debug.WriteLine("中英-> m_AppHWnd=" + m_AppHWnd);
            int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                2);
        }

        private void buttonHantHans_MouseUp(object sender, MouseEventArgs e) //繁簡
        {
            //Debug.WriteLine("繁簡-> m_AppHWnd=" + m_AppHWnd);
            int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                4);
        } 

        private void buttonPref_MouseUp(object sender, MouseEventArgs e) //設定
        {
            Process proc = new System.Diagnostics.Process();
            proc.StartInfo.FileName =
                m_baseDir + System.IO.Path.DirectorySeparatorChar +
                "OVPreferences.exe";
            proc.StartInfo.WorkingDirectory = m_baseDir;
            System.IO.FileInfo configFile =
                new System.IO.FileInfo(m_confingPath);

            int ret = UtilFuncs.SendMessage(
                   new IntPtr((long)m_AppHWnd),
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);

            proc.Start();
            proc.WaitForExit();
            /*
            configFile.Refresh();
            DateTime before = configFile.LastWriteTime;
            System.Threading.Thread.Sleep(1000); //is necessary!!
            while (true)
            {
                configFile.Refresh();
                DateTime after = configFile.LastWriteTime;
                if (!after.Equals(before))
                {
                    if(!proc.HasExited)
                        proc.Kill();
                    break;
                }
                System.Threading.Thread.Sleep(1000);                
            }
            */

            ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_CREATE,
                0, 0);

            ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0x0002, //IMN_OPENSTATUSWINDOW
                0);

            ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_RELOADCONFIG,
                0, 0);

            /*
            ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_SETCONTEXT,
                1, 0);
             */

            /*
            ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_STARTCOMPOSITION,
                0, 0);
             */
        }

        private void ResizeAll()
        {
            this.Width =
                this.panelHead.Width +
                this.buttonMenu.Width +
                (this.buttonHantHans.Visible ? this.buttonHantHans.Width : 0) +
                this.buttonZhEn.Width +
                this.buttonPref.Width;
            this.panelBody.Width = this.Width - this.panelHead.Width;
            this.buttonZhEn.Location = new Point(this.buttonMenu.Bounds.Right, 0);
            this.buttonPref.Location = new Point(this.buttonZhEn.Bounds.Right, 0);
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


