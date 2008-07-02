using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreferences
{
    public partial class PanelSharedSettings : UserControl
    {
        

        public PanelSharedSettings()
        {
            InitializeComponent();
            m_tbOpacity.Value = 19;
            m_lbptOpacity.Text = "95%";
            
            m_cbFont.Populate(this.m_lbFont.Font.Name ,false);
            m_cbCandiFont.Populate(this.m_lbFont.Font.Name, false);
            
            //m_cbFontColor.Items = new KnownColorCollection(KnownColorFilter.All);
            //m_cbBackColor.Items = new KnownColorCollection(KnownColorFilter.All);
        }

        protected XmlDocument m_ovConfDOM = null;
        protected OVConfigDom m_ovConfDomHandler = null;

        public float m_opacity;
        public delegate void OpacityChangedHandler();

        [Category("Action")]
        [Description("Fires when opacity value changed.")]
        public event OpacityChangedHandler OpacityChanged;

        protected virtual void OnOpacityChanged()
        {
            m_opacity = (float)(m_tbOpacity.Value * 2.5f +50f) / 100.0f;
            System.Diagnostics.Debug.WriteLine("value:" + m_tbOpacity.Value.ToString() + "m_opacity:" + m_opacity.ToString());
        }

        public virtual void Init(XmlDocument confDOM)
        {
            
            m_ovConfDOM = confDOM;
            m_ovConfDomHandler = new OVConfigDom(m_ovConfDOM);

        }

        public void LoadSettings()
        {
            int opacity = Convert.ToInt32(
                 m_ovConfDomHandler.GetGlobalAttribute(
                 "SharedSettings", "Opacity", "95"));
            if (opacity>0)
                this.m_tbOpacity.Value = (opacity - 50) / 5 * 2;
            

            this.m_cbFont.setFont(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "NotifyFont", "Microsoft JhengHei"));
            
            int notifyfontsize =
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "NotifyFontSize", "14"));
            if (notifyfontsize > 0)
            {
                for (int i = 0; i < m_cbFontSize.Items.Count; i++)
                {
                    if (notifyfontsize <=
                        Convert.ToInt32(m_cbFontSize.Items[i].ToString()))
                    {
                        m_cbFontSize.SelectedIndex = i;
                        break;
                    }
                }

            }
            Color fontcolor= Color.FromArgb(
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "NotifyFontColor", "-1")));
            m_cbFontColor.setColor(fontcolor);
            m_lbNotifyPreview.ForeColor = fontcolor;
            
            Color backcolor =Color.FromArgb(
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "NotifyBackColor", "-10197916"))); //0xff646464
            m_cbBackColor.setColor(backcolor);
            m_lbNotifyPreview.BackColor = backcolor;
            
            this.m_cbFont_SelectedIndexChanged(null, null);


            this.m_cbCandiFont.setFont(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "CandiFont", "Microsoft JhengHei"));
            
            int candifontsize =
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "CandiFontSize", "12"));
            if (candifontsize > 0)
            {
                for (int i = 0; i < m_cbCandiFontSize.Items.Count; i++)
                {
                    if (candifontsize <=
                        Convert.ToInt32(m_cbCandiFontSize.Items[i].ToString()))
                    {
                        m_cbCandiFontSize.SelectedIndex = i;
                        break;
                    }
                }

            }
            
            Color candifontcolor = Color.FromArgb(
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "CandiFontColor", "-16777216"))); 
            m_cbCandiFontColor.setColor(candifontcolor);
            m_lbCandiPreview.ForeColor = candifontcolor;

            Color candibackcolor = Color.FromArgb(
                Convert.ToInt32(
                m_ovConfDomHandler.GetGlobalAttribute(
                "SharedSettings", "CandiBackColor", "-460545"))); //Colors.Ghostwhite 0xfff8f8ff
            m_cbCandiBackColor.setColor(candibackcolor);
            m_lbCandiPreview.BackColor = candibackcolor;
            
            this.m_cbCandiFont_SelectedIndexChanged(null, null);

            m_cbEnableNotify.Checked =(
                Convert.ToInt32(
                    m_ovConfDomHandler.GetGlobalAttribute(
                    "SharedSettings", "NotifyWindowEnabled", "1")
                    )>0);
            




        }

        public void SaveSettings()
        {
            m_ovConfDomHandler.SetGlobalAttribute(
                 "SharedSettings", "Opacity",
                 ((this.m_tbOpacity.Value)*5/2+50).ToString());
            
            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "NotifyFont", this.m_cbFont.nfont.FontFamily.GetName(1033));

            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "NotifyFontSize"
                , m_cbFontSize.SelectedItem.ToString());
                        
            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "NotifyFontColor", m_cbFontColor.m_Color.ToArgb().ToString() );
            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "NotifyBackColor", m_cbBackColor.m_Color.ToArgb().ToString());

            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "CandiFont", m_cbCandiFont.nfont.FontFamily.GetName(1033));

            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "CandiFontSize", m_cbCandiFontSize.SelectedItem.ToString());

            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "CandiFontColor", m_cbCandiFontColor.m_Color.ToArgb().ToString());
            m_ovConfDomHandler.SetGlobalAttribute(
                "SharedSettings", "CandiBackColor", m_cbCandiBackColor.m_Color.ToArgb().ToString());
 
            m_ovConfDomHandler.GetGlobalAttribute(
                    "SharedSettings", "NotifyWindowEnabled", 
                    Convert.ToInt32(m_cbEnableNotify.Checked).ToString());

        }

        private void m_tbOpacity_Scroll(object sender, EventArgs e)
        {
            m_lbptOpacity.Text = (m_tbOpacity.Value*2.5+50).ToString() + "%";


            if (OpacityChanged != null)
            {
                OnOpacityChanged();
                OpacityChanged();
            }
            
        }

        private void m_cbFontColor_Click(object sender, EventArgs e)
        {
            m_lbNotifyPreview.ForeColor = m_cbFontColor.m_Color;
        }

        private void m_cbBackColor_Click(object sender, EventArgs e)
        {
            m_lbNotifyPreview.BackColor = m_cbBackColor.m_Color;
        }

        private void m_cbFontSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_cbFont.nfont != null)
            {
                m_lbNotifyPreview.Font = new Font(m_cbFont.nfont.FontFamily.GetName(1033),
                Convert.ToSingle(m_cbFontSize.Items[m_cbFontSize.SelectedIndex]));

                m_lbNotifyPreview.Location = new Point
                    ((panelPreview.Size.Width - m_lbNotifyPreview.Size.Width) / 2
                       , m_lbNotifyPreview.Location.Y);

                System.Diagnostics.Debug.WriteLine(m_lbNotifyPreview.Font.FontFamily.GetName(1033)); //Get the "en_US" fontname
            }

        }

        void m_cbCandiFontSize_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (m_cbCandiFont.nfont != null)
            {
                m_lbCandiPreview.Font = new Font(m_cbCandiFont.nfont.FontFamily.GetName(1033),
                Convert.ToSingle(m_cbCandiFontSize.Items[m_cbCandiFontSize.SelectedIndex]));

                m_lbCandiPreview.Location = new Point
                    ((panelPreview.Size.Width - m_lbCandiPreview.Size.Width) / 2
                       , m_lbCandiPreview.Location.Y);

                System.Diagnostics.Debug.WriteLine(m_lbCandiPreview.Font.FontFamily.GetName(1033)); //Get the "en_US" fontname
            }

        }


        private void m_cbFont_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_cbFont.nfont != null)
            {
                m_lbNotifyPreview.Font = new Font(m_cbFont.nfont.FontFamily.GetName(1033),
                    Convert.ToSingle(m_cbFontSize.Items[m_cbFontSize.SelectedIndex]));
            }

        }

        void m_cbCandiFont_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (m_cbCandiFont.nfont != null)
            {
                m_lbCandiPreview.Font = new Font( m_cbCandiFont.nfont.FontFamily.GetName(1033),
                    Convert.ToSingle(m_cbCandiFontSize.Items[m_cbCandiFontSize.SelectedIndex]));
                
            }
        }

        private void m_cbCandiFontColor_Click(object sender, EventArgs e)
        {
            m_lbCandiPreview.ForeColor = m_cbCandiFontColor.m_Color;
        }

        private void m_cbCandiBackColor_Click(object sender, EventArgs e)
        {
            m_lbCandiPreview.BackColor = m_cbCandiBackColor.m_Color;

        }



      
       
       
        
    }


}
