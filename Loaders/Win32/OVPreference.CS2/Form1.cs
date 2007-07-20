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

        public Form1()
        {
            InitializeComponent();

            LoadData();

            ComparerOVConfig comparer = new ComparerOVConfig();
            comparer.SortBy = EnumSortBy.Priority;
            m_ovConfList.Sort(comparer);
            //<comment author='b6s'>
            // The following code is useless
            // since C# does not have a stable sort.
            //comparer.SortBy = EnumSortBy.Enabled;
            //m_ovConfList.Sort(comparer);
            //</commnet>

            PanelModuleList pnModuleList = new PanelModuleList(m_ovConfDOM);
            foreach (OVConfig conf in m_ovConfList)
            {
                bool isEnabled = false;
                if(conf.settings.ContainsKey("enable"))
                    isEnabled =
                        Convert.ToBoolean(
                            Convert.ToInt32(
                                conf.settings["enable"]));
                pnModuleList.AddModule(conf.moduleName, isEnabled);

                if (conf.moduleName.Equals("TLIM"))
                {
                    this.AddTabTLIM(conf, m_ovConfDOM);
                }
                else if (conf.moduleName.StartsWith("OVIMGeneric"))
                {
                        this.AddTabGeneric(conf, m_ovConfDOM);
                }
            }
            this.AddTabModuleList(pnModuleList);
        }

        protected void AddTabModuleList(PanelModuleList pnModuleList)
        {
            TabPage tpModuleList = new TabPage("Module List");
            tpModuleList.Controls.Add(pnModuleList);
            //this.m_tcSelf.Controls.Add(tpModuleList);

            //<comment author='b6s'>
            IntPtr h = this.m_tcSelf.Handle;
            // It's a bug that TabControl.Handle has to be touched manually
            // Before TabControl.TabPages.Insert() is invoked.
            this.m_tcSelf.TabPages.Insert(0, tpModuleList);
            //</comment>
            this.m_tcSelf.SelectTab(0);

            this.SetSize(tpModuleList);
        }

        protected void AddTabGeneric(OVConfig conf, XmlDocument confDOM)
        {
            PanelGeneric pnGeneric = new PanelGeneric(conf, confDOM);
            TabPage tpGeneric = new TabPage(conf.moduleName);

            tpGeneric.Controls.Add(pnGeneric);
            this.m_tcSelf.TabPages.Add(tpGeneric);

            this.SetSize(pnGeneric);
        }

        protected void AddTabTLIM(OVConfig conf, XmlDocument confDOM)
        {
            PanelTLIM pnTLIM = new PanelTLIM(conf, confDOM);
            TabPage tpTLIM = new TabPage(conf.moduleName);

            tpTLIM.Controls.Add(pnTLIM);
            this.m_tcSelf.TabPages.Add(tpTLIM);

            this.SetSize(pnTLIM);
        }

        private void SetSize(Control innerControl)
        {
            if (innerControl == null) return;
            //<comment author='b6s'>
            innerControl.ClientSize = innerControl.PreferredSize;
            // It's a trick to change the smaller ClientSize to
            // a larger PreferredSize to reserve more spaces.
            //</comment>

            //<comment author='b6s'>
            // A while loop insteads recusive calls of SetSize()
            // since the size of Parent must connect to Child's.
            //</comment>
            while (innerControl.Parent != null)
            {
                if (innerControl.Parent.ClientSize.Width
                        < innerControl.PreferredSize.Width
                    ||
                    innerControl.Parent.ClientSize.Height
                        < innerControl.PreferredSize.Height)
                {
                    //<comment author='b6s'>
                    innerControl.Parent.ClientSize =
                        innerControl.PreferredSize;
                    innerControl.Parent.ClientSize =
                        innerControl.Parent.PreferredSize;
                    // It's a trick to change ClientSize twice.
                    //</comment>
                }
                
                innerControl = innerControl.Parent;
            }
        }

        private void LoadData()
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

        private void SaveData()
        {
            //MessageBox.Show("i=" + m_inputType + ", o=" + m_outputType + ", m_diacriticOption=" + m_diacriticOption + ", m_doNormalize=" + m_doNormalize + ", m_doForcePOJStyle=" + m_doForcePOJStyle);
            m_ovConfDOM.Save(m_ovConfPath);
            this.Close();
        }

        private void m_btSave_Click(object sender, EventArgs e)
        {
            SaveData();
        }
    }
}