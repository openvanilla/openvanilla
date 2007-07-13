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
        private XmlDocument m_ovConfDOM = new XmlDocument();

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
            this.rbDiacriticFront.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbDiacriticEnd.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputTLPA.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputDT.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbOutputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbOutputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
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
                    rbInputTL.Checked = true;
                    break;
                case 0:
                    rbInputPOJ.Checked = true;
                    break;
                case 2:
                    rbInputTLPA.Checked = true;
                    break;
                case 3:
                    rbInputDT.Checked = true;
                    break;
            }

            switch (m_outputType)
            {
                case 1:
                    rbOutputTL.Checked = true;
                    break;
                case 0:
                    rbOutputPOJ.Checked = true;
                    break;
            }

            if (m_diacriticOption == 0)
                rbDiacriticFront.Checked = true;
            else
                rbDiacriticEnd.Checked = true;

            if (m_doNormalize)
                cbNormalize.Checked = true;
            if (m_doForcePOJStyle)
                cbForcePOJStyle.Checked = true;
        }

        private void checkOutputType()
        {
            if (m_outputType == 1) // TL
                cbForcePOJStyle.Enabled = true;
            else
            {
                cbForcePOJStyle.Enabled = false;
                cbForcePOJStyle.Checked = false;
            }
        }

        private void normalizeButton_CheckedChanged(object sender, EventArgs e)
        {
            m_doNormalize = cbNormalize.Checked;
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
            if (button == rbInputPOJ)
            {
                m_inputType = 0;
                isInputGroup = true;
            }
            if (button == rbInputTL)
            {
                m_inputType = 1;
                isInputGroup = true;
            }
            if (button == rbInputTLPA)
            {
                m_inputType = 2;
                isInputGroup = true;
            }
            if (button == rbInputDT)
            {
                m_inputType = 3;
                isInputGroup = true;
            }

            if (button == rbOutputPOJ)
            {
                m_outputType = 0;
                isOutputGroup = true;
            }
            if (button == rbOutputTL)
            {
                m_outputType = 1;
                isOutputGroup = true;
            }

            if (button == rbDiacriticFront)
            {
                m_diacriticOption = 0;
                isDiacriticGroup = true;
            }
            if (button == rbDiacriticEnd)
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
            m_doForcePOJStyle = cbForcePOJStyle.Checked;
            XmlNode nodeForcePOJStyle = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='forcePOJStyleWithTL']");
            nodeForcePOJStyle.Attributes["value"].Value =
                m_doForcePOJStyle ? "1" : "0";
        }
    }
}
