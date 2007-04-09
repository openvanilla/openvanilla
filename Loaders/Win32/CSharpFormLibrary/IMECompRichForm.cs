using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace CSharpFormLibrary
{
    public partial class IMECompRichForm : Form
    {        
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;        
        private const int WM_NCACTIVATE = 0x0086;
        private int formInitWidth=277;
        private int compSelStart = 0;
        private int compSelEnd = 0;
        private int m_caretX = 0;
        private string m_text="";
        private int caretIndex = 0;
        private IntPtr m_AppHWnd;

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
            if (Buf.Length > 0)
            {
                Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
                Size proposedSize = new Size(int.MaxValue, int.MaxValue);
                TextFormatFlags flags = TextFormatFlags.NoPadding;
                Size size = TextRenderer.MeasureText(e.Graphics, Buf, Font, proposedSize, flags);
                Size sizeText = TextRenderer.MeasureText(e.Graphics, Buf.Substring(0, caretIndex), Font, proposedSize, flags);

                int offsetX = 2;
                int offsetY = 3;
                //SizeF sizeF = e.Graphics.MeasureString(Buf.Substring(0, caretIndex), Font);

                //resize
                if (this.Width - 100 < (int)size.Width)
                {
                    /*int ret = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                   0xE, 10); // 暫時改成擠出字*/                    
                    this.Width += 100;                    
                }

                //draw backcolor
                //Brush b = new System.Drawing.SolidBrush(Color.GhostWhite);
                Brush b = new System.Drawing.SolidBrush(Color.LemonChiffon);
                e.Graphics.FillRectangle(b, this.ClientRectangle);
                b.Dispose();

                //draw string
                TextRenderer.DrawText(e.Graphics, Buf, Font, new Point(offsetX, offsetY), System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(66)))), ((int)(((byte)(0))))), flags);                
                if (compSelEnd - compSelStart > 0)
                {                
                    if (compSelStart == 0)
                        TextRenderer.DrawText(e.Graphics, Buf.Substring(compSelStart, compSelEnd - compSelStart), Font, new Point(offsetX, offsetY), Color.Red, flags);
                    else
                    {
                        Size sizeSelectionText = TextRenderer.MeasureText(e.Graphics, Buf.Substring(0, compSelStart), Font, proposedSize, flags);
                        TextRenderer.DrawText(e.Graphics, Buf.Substring(compSelStart, compSelEnd - compSelStart), Font, new Point(sizeSelectionText.Width + offsetX, offsetY), Color.Red, flags);
                    }
                }

                //save caret  for c++ using  and draw it                    
                CaretX = (int)sizeText.Width;
                Pen p = new Pen(Color.Black, 1);
                e.Graphics.DrawLine(p, sizeText.Width + offsetX, offsetY, sizeText.Width + offsetX, sizeText.Height + offsetY);
                p.Dispose();
            }
            else
            {
                //Brush b = new SolidBrush(SystemColors.ButtonFace);
                Brush b = new System.Drawing.SolidBrush(Color.Khaki);
                e.Graphics.FillRectangle(b, this.Bounds);
                b.Dispose();
            }
            Debug.WriteLine("after paint words" + this.Height.ToString());
            //draw border
            //ControlPaint.DrawBorder(
            //    e.Graphics, ClientRectangle,
            //        Color.LightGray, ButtonBorderStyle.Outset);

            ControlPaint.DrawBorder(
                e.Graphics, ClientRectangle,
                    Color.Moccasin, ButtonBorderStyle.Outset);

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
            m_AppHWnd = HWND;
            Debug.WriteLine("m_AppHWnd (comp) =" + HWND);
            Debug.WriteLine("after set hwnd " + this.Handle.ToString() + " " + this.Height.ToString());
        }

        public void ShowNoActive()
		{            
            //this.richTextBox1.Text = Buf;            
            this.Height = 30;//不知道為什麼之前會被亂改 只好這邊再改一次
            Debug.WriteLine("before refresh " + this.Height.ToString());
            this.Refresh();
            Debug.WriteLine("after refresh " + this.Height.ToString());
            //this.timer1.Enabled = true;
            if(!this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
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
            this.Refresh();
            //compSelStart = 0;
            //compSelEnd = 0;
            this.Width = formInitWidth;
            Debug.WriteLine("after clear " + this.Height.ToString());
		}

        public void SetLocation(int x, int y)
        {
            Debug.WriteLine("before location " + this.Height.ToString());
            this.Location = new Point(x, y);
            Debug.WriteLine("after location " + this.Height.ToString());
        }

		public void SetComp(string inputs)
		{

			if(inputs==null) return;
            //先設定至 Buf -> 給 onPaint 算 Caret
            Debug.WriteLine(this.Height.ToString());
            Buf = inputs;
            Debug.WriteLine(this.Height.ToString());
            //CaretX = caretIndex;
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

            this.caretIndex = x;
            

            /*Point pt2 = 
                this.richTextBox1.GetPositionFromCharIndex(
                    this.richTextBox1.Text.Length);
            if (this.Width - this.richTextBox1.Bounds.Left < pt2.X)
                this.Width = pt2.X + 100;*/
            /*
            if (compSelStart > 0 && compSelStart == compSelEnd) //已經組字
                this.richTextBox1.Select(compSelStart - 1, 1);
            else //正在組字
                this.richTextBox1.Select(compSelStart, compSelEnd - compSelStart);
            this.richTextBox1.SelectionColor = Color.Red;*/
        }

        public void SetCompMarkFrom(int x)
        {
            compSelStart = x;
        }
        
        public void SetCompMarkTo(int x)
        {
            compSelEnd = x;
        }

        public void DisposeForm()
        {
            this.Dispose(true);
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