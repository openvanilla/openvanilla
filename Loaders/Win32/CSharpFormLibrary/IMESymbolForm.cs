using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
    public class IMESymbolForm : System.Windows.Forms.Form
    {        
        public IMEListView m_lv;
        private int currentPage = 0; //目前頁次 
        private int totalPage = 0; //傳入的 Symbol 共有幾頁                 
        private ArrayList m_candArray; //傳入的 candi array
        private string[] m_inputs;
        private int m_mode = 0; //0:直式  1:展開

        //當頁的index
        //int now_index;		
        //int baseSize;	//基本長度(沒有Symbol時form的大小)
        private IntPtr m_hwnd;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ImageList imageList1;
        private Label label1;
        private Panel panel1;
        private Panel panel2;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private Point mouseOffset;
        
        /*private TabPage tabPage1;
        private TabPage tabPage2;
        private TabPage tabPage3;*/
        private IContainer components;
        private Panel panel3;
        private IMEButton buttonUp;
        private IMEButton buttonDown;
        private Panel panel4;
        private TabControl tabControl1;
        private bool isMouseDown = false;

        public IMESymbolForm()
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        public IMESymbolForm(string[] Symbols)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;
                return;
            }            
        }

        protected override void Dispose(bool disposing)
        {        
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.buttonUp = new CSharpFormLibrary.IMEButton();
            this.buttonDown = new CSharpFormLibrary.IMEButton();
            this.panel4 = new System.Windows.Forms.Panel();
            //this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabControl1 = new IMETabControl();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 25;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Width = 80;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.label1.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.label1.Location = new System.Drawing.Point(3, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "符號表(施工中)";
            this.label1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseDown);
            this.label1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseMove);
            this.label1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseUp);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(236, 21);
            this.panel1.TabIndex = 1;
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseDown);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseMove);
            this.panel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseUp);
            // 
            // panel2
            // 
            this.panel2.AutoSize = true;
            this.panel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel2.Controls.Add(this.panel3);
            this.panel2.Controls.Add(this.panel4);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 21);
            this.panel2.Margin = new System.Windows.Forms.Padding(0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(236, 187);
            this.panel2.TabIndex = 0;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.buttonUp);
            this.panel3.Controls.Add(this.buttonDown);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel3.Location = new System.Drawing.Point(201, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(35, 187);
            this.panel3.TabIndex = 4;
            // 
            // buttonUp
            // 
            this.buttonUp.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonUp.Location = new System.Drawing.Point(0, 2);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Size = new System.Drawing.Size(35, 98);
            this.buttonUp.TabIndex = 5;
            this.buttonUp.Text = "上";
            this.buttonUp.UseVisualStyleBackColor = true;
            this.buttonUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonUp_MouseUp);
            // 
            // buttonDown
            // 
            this.buttonDown.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonDown.Location = new System.Drawing.Point(0, 100);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Size = new System.Drawing.Size(35, 87);
            this.buttonDown.TabIndex = 4;
            this.buttonDown.Text = "下";
            this.buttonDown.UseVisualStyleBackColor = true;
            this.buttonDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonDown_MouseUp);
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.tabControl1);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel4.Location = new System.Drawing.Point(0, 0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(236, 187);
            this.panel4.TabIndex = 2;
            // 
            // tabControl1
            // 
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.Padding = new System.Drawing.Point(0, 0);
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(236, 187);
            this.tabControl1.TabIndex = 0;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // IMESymbolForm
            // 
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(236, 208);
            this.ControlBox = false;
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "IMESymbolForm";
            this.ShowInTaskbar = false;
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel4.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();
            this.CanTakeFocus = false;

        }
        #endregion

        #region public methods

        public void SetSymbols(string[] inputs)
        {
            tabControl1.Controls.Clear();
            int counter=1;
            foreach (string input in inputs)
            {
                string[] a_input = input.Split(' ');
                if (a_input == null) continue;
//                string fakeInputs2 = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                
                //new tabpage
                TabPage tp = new TabPage();                
                //tp.Location = new System.Drawing.Point(4, 24);
                tp.Margin = new System.Windows.Forms.Padding(0);
                //tp.Name = "tabPage1";
                //tp.Size = new System.Drawing.Size(228, 159);
                //tp.TabIndex = 2;               
                tp.Text = "class"+counter.ToString();
                tp.UseVisualStyleBackColor = true;                

                //add tabpage to tabcontrol                
                tabControl1.Controls.Add(tp);

                //new listview
                IMEListView lv= new IMEListView();
                lv.SetCapacity(6, 6);
                lv.SetContent(input);

                //add listview to tabpage
                tp.Controls.Add(lv);

                counter++;
            }
            this.panel2.Margin = Padding.Empty;
            this.panel2.Padding = Padding.Empty;                        
            m_lv = (IMEListView)tabControl1.SelectedTab.Controls[0];
            this.panel2.Width = m_lv.Width;
            this.buttonUp.Height = m_lv.Height / 2;
            this.buttonDown.Height = m_lv.Height / 2;
        }

        public string GetSelectedItem()
        {
            return m_lv.GetSelectedItem();
        }

        public void SelectUp()
        {


        }

        public void SelectDown()
        { 
        
        }

        public void SelectLeft()
        {

        }

        public void SelectRight()
        { 
        
        }

        public void SetLocation(int x, int y)
        {
            this.Location = new Point(x, y);
        }

        public void DisposeForm()
        {
            this.Dispose(true);
        }

        public void ClearSymbols()
        {
            this.m_lv.Items.Clear();
        }

        public void SetHWND(IntPtr hwnd)
        {
            m_hwnd = hwnd;
        }

        public bool CanTakeFocus
        {
            get { return this.CanFocus; }
            set
            {
                this.SetStyle(ControlStyles.Selectable, value);    // <--- making control non-selectable
            }
        }

        #endregion

        #region a protected override method for WS_POPUP

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.Style = unchecked((int)0x80000000); //WS_POPUP
                return cp;
                /*const int WS_EX_NOACTIVATE = 0x08000000;
                CreateParams cp = base.CreateParams;
                cp.ExStyle = cp.ExStyle | WS_EX_NOACTIVATE;
                return cp;*/
            }
        }

        #endregion

        #region private methods

        private void IMESymbolForm_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
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

        private void IMESymbolForm_MouseMove(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (isMouseDown)
            {
                Point mousePos = Control.MousePosition;
                mousePos.Offset(mouseOffset.X, mouseOffset.Y);
                Location = mousePos;
            }
        }

        private void IMESymbolForm_MouseUp(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                isMouseDown = false;
            }
        }   
        #endregion

        public int GetWidth()
        {
            return this.Width;
        }

        public int GetHeight()
        {
            return this.Height;
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

        private void buttonUp_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv=(IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }


        private void buttonDown_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv = (IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }


        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {            
            if (this.tabControl1.SelectedTab.Controls.Count!=0)
            {
               m_lv = (IMEListView)tabControl1.SelectedTab.Controls[0];      
            }   
        }
        
    }
}


