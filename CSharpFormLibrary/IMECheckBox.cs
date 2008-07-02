using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace OVManagedUI
{
    public class IMECheckBox : CheckBox
    {
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        public IMECheckBox()
        {
            //this.ForeColor = Color.Black;
            //this.BackColor = Color.LawnGreen;
            this.Visible = false;
            this.CheckedChanged += new EventHandler(IMECheckBox_CheckedChanged);
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;                
            }
        }

        private void IMECheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (this.Checked)
            {
                //this.BackgroundImage=BackgroundImage.
                //this.ForeColor = Color.White;
                //                this.BackColor = Color.Black;                

            }
            else
            {
                //     this.ForeColor = Color.Black;
                //   this.BackColor = Color.LawnGreen;
            }
        }

    }
}
