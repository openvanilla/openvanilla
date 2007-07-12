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

        private bool isGenericTabAdded = false;

        public Form1()
        {
            InitializeComponent();

            loadData();

            foreach (string moduleKey in m_ovConfDict.Keys)
            {
                if (moduleKey.Equals("TLIM"))
                {
                    this.AddTabTLIM();
                    //set TabTLIM's behavior here
                }
                else
                {
                    if (!isGenericTabAdded && moduleKey.StartsWith("OVIMGeneric"))
                    {
                        isGenericTabAdded = true;
                        this.AddTabGeneric();
                    }

                    OVConfig conf = m_ovConfDict[moduleKey];
                    foreach (string entryKey in conf.settings.Keys)
                    {
                        string entryValue = conf.settings[entryKey];
                    }
                }
            }
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

        private void saveButton_Click(object sender, EventArgs e)
        {
            saveData();
        }
    }
}