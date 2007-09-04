using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Diagnostics;

namespace OVPreferences
{
    public partial class PanelGeneric : UserControl
    {
        private OVConfig m_ovConf = null;
        private XmlDocument m_ovConfDOM = null;
        private OVConfigDom m_ovConfDomHandler = null;

        private bool m_doAutoCompose;
        private bool m_doHitMaxAndCompose;
        private uint m_maxKeySequenceLength;
        private bool m_doShiftSelectionKey;
        private bool m_doWarningBeep;

        public PanelGeneric()
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

        protected virtual void LoadSettings()
        {
            m_doAutoCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "autoCompose", "0")));
            m_doHitMaxAndCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "hitMaxAndCompose", "0")));
            m_maxKeySequenceLength =
                Convert.ToUInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "maxKeySequenceLength", "5"));
            m_doShiftSelectionKey =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "shiftSelectionKey", "0")));
            m_doWarningBeep =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "warningBeep", "0")));

            m_cbAutoCompose.Checked = m_doAutoCompose;
            m_cbHitMaxAndCompose.Checked = m_doHitMaxAndCompose;
            m_tbMaxKeySequenceLength.Text = m_maxKeySequenceLength.ToString();
            m_cbShiftSelectionKey.Checked = m_doShiftSelectionKey;
            m_cbWarningBeep.Checked = m_doWarningBeep;
        }

        private void m_cbShiftSelectionKey_CheckedChanged(object sender, EventArgs e)
        {
            m_doShiftSelectionKey = m_cbShiftSelectionKey.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "shiftSelectionKey",
                m_doShiftSelectionKey ? "1" : "0");
        }

        private void m_cbAutoCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doAutoCompose = m_cbAutoCompose.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "autoCompose",
                m_doAutoCompose ? "1" : "0");
        }

        private void m_cbHitMaxAndCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doHitMaxAndCompose = m_cbHitMaxAndCompose.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "hitMaxAndCompose",
                m_doHitMaxAndCompose ? "1" : "0");
        }

        private void m_cbWarningBeep_CheckedChanged(object sender, EventArgs e)
        {
            m_doWarningBeep = m_cbWarningBeep.Checked;
            m_ovConfDomHandler.SetAttribute(
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

            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "maxKeySequenceLength",
                m_maxKeySequenceLength.ToString());
        }
    }
}