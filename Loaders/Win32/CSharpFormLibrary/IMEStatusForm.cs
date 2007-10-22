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
        private IntPtr m_AppHWnd;

        //@warning OV-specific paths here!
        private static string m_baseDir =
            "C:" +
            //Environment.GetEnvironmentVariable("WINDIR") +
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
		private System.ComponentModel.Container m_components = null;
        private IMEButton m_btMenu;
        private IMEButton m_btZhEn;
        private IMEButton m_btHantHans;
        private IMEButton m_btPref;
        private IMEButton m_btSymbol;
        private ContextMenu m_cmModule;
        private Panel m_pnHead;
		private Panel m_pnBody;
        private bool m_isHanji = true;

        //private CSharpFormLibrary.IMESymbolForm m_symbolForm = null;
        private IMEFilterForm m_symbolForm = null;
        
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
                cp.ExStyle |= 0x00000004; //<WS_EX_NOPARENTNOTIFY
                cp.ExStyle |= 0x80; //<WS_EX_TOOLWINDOW to hide from alt-tab
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
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if (m_components != null)
				{
					m_components.Dispose();
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
            this.m_cmModule = new System.Windows.Forms.ContextMenu();
            this.m_pnHead = new System.Windows.Forms.Panel();
            this.m_pnBody = new System.Windows.Forms.Panel();
            this.m_btMenu = new CSharpFormLibrary.IMEButton();
            this.m_btZhEn = new CSharpFormLibrary.IMEButton();
            this.m_btPref = new CSharpFormLibrary.IMEButton();
            this.m_btHantHans = new CSharpFormLibrary.IMEButton();
            this.m_btSymbol = new CSharpFormLibrary.IMEButton();
            this.m_pnBody.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnHead
            // 
            this.m_pnHead.BackColor = System.Drawing.Color.Gray;
            this.m_pnHead.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_pnHead.Cursor = System.Windows.Forms.Cursors.SizeAll;
            //this.m_pnHead.Dock = System.Windows.Forms.DockStyle.Left;
            this.m_pnHead.Location = new System.Drawing.Point(0, 0);
            this.m_pnHead.Margin = new System.Windows.Forms.Padding(0);
            this.m_pnHead.Name = "m_pnHead";
            this.m_pnHead.Size = new System.Drawing.Size(8, 36);
            this.m_pnHead.TabIndex = 1;
            this.m_pnHead.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.m_pnHead.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.m_pnHead.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            // 
            // m_pnBody
            // 
            this.m_pnBody.AutoSize = true;
            this.m_pnBody.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_pnBody.BackColor = System.Drawing.SystemColors.ControlDark;
            this.m_pnBody.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_pnBody.CausesValidation = false;
            this.m_pnBody.Controls.Add(this.m_btMenu);
            this.m_pnBody.Controls.Add(this.m_btZhEn);
            this.m_pnBody.Controls.Add(this.m_btPref);
            //this.m_pnBody.Dock = System.Windows.Forms.DockStyle.Left;
            this.m_pnBody.Location = new System.Drawing.Point(10, 0);
            this.m_pnBody.Margin = new System.Windows.Forms.Padding(0);
            this.m_pnBody.Name = "m_pnBody";
            this.m_pnBody.Size = new System.Drawing.Size(154, 36);
            this.m_pnBody.TabIndex = 0;
            // 
            // m_btMenu
            // 
            this.m_btMenu.AutoSize = true;
            this.m_btMenu.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_btMenu.BackColor = System.Drawing.Color.Transparent;
            this.m_btMenu.ContextMenu = this.m_cmModule;
            this.m_btMenu.Dock = System.Windows.Forms.DockStyle.Left;
            this.m_btMenu.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btMenu.ForeColor = System.Drawing.Color.White;
            this.m_btMenu.Location = new System.Drawing.Point(0, 0);
            this.m_btMenu.Margin = new System.Windows.Forms.Padding(0);
            this.m_btMenu.Name = "m_btMenu";
            this.m_btMenu.Size = new System.Drawing.Size(60, 34);
            this.m_btMenu.TabIndex = 0;
            this.m_btMenu.TextAlign = ContentAlignment.MiddleCenter;
            this.m_btMenu.UseVisualStyleBackColor = false;
            this.m_btMenu.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMenu_MouseUp);
            // 
            // m_btZhEn
            // 
            this.m_btZhEn.BackColor = System.Drawing.Color.Transparent;
            this.m_btZhEn.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btZhEn.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btZhEn.Location = new System.Drawing.Point(60, 0);
            this.m_btZhEn.Margin = new System.Windows.Forms.Padding(0);
            this.m_btZhEn.Name = "m_btZhEn";
            this.m_btZhEn.Size = new System.Drawing.Size(34, 34);
            this.m_btZhEn.TabIndex = 2;
            this.m_btZhEn.Text = "字";
            this.m_btZhEn.TextAlign = ContentAlignment.MiddleCenter;
            this.m_btZhEn.UseVisualStyleBackColor = false;
            this.m_btZhEn.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonZhEn_MouseUp);
            // 
            // m_btPref
            // 
            this.m_btPref.BackColor = System.Drawing.Color.Transparent;
            this.m_btPref.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btPref.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btPref.Location = new System.Drawing.Point(94, 0);
            this.m_btPref.Margin = new System.Windows.Forms.Padding(0);
            this.m_btPref.Name = "m_btPref";
            this.m_btPref.Size = new System.Drawing.Size(60, 34);
            this.m_btPref.TabIndex = 3;
            this.m_btPref.Text = "設定";
            this.m_btPref.TextAlign = ContentAlignment.MiddleCenter;
            this.m_btPref.UseVisualStyleBackColor = false;
            this.m_btPref.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPref_MouseUp);
            // 
            // m_btHantHans
            // 
            this.m_btHantHans.BackColor = System.Drawing.Color.Transparent;
            this.m_btHantHans.ForeColor = System.Drawing.Color.White;
            this.m_btHantHans.Location = new System.Drawing.Point(154, 0);
            this.m_btHantHans.Margin = new System.Windows.Forms.Padding(0);
            this.m_btHantHans.Name = "m_btHantHans";
            this.m_btHantHans.Size = new System.Drawing.Size(34, 34);
            this.m_btHantHans.TabIndex = 1;
            this.m_btHantHans.UseVisualStyleBackColor = false;
            this.m_btHantHans.Visible = false;
            this.m_btHantHans.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonHantHans_MouseUp);
            // 
            // m_btSymbol
            // 
            this.m_btSymbol.BackColor = System.Drawing.Color.Transparent;
            this.m_btSymbol.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btSymbol.Location = new System.Drawing.Point(188, 0);
            this.m_btSymbol.Margin = new System.Windows.Forms.Padding(0);
            this.m_btSymbol.Name = "m_btSymbol";
            this.m_btSymbol.Size = new System.Drawing.Size(34, 34);
            this.m_btSymbol.TabIndex = 4;
            this.m_btSymbol.Text = "€";
            this.m_btSymbol.UseVisualStyleBackColor = false;
            this.m_btSymbol.Visible = false;
            this.m_btSymbol.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonSymbol_MouseUp);
            // 
            // IMEStatusForm
            // 
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.Color.LightGray;
            this.ClientSize = new System.Drawing.Size(166, 36);
            this.ControlBox = false;
            this.Controls.Add(this.m_pnHead);
            this.Controls.Add(this.m_pnBody);
            this.ForeColor = System.Drawing.Color.Transparent;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.Name = "IMEStatusForm";
            this.ShowInTaskbar = false;
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.m_pnBody.ResumeLayout(false);
            this.m_pnBody.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

        #region public methods
        public void SetAppHWnd(IntPtr HWND)
        {
            m_AppHWnd = HWND;
            Debug.WriteLine("m_AppHWnd (status) =" + HWND);
        }

        public IntPtr GetHandle()
        {
            return this.Handle;
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

		public void SetChiEng()
		{
            m_isHanji = !m_isHanji;
            if (m_isHanji)
                this.m_btZhEn.Text = "字";
			else
                this.m_btZhEn.Text = "A";
		}

        public void SetSimpifiedOrTraditional(bool isTraditional)
        {
            if (isTraditional)
                this.m_btHantHans.Text = "繁";
            else
                this.m_btHantHans.Text = "簡";
        }

        public void SetModString(int index)
        {
            if (index > this.m_cmModule.MenuItems.Count) return;
            this.m_btMenu.Text = this.m_cmModule.MenuItems[index].Text;
            ResizeAll();            
        }

        public void ClearMenuModString()
        {
            this.m_cmModule.MenuItems.Clear();
        }

        public void SetMenuModString(string inputs)
		{
			if(inputs == null) return;

            System.Diagnostics.Debug.WriteLine("\tstatus: set menu=\t" + inputs);
            this.m_cmModule.MenuItems.Add(
                inputs, new EventHandler(menuItem_Click));
            //System.Diagnostics.Debug.WriteLine("\tstatus: menu size=\t" + this.m_cmModule.MenuItems.Count);
            if(this.m_btMenu.Text=="")
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
            if (m_selectedModuleIndex == m_cmModule.MenuItems.Count)
                m_selectedModuleIndex = 0;
            this.m_btMenu.Text =
                m_cmModule.MenuItems[m_selectedModuleIndex].Text;
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
            this.m_btMenu.ContextMenu.Show(m_btMenu, new System.Drawing.Point(0, m_btMenu.Bottom));
            //System.Diagnostics.Debug.WriteLine("status: context menu shown.");
        }

        private void buttonZhEn_MouseUp(object sender, MouseEventArgs e) //中英
        {
            Debug.WriteLine("中英-> m_AppHWnd=" + m_AppHWnd);
            SendKeys.SendWait("+"); //send shift
            //comment below because handing over the job to modules
            /* int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                2);*/
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

        private void proc_Exited(object sender, EventArgs e) //
        {
            //System.Threading.Thread.Sleep(1000);
            int ret;
            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_CREATE,
                0, 0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0x0002, //IMN_OPENSTATUSWINDOW
                0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_RELOADCONFIG,
                0, 0);
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

            int ret = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);

            proc.Start();
            proc.Exited+=new EventHandler(proc_Exited);           
        }

        private void buttonSymbol_MouseUp(object sender, MouseEventArgs e) //符號表
        {            
         /*   int ret = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);*/
            if (m_symbolForm == null)
            {
                //m_symbolForm = new IMESymbolForm();
                m_symbolForm = new IMEFilterForm();
                /*
                string[] fakeInput = new string[4];
                fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                fakeInput[1] = "1 2 3 4 5 6 7";
                fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
                fakeInput[3] = "阿 ㄇ ㄌ";
                m_symbolForm.SetSymbols(fakeInput);
                //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
                m_symbolForm.SetLocation(800, 600);
                m_symbolForm.ShowNoActive();
                 */
                string[] fakeInput = new string[4];
                fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                fakeInput[1] = "1 2 3 4 5 6 7";
                fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
                fakeInput[3] = "阿 ㄇ ㄌ";
                m_symbolForm.SetPossibleList("ㄗㄢˇ ㄗㄤ");//ㄓㄢ ㄓㄢˊ 
                m_symbolForm.SetSymbols(fakeInput);
                //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
                // m_symbolForm.SetLocation(800, 600);
                m_symbolForm.ShowNoActive();
            }
            else
            {/*
                if (m_symbolForm.IsDisposed)
                {
                    m_symbolForm = null;
                    m_symbolForm = new IMESymbolForm();6
                    string[] fakeInput = new string[4];
                    fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                    fakeInput[1] = "1 2 3 4 5 6 7";
                    fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
                    fakeInput[3] = "阿 ㄇ ㄌ";
                    m_symbolForm.SetSymbols(fakeInput);
                    //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
                    m_symbolForm.SetLocation(800, 600);
                    m_symbolForm.ShowNoActive();
                }
                else
                {
                    m_symbolForm.Dispose();
                    m_symbolForm = null;
                }*/
            }
        }

        private void ResizeAll()
        {
            Debug.WriteLine("ReSize");
            this.m_btMenu.Location = new Point(0, 0);
            this.m_btZhEn.Location = new Point(this.m_btMenu.Bounds.Right, 0);
            this.m_btPref.Location = new Point(this.m_btZhEn.Bounds.Right, 0);

            this.m_pnBody.Width =
                this.m_btMenu.Width +
                (this.m_btHantHans.Visible ? this.m_btHantHans.Width : 0) +
                this.m_btZhEn.Width + this.m_btPref.Width +
                (this.m_btSymbol.Visible ? this.m_btSymbol.Width : 0);
            this.Width = this.m_pnBody.Width + this.m_pnHead.Width + 2;
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


