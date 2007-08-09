using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace OVPreference.CS2
{
    public partial class PanelSharedSettings : UserControl
    {
        public PanelSharedSettings()
        {
            InitializeComponent();
            
            m_cbFont.Populate(false);
            m_cbFontColor.Items = new KnownColorCollection(KnownColorFilter.All);
            m_cbBackColor.Items = new KnownColorCollection(KnownColorFilter.All);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.m_cdMoreColors.ShowDialog();
        }

        private void fontColorPickerNotiFunction(object sender, EventArgs e)
        {
            if(m_cbFontColor.SelectedIndex.Equals(168))
            {
                this.m_cdMoreColors.ShowDialog();
            }
        }
        private void bgColorPickerNotiFunction(object sender, EventArgs e)
        {
            if (m_cbFontColor.SelectedIndex.Equals(168))
            {
                this.m_cdMoreColors.ShowDialog();
            }
        }
    }


}
