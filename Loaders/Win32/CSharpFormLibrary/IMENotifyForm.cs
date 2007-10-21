using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
    public class IMENotifyForm : System.Windows.Forms.Form
    {
        private IContainer components;
        //<comment author='b6s'>Not used codes
        //private int currentPage = 0; //目前頁次
        //private int totalPage = 0; //傳入的 Notify 共有幾頁
        //private ArrayList m_NotifyArray; //傳入的 Notify array
        //private string[] m_inputs;
        //private int m_mode = 0; //0:直式  1:展開
        //</comment>

        //當頁的index
        //int now_index;		
        //int baseSize;	//基本長度(沒有Notify時form的大小)
        private IntPtr m_hwnd;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        //<comment author='b6s'>Not used codes
        //private ColumnHeader columnHeader1;
        //private ColumnHeader columnHeader2;
        //</comment>
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        public IMENotifyForm()
        {
            
            InitializeComponent();
            //baseSize = this.lbNotifys.Location.Y+100;
            //this.Opacity = 1;
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        public IMENotifyForm(string[] Notifys)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                //m.Result = (IntPtr)MA_NOACTIVATE;
                return;
            }

            base.WndProc(ref m);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // IMENotifyForm
            // 
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.Info;
            this.ClientSize = new System.Drawing.Size(208, 137);
            this.Font = new System.Drawing.Font("MS PGothic", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "IMENotifyForm";
            this.ShowInTaskbar = false;
            this.Deactivate += new System.EventHandler(this.IMENotifyForm_Deactivate);
            this.Enter += new System.EventHandler(this.IMENotifyForm_Enter);
            this.Activated += new System.EventHandler(this.IMENotifyForm_Activated);
            this.Validated += new System.EventHandler(this.IMENotifyForm_Validated);
            this.Click += new System.EventHandler(this.IMENotifyForm_Click);
            this.Validating += new System.ComponentModel.CancelEventHandler(this.IMENotifyForm_Validating);
            this.Leave += new System.EventHandler(this.IMENotifyForm_Leave);
            this.ResumeLayout(false);

        }
        #endregion

        #region public methods

        public void SetNotifyStr(string inputs)
        {            
            NotifyWindow nw;
            nw = new NotifyWindow("OpenVanilla Notify", inputs);
            nw.TitleClicked += new System.EventHandler(titleClick);
            nw.TextClicked += new System.EventHandler(textClick);
            nw.SetDimensions(200, 120);
            nw.Notify();
        }

        public void SetLocation(int x, int y)
        {
            this.Location = new Point(x, y);
        }

        public void DisposeForm()
        {
            this.Dispose(true);
        }

        public void ClearNotify()
        {
            
        }

        public void SetHWND(IntPtr hwnd)
        {
            m_hwnd = hwnd;
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

        private void IMENotifyForm_Click(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Click");
        }

        private void lbNotifys_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            //MessageBox.Show(this.lbNotifys.Parent.ToString());
            //SendKeys.SendWait((this.lbNotifys.SelectedIndex+1).ToString());
        }

        private void IMENotifyForm_Activated(object sender, System.EventArgs e)
        {
            //ShowNoActive();
            //if(!isTouched) 
            {
                //UtilFuncs.GiveBackFocus(m_hwnd);
                System.Diagnostics.Debug.Write("Notify Active(); give back focus to ");
                System.Diagnostics.Debug.WriteLine(m_hwnd);
                //	isTouched = true;
            }//else
            //	isTouched = false;
        }

        private void IMENotifyForm_Deactivate(object sender, System.EventArgs e)
        {
            //HideNoActive();
            System.Diagnostics.Debug.WriteLine("Notify DeActive();");
        }

        private void lbNotifys_Enter(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("lbNotifys_Enter");
        }

        private void lbNotifys_Leave(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("lbNotifys_Leave");
        }

        private void IMENotifyForm_Enter(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Notify Enter");
        }

        private void IMENotifyForm_Leave(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Notify Leave");
        }

        private void IMENotifyForm_Validated(object sender, System.EventArgs e)
        {

            System.Diagnostics.Debug.WriteLine("Notify Validated");
        }
        private void IMENotifyForm_Validating(object sender, System.ComponentModel.CancelEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Notify Validating");

        }

        private void lbNotifys_KeyDown(object sender, KeyEventArgs e)
        {
            /* if (totalPage > 1 && e.KeyCode==Keys.Right) //超過一頁才有展開功能
             {
                 //expandable = true;                
                 MessageBox.Show("fuck");
             }*/
        }

        private void lbNotifys_KeyPress(object sender, KeyPressEventArgs e)
        {
            /*    if (totalPage > 1)              
                {
                    e.Handled = true;
                    MessageBox.Show("fuck");
                }*/
        }

        protected void textClick(object sender, System.EventArgs e)
        {
            MessageBox.Show("Text clicked");
        }

        protected void titleClick(object sender, System.EventArgs e)
        {
            MessageBox.Show("Title text clicked");
        }
    }
}


