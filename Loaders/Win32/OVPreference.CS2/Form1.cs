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
        private List<OVConfig> m_ovConfList = new List<OVConfig>();
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

            ComparerOVConfig comparer = new ComparerOVConfig();
            comparer.SortBy = EnumSortBy.Priority;
            m_ovConfList.Sort(comparer);
            comparer.SortBy = EnumSortBy.Enabled;
            m_ovConfList.Sort(comparer);

            PanelModuleList pnModuleList = new PanelModuleList();
            foreach (OVConfig conf in m_ovConfList)
            {
                pnModuleList.AddModule(
                    conf.moduleName,
                    Convert.ToBoolean(
                        Convert.ToInt32(
                            conf.settings["enabled"])));

                if (conf.moduleName.Equals("TLIM"))
                {
                    this.AddTabTLIM(conf, m_ovConfDOM);
                }
                else
                {
                    if (!isGenericTabAdded &&
                        conf.moduleName.StartsWith("OVIMGeneric"))
                    {
                        isGenericTabAdded = true;
                        this.AddTabGeneric(conf, m_ovConfDOM);
                    }
                }
            }
            this.AddTabModuleList(pnModuleList);
        }

        protected void AddTabModuleList(PanelModuleList pnModuleList)
        {
            TabPage tpModuleList = new TabPage("Module List");
            tpModuleList.Controls.Add(pnModuleList);
            this.m_tcSelf.Controls.Add(tpModuleList);

            Control control = tpModuleList;
            while (control.Parent != null)
            {
                control.Parent.ClientSize = control.Size;
                control = control.Parent;
            }
        }

        protected void AddTabGeneric(OVConfig conf, XmlDocument confDOM)
        {
            PanelGeneric pnGeneric = new PanelGeneric(conf, confDOM);
            TabPage tpGeneric = new TabPage(conf.moduleName);

            tpGeneric.Controls.Add(pnGeneric);
            tpGeneric.ClientSize = pnGeneric.Size;

            this.m_tcSelf.Controls.Add(tpGeneric);
            this.m_tcSelf.ClientSize = tpGeneric.Size;

            this.m_tlSelf.ClientSize = this.m_tcSelf.Size;

            this.ClientSize = this.m_tlSelf.Size;
        }

        protected void AddTabTLIM(OVConfig conf, XmlDocument confDOM)
        {
            PanelTLIM pnTLIM = new PanelTLIM(conf, confDOM);
            TabPage tpTLIM = new TabPage(conf.moduleName);

            tpTLIM.Controls.Add(pnTLIM);
            tpTLIM.ClientSize = pnTLIM.Size;

            this.m_tcSelf.Controls.Add(tpTLIM);
            this.m_tcSelf.ClientSize = tpTLIM.Size;

            this.m_tlSelf.ClientSize = this.m_tcSelf.Size;

            this.ClientSize = this.m_tlSelf.Size;
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
                        m_ovConfList.Add(ovConfSet);

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
            //MessageBox.Show("i=" + m_inputType + ", o=" + m_outputType + ", m_diacriticOption=" + m_diacriticOption + ", m_doNormalize=" + m_doNormalize + ", m_doForcePOJStyle=" + m_doForcePOJStyle);
            m_ovConfDOM.Save(m_ovConfPath);
            this.Close();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            saveData();
        }
    }
}