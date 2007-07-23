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
        private XmlDocument m_ovConfDOM = null;
        private OVConfigDomWriter m_ovConfDomWriter = null;

        private bool m_doAutoCompose;
        private bool m_isEnabled;
        private bool m_doHitMaxAndCompose;
        private uint m_maxKeySequenceLength;
        private int m_priority;
        private bool m_doShiftSelectionKey;
        private bool m_doWarningBeep;

        public PanelGeneric(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;
            m_ovConfDomWriter = new OVConfigDomWriter(m_ovConfDOM);

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
                Convert.ToUInt32(
                    m_ovConf.settings["maxKeySequenceLength"]);
            if (m_maxKeySequenceLength == 0) m_maxKeySequenceLength = 5;
            m_priority =
                Convert.ToInt32(
                    m_ovConf.settings["priority"]);
            m_doShiftSelectionKey =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["shiftSelectionKey"]));
            m_doWarningBeep =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["warningBeep"]));

            m_cbAutoCompose.Checked = m_doAutoCompose;
            m_cbHitMaxAndCompose.Checked = m_doHitMaxAndCompose;
            m_tbMaxKeySequenceLength.Text = m_maxKeySequenceLength.ToString();
            m_cbShiftSelectionKey.Checked = m_doShiftSelectionKey;
            m_cbWarningBeep.Checked = m_doWarningBeep;
        }

        private void m_cbShiftSelectionKey_CheckedChanged(object sender, EventArgs e)
        {
            m_doShiftSelectionKey = m_cbShiftSelectionKey.Checked;
            m_ovConfDomWriter.SetAttribute(
                m_ovConf.moduleName,
                "shiftSelectionKey",
                m_doShiftSelectionKey ? "1" : "0");
        }

        private void m_cbAutoCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doAutoCompose = m_cbAutoCompose.Checked;
            m_ovConfDomWriter.SetAttribute(
                m_ovConf.moduleName,
                "autoCompose",
                m_doAutoCompose ? "1" : "0");
        }

        private void m_cbHitMaxAndCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doHitMaxAndCompose = m_cbHitMaxAndCompose.Checked;
            m_ovConfDomWriter.SetAttribute(
                m_ovConf.moduleName,
                "hitMaxAndCompose",
                m_doHitMaxAndCompose ? "1" : "0");
        }

        private void m_cbWarningBeep_CheckedChanged(object sender, EventArgs e)
        {
            m_doWarningBeep = m_cbWarningBeep.Checked;
            m_ovConfDomWriter.SetAttribute(
                m_ovConf.moduleName,
                "warningBeep",
                m_doWarningBeep ? "1" : "0");
        }

        private void m_tbMaxKeySequenceLength_TextChanged(object sender, EventArgs e)
        {
            if (m_tbMaxKeySequenceLength.Text.Length == 0) return;
            try
            {
                m_maxKeySequenceLength =
                    Convert.ToUInt32(m_tbMaxKeySequenceLength.Text);
            }
            catch
            {
                m_maxKeySequenceLength = 0;
            }
            if (m_maxKeySequenceLength == 0)
            {
                MessageBox.Show("Positive Integer Please!");
                m_maxKeySequenceLength = 5;
                return;
            }

            m_ovConfDomWriter.SetAttribute(
                m_ovConf.moduleName,
                "maxKeySequenceLength",
                m_maxKeySequenceLength.ToString());
        }
    }
}