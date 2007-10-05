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
        private bool m_doChooseInFrontOfCursor;
        private bool m_doClearOnError;

        public PanelTobacco()
        {
            InitializeComponent();
        }

        public override void Init(OVConfig conf, XmlDocument confDOM)
        {
            base.Init(conf, confDOM);
        }

        protected override void LoadSettings()
        {
            base.LoadSettings();

            m_doChooseInFrontOfCursor =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "chooseInFrontOfCursor", "0")));
            m_doClearOnError =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "clearSequenceOnError", "0")));

            m_cbChooseInFront.Checked = m_doChooseInFrontOfCursor;
            m_cbClearOnError.Checked = m_doClearOnError;
        }

        private void m_cbClearOnError_CheckedChanged(object sender, EventArgs e)
        {
            m_doClearOnError = m_cbClearOnError.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "clearSequenceOnError",
                m_doClearOnError ? "1" : "0");
        }

        private void m_cbChooseInFront_CheckedChanged(object sender, EventArgs e)
        {
            m_doChooseInFrontOfCursor = m_cbChooseInFront.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "chooseInFrontOfCursor",
                m_doChooseInFrontOfCursor ? "1" : "0");
        }
    }
}

