namespace OVPreferences
{
    partial class OVPreferencesForm
    {
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OVPreferencesForm));
            this.m_btSave = new System.Windows.Forms.Button();
            this.panelModuleList = new System.Windows.Forms.Panel();
            this.panelOptions = new System.Windows.Forms.Panel();
            this.labelModuleList = new System.Windows.Forms.Label();
            this.labelOptions = new System.Windows.Forms.Label();
            this.panelOFModulelist = new System.Windows.Forms.Panel();
            this.labelOFModuleList = new System.Windows.Forms.Label();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPageShareSettings = new System.Windows.Forms.TabPage();
            this.tabPageModuleSettings = new System.Windows.Forms.TabPage();
            this.tabPageOFModulelist = new System.Windows.Forms.TabPage();
            this.panelShareSettings = new System.Windows.Forms.Panel();
            this.tabControl.SuspendLayout();
            this.tabPageShareSettings.SuspendLayout();
            this.tabPageModuleSettings.SuspendLayout();
            this.tabPageOFModulelist.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_btSave
            // 
            resources.ApplyResources(this.m_btSave, "m_btSave");
            this.m_btSave.Name = "m_btSave";
            this.m_btSave.UseVisualStyleBackColor = true;
            this.m_btSave.Click += new System.EventHandler(this.m_btSave_Click);
            // 
            // panelModuleList
            // 
            resources.ApplyResources(this.panelModuleList, "panelModuleList");
            this.panelModuleList.Name = "panelModuleList";
            // 
            // panelOptions
            // 
            resources.ApplyResources(this.panelOptions, "panelOptions");
            this.panelOptions.Name = "panelOptions";
            // 
            // labelModuleList
            // 
            resources.ApplyResources(this.labelModuleList, "labelModuleList");
            this.labelModuleList.Name = "labelModuleList";
            // 
            // labelOptions
            // 
            resources.ApplyResources(this.labelOptions, "labelOptions");
            this.labelOptions.Name = "labelOptions";
            // 
            // panelOFModulelist
            // 
            resources.ApplyResources(this.panelOFModulelist, "panelOFModulelist");
            this.panelOFModulelist.Name = "panelOFModulelist";
            // 
            // labelOFModuleList
            // 
            resources.ApplyResources(this.labelOFModuleList, "labelOFModuleList");
            this.labelOFModuleList.Name = "labelOFModuleList";
            // 
            // tabControl
            // 
            resources.ApplyResources(this.tabControl, "tabControl");
            this.tabControl.Controls.Add(this.tabPageShareSettings);
            this.tabControl.Controls.Add(this.tabPageModuleSettings);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            // 
            // tabPageShareSettings
            // 
            this.tabPageShareSettings.BackColor = System.Drawing.Color.Transparent;
            this.tabPageShareSettings.Controls.Add(this.panelShareSettings);
            resources.ApplyResources(this.tabPageShareSettings, "tabPageShareSettings");
            this.tabPageShareSettings.Name = "tabPageShareSettings";
            this.tabPageShareSettings.UseVisualStyleBackColor = true;
            // 
            // tabPageModuleSettings
            // 
            this.tabPageModuleSettings.BackColor = System.Drawing.Color.Transparent;
            this.tabPageModuleSettings.Controls.Add(this.labelModuleList);
            this.tabPageModuleSettings.Controls.Add(this.panelOptions);
            this.tabPageModuleSettings.Controls.Add(this.labelOptions);
            this.tabPageModuleSettings.Controls.Add(this.panelModuleList);
            resources.ApplyResources(this.tabPageModuleSettings, "tabPageModuleSettings");
            this.tabPageModuleSettings.Name = "tabPageModuleSettings";
            this.tabPageModuleSettings.UseVisualStyleBackColor = true;
            this.tabPageModuleSettings.Enter += new System.EventHandler(this.tabPageModuleSettings_Enter);
            // 
            // tabPageOFModulelist
            // 
            this.tabPageOFModulelist.BackColor = System.Drawing.Color.Transparent;
            this.tabPageOFModulelist.Controls.Add(this.panelOFModulelist);
            this.tabPageOFModulelist.Controls.Add(this.labelOFModuleList);
            resources.ApplyResources(this.tabPageOFModulelist, "tabPageOFModulelist");
            this.tabPageOFModulelist.Name = "tabPageOFModulelist";
            this.tabPageOFModulelist.UseVisualStyleBackColor = true;
            this.tabPageOFModulelist.Enter += new System.EventHandler(this.tabPageOFModulelist_Enter);
            // 
            // panelShareSettings
            // 
            resources.ApplyResources(this.panelShareSettings, "panelShareSettings");
            this.panelShareSettings.Name = "panelShareSettings";
            // 
            // OVPreferencesForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.m_btSave);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "OVPreferencesForm";
            this.tabControl.ResumeLayout(false);
            this.tabPageShareSettings.ResumeLayout(false);
            this.tabPageModuleSettings.ResumeLayout(false);
            this.tabPageModuleSettings.PerformLayout();
            this.tabPageOFModulelist.ResumeLayout(false);
            this.tabPageOFModulelist.PerformLayout();
            this.ResumeLayout(false);

        }

        

        

        

        
        #endregion

        private System.Windows.Forms.Button m_btSave;
        private System.Windows.Forms.Panel panelModuleList;
        public System.Windows.Forms.Panel panelOptions;
        private System.Windows.Forms.Label labelModuleList;
        private System.Windows.Forms.Label labelOptions;
        private System.Windows.Forms.Panel panelOFModulelist;
        private System.Windows.Forms.Label labelOFModuleList;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPageShareSettings;
        private System.Windows.Forms.TabPage tabPageModuleSettings;
        private System.Windows.Forms.TabPage tabPageOFModulelist;
        private System.Windows.Forms.Panel panelShareSettings;


    }
}

