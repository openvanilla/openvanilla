using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreference.CS2
{
    public partial class Form1 : Form
    {
        private Dictionary<string, OVConfig> m_ovConfDict =
            new Dictionary<string, OVConfig>();
        private static string m_ovConfPath =
            Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData) +
            System.IO.Path.DirectorySeparatorChar +
            "OpenVanilla" + System.IO.Path.DirectorySeparatorChar +
            "config.xml";
        private XmlDocument m_ovConfDOM = new XmlDocument();

        public Form1()
        {
            InitializeComponent();
            this.diacriticFrontButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.diacriticEndButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.inputPOJButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.inputTLButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.inputTLPAButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.inputDTButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.outputPOJButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);
            this.outputTLButton.CheckedChanged += new System.EventHandler(this.buttons_CheckedChanged);

            loadData();

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
            OVConfig ovConfTLIM = m_ovConfDict["TLIM"];
            inputType =
                Convert.ToInt32(
                    ovConfTLIM.settings["inputSyllableType"]);
            outputType =
                Convert.ToInt32(
                    ovConfTLIM.settings["outputSyllableType"]);
            diacritic =
                Convert.ToInt32(
                    ovConfTLIM.settings["diacriticInputOption"]);
            normalize =
                Convert.ToBoolean(Convert.ToInt32(
                    ovConfTLIM.settings["shouldNormalize"]));
            forcePOJStyle =
                Convert.ToBoolean(Convert.ToInt32(
                    ovConfTLIM.settings["forcePOJStyleWithTL"]));

            switch (inputType)
            {
                case 1:
                    inputTLButton.Checked = true;
                    break;
                case 0:
                    inputPOJButton.Checked = true;
                    break;
                case 2:
                    inputTLPAButton.Checked = true;
                    break;
                case 3:
                    inputDTButton.Checked = true;
                    break;
            }

            switch (outputType)
            {
                case 1:
                    outputTLButton.Checked = true;
                    break;
                case 0:
                    outputPOJButton.Checked = true;
                    break;
            }

            if (diacritic == 0)
                diacriticFrontButton.Checked = true;
            else
                diacriticEndButton.Checked = true;

            if (normalize)
                normalizeButton.Checked = true;
            if (forcePOJStyle)
                forcePOJStyleButton.Checked = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void loadData()
        {
            //MessageBox.Show("Loads XML config here");
            m_ovConfDOM.Load(m_ovConfPath);
            OVConfig ovConfSet = new OVConfig();
            using (XmlReader ovConfReader = XmlReader.Create(m_ovConfPath))
            {
                #region Read Elements
                while (ovConfReader.Read())
                {
                    ovConfReader.MoveToElement();
                    /*
                    System.Diagnostics.Debug.WriteLine("Elem:" +
                        ovConfReader.Name + ":=" +
                        ovConfReader.Value);
                     */

                    bool isDictStart = false, isDictEnd = false;
                    bool isKey = false;
                    if (ovConfReader.Name == "dict")
                    {
                        if (ovConfReader.IsStartElement())
                            isDictStart = true;
                        else
                            isDictEnd = true;
                    }
                    else if (ovConfReader.Name == "key")
                        isKey = true;

                    if (isDictEnd)
                        m_ovConfDict.Add(ovConfSet.moduleName, ovConfSet);

                    string attrNameTemp = "";
                    #region Read attributes
                    while (ovConfReader.MoveToNextAttribute())
                    {
                        /*
                        System.Diagnostics.Debug.WriteLine("Attr:" +
                            ovConfReader.Name + ":=" +
                            ovConfReader.Value);
                         */
                        if (isDictStart)
                        {
                            ovConfSet = new OVConfig();
                            ovConfSet.moduleName = ovConfReader.Value;
                        }
                        else if (isKey)
                        {
                            if (ovConfReader.Name == "name")
                                attrNameTemp = ovConfReader.Value;
                            else if (ovConfReader.Name == "value")
                            {
                                ovConfSet.settings.Add(
                                    attrNameTemp, ovConfReader.Value);
                                attrNameTemp = "";
                            }
                        }
                    }
                    #endregion
                }
                #endregion
            }
        }

        private void saveData()
        {
            //MessageBox.Show("i=" + inputType + ", o=" + outputType + ", diacritic=" + diacritic + ", normalize=" + normalize + ", forcePOJStyle=" + forcePOJStyle);
            m_ovConfDOM.Save(m_ovConfPath);
            this.Close();
        }

        private void checkOutputType()
        {
            if (outputType == 1) // TL
                forcePOJStyleButton.Enabled = true;
            else
            {
                forcePOJStyleButton.Enabled = false;
                forcePOJStyleButton.Checked = false;
            }
        }

        protected int inputType;
        protected int outputType;
        protected int diacritic;
        protected bool normalize;
        protected bool forcePOJStyle;

        private void normalizeButton_CheckedChanged(object sender, EventArgs e)
        {
            normalize = normalizeButton.Checked;
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
            if (button == inputPOJButton)
            {
                inputType = 0;
                isInputGroup = true;
            }
            if (button == inputTLButton)
            {
                inputType = 1;
                isInputGroup = true;
            }
            if (button == inputTLPAButton)
            {
                inputType = 2;
                isInputGroup = true;
            }
            if (button == inputDTButton)
            {
                inputType = 3;
                isInputGroup = true;
            }

            if (button == outputPOJButton)
            {
                outputType = 0;
                isOutputGroup = true;
            }
            if (button == outputTLButton)
            {
                outputType = 1;
                isOutputGroup = true;
            }

            if (button == diacriticFrontButton)
            {
                diacritic = 0;
                isDiacriticGroup = true;
            }
            if (button == diacriticEndButton)
            {
                diacritic = 1;
                isDiacriticGroup = true;
            }

            string attrName = "";
            string attrValue = "";
            if (isInputGroup) {
                attrName = "inputSyllableType";
                attrValue = inputType.ToString();
            }
            else if (isOutputGroup) {
                attrName = "outputSyllableType";
                attrValue = outputType.ToString();
            }
            else if (isDiacriticGroup) {
                attrName = "diacriticInputOption";
                attrValue = diacritic.ToString();
            }

            XmlNode nodeOption = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='" + attrName +"']");
            nodeOption.Attributes["value"].Value = attrValue;

            checkOutputType();
        }

        private void forcePOJStyleButton_CheckedChanged(object sender, EventArgs e)
        {
            forcePOJStyle = forcePOJStyleButton.Checked;
            XmlNode nodeForcePOJStyle = m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/dict[@name='TLIM']/key[@name='forcePOJStyleWithTL']");
            nodeForcePOJStyle.Attributes["value"].Value =
                forcePOJStyle ? "1" : "0";
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            saveData();
        }
    }
}