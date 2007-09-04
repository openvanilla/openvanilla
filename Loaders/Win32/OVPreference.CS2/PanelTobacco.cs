using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreferences
{
    public partial class PanelTobacco : PanelGeneric
    {
        public PanelTobacco()
        {
            InitializeComponent();
        }

        public override void Init(OVConfig conf, XmlDocument confDOM)
        {
            base.Init(conf, confDOM);
        }
    }
}

