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
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.colorDialog1.ShowDialog();
        }
    }
}
