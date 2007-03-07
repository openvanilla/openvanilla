using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;


namespace CSharpFormLibrary
{
    class IMETabControl:System.Windows.Forms.TabControl
    {
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                return cp;
            }
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);        
            
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {                
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;                                
                SetIndex(MousePosition);                                    
            }                        
        }        

        private void SetIndex(Point pt)
        {
            
            for (int i = 0; i < TabPages.Count; i++)
            {
                if (GetTabRect(i).Contains(this.PointToClient(pt)))
                {
                    //先把 Visible 關掉再打開，可以避免搶focus
                    this.Visible = false;
                    this.SelectedIndex = i;
                    this.Visible = true;
                    break;
                }                
            }            
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // IMETabControl
            // 
            this.TabIndexChanged += new System.EventHandler(this.IMETabControl_TabIndexChanged);
            this.ResumeLayout(false);

        }

        private void IMETabControl_TabIndexChanged(object sender, EventArgs e)
        {
            MessageBox.Show("event:IMETabControl_TabIndexChanged");
        }
    }
}
