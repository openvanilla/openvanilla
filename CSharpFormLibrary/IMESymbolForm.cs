using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace OVManagedUI
{
    public partial class IMESymbolForm : Form
    {
        public IMEListView m_lv;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private Point mouseOffset;
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
                
                //new tabpage
                TabPage tp = new TabPage();                
                tp.Margin = new System.Windows.Forms.Padding(0);                
                tp.Text = a_input[0];
                tp.Font =  new System.Drawing.Font("PMingLiU", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));                
                tp.UseVisualStyleBackColor = true;                
                
                //add tabpage to tabcontrol
                tabControl1.Controls.Add(tp);

                //new listview
                IMEListView lv= new IMEListView();
                lv.BackColor = Color.GhostWhite;
                lv.BorderStyle = BorderStyle.Fixed3D;
                lv.SetCapacity(7, 7);
                lv.SetContent(input);

                //add listview to tabpage
                tp.Controls.Add(lv);
                counter++;
            }
                     
            m_lv = (IMEListView)tabControl1.SelectedTab.Controls[0];
       
            //width
            this.Width = m_lv.Width + this.panel4.Width+10;
            this.panel3.Width = m_lv.Width+10;
           
            //height
            int tabsPerLine = panel3.Width / this.tabControl1.ItemSize.Width ;
            int tabLines = 1;
            if (this.tabControl1.TabCount > 0)
            {
                tabLines = (this.tabControl1.TabCount - 1) / tabsPerLine + 1;
            }                        
            this.Height = this.panel1.Height+ m_lv.Height + (this.tabControl1.ItemSize.Height)*tabLines+20;
            this.panel2.Height = this.Height - this.panel1.Height;
            this.panel3.Height = this.panel2.Height;
            this.button1.Top = panel4.Top;
            this.button1.Height = panel4.Height / 2;
            this.button2.Top = this.button1.Bottom;
            this.button2.Height = panel4.Height / 2;            
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
        //public bool CanTakeFocus
        //{
        //    get { return this.CanFocus; }
        //    set
        //    {
        //        this.SetStyle(ControlStyles.Selectable, value);    // <--- making control non-selectable
        //    }
        //}

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
        private void button3_MouseUp(object sender, MouseEventArgs e)
        {
            this.Dispose();
        }
    }
}