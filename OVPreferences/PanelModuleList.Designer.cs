using System.Globalization;
namespace OVPreferences
{
    partial class PanelModuleList
    {

        public void AddModule(OVConfig conf, bool isChecked, CultureInfo ci)
        {
            if (ci.Name == "zh-CN")
                this.m_clModuleList.Items.Add(conf.zh_CNName, isChecked);
            else if (ci.Name.StartsWith("zh"))
                this.m_clModuleList.Items.Add(conf.zh_TWName, isChecked);
            else
                this.m_clModuleList.Items.Add(conf.enName, isChecked);

            


            if (conf.moduleType == "OVInputMethod") imCount++;
            //System.Diagnostics.Debug.WriteLine("imCount:" + imCount.ToString());
            if (conf.moduleType == "OVKeyPreprocessor") kpCount++;
            //System.Diagnostics.Debug.WriteLine("kpCount:" + kpCount.ToString());
            if (conf.moduleType == "OVOutputFilter") ofCount++;
            //System.Diagnostics.Debug.WriteLine("ofCount:" + ofCount.ToString());
                  
            
        }

        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer m_components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (m_components != null))
            {
                m_components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelModuleList));
            this.m_pnSelf = new System.Windows.Forms.Panel();
            this.m_clModuleList = new System.Windows.Forms.CheckedListBox();
            this.m_pnSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.Controls.Add(this.m_clModuleList);
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_clModuleList
            // 
            this.m_clModuleList.AllowDrop = true;
            this.m_clModuleList.CheckOnClick = false;
            resources.ApplyResources(this.m_clModuleList, "m_clModuleList");
            this.m_clModuleList.FormattingEnabled = true;
            this.m_clModuleList.Name = "m_clModuleList";
            this.m_clModuleList.MouseUp += new System.Windows.Forms.MouseEventHandler(this.m_clModuleList_MouseUp);
            this.m_clModuleList.DragOver += new System.Windows.Forms.DragEventHandler(this.m_clModuleList_DragOver);
            this.m_clModuleList.SelectedIndexChanged += new System.EventHandler(this.m_clModuleList_SelectedIndexChanged);
            this.m_clModuleList.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.m_clModuleList_ItemCheck);
            this.m_clModuleList.DragDrop += new System.Windows.Forms.DragEventHandler(this.m_clModuleList_DragDrop);
            this.m_clModuleList.MouseMove += new System.Windows.Forms.MouseEventHandler(this.m_clModuleList_MouseMove);
            this.m_clModuleList.MouseDown += new System.Windows.Forms.MouseEventHandler(this.m_clModuleList_MouseDown);
            this.m_clModuleList.KeyUp += new System.Windows.Forms.KeyEventHandler(m_clModuleList_KeyUp);
            // 
            // PanelModuleList
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelModuleList";
            this.m_pnSelf.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        

        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.CheckedListBox m_clModuleList;
    }
}
