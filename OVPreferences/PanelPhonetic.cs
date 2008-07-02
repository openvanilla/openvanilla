using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreferences
{
    public partial class PanelPhonetic : UserControl
    {
        private OVConfig m_ovConf = null;
        private XmlDocument m_ovConfDOM = null;
        private OVConfigDom m_ovConfDomHandler = null;

        private int m_keyboardLayout;
        private string m_selectionKeys;

        public PanelPhonetic()
        {
            InitializeComponent();
        }

        public virtual void Init(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;
            m_ovConfDomHandler = new OVConfigDom(m_ovConfDOM);

            LoadSettings();
        }

        public virtual void LoadSettings()
        {
            m_keyboardLayout =
                Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "keyboardLayout", "0"));
            m_selectionKeys =
                m_ovConfDomHandler.GetAttribute(
                    m_ovConf.moduleName, "selectKey", "123456789");

            m_cbKeyboardLayout.SelectedIndex = m_keyboardLayout;
            m_tbSelectionKeys.Text = m_selectionKeys;
        }

        private void m_cbKeyboardLayout_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_keyboardLayout = m_cbKeyboardLayout.SelectedIndex;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "keyboardLayout",
                m_keyboardLayout.ToString());
        }

        private void m_tbSelectionKeys_TextChanged(object sender, EventArgs e)
        {
            if (m_tbSelectionKeys.Text.Length == 9)
            {
                m_selectionKeys = m_tbSelectionKeys.Text;
                m_ovConfDomHandler.SetAttribute(
                    m_ovConf.moduleName, "selectKey", m_selectionKeys);
            }
        }
    }
}
