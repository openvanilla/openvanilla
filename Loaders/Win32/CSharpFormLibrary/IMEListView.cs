using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace CSharpFormLibrary
{            
    public class IMEListView : System.Windows.Forms.ListView
    {
        private const int MA_NOACTIVATEANDEAT = 0x0004;
        public IMEListView()
        {
          /*  this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();*/
        }
        /*
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                return cp;
            }
        }*/

        protected void MyOnMouseDown()
        {
            MessageBox.Show("fuck!");
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);            
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;                
                MyOnMouseDown();                
            }
        }
    }
}
