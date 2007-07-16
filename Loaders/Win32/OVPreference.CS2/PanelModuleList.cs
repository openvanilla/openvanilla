using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreference.CS2
{
    public partial class PanelModuleList : UserControl
    {
        private XmlDocument m_ovConfDOM = null;

        public PanelModuleList(XmlDocument confDOM)
        {
            m_ovConfDOM = confDOM;

            InitializeComponent();
        }

        private void m_clModuleList_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            int selectedItemIndex = ((CheckedListBox)sender).SelectedIndex;
            string selectedModuleName =
                m_clModuleList.Items[selectedItemIndex].ToString();
            bool isChecked = m_clModuleList.GetItemChecked(selectedItemIndex);

            XmlNode nodeSelectedModule = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='" + selectedModuleName +
                "']/key[@name='enable']");
            nodeSelectedModule.Attributes["value"].Value =
                isChecked ? "1" : "0";
        }
    }
}
