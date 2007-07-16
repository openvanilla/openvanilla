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
    public partial class PanelTLIM : UserControl
    {
        private OVConfig m_ovConf = null;
        private XmlDocument m_ovConfDOM = null;

        protected int m_inputType;
        protected int m_outputType;
        protected int m_diacriticOption;
        protected bool m_doNormalize;
        protected bool m_doForcePOJStyle;

        public PanelTLIM(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;

            InitializeComponent();

            SetEventHandler();
            LoadSettings();
        }

        protected void SetEventHandler()
        {
            this.m_rbDiacriticFront.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbDiacriticEnd.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbInputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbInputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbInputTLPA.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbInputDT.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbOutputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.m_rbOutputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
        }

        protected void LoadSettings()
        {
            m_inputType =
                Convert.ToInt32(
                    m_ovConf.settings["inputSyllableType"]);
            m_outputType =
                Convert.ToInt32(
                    m_ovConf.settings["outputSyllableType"]);
            m_diacriticOption =
                Convert.ToInt32(
                    m_ovConf.settings["diacriticInputOption"]);
            m_doNormalize =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["shouldNormalize"]));
            m_doForcePOJStyle =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["forcePOJStyleWithTL"]));

            switch (m_inputType)
            {
                case 1:
                    m_rbInputTL.Checked = true;
                    break;
                case 0:
                    m_rbInputPOJ.Checked = true;
                    break;
                case 2:
                    m_rbInputTLPA.Checked = true;
                    break;
                case 3:
                    m_rbInputDT.Checked = true;
                    break;
            }

            switch (m_outputType)
            {
                case 1:
                    m_rbOutputTL.Checked = true;
                    break;
                case 0:
                    m_rbOutputPOJ.Checked = true;
                    break;
            }

            if (m_diacriticOption == 0)
                m_rbDiacriticFront.Checked = true;
            else
                m_rbDiacriticEnd.Checked = true;

            if (m_doNormalize)
                m_cbNormalize.Checked = true;
            if (m_doForcePOJStyle)
                m_cbForcePOJStyle.Checked = true;
        }

        private void checkOutputType()
        {
            if (m_outputType == 1) // TL
                m_cbForcePOJStyle.Enabled = true;
            else
            {
                m_cbForcePOJStyle.Enabled = false;
                m_cbForcePOJStyle.Checked = false;
            }
        }

        private void normalizeButton_CheckedChanged(object sender, EventArgs e)
        {
            m_doNormalize = m_cbNormalize.Checked;
            XmlNode nodeNormalize = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='shouldNormalize']");
            nodeNormalize.Attributes["value"].Value =
                m_doNormalize ? "1" : "0";
        }

        private void buttons_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton button = (RadioButton)sender;
            if (button.Checked == false) return;

            bool isInputGroup = false;
            bool isOutputGroup = false;
            bool isDiacriticGroup = false;
            if (button == m_rbInputPOJ)
            {
                m_inputType = 0;
                isInputGroup = true;
            }
            if (button == m_rbInputTL)
            {
                m_inputType = 1;
                isInputGroup = true;
            }
            if (button == m_rbInputTLPA)
            {
                m_inputType = 2;
                isInputGroup = true;
            }
            if (button == m_rbInputDT)
            {
                m_inputType = 3;
                isInputGroup = true;
            }

            if (button == m_rbOutputPOJ)
            {
                m_outputType = 0;
                isOutputGroup = true;
            }
            if (button == m_rbOutputTL)
            {
                m_outputType = 1;
                isOutputGroup = true;
            }

            if (button == m_rbDiacriticFront)
            {
                m_diacriticOption = 0;
                isDiacriticGroup = true;
            }
            if (button == m_rbDiacriticEnd)
            {
                m_diacriticOption = 1;
                isDiacriticGroup = true;
            }

            string attrName = "";
            string attrValue = "";
            if (isInputGroup)
            {
                attrName = "inputSyllableType";
                attrValue = m_inputType.ToString();
            }
            else if (isOutputGroup)
            {
                attrName = "outputSyllableType";
                attrValue = m_outputType.ToString();
            }
            else if (isDiacriticGroup)
            {
                attrName = "diacriticInputOption";
                attrValue = m_diacriticOption.ToString();
            }

            XmlNode nodeOption = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='" + attrName + "']");
            nodeOption.Attributes["value"].Value = attrValue;

            checkOutputType();
        }

        private void forcePOJStyleButton_CheckedChanged(object sender, EventArgs e)
        {
            m_doForcePOJStyle = m_cbForcePOJStyle.Checked;
            XmlNode nodeForcePOJStyle = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='forcePOJStyleWithTL']");
            nodeForcePOJStyle.Attributes["value"].Value =
                m_doForcePOJStyle ? "1" : "0";
        }
    }
}
