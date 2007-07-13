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

        public PanelTLIM(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;

            InitializeComponent();
        }

        public void SetBehaviors()
        {
            this.rbDiacriticFront.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbDiacriticEnd.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputTLPA.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbInputDT.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbOutputPOJ.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.rbOutputTL.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            /*
            inputType = outputType = 1;
            diacritic = 0;
            normalize = forcePOJStyle = false;
             */
            //<comment author='b6s'>
            //The variable settings here should use the variable name itself
            //to retrieve the value. This will be implemented in the near
            //future.
            //</comment>
            inputType =
                Convert.ToInt32(
                    m_ovConf.settings["inputSyllableType"]);
            outputType =
                Convert.ToInt32(
                    m_ovConf.settings["outputSyllableType"]);
            diacritic =
                Convert.ToInt32(
                    m_ovConf.settings["diacriticInputOption"]);
            normalize =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["shouldNormalize"]));
            forcePOJStyle =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConf.settings["forcePOJStyleWithTL"]));

            switch (inputType)
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

            switch (outputType)
            {
                case 1:
                    rbOutputTL.Checked = true;
                    break;
                case 0:
                    rbOutputPOJ.Checked = true;
                    break;
            }

            if (diacritic == 0)
                rbDiacriticFront.Checked = true;
            else
                rbDiacriticEnd.Checked = true;

            if (normalize)
                cbNormalize.Checked = true;
            if (forcePOJStyle)
                cbForcePOJStyle.Checked = true;
        }

        private void checkOutputType()
        {
            if (outputType == 1) // TL
                cbForcePOJStyle.Enabled = true;
            else
            {
                cbForcePOJStyle.Enabled = false;
                cbForcePOJStyle.Checked = false;
            }
        }

        protected int inputType;
        protected int outputType;
        protected int diacritic;
        protected bool normalize;
        protected bool forcePOJStyle;

        private void normalizeButton_CheckedChanged(object sender, EventArgs e)
        {
            normalize = cbNormalize.Checked;
            XmlNode nodeNormalize = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='shouldNormalize']");
            nodeNormalize.Attributes["value"].Value =
                normalize ? "1" : "0";
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
                inputType = 0;
                isInputGroup = true;
            }
            if (button == rbInputTL)
            {
                inputType = 1;
                isInputGroup = true;
            }
            if (button == rbInputTLPA)
            {
                inputType = 2;
                isInputGroup = true;
            }
            if (button == rbInputDT)
            {
                inputType = 3;
                isInputGroup = true;
            }

            if (button == rbOutputPOJ)
            {
                outputType = 0;
                isOutputGroup = true;
            }
            if (button == rbOutputTL)
            {
                outputType = 1;
                isOutputGroup = true;
            }

            if (button == rbDiacriticFront)
            {
                diacritic = 0;
                isDiacriticGroup = true;
            }
            if (button == rbDiacriticEnd)
            {
                diacritic = 1;
                isDiacriticGroup = true;
            }

            string attrName = "";
            string attrValue = "";
            if (isInputGroup)
            {
                attrName = "inputSyllableType";
                attrValue = inputType.ToString();
            }
            else if (isOutputGroup)
            {
                attrName = "outputSyllableType";
                attrValue = outputType.ToString();
            }
            else if (isDiacriticGroup)
            {
                attrName = "diacriticInputOption";
                attrValue = diacritic.ToString();
            }

            XmlNode nodeOption = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='" + attrName + "']");
            nodeOption.Attributes["value"].Value = attrValue;

            checkOutputType();
        }

        private void forcePOJStyleButton_CheckedChanged(object sender, EventArgs e)
        {
            forcePOJStyle = cbForcePOJStyle.Checked;
            XmlNode nodeForcePOJStyle = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='forcePOJStyleWithTL']");
            nodeForcePOJStyle.Attributes["value"].Value =
                forcePOJStyle ? "1" : "0";
        }
    }
}
