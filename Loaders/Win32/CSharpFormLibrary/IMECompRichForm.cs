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

        //private int caretWidth = 0;
        //just give the caret a small point;
        private int caretWidth = 2;
        private int CurrentAll = 0; //現在input字數的大小
        private int currentAllCompStrLength = 0;
        private Label label2;//composition window那塊圖的大小
        private int singleCaretHightlight = 0;
        private int caretPosX = 0;
        private int caretPosIndex = 0;
        private int compSelStart = 0;
        private int compSelEnd = 0;
        
        public IMECompRichForm()
        {
            InitializeComponent();
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
            Point pt = this.richTextBox1.GetPositionFromCharIndex(caretPosIndex);
            caretPosX = pt.X + 6; 

		}
		public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}
		public void SetComp(string inputs)
		{
			if(inputs==null) return;            
            this.richTextBox1.Text = inputs;            
            Point pt = this.richTextBox1.GetPositionFromCharIndex(this.richTextBox1.Text.Length);
            this.Width = pt.X+25;                        
        }

        public int GetHeight()
        {
            return this.Height;
        }
        public int GetCaretPosX()
        {
            /*try
            {
                System.Diagnostics.Debug.WriteLine("c# comp GetCaretPosX = " + caretPosX);
            }
            catch (Exception e)
            {
                System.Diagnostics.Debug.WriteLine(e.StackTrace);
            }*/
            return caretPosX;
        }

		public void SetCaretX(int x)
		{
            caretPosIndex = x;
            Point pt = this.richTextBox1.GetPositionFromCharIndex(caretPosIndex);
            caretPosX = pt.X + 6;  
            System.Diagnostics.Debug.WriteLine("========SetCaretX=========");
            System.Diagnostics.Debug.WriteLine("cursor:" +x);
            System.Diagnostics.Debug.WriteLine("caretPosX=" + caretPosX);
            System.Diagnostics.Debug.WriteLine("compSelStart=" + compSelStart);
            System.Diagnostics.Debug.WriteLine("compSelEnd=" + compSelEnd);
            System.Diagnostics.Debug.WriteLine("labelX=" +label1.Left.ToString());               
             
            if (compSelStart == compSelEnd) //已經組字
            {                              
                if (compSelStart > 0)
                {
                    this.richTextBox1.Select(compSelStart - 1, 1);
                }
                this.label1.Left = caretPosX;
                //this.Width = this.richTextBox1.GetPositionFromCharIndex(this.richTextBox1.Text.Length).X + 30;
            }
            else //正在組字
            {                
                this.richTextBox1.Select(compSelStart, compSelEnd - compSelStart);                
                this.richTextBox1.SelectionColor = Color.Red;                
            }            
        }

        public void CompMarkFrom(int x)
        {
            compSelStart = x;
        }
        
        public void CompMarkTo(int x)
        {
            compSelEnd = x;
        }


    }
}