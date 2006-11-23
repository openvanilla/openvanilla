using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
    public class IMEButton : System.Windows.Forms.Button
    {
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private static int msgCounter = 0;
        private UInt64 m_AppHWnd;
        private Color m_origForeColor;
        private bool m_wasMouseDown = false;

        public IMEButton()
            : base()
        {
            this.SetStyle(ControlStyles.StandardClick, false);
            this.SetStyle(ControlStyles.EnableNotifyMessage, false);
            this.SetStyle(ControlStyles.UserMouse, true);
        }

        public UInt64 AppHWnd
        {
            get { return m_AppHWnd; }
            set { m_AppHWnd = value; }
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                //System.Diagnostics.Debug.WriteLine(
                //    "Button: CreateParams.ExStyle (before) =" + cp.ExStyle);
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                //System.Diagnostics.Debug.WriteLine(
                //    "Button: CreateParams.ExStyle (after) =" + cp.ExStyle);
                return cp;
            }
        }

        protected void MyOnMouseDown()
        {
            m_origForeColor = this.ForeColor;
            this.ForeColor = this.BackColor;
            this.BackColor = System.Drawing.Color.Transparent;
            m_wasMouseDown = true;
        }

        protected override void OnMouseUp(MouseEventArgs mevent)
        {
            base.OnMouseUp(mevent);
            if (m_wasMouseDown)
            {
                this.BackColor = this.ForeColor;
                this.ForeColor = m_origForeColor;
                m_wasMouseDown = false;
            }
        }

        protected override void WndProc(ref Message m)
        {
            /*
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Button[" + msgCounter + "]: (before base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
             */

            /*
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_IME_SETCONTEXT)
            {
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: Result->\t0x" + m.Result.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam->\t0x" + m.WParam.ToString("X"));
            }
             */

            base.WndProc(ref m);

            /*
            System.Diagnostics.Debug.WriteLine(
                "Button[" + msgCounter + "]: (after base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
             */

            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                /*
                System.Diagnostics.Debug.WriteLine("==================");
                System.Diagnostics.Debug.WriteLine("WM_MOUSEACTIVATE");
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: this.Handle->\t0x" + this.Handle.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: Parent.Handle->\t0x" + this.Parent.Handle.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: App.Handle->\t0x" + m_AppHWnd.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: m.HWnd->\t0x" + m.HWnd.ToString("X"));
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = this.Handle;
                //m.WParam = new IntPtr((long)m_AppHWnd);
                //m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));
                 */

                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                //m.Result = (IntPtr)MA_NOACTIVATE;

                //System.Diagnostics.Debug.WriteLine(
                //    "Button[" + msgCounter + "]: Result->\t0x" + m.Result.ToString("X"));

                MyOnMouseDown();
            }
            /*
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_SETCURSOR)
            {
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: (before) Result->\t0x" + m.Result.ToString("X"));

                m.Result = new IntPtr(1);

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: (after) Result->\t0x" + m.Result.ToString("X"));

            }
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_IME_NOTIFY)
            {
                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: Result->\t0x" + m.Result.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Button[" + msgCounter + "]: WParam->\t0x" + m.WParam.ToString("X"));
            }
            //else
            //    base.WndProc(ref m);
             */
        }
    }
}
