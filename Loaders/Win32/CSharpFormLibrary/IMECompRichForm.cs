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
        private int formInitWidth=199;
        private int compSelStart = 0;
        private int compSelEnd = 0;
        private int m_caretX = 0;
        private string m_text="";
        private int caretIndex = 0;

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
            
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
             
            Application.EnableVisualStyles();
        }
      

        private void IMECompRichForm_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
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
                if (this.Width - 50 < (int)size.Width)
                    this.Width += 100;

                //draw backcolor- turn off
                /*b = new System.Drawing.SolidBrush(SystemColors.ButtonFace);
                e.Graphics.FillRectangle(b, this.Bounds);
                b.Dispose();*/

                //draw string
                //Brush b;
                //b = new SolidBrush(Color.Black);
                TextRenderer.DrawText(e.Graphics, Buf, Font, new Point(offsetX, offsetY), Color.Black, flags);
                //TextRenderer.DrawText(e.Graphics, Buf, Font, new Point(offsetX, offsetY), Color.Yellow, flags);

                if (compSelEnd - compSelStart > 0)
                {
                    //Debug.WriteLine("(" + compSelStart + "," + compSelEnd + ")");
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
                Brush b = new SolidBrush(SystemColors.ButtonFace);
                e.Graphics.FillRectangle(b, this.Bounds);
                b.Dispose();
            }

            //draw border
            ControlPaint.DrawBorder(
                e.Graphics, ClientRectangle,
                    Color.LightGray, ButtonBorderStyle.Inset);
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

        public void ShowNoActive()
		{            
            //this.richTextBox1.Text = Buf;
            this.Refresh();
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
            //this.richTextBox1.Text="";
            Buf = "";
            this.Refresh();
            //compSelStart = 0;
            //compSelEnd = 0;
            this.Width = formInitWidth;
		}

        public void SetLocation(int x, int y)
        {
            this.Location = new Point(x, y);
        }

		public void SetComp(string inputs)
		{
			if(inputs==null) return;
            //先設定至 Buf -> 給 onPaint 算 Caret
            Buf = inputs;
            
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