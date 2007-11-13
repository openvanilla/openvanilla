using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Resources;
using System.Globalization;
using System.Threading;

namespace OVPreferences
{
    public partial class OVPreferences : Form
    {
        //@warning OV-specific paths here!
        private static string m_ovPrefWorkingFolder =
            "C:" +
            //Environment.GetEnvironmentVariable("WINDIR") +
            Path.DirectorySeparatorChar + "OpenVanilla" +
            Path.DirectorySeparatorChar;
        private List<OVConfig> m_ovConfList = new List<OVConfig>();
        private static string m_ovConfPath =
            Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData) +
            Path.DirectorySeparatorChar +
            "OpenVanilla" + Path.DirectorySeparatorChar +
            "config.xml";
        private XmlDocument m_ovConfDom = new XmlDocument();

        //ResourceManager resourceMgr = Resource_zh_TW.ResourceManager;
        
        ResourceManager resourceMgr = 
            ResourceManager.CreateFileBasedResourceManager(
                "Resource",
                m_ovPrefWorkingFolder + "Resources",
                null);
        
        //CultureInfo ci =  Resource_ch_TW.Culture;// new CultureInfo("ch-TW");
        
//        CultureInfo ci = Thread.CurrentThread.CurrentUICulture;
        CultureInfo ci;

        public OVPreferences()
        {

            //Runtime Culture
            //The main thread・s UICulture must be set to the appropriate value prior to 
            //initializing form and control properties with a ResourceManager instance. 
            //Typically you・ll see this handled in the main form・s constructor.
            //However, it is better initialize the thread・s UICulture and Culture 
            //when the thread is first created, and decouple it from a specific form・s code.
            //System.Threading.Thread.CurrentThread.CurrentUICulture = 
            //    new System.Globalization.CultureInfo(Properties.Settings.Default.UICulture);

            //System.Threading.Thread.CurrentThread.CurrentCulture =
            //System.Globalization.CultureInfo.CreateSpecificCulture
            //    (Properties.Settings.Default.Culture);

            //Thread.CurrentThread.CurrentUICulture = new CultureInfo("zh-TW");
            //Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-US");
            ci = Thread.CurrentThread.CurrentUICulture;
            
            InitializeComponent();

            LoadData();

            SortModules();

            SetUI();
        }

        private void SortModules()
        {
            ComparerOVConfig comparer = new ComparerOVConfig();
            comparer.SortBy = EnumSortBy.Priority;
            m_ovConfList.Sort(comparer);
            //<comment author='b6s'>
            // The following code is useless
            // since C# does not have a stable sort.
            //comparer.SortBy = EnumSortBy.Enabled;
            //m_ovConfList.Sort(comparer);
            //</commnet>
        }

        private void SetUI()        
        {
            //@warning Shared Settings are not ready yet.
            //AddPanelSharedSettings();
            PanelModuleList pnModuleList = new PanelModuleList(m_ovConfDom, panelOptions, labelOptions);
            foreach (OVConfig conf in m_ovConfList)
            {
                bool isEnabled = false;
                if (conf.settings.ContainsKey("enable"))
                    isEnabled =
                        Convert.ToBoolean(
                            Convert.ToInt32(
                                conf.settings["enable"]));
                pnModuleList.AddModule(conf.moduleName, isEnabled);

                if (conf.moduleName.StartsWith("OVIMGeneric"))
                {
                    AddPanelGeneric(conf, m_ovConfDom);
                }
                if (conf.moduleName.Equals("OVIMPhonetic"))
                {
                    AddPanelPhonetic(conf, m_ovConfDom);
                }
                if (conf.moduleName.Equals("OVIMPOJ-Holo"))
                {
                    AddPanelPOJ(conf, m_ovConfDom);
                }
                if (conf.moduleName.Equals("OVIMTibetan"))
                {
                    AddPanelTibetan(conf, m_ovConfDom);
                }
                if (conf.moduleName.StartsWith("OVIMTobacco"))
                {
                    AddPanelTobacco(conf, m_ovConfDom);
                }
                if (conf.moduleName.Equals("TLIM"))
                {
                    AddPanelTLIM(conf, m_ovConfDom);
                }
            }
            if (m_ovConfList.Count > 1)
            {
                pnModuleList.Height = this.panelModuleList.Size.Height;
                pnModuleList.Width = this.panelModuleList.Size.Width;
                pnModuleList.start();
                this.panelModuleList.Controls.Add(pnModuleList); 
            }
        }

        protected void AddPanelGeneric(OVConfig conf, XmlDocument confDom)
        {
            PanelGeneric pnGeneric = new PanelGeneric();
            pnGeneric.Init(conf, confDom);
            string msg = resourceMgr.GetString("OVIMGeneric", ci);
            string inputMethod = conf.moduleName.Replace("OVIMGeneric-", "");
            inputMethod = inputMethod.Replace(".cin", "");
            AddPanel(conf, pnGeneric, msg + " (" + inputMethod + ")");
        }

        protected void AddPanelPhonetic(OVConfig conf, XmlDocument confDom)
        {
            PanelPhonetic pnPhonetic = new PanelPhonetic();
            pnPhonetic.Init(conf, confDom);
            string msg = resourceMgr.GetString("OVIMPhonetic", ci);
            //AddPanel(pnPhonetic, conf.moduleName);
            AddPanel(conf,pnPhonetic, msg);
        }

        protected void AddPanelPOJ(OVConfig conf, XmlDocument confDom)
        {
            PanelPOJ pnPOJ = new PanelPOJ();
            pnPOJ.Init(conf, confDom);
            AddPanel(conf,pnPOJ, conf.moduleName);
        }

        protected void AddPanelTibetan(OVConfig conf, XmlDocument confDom)
        {
            PanelTibetan pnTibetan = new PanelTibetan();
            pnTibetan.Init(conf, confDom);
            AddPanel(conf,pnTibetan, conf.moduleName);
        }

        protected void AddPanelTobacco(OVConfig conf, XmlDocument confDom)
        {
            PanelTobacco pnTobacco = new PanelTobacco();
            pnTobacco.Init(conf, confDom);
            string msg = resourceMgr.GetString(conf.moduleName, ci);
            AddPanel(conf,pnTobacco, msg);
        }

        protected void AddPanelTLIM(OVConfig conf, XmlDocument confDom)
        {
            PanelTLIM pnTLIM = new PanelTLIM(conf, confDom);
            AddPanel(conf,pnTLIM, conf.moduleName);
        }

        private void AddPanel(OVConfig conf,Control panel, string name)
        {
            Panel tp = new Panel();
            tp.Name = conf.moduleName;
            tp.Controls.Add(panel);
            tp.Visible = false;
            this.panelOptions.Controls.Add(tp);
            this.SetSize(panel);
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
            m_ovConfDom.Load(m_ovConfPath);
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
            m_ovConfDom.Save(m_ovConfPath);
            this.Close();
        }

        private void m_btSave_Click(object sender, EventArgs e)
        {
            SaveData();
        }
    }
}