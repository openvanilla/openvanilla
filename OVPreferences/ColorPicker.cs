using System;
using System.ComponentModel;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace OVPreferences
{
    public interface ColorCollection : IEnumerable
    {
        
        int Count { get; }
        
        Color this[int i] { get; }
        Color this[string s] { get; }
        
        IEnumerator  GetEnumerator();
        
        int IndexOf(string ColorName);
    }

    public partial class ColorPicker : ComboBox
    {
        #region Constantes
        private const int RECTCOLOR_LEFT = 4;
        private const int RECTCOLOR_TOP = 2;
        private const int RECTCOLOR_WIDTH = 40;
        private const int RECTTEXT_MARGIN = 10;
        private const int RECTTEXT_LEFT = RECTCOLOR_LEFT + RECTCOLOR_WIDTH + RECTTEXT_MARGIN;
        #endregion Constantes

        #region Variables
        /// <summary>Contient la collection de couleurs ?afficher</summary>
        private ColorCollection m_ColorCollection;
        #endregion Variables
        
        
        public ColorPicker()
        {
            this.DrawMode = DrawMode.OwnerDrawFixed;
            this.DropDownStyle = ComboBoxStyle.DropDownList;
            InitializeComponent();
        }

        //public ColorPicker(IContainer container)
        //{
        //    container.Add(this);

        //    InitializeComponent();
        //}

        public new ColorCollection Items
        {
            get { return m_ColorCollection; }
            set
            {
                
                if (m_ColorCollection != value && value != null)
                {
                    m_ColorCollection = value;
                    foreach (Color color in value) base.Items.Add(color.Name);
                    Refresh();
                }
                // add this for show the VS.Net 2005 color picker
                base.Items.Add("More Colors");
                
            }
        }
        
        public new string SelectedText
        {
            get { return Items[SelectedIndex].Name; }
            set
            {
                int selidx = Items.IndexOf(value);
                if (selidx > 0) SelectedIndex = selidx;
            }
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            Graphics Grphcs = e.Graphics;
            Color BlockColor = Color.Empty;
            int left = RECTCOLOR_LEFT;
            
            if (e.State == DrawItemState.Selected || e.State == DrawItemState.None) e.DrawBackground();
            
            if (e.Index == -1) BlockColor = SelectedIndex < 0 ? BackColor : Color.FromName(SelectedText);
            else BlockColor = Color.FromName((string)base.Items[e.Index]);
            
            Grphcs.FillRectangle(new SolidBrush(BlockColor), left, e.Bounds.Top + RECTCOLOR_TOP, RECTCOLOR_WIDTH, ItemHeight - 2 * RECTCOLOR_TOP);
            
            Grphcs.DrawRectangle(Pens.Black, left, e.Bounds.Top + RECTCOLOR_TOP, RECTCOLOR_WIDTH, ItemHeight - 2 * RECTCOLOR_TOP);
            
            Grphcs.DrawString(BlockColor.Name, e.Font, new SolidBrush(ForeColor), new Rectangle(RECTTEXT_LEFT, e.Bounds.Top, e.Bounds.Width - RECTTEXT_LEFT, ItemHeight));
            
            base.OnDrawItem(e);
        }
        protected override void OnDropDownStyleChanged(EventArgs e)
        {
            if (this.DropDownStyle != ComboBoxStyle.DropDownList) this.DropDownStyle = ComboBoxStyle.DropDownList;
        }
    }


    public class ColorButton : System.Windows.Forms.Button
    {
        public Color m_Color = Color.GhostWhite;//SystemColors.Info;

        public ColorButton()
        {
            this.SetStyle(ControlStyles.Selectable,true);
            Application.EnableVisualStyles();
        }
        public void setColor(Color color)
        {
            m_Color = color;
            Refresh();
        }
        protected override void OnClick(EventArgs e)
        {
            
            System.Windows.Forms.ColorDialog m_cdColors = new System.Windows.Forms.ColorDialog();
            m_cdColors.Color = m_Color;
            m_cdColors.ShowDialog();
            m_Color = m_cdColors.Color;
            Refresh();
            base.OnClick(e);
            //MessageBox.Show(m_cdColors.Color.ToString());
           
        }
        
        
        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            SizeF sizeF = new SizeF(
                   (float)ClientRectangle.Width -14.0f,
                   (float)ClientRectangle.Height -14.0f);
            RectangleF rectF = new RectangleF(new PointF(7.0f, 7.0f), sizeF);
            Brush b = new System.Drawing.SolidBrush(m_Color);
            pe.Graphics.FillRectangle(b, rectF);
            
            b.Dispose();
            //ControlPaint.DrawBorder(pe.Graphics, ClientRectangle,
            //        BackColor, ButtonBorderStyle.Outset);
        }
        
    }
}
