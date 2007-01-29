using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
    public partial class IMECompRichForm : Form
    {
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private int caretPosX = 0;
        private int caretPosIndex = 0;
        private int compSelStart = 0;
        private int compSelEnd = 0;
        
        public IMECompRichForm()
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
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                return;
            }
            base.WndProc(ref m);
        }

        public void ShowNoActive()
		{
            if(!this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}

		public void HideNoActive()
		{
            if (this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}

		public void ClearComp()
		{
			this.richTextBox1.Clear();
            compSelStart = 0;
            compSelEnd = 0;
		}

        public void SetLocation(int x, int y)
        {
            this.Location = new Point(x, y);
        }

		public void SetComp(string inputs)
		{
			if(inputs==null) return;
            this.richTextBox1.Text = inputs;
        }

        public int GetCaretPosX()
        {
            return caretPosX;
        }

		public void SetCaretX(int x)
		{
            /*StringBuilder sb =
                new StringBuilder(this.richTextBox1.Text);
            sb.Insert(x, "︳");
            this.richTextBox1.Text = sb.ToString();*/

            caretPosIndex = x;
            Point pt =             
                this.richTextBox1.GetPositionFromCharIndex(caretPosIndex);
            caretPosX = pt.X + 6;
            this.label1.Left = caretPosX;
            
            Point pt2 = 
                this.richTextBox1.GetPositionFromCharIndex(
                    this.richTextBox1.Text.Length);
            if (this.Width - this.richTextBox1.Bounds.Left < pt2.X)
                this.Width = pt2.X + 100;
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
    }
}