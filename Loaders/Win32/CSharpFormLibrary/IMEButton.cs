using System;
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
        
        private bool m_wasMouseDown = false;
        private bool m_wasButtonPressed = false;

        private static Color m_btTopColor = Color.DarkGray;
        private static Color m_btMiddleColor =
            Color.FromArgb(
                ((int)(((byte)(55)))),
                ((int)(((byte)(55)))),
                ((int)(((byte)(55)))));
        private static Color m_btBottomColor = Color.Black;
        private static Color m_btBorderColor = Color.Gray;
        private static Color m_btTextColor = Color.White;

        private Color m_topColor = m_btTopColor;
        private Color m_middleColor = m_btMiddleColor;
        private Color m_bottomColor = m_btBottomColor;
        private Color m_borderColor = m_btBorderColor;
        private Color m_textColor = m_btTextColor;

        private ButtonBorderStyle m_buttonBorderStyle = ButtonBorderStyle.Outset;

        public IMEButton()
        {
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                return cp;
            }
        }

        private void UpdateAppearance()
        {
            if (m_wasMouseDown)
            {                
                m_topColor = m_btMiddleColor;
                m_buttonBorderStyle = ButtonBorderStyle.Inset;
                m_wasButtonPressed = true;
                this.Refresh();
            }
            else
            {
                if(m_wasButtonPressed) {                    
                    m_topColor = m_btTopColor;
                    m_buttonBorderStyle = ButtonBorderStyle.Outset;
                    this.Refresh();
                }
                m_wasButtonPressed = false;
            }
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);

            //draw 2-color gradient button
            SizeF sizeF =
                new SizeF(
                    (float)ClientRectangle.Width,
                    (float)ClientRectangle.Height / 2.0f);

            RectangleF rectF = new RectangleF(new PointF(0.0f, 0.0f), sizeF);
            Brush b = new LinearGradientBrush(
                rectF, m_topColor, m_middleColor,
                    LinearGradientMode.Vertical);
            pe.Graphics.FillRectangle(b, rectF);

            rectF = new RectangleF(
                new PointF(0.0f, sizeF.Height), sizeF);
            b = new System.Drawing.SolidBrush(m_bottomColor);
            pe.Graphics.FillRectangle(b, rectF);

            //draw text
            b = new SolidBrush(m_textColor);
            sizeF = pe.Graphics.MeasureString(Text, Font);
            PointF pt = new PointF(
                (Width - sizeF.Width) / 2.0f, (Height - sizeF.Height) / 2.0f);
            pe.Graphics.DrawString(Text, Font, b, pt);

            b.Dispose();
            ControlPaint.DrawBorder(
                pe.Graphics, ClientRectangle,
                    m_borderColor, m_buttonBorderStyle);
        }

        protected void MyOnMouseDown()
        {
            m_wasMouseDown = true;            
            UpdateAppearance();
            System.Diagnostics.Debug.WriteLine("mousedown");
        }

        protected override void OnMouseUp(MouseEventArgs mevent)
        {
            base.OnMouseUp(mevent);
            m_wasMouseDown = false;            
            UpdateAppearance();
            System.Diagnostics.Debug.WriteLine("mouseup");
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
