using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace CSharpFormLibrary
{
    public partial class IMECompRichForm : Form
    {        
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;        
        private const int WM_NCACTIVATE = 0x0086;
        private int m_formInitWidth=277;
        private int m_compSelStart = 0;
        private int m_compSelEnd = 0;
        private int m_caretX = 0;
        private int m_caretY = 0;
        private string m_text="";
        private int m_caretIndex = 0;
        private IntPtr m_appHWnd;
        private bool m_doClear = false;

        public int CaretX
        {
            get { return m_caretX; }
            set { m_caretX = value; }
        }

        public string Buf
        {
            get { return m_text; }
            set { m_text = value; }
        }
        
        public IMECompRichForm()
        {
            InitializeComponent();
            Debug.WriteLine("after init" + this.Height.ToString());
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
             
            Application.EnableVisualStyles();
            Debug.WriteLine("after setstyle" + this.Handle.ToString() + " " + this.Height.ToString());            
        }
      
        private void IMECompRichForm_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            Debug.WriteLine("before paint" + this.Height.ToString());
            e.Graphics.Clear(Color.Transparent);
            if (Buf.Length > 0)
            {
                Font =
                    new System.Drawing.Font(
                        "PMingLiU", 12F, System.Drawing.FontStyle.Regular,
                        System.Drawing.GraphicsUnit.Point);
                Size proposedSize = new Size(int.MaxValue, int.MaxValue);
                TextFormatFlags flags = TextFormatFlags.NoPadding;
                Size size =
                    TextRenderer.MeasureText(
                        e.Graphics, Buf, Font, proposedSize, flags);
                Size sizeText =
                    TextRenderer.MeasureText(
                        e.Graphics, Buf.Substring(0, m_caretIndex), Font,
                        proposedSize, flags);

                int offsetX = 2;
                int offsetY = 3;
                //SizeF sizeF = e.Graphics.MeasureString(Buf.Substring(0, m_caretIndex), Font);

                //resize
                if (this.Width - 100 < (int)size.Width)
                {
                    /*int ret = UtilFuncs.SendMessage(m_appHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                   0xE, 10); // 暫時改成擠出字*/                    
                    this.Width += 100;                    
                }

                //draw backcolor
                //Brush b = new System.Drawing.SolidBrush(Color.Transparent);
                //e.Graphics.FillRectangle(b, this.ClientRectangle);                
                //b.Dispose();

                //draw string
                TextRenderer.DrawText(
                    e.Graphics, Buf, Font, new Point(offsetX, offsetY),
                    System.Drawing.Color.Red, flags);
                if (m_compSelEnd - m_compSelStart > 0)
                {                
                    if (m_compSelStart == 0)
                        TextRenderer.DrawText(
                            e.Graphics,
                            Buf.Substring(
                                m_compSelStart, m_compSelEnd - m_compSelStart),
                            Font, new Point(offsetX, offsetY),
                            Color.Red, flags);
                    else
                    {
                        Size sizeSelectionText =
                            TextRenderer.MeasureText(
                                e.Graphics, Buf.Substring(0, m_compSelStart),
                                Font, proposedSize, flags);
                        TextRenderer.DrawText(
                            e.Graphics,
                            Buf.Substring(
                                m_compSelStart, m_compSelEnd - m_compSelStart),
                            Font,
                            new Point(
                                sizeSelectionText.Width + offsetX, offsetY),
                            Color.Red, flags);
                    }
                }

                //save caret for c++ using and draw it                    
                CaretX = sizeText.Width;
                Pen pCaret = new Pen(Color.Green, 1);
                e.Graphics.DrawLine(
                    pCaret, sizeText.Width + offsetX, offsetY,
                    sizeText.Width + offsetX, sizeText.Height + offsetY);
                pCaret.Dispose();
            }
            //else
            //{
            //    Brush b = new System.Drawing.SolidBrush(Color.Transparent);
            //    e.Graphics.FillRectangle(b, this.Bounds);
            //    b.Dispose();
            //}
            Debug.WriteLine("after paint words" + this.Height.ToString());
            //draw border
            //ControlPaint.DrawBorder(
            //    e.Graphics, ClientRectangle,
            //        Color.LightGray, ButtonBorderStyle.Dashed);
            Debug.WriteLine("after paint border" + this.Height.ToString());
        }

        /*private void MeasureStringMin(PaintEventArgs e)
        {
            // Measure string.
            SizeF stringSize = new SizeF();
            stringSize = e.Graphics.MeasureString(Buf.Substring(0, CaretX), this.richTextBox1.Font);

            // Draw rectangle representing size of string.
            e.Graphics.DrawRectangle(new Pen(Color.Red, 1), 0.0F, 0.0F, stringSize.Width, stringSize.Height);

            // Draw string to screen.
            e.Graphics.DrawString(Buf.Substring(0, CaretX), this.richTextBox1.Font, Brushes.Black, new PointF(0, 0));
            System.Diagnostics.Debug.WriteLine("Buf=" + Buf);
            System.Diagnostics.Debug.WriteLine("Buf sub=" + Buf.Substring(0, CaretX));
            System.Diagnostics.Debug.WriteLine("size=" + stringSize.Width.ToString());
        }*/

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x00000020; //WS_EX_TRANSPARENT                 
                return cp;
            }
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            //do nothing 
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_MOUSEACTIVATE)
            {                
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                return;
            }
            /*if (m.Msg == WM_NCACTIVATE)
            {
                if (m.WParam == (IntPtr)1)
                {
                    m.WParam = (IntPtr)0;
                    System.Diagnostics.Debug.WriteLine("WM_NCACTIVATE w=1");
                    //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                    //return;
                    base.WndProc(ref m);
                }
                else
                {
                    m.WParam = (IntPtr)1;
                    base.WndProc(ref m);
                }
            }
            else */
             base.WndProc(ref m);
        }

        public void SetAppHWnd(IntPtr HWND)
        {
            Debug.WriteLine("before set hwnd " + this.Handle.ToString() + " " + this.Height.ToString());
            m_appHWnd = HWND;
            Debug.WriteLine("m_appHWnd (comp) =" + HWND);
            Debug.WriteLine("after set hwnd " + this.Handle.ToString() + " " + this.Height.ToString());
        }

        public void ShowNoActive()
		{
            if (!this.Visible)
                UtilFuncs.SetVisibleNoActivate(this, true); // true to show.
            //this.richTextBox1.Text = Buf;            
            this.Height = 30;//不知道為什麼之前會被亂改 只好這邊再改一次
            Debug.WriteLine("before refresh " + this.Height.ToString());
            this.Refresh();
            Debug.WriteLine("after refresh " + this.Height.ToString());
            //this.timer1.Enabled = true;
		}

		public void HideNoActive()
		{
            //this.timer1.Enabled = false;
            if (this.Visible)
                UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.
		}

		public void ClearComp()
        {
            Debug.WriteLine("before clear " + this.Height.ToString());
            //this.richTextBox1.Text="";
            Buf = "";
            //m_compSelStart = 0;
            //m_compSelEnd = 0;
            this.Width = m_formInitWidth;
            this.Refresh();
            Debug.WriteLine("after clear " + this.Height.ToString());
		}

        public void SetLocation(int x, int y)
        {
            Debug.WriteLine("before location " + this.Height.ToString());
            this.Location = new Point(x, y);
            Debug.WriteLine("after location " + this.Height.ToString());
            m_caretY = y;
        }

		public void SetComp(string inputs)
		{
			if(inputs==null) return;
            //先設定至 Buf -> 給 onPaint 算 Caret
            Debug.WriteLine(this.Height.ToString());
            Buf = inputs;
            Debug.WriteLine(this.Height.ToString());
            //CaretX = m_caretIndex;
            //this.richTextBox1.Text = inputs;
        }

        public int GetCaretPosX()
        {
            return CaretX;
        }

		public void SetCaretX(int x)
		{
            /*StringBuilder sb =
                new StringBuilder(this.richTextBox1.Text);
            sb.Insert(x, "︳");
            this.richTextBox1.Text = sb.ToString();*/
            
            /*Point pt =             
                this.richTextBox1.GetPositionFromCharIndex(x);
            this.label1.Left  = pt.X + 6;            
            System.Diagnostics.Debug.WriteLine("X=" + x.ToString());
            System.Diagnostics.Debug.WriteLine("labelX=" + label1.Left.ToString());*/

            this.m_caretIndex = x;            

            /*Point pt2 = 
                this.richTextBox1.GetPositionFromCharIndex(
                    this.richTextBox1.Text.Length);
            if (this.Width - this.richTextBox1.Bounds.Left < pt2.X)
                this.Width = pt2.X + 100;*/
            /*
            if (m_compSelStart > 0 && m_compSelStart == m_compSelEnd) //已經組字
                this.richTextBox1.Select(m_compSelStart - 1, 1);
            else //正在組字
                this.richTextBox1.Select(m_compSelStart, m_compSelEnd - m_compSelStart);
            this.richTextBox1.SelectionColor = Color.Red;*/
        }

        public void SetCompMarkFrom(int x)
        {
            m_compSelStart = x;
        }
        
        public void SetCompMarkTo(int x)
        {
            m_compSelEnd = x;
        }

        public void DisposeForm()
        {
            this.Dispose(true);
        }

        public void MyCallbackFunction()
        {
            string target_google = "http://www.google.com/search?q=";
            //string target_asqa = "http://asqa.iis.sinica.edu.tw/clqa2006/default.aspx?q=";
            string encodeBuf = System.Web.HttpUtility.UrlEncode(Buf);
            try
            {
                //if (Buf.Length >= 5)
                 //   System.Diagnostics.Process.Start(target_asqa + encodeBuf);
                //else
                    System.Diagnostics.Process.Start(target_google + encodeBuf);                
            }
            catch
            (
                System.ComponentModel.Win32Exception noBrowser)
            {
                if (noBrowser.ErrorCode == -2147467259)
                    MessageBox.Show(noBrowser.Message);
            }
            catch (System.Exception other)
            {
                MessageBox.Show(other.Message);
            }
        }

        private void RunProcess()
        {
            Thread MyThread = new Thread(new ThreadStart(MyCallbackFunction));
            MyThread.Start();
          
        }
        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            //MessageBox.Show("google!!");
            RunProcess();
        }                       

        /*private void timer2_Tick(object sender, EventArgs e)
        {            
            if (label1.Visible == true)
            {
                label1.Visible = false;
                System.Diagnostics.Debug.WriteLine("label1.Visible=false");
            }
            else
            {
                label1.Visible = true;
                System.Diagnostics.Debug.WriteLine("label1.Visible=true");
            }
        }*/
    }
}