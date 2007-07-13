using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreference.CS2
{
    public partial class PanelGeneric : UserControl
    {
        private OVConfig m_ovConf = null;
        private XmlDocument m_ovConfDOM = new XmlDocument();

        private bool m_doAutoCompose;
        private bool m_isEnabled;
        private bool m_doHitMaxAndCompose;
        private int m_maxKeySequenceLength;
        private int m_priority;
        private bool m_doShiftSelectionKey;
        private bool m_doWarningBeep;

        public PanelGeneric(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;

            InitializeComponent();

            LoadSettings();
        }

        protected void LoadSettings()
        {
            m_doAutoCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["autoCompose"]));
            m_isEnabled =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["enable"]));
            m_doHitMaxAndCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["hitMaxAndCompose"]));
            m_maxKeySequenceLength =
                Convert.ToInt32(
                    m_ovConf.settings["maxKeySequenceLength"]);
            m_priority =
                Convert.ToInt32(
                    m_ovConf.settings["priority"]);
            m_doShiftSelectionKey =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["shiftSelectionKey"]));
            m_doWarningBeep =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["warningBeep"]));

            cbAutoCompose.Checked = m_doAutoCompose;
            cbHitMaxAndCompose.Checked = m_doHitMaxAndCompose;
            tbMaxKeySequenceLength.Text = m_maxKeySequenceLength.ToString();
            cbShiftSelectionKey.Checked = m_doShiftSelectionKey;
            cbWarningBeep.Checked = m_doWarningBeep;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_doShiftSelectionKey = cbShiftSelectionKey.Checked;
            XmlNode nodeShiftSelectionKey =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" +
                    m_ovConf.moduleName +
                    "']/key[@name='shiftSelectionKey']");
            nodeShiftSelectionKey.Attributes["value"].Value =
                m_doShiftSelectionKey ? "1" : "0";
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            m_doAutoCompose = cbAutoCompose.Checked;
            XmlNode nodeAutoCompose =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" +
                    m_ovConf.moduleName +
                    "']/key[@name='autoCompose']");
            nodeAutoCompose.Attributes["value"].Value =
                m_doAutoCompose ? "1" : "0";
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            m_doHitMaxAndCompose = cbHitMaxAndCompose.Checked;
            XmlNode nodeHitMaxAndCompose =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" +
                    m_ovConf.moduleName +
                    "']/key[@name='hitMaxAndCompose']");
            nodeHitMaxAndCompose.Attributes["value"].Value =
                m_doHitMaxAndCompose ? "1" : "0";
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            m_doWarningBeep = cbWarningBeep.Checked;
            XmlNode nodeWarningBeep =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" +
                    m_ovConf.moduleName +
                    "']/key[@name='warningBeep']");
            nodeWarningBeep.Attributes["value"].Value =
                m_doWarningBeep ? "1" : "0";
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (tbMaxKeySequenceLength.Text.Length == 0) return;
            m_maxKeySequenceLength = Convert.ToInt32(tbMaxKeySequenceLength.Text);
            XmlNode nodeMaxKeySequenceLength =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" +
                    m_ovConf.moduleName +
                    "']/key[@name='maxKeySequenceLength']");
            nodeMaxKeySequenceLength.Attributes["value"].Value =
                m_maxKeySequenceLength.ToString();
        }
    }
}
