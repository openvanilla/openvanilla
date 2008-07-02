using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;

namespace OVManagedUI
{
    public class IMENotifyForm : System.Windows.Forms.Form
    {
        private IContainer components;

        //DarkGray= {255,169,169,169};
        private Color m_topColor = Color.DarkGray;
        private Color m_middleColor =
                Color.FromArgb(255,
                ((int)(((byte)(100)))),
                ((int)(((byte)(100)))),
                ((int)(((byte)(100)))));
        //private Color m_bottomColor = Color.Black;
        private Color m_borderColor = Color.Gray;
        private Color m_textColor = Color.White;
        private bool notifyEnabled=true;

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
        private Label m_lbtext;
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
            this.Opacity = 0.9;
        }

        public IMENotifyForm(string[] Notifys)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            this.Opacity = 0.9;
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;
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
            this.m_lbtext = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // m_lbtext
            // 
            this.m_lbtext.AutoSize = true;
            this.m_lbtext.BackColor = System.Drawing.Color.Transparent;
            this.m_lbtext.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_lbtext.Font = new System.Drawing.Font("Microsoft JhengHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.m_lbtext.ForeColor = System.Drawing.SystemColors.Info;
            this.m_lbtext.Location = new System.Drawing.Point(5, 3);
            this.m_lbtext.Margin = new System.Windows.Forms.Padding(0);
            this.m_lbtext.Name = "m_lbtext";
            this.m_lbtext.Padding = new System.Windows.Forms.Padding(3);
            this.m_lbtext.Size = new System.Drawing.Size(60, 26);
            this.m_lbtext.TabIndex = 0;
            this.m_lbtext.Text = "label1";
            // 
            // IMENotifyForm
            // 
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ClientSize = new System.Drawing.Size(141, 34);
            this.ControlBox = false;
            this.Controls.Add(this.m_lbtext);
            this.Font = new System.Drawing.Font("Microsoft JhengHei", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IMENotifyForm";
            this.Opacity = 0.95;
            this.Padding = new System.Windows.Forms.Padding(5, 3, 0, 0);
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Deactivate += new System.EventHandler(this.IMENotifyForm_Deactivate);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.IMENotifyForm_Paint);
            this.Activated += new System.EventHandler(this.IMENotifyForm_Activated);
            this.Enter += new System.EventHandler(this.IMENotifyForm_Enter);
            this.Leave += new System.EventHandler(this.IMENotifyForm_Leave);
            this.Click += new System.EventHandler(this.IMENotifyForm_Click);
            this.Validating += new System.ComponentModel.CancelEventHandler(this.IMENotifyForm_Validating);
            this.Validated += new System.EventHandler(this.IMENotifyForm_Validated);
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        #region public methods

        public void SetNotifyStr(string inputs)
        {
            //if (this.Text.CompareTo("")==0)
            //    this.Text = inputs;
            //else
            this.m_lbtext.Text = this.m_lbtext.Text + inputs;
            this.Height = this.m_lbtext.Height;// +4;
            this.Width = this.m_lbtext.Width+6;// +4;
            //this.Padding = new System.Windows.Forms.Padding( 5,3,
                //this.m_lbtext.Width *1/10, this.m_lbtext.Height * 1 / 10,    0, 0);
            
            
                this.Refresh();
            /*
            NotifyWindow nw;
            nw = new NotifyWindow("OpenVanilla Notify", inputs);
            nw.TitleClicked += new System.EventHandler(titleClick);
            nw.TextClicked += new System.EventHandler(textClick);
            nw.SetDimensions(200, 120);
            nw.Notify();
             */
            
        }
        public void SetColor(int forecolor, int backcolor)
        {
            m_textColor = Color.FromArgb(forecolor);
            m_middleColor = Color.FromArgb(backcolor);
            m_topColor =Color.FromArgb(
                (int)m_middleColor.A, 
                ((int)m_middleColor.R)*169/100,
                ((int)m_middleColor.G)*169/100,
                ((int)m_middleColor.B)*169/100);
            m_borderColor = Color.FromArgb(
                (int)m_middleColor.A,
                ((int)m_middleColor.R) * 80 / 100,
                ((int)m_middleColor.G) * 80 / 100,
                ((int)m_middleColor.B) * 80 / 100);
        }
        public void SetFont(string ff, int fontsize)
        {
            m_lbtext.Font = new Font(ff, fontsize);
        }

        public void SetNotifyEnabled(bool b)
        {
            notifyEnabled = b;
        }

        public void SetLocation(int x, int y)
        {   // temp. fix for overlapping with candi window because of offsetx
            this.Location = new Point(x - (int)m_lbtext.Font.Size*2 , y);
        }

        public void SetOpacity(int o)
        {
            this.Opacity = o / 100f;
        }

        public void DisposeForm()
        {
            this.Dispose(true);
        }

        public void ClearNotify()
        {
            this.m_lbtext.Text = "";
            
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
            if (!this.Visible && notifyEnabled)
            {
                System.Diagnostics.Debug.WriteLine("Show Notify Page:"+this.Location.ToString());
                UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
                this.Refresh();
            }
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
        private void IMENotifyForm_Paint(object sender, PaintEventArgs pea)
        {

            SizeF stringSize = new SizeF();
            SizeF charSize = new SizeF();
            stringSize = pea.Graphics.MeasureString(Text, Font);
            charSize = pea.Graphics.MeasureString("A", Font);
     
     

            ButtonBorderStyle m_buttonBorderStyle = ButtonBorderStyle.Outset;

           
            SizeF sizeF =
                new SizeF(
                    (float)ClientRectangle.Width,
                    (float)ClientRectangle.Height / 5.0f);

            RectangleF rectF = new RectangleF(new PointF(0.0f, 0.0f), sizeF);
            Brush b = new LinearGradientBrush(
                rectF, m_topColor, m_middleColor,
                    LinearGradientMode.Vertical);
            pea.Graphics.FillRectangle(b, rectF);
            
            rectF = new RectangleF(
                new PointF(0.0f, sizeF.Height), new SizeF(sizeF.Width, sizeF.Height*4) );
            //b = new System.Drawing.SolidBrush(m_bottomColor);
            b = new System.Drawing.SolidBrush(m_middleColor);
            pea.Graphics.FillRectangle(b, rectF);

            //draw text
            b = new SolidBrush(m_textColor);
            //b = new LinearGradientBrush(
            //    rectF, m_textColor, m_middleColor,
            //        LinearGradientMode.ForwardDiagonal);
            PointF pt = new PointF(
                  (Width - stringSize.Width) *0.6f, (Height - stringSize.Height) * 0.8f);
            //pea.Graphics.DrawString(Text, Font, b, pt);

            b.Dispose();
            ControlPaint.DrawBorder(
                pea.Graphics, ClientRectangle,
                    m_borderColor, m_buttonBorderStyle);
                        
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


