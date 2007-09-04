namespace OVPreferences
{
    partial class PanelPhonetic
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelPhonetic));
            this.m_pnSelf = new System.Windows.Forms.Panel();
            this.m_lbSelectionKeys = new System.Windows.Forms.Label();
            this.m_tbSelectionKeys = new System.Windows.Forms.TextBox();
            this.m_lbKeyboardLayout = new System.Windows.Forms.Label();
            this.m_cbKeyboardLayout = new System.Windows.Forms.ComboBox();
            this.m_pnSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.Controls.Add(this.m_lbSelectionKeys);
            this.m_pnSelf.Controls.Add(this.m_tbSelectionKeys);
            this.m_pnSelf.Controls.Add(this.m_lbKeyboardLayout);
            this.m_pnSelf.Controls.Add(this.m_cbKeyboardLayout);
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_lbSelectionKeys
            // 
            resources.ApplyResources(this.m_lbSelectionKeys, "m_lbSelectionKeys");
            this.m_lbSelectionKeys.Name = "m_lbSelectionKeys";
            // 
            // m_tbSelectionKeys
            // 
            resources.ApplyResources(this.m_tbSelectionKeys, "m_tbSelectionKeys");
            this.m_tbSelectionKeys.Name = "m_tbSelectionKeys";
            this.m_tbSelectionKeys.TextChanged += new System.EventHandler(this.m_tbSelectionKeys_TextChanged);
            // 
            // m_lbKeyboardLayout
            // 
            resources.ApplyResources(this.m_lbKeyboardLayout, "m_lbKeyboardLayout");
            this.m_lbKeyboardLayout.Name = "m_lbKeyboardLayout";
            // 
            // m_cbKeyboardLayout
            // 
            this.m_cbKeyboardLayout.FormattingEnabled = true;
            this.m_cbKeyboardLayout.Items.AddRange(new object[] {
            resources.GetString("m_cbKeyboardLayout.Items"),
            resources.GetString("m_cbKeyboardLayout.Items1")});
            resources.ApplyResources(this.m_cbKeyboardLayout, "m_cbKeyboardLayout");
            this.m_cbKeyboardLayout.Name = "m_cbKeyboardLayout";
            this.m_cbKeyboardLayout.SelectedIndexChanged += new System.EventHandler(this.m_cbKeyboardLayout_SelectedIndexChanged);
            // 
            // PanelPhonetic
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelPhonetic";
            this.m_pnSelf.ResumeLayout(false);
            this.m_pnSelf.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.Label m_lbKeyboardLayout;
        private System.Windows.Forms.ComboBox m_cbKeyboardLayout;
        private System.Windows.Forms.Label m_lbSelectionKeys;
        private System.Windows.Forms.TextBox m_tbSelectionKeys;
    }
}
