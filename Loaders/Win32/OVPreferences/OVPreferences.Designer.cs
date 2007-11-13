namespace OVPreferences
{
    partial class OVPreferences
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OVPreferences));
            this.m_btSave = new System.Windows.Forms.Button();
            this.panelModuleList = new System.Windows.Forms.Panel();
            this.panelOptions = new System.Windows.Forms.Panel();
            this.labelModuleList = new System.Windows.Forms.Label();
            this.labelOptions = new System.Windows.Forms.Label();
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
            // OVPreferences
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.labelOptions);
            this.Controls.Add(this.labelModuleList);
            this.Controls.Add(this.panelOptions);
            this.Controls.Add(this.panelModuleList);
            this.Controls.Add(this.m_btSave);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "OVPreferences";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_btSave;
        private System.Windows.Forms.Panel panelModuleList;
        public System.Windows.Forms.Panel panelOptions;
        private System.Windows.Forms.Label labelModuleList;
        private System.Windows.Forms.Label labelOptions;


    }
}

