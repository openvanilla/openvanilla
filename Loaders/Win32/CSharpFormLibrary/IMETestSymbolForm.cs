using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
    public partial class IMETestSymbolForm : Form
    {
        public IMEListView m_lv;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;
        private IntPtr m_hwnd;
        private Point mouseOffset;
        private bool isMouseDown = false;
        
        public IMETestSymbolForm()
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        public IMETestSymbolForm(string[] Symbols)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATE;
                return;
            }
        }

        public void SetSymbols(string[] inputs)
        {
            tabControl1.Controls.Clear();
            int counter=1;
            foreach (string input in inputs)
            {
                string[] a_input = input.Split(' ');
                if (a_input == null) continue;
//                string fakeInputs2 = "¡A ¡C ¡B ¡F ¡I ¡H ¡¡ ¡y ¡] ¡i ¡­ ¢H ¡® ¡¯ ¡° ¡³ ¡· ¡¼ ¡À ¡Ï ¡¯ ¡° ¡³ ¡· ¡Õ ¡Ö ¡× ¡× ¢C ¢D ¢W £á ¢J ¢P ¢R ¢Q ¢V ¡j ¡^ ¡z ¡¢";
                
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
            this.button1.Height = m_lv.Height / 2;
            this.button2.Height = m_lv.Height / 2;
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
        public bool CanTakeFocus
        {
            get { return this.CanFocus; }
            set
            {
                this.SetStyle(ControlStyles.Selectable, value);    // <--- making control non-selectable
            }
        }

        private void IMETestSymbolForm_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
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

        private void IMETestSymbolForm_MouseMove(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (isMouseDown)
            {
                Point mousePos = Control.MousePosition;
                mousePos.Offset(mouseOffset.X, mouseOffset.Y);
                Location = mousePos;
            }
        }

        private void IMETestSymbolForm_MouseUp(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                isMouseDown = false;
            }
        }   

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedTab.Controls.Count != 0)
            {
                m_lv = (IMEListView)tabControl1.SelectedTab.Controls[0];
            }
        }

        private void button1_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv = (IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }
        private void button2_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv = (IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }
    }
}