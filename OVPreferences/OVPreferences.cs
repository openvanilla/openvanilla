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
using System.Runtime.InteropServices;


namespace OVPreferences
{
    public partial class OVPreferencesForm : Form
    {
        
        //@warning OV-specific paths here!
        private static string m_ovPrefWorkingFolder =  Directory.GetCurrentDirectory()+
            //"C:" +
            //Environment.GetEnvironmentVariable("WINDIR") +
            //Path.DirectorySeparatorChar + "OpenVanilla" +
            Path.DirectorySeparatorChar;
       
        
        private List<OVConfig> m_ovConfList = new List<OVConfig>();

        private PanelModuleList pnModuleList;
        
        private PanelSharedSettings pnShareSettings;

        private int OVIMEUIHwnd=0;

        private static string m_ovConfPath =
            Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData) +
            Path.DirectorySeparatorChar +
            "OpenVanilla" + Path.DirectorySeparatorChar +
            "config.xml";
        private XmlDocument m_ovConfDom = new XmlDocument();

        private ResourceManager resourceMgr = new ResourceManager("OVPreferences.Properties.Resource",
            System.Reflection.Assembly.GetExecutingAssembly());
        
                
        //ResourceManager resourceMgr = Resource_zh_TW.ResourceManager;
                
        //ResourceManager resourceMgr =  ResourceManager.CreateFileBasedResourceManager( 
        //        "Resource", m_ovPrefWorkingFolder + "Resources",  null);
        
        //CultureInfo ci =  Resource_ch_TW.Culture;// new CultureInfo("ch-TW");
        
//        CultureInfo ci = Thread.CurrentThread.CurrentUICulture;
        CultureInfo ci;
        

        public OVPreferencesForm()
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
            System.Diagnostics.Debug.WriteLine("locale name:" + ci.Name);
            
            InitializeComponent();

            OVIMEUIHwnd = UtilFuncs.FindWindow("OVIMEUI", null);
            if (0 != OVIMEUIHwnd ) 
            {   // add tabPageOFModulelist to tabcontrol if any OVIMEUI instance was found
               // this.tabControl.Controls.Add(this.tabPageOFModulelist);
            }

            LoadData();

            SortModules();

            SetUI();

            //MessageBox.Show("OVIMEUI handle:" + Win32API.Win32.FindWindow("OVIMEUI", null));
                      
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
            AddPanelSharedSettings();


            pnModuleList = new PanelModuleList(m_ovConfDom,m_ovConfList, panelOptions, labelOptions);
            
            foreach (OVConfig conf in m_ovConfList)
            {
                bool isEnabled = false;
                if (conf.settings.ContainsKey("enable"))
                    isEnabled = 
                        Convert.ToBoolean(
                            Convert.ToInt32(
                                conf.settings["enable"]));
              
                

                if (conf.moduleName.StartsWith("OVIMGeneric"))
                {
                    AddPanelGeneric(conf, m_ovConfDom);
                }
                if (conf.moduleName.StartsWith("OVIMDayi"))
                {
                    AddPanelDayi(conf, m_ovConfDom);
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

                pnModuleList.AddModule(conf, isEnabled, ci);

            }
            if (m_ovConfList.Count > 1)
            {
                
                pnModuleList.start();
                this.panelModuleList.Controls.Add(pnModuleList);
                pnModuleList.Dock = DockStyle.Fill;
                //pnModuleList.Height = this.panelModuleList.Size.Height;
                //pnModuleList.Width = this.panelModuleList.Size.Width;
                //ofModuleList.Height = this.panelOFModulelist.Size.Height;
                //ofModuleList.Width = this.panelOFModulelist.Size.Width;
                //ofModuleList.start();
                //this.panelOFModulelist.Controls.Add(ofModuleList);
            }
            labelModuleList.Text = resourceMgr.GetString("TabModuleList", ci);
            labelOFModuleList.Text = resourceMgr.GetString("OFModuleList", ci);
        }

        protected void AddPanelGeneric(OVConfig conf, XmlDocument confDom)
        {
            PanelGeneric pnGeneric = new PanelGeneric();
            pnGeneric.Init(conf, confDom);
            string msg = resourceMgr.GetString("OVIMGeneric", ci);
            if(ci.Name=="zh-CN")
                conf.zh_CNName = msg + "-" + conf.zh_TWName;
            else if(ci.Name.StartsWith("zh"))
                conf.zh_TWName = msg + "-" + conf.zh_TWName;
            else 
                conf.enName = msg + "-"+conf.enName;

            string inputMethod = conf.moduleName.Replace("OVIMGeneric-", "");
            inputMethod = inputMethod.Replace(".cin", "");
            AddPanel(conf, pnGeneric, msg + " (" + inputMethod + ")");
        }

        protected void AddPanelDayi(OVConfig conf, XmlDocument confDom)
        {
            PanelGeneric pnGeneric = new PanelGeneric();
            pnGeneric.Init(conf, confDom);
            string msg = resourceMgr.GetString("OVIMDayi", ci);
            //string inputMethod = conf.moduleName.Replace("OVIMGeneric-", "");
            //inputMethod = inputMethod.Replace(".cin", "");
            AddPanel(conf, pnGeneric, msg);
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
            string msg = resourceMgr.GetString("OVIMTobacco", ci);//conf.moduleName, ci);
            if(ci.Name=="zh-CN")
                conf.zh_CNName = msg + "-" + conf.zh_TWName;
            else if(ci.Name.StartsWith("zh"))
                conf.zh_TWName = msg + "-" + conf.zh_TWName;
            else 
                conf.enName = msg + "-"+conf.enName;
            
            AddPanel(conf,pnTobacco, msg);
        }

        protected void AddPanelTLIM(OVConfig conf, XmlDocument confDom)
        {
            PanelTLIM pnTLIM = new PanelTLIM(conf, confDom);
            AddPanel(conf,pnTLIM, conf.moduleName);
        }

        private void AddPanel(OVConfig conf,Control panel, string name)
        {
            //Panel tp = new Panel();
           // tp.Name = conf.moduleName;
            
            //tp.Controls.Add(panel);
            //tp.Visible = false;
            //tp.BorderStyle = BorderStyle.Fixed3D;
            panel.Name = conf.moduleName;
                        
            this.panelOptions.Controls.Add(panel);

            //tp.Dock = DockStyle.Fill;
            panel.Dock = DockStyle.Fill;

            this.SetSize(panel);
            
            
        }

        private void AddPanelSharedSettings()
        {
            pnShareSettings = new PanelSharedSettings();
            this.panelShareSettings.Controls.Add(pnShareSettings);
            pnShareSettings.Init(m_ovConfDom);
            pnShareSettings.OpacityChanged += new PanelSharedSettings.OpacityChangedHandler(pnShareSettings_OpacityChanged);
            pnShareSettings.LoadSettings();

        }

        void pnShareSettings_OpacityChanged()
        {
            //throw new Exception("The method or operation is not implemented.");
            //System.Diagnostics.Debug.WriteLine(this.panelSharedSettings.m_opacity.ToString());
            this.Opacity = pnShareSettings.m_opacity;
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
            /*
            System.Diagnostics.Debug.WriteLine("Innercontrolt:" + innerControl.ToString());
            System.Diagnostics.Debug.WriteLine("PreferredWidth:" + innerControl.PreferredSize.Width.ToString());
            System.Diagnostics.Debug.WriteLine("PreferredHeight:" + innerControl.PreferredSize.Height.ToString());
            System.Diagnostics.Debug.WriteLine("ClientsizeWidth:" + innerControl.ClientSize.Width.ToString());
            System.Diagnostics.Debug.WriteLine("ClientsizeHeight:" + innerControl.ClientSize.Height.ToString());
            */
            while (innerControl.Parent != null)
            {
              //  System.Diagnostics.Debug.WriteLine("Innercontrol.parent:" + innerControl.Parent.ToString());
              //  System.Diagnostics.Debug.WriteLine("PrefferedWidth:" + innerControl.Parent.PreferredSize.Width.ToString());
              //  System.Diagnostics.Debug.WriteLine("PrefferedHeight:" + innerControl.Parent.PreferredSize.Height.ToString());
                
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
              //  System.Diagnostics.Debug.WriteLine("ClientsizeWidth:" + innerControl.Parent.ClientSize.Width.ToString());
              //  System.Diagnostics.Debug.WriteLine("ClientsizeHeight:" + innerControl.Parent.ClientSize.Height.ToString());
                
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
                                if (attrNameTemp.Equals("moduleType"))
                                {
                                    ovConfSet.moduleType = ovConfReader.Value;
                                    System.Diagnostics.Debug.WriteLine("moduleType:" + ovConfReader.Value);
                                }
                                
                                else if(attrNameTemp.Equals("LocalizedName-zh_TW"))
                                {
                                    ovConfSet.zh_TWName= ovConfReader.Value;
                                    System.Diagnostics.Debug.WriteLine("LocalizedName-zh_TW:"+ovConfReader.Value);
                                }
                                else if (attrNameTemp.Equals("LocalizedName-zh_CN"))
                                {
                                    ovConfSet.zh_CNName = ovConfReader.Value;
                                    System.Diagnostics.Debug.WriteLine("LocalizedName-zh_CN:" + ovConfReader.Value);
                                }
                                else if (attrNameTemp.Equals("LocalizedName-en"))
                                {
                                    ovConfSet.enName = ovConfReader.Value;
                                    System.Diagnostics.Debug.WriteLine("LocalizedName-en:" + ovConfReader.Value);
                                }
                                else
                                {
                                    ovConfSet.settings.Add(
                                        attrNameTemp, ovConfReader.Value);
                                    System.Diagnostics.Debug.WriteLine("Attribute:"+attrNameTemp + ":=" + ovConfReader.Value);
                                    attrNameTemp = "";

                                }
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
            pnShareSettings.SaveSettings();
            SaveData();
            
        }
       

        void tabPageModuleSettings_Enter(object sender, System.EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Moudlelist page entered.");
            pnModuleList.start();
            panelModuleList.Focus();
        }

        void tabPageOFModulelist_Enter(object sender, System.EventArgs e)
        {
            /*
            int ret = UtilFuncs.SendMessage(
                   new IntPtr((long) OVIMEUIHwnd),
                   (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                   0xE, //IMN_PRIVATE
                   10);
             */

        }

    private const int WM_COPYDATA = 0x4A;

    // Window proc that receives the WM_ messages for the associated window handle
    protected override void WndProc(ref Message m)
    {
    switch(m.Msg)
        {
            case WM_COPYDATA:
            COPYDATASTRUCT CD =
            (COPYDATASTRUCT)m.GetLParam(typeof(COPYDATASTRUCT));
            byte[] B = new byte[CD.cbData];
            IntPtr lpData = new IntPtr(CD.lpData);
            Marshal.Copy(lpData, B, 0, CD.cbData);
            string strData = Encoding.Default.GetString(B);
            //form.OpenGamls(new string[] {strData});
            break;
            
            default:
            // let the base class deal with it
            base.WndProc(ref m);
            break;
        }
        }

        

        
    }



}

