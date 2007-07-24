using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace OVPreference.CS2
{
    public class FontComboBox : ComboBox
    {
        public FontComboBox()
        {
            MaxDropDownItems = 20;
            IntegralHeight = false;
            Sorted = false;
            DropDownStyle = ComboBoxStyle.DropDownList;
            DrawMode = DrawMode.OwnerDrawVariable;
        }

        public void Populate(bool b)
        {
            both = b;
            foreach (FontFamily ff in FontFamily.Families)
            {
                if (ff.IsStyleAvailable(FontStyle.Regular))
                    Items.Add(ff.Name);
            }

            if (Items.Count > 0)
                SelectedIndex = 0;
            ttimg = new Bitmap(GetType(), "ttfbmp.bmp");
        }

        protected override void OnMeasureItem(System.Windows.Forms.MeasureItemEventArgs e)
        {
            if (e.Index > -1)
            {
                int w = 0;
                string fontstring = Items[e.Index].ToString();
                Graphics g = CreateGraphics();
                e.ItemHeight = (int)g.MeasureString(fontstring, new Font(fontstring, 10)).Height;
                w = (int)g.MeasureString(fontstring, new Font(fontstring, 10)).Width;
                if (both)
                {
                    int h1 = (int)g.MeasureString(samplestr, new Font(fontstring, 10)).Height;
                    int h2 = (int)g.MeasureString(Items[e.Index].ToString(), new Font("Arial", 10)).Height;
                    int w1 = (int)g.MeasureString(samplestr, new Font(fontstring, 10)).Width;
                    int w2 = (int)g.MeasureString(Items[e.Index].ToString(), new Font("Arial", 10)).Width;
                    if (h1 > h2)
                        h2 = h1;
                    e.ItemHeight = h2;
                    w = w1 + w2;
                }
                w += ttimg.Width * 2;
                if (w > maxwid)
                    maxwid = w;
                if (e.ItemHeight > 20)
                    e.ItemHeight = 20;
            }



            base.OnMeasureItem(e);
        }

        protected override void OnDrawItem(System.Windows.Forms.DrawItemEventArgs e)
        {
            if (e.Index > -1)
            {
                string fontstring = Items[e.Index].ToString();
                nfont = new Font(fontstring, 10);
                Font afont = new Font("Arial", 10);

                if (both)
                {
                    Graphics g = CreateGraphics();
                    int w = (int)g.MeasureString(fontstring, afont).Width;

                    if ((e.State & DrawItemState.Focus) == 0)
                    {
                        e.Graphics.FillRectangle(new SolidBrush(SystemColors.Window),
                            e.Bounds.X + ttimg.Width, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
                        e.Graphics.DrawString(fontstring, afont, new SolidBrush(SystemColors.WindowText),
                            e.Bounds.X + ttimg.Width * 2, e.Bounds.Y);
                        e.Graphics.DrawString(samplestr, nfont, new SolidBrush(SystemColors.WindowText),
                            e.Bounds.X + w + ttimg.Width * 2, e.Bounds.Y);
                    }
                    else
                    {
                        e.Graphics.FillRectangle(new SolidBrush(SystemColors.Highlight),
                            e.Bounds.X + ttimg.Width, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
                        e.Graphics.DrawString(fontstring, afont, new SolidBrush(SystemColors.HighlightText),
                            e.Bounds.X + ttimg.Width * 2, e.Bounds.Y);
                        e.Graphics.DrawString(samplestr, nfont, new SolidBrush(SystemColors.HighlightText),
                            e.Bounds.X + w + ttimg.Width * 2, e.Bounds.Y);
                    }
                }
                else
                {

                    if ((e.State & DrawItemState.Focus) == 0)
                    {
                        e.Graphics.FillRectangle(new SolidBrush(SystemColors.Window),
                            e.Bounds.X + ttimg.Width, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
                        e.Graphics.DrawString(fontstring, nfont, new SolidBrush(SystemColors.WindowText),
                            e.Bounds.X + ttimg.Width * 2, e.Bounds.Y);

                    }
                    else
                    {
                        e.Graphics.FillRectangle(new SolidBrush(SystemColors.Highlight),
                            e.Bounds.X + ttimg.Width, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
                        e.Graphics.DrawString(fontstring, nfont, new SolidBrush(SystemColors.HighlightText),
                            e.Bounds.X + ttimg.Width * 2, e.Bounds.Y);
                    }

                }

                e.Graphics.DrawImage(ttimg, new Point(e.Bounds.X, e.Bounds.Y));
            }
            base.OnDrawItem(e);
        }

        Font nfont;
        bool both = false;
        int maxwid = 0;
        string samplestr = " - Hello World";
        Image ttimg;

        protected override void OnDropDown(System.EventArgs e)
        {
            this.DropDownWidth = maxwid + 30;
        }

    }
}
