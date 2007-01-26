using System;
//using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
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
        
        private UInt64 m_AppHWnd;
        private Color m_origForeColor;
        private Color m_origBackColor;
        private bool m_wasMouseDown = false;

        //======== Gradient Color =================//
        private int colorTransparentcy = 128;
        private Color _color0;
        private Color _color1;
        
        public Color Color0
        {
            get { return _color0; }
            set { _color0 = value; Invalidate(); }//invokeColorChange(); }
        }

        public Color Color1 
		{
			get { return _color1; }
			set { _color1 = value; Invalidate ();}// invokeColorChange (); }
		}

        public IMEButton(): base()
        {
            Color0 = Color.White;
            Color1 = Color.Black;
            //this.ForeColor = Color.White;
            //this.Margin = new Padding(3, 3, 3, 3);
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

        protected override void OnPaint(PaintEventArgs pe)
        {
            // Calling the base class OnPaint
            base.OnPaint(pe);
            //base.OnPaintBackground;

            // create the first transparent colours
            //Color c0 = Color.FromArgb(colorTransparentcy, Color0);
            //Color c1 = Color.FromArgb(colorTransparentcy, Color1);
            Color c0 = Color.LightGray;
            Color c1 = Color.Black;
            SizeF sizeF =
                new SizeF(this.Size.Width - 3.0f, this.Size.Height / 2 - 2.0f);
            //PointF pointF;
            // getting rectangle to fill
            //for (int i = 0; i < 3; i++)
            {
                //float percentage = ClientRectangle.Width * 0.5f;
                RectangleF pRect = new RectangleF(new PointF(2.0f, 2.0f), sizeF);
                Brush b = new LinearGradientBrush
                    (pRect, c0, c1, System.Drawing.Drawing2D.LinearGradientMode.Vertical); //90度 (垂直)

                pe.Graphics.FillRectangle(b, pRect);
                

                pRect = new RectangleF(new PointF(2.0f, sizeF.Height + 2.0f), sizeF);
                b = new LinearGradientBrush
                    (pRect, c1, c1, System.Drawing.Drawing2D.LinearGradientMode.Vertical); //90度 (垂直)
                pe.Graphics.FillRectangle(b, pRect);

                Brush foreBrush = new SolidBrush(Color.White);
                SizeF size2 = pe.Graphics.MeasureString(Text, Font);
                PointF pt = new PointF((Width - size2.Width) / 2, (Height - size2.Height) / 2);
                
                pe.Graphics.DrawString(Text, Font, foreBrush, pt);
            }
        }

        protected void MyOnMouseDown()
        {
            m_origForeColor = this.ForeColor;
            m_origBackColor = this.BackColor;
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.BackColor = System.Drawing.Color.Transparent;
            m_wasMouseDown = true;
        }

        protected override void OnMouseUp(MouseEventArgs mevent)
        {
            base.OnMouseUp(mevent);
            if (m_wasMouseDown)
            {
                this.BackColor = m_origBackColor;
                this.ForeColor = m_origForeColor;
                m_wasMouseDown = false;
            }
        }
/*
        private void invokeColorChange()
        {
            if (ColorChanged != null)
            {
                EventArgs e = new EventArgs();
                ColorChanged(this, e);
            }
        }*/

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
