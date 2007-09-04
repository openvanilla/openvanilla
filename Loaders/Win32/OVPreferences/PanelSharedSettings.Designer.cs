namespace OVPreferences
{
    partial class PanelSharedSettings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelSharedSettings));
            this.m_pnSelf = new System.Windows.Forms.Panel();
            this.m_gbPreview = new System.Windows.Forms.GroupBox();
            this.m_cbBackColor = new ColorPicker();
            this.m_lbBackColor = new System.Windows.Forms.Label();
            this.m_cbFontColor = new ColorPicker();
            this.m_lbFontColor = new System.Windows.Forms.Label();
            this.m_lbOpacity = new System.Windows.Forms.Label();
            this.m_tbOpacity = new System.Windows.Forms.TrackBar();
            this.m_cbFontSize = new System.Windows.Forms.ComboBox();
            this.m_lbFont = new System.Windows.Forms.Label();
            this.m_cbFont = new FontComboBox();
            this.m_cdMoreColors = new System.Windows.Forms.ColorDialog();
            this.m_pnSelf.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_tbOpacity)).BeginInit();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.Controls.Add(this.m_gbPreview);
            this.m_pnSelf.Controls.Add(this.m_cbBackColor);
            this.m_pnSelf.Controls.Add(this.m_lbBackColor);
            this.m_pnSelf.Controls.Add(this.m_cbFontColor);
            this.m_pnSelf.Controls.Add(this.m_lbFontColor);
            this.m_pnSelf.Controls.Add(this.m_lbOpacity);
            this.m_pnSelf.Controls.Add(this.m_tbOpacity);
            this.m_pnSelf.Controls.Add(this.m_cbFontSize);
            this.m_pnSelf.Controls.Add(this.m_lbFont);
            this.m_pnSelf.Controls.Add(this.m_cbFont);
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_gbPreview
            // 
            resources.ApplyResources(this.m_gbPreview, "m_gbPreview");
            this.m_gbPreview.Name = "m_gbPreview";
            this.m_gbPreview.TabStop = false;
            // 
            // m_cbBackColor
            // 
            this.m_cbBackColor.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.m_cbBackColor.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbBackColor.FormattingEnabled = true;
            this.m_cbBackColor.Items = null;
            resources.ApplyResources(this.m_cbBackColor, "m_cbBackColor");
            this.m_cbBackColor.Name = "m_cbBackColor";
            this.m_cbBackColor.SelectedIndexChanged += new System.EventHandler(this.bgColorPickerNotiFunction);
            // 
            // m_lbBackColor
            // 
            resources.ApplyResources(this.m_lbBackColor, "m_lbBackColor");
            this.m_lbBackColor.Name = "m_lbBackColor";
            // 
            // m_cbFontColor
            // 
            this.m_cbFontColor.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.m_cbFontColor.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbFontColor.FormattingEnabled = true;
            this.m_cbFontColor.Items = null;
            resources.ApplyResources(this.m_cbFontColor, "m_cbFontColor");
            this.m_cbFontColor.Name = "m_cbFontColor";
            this.m_cbFontColor.SelectedIndexChanged += new System.EventHandler(this.fontColorPickerNotiFunction);
            // 
            // m_lbFontColor
            // 
            resources.ApplyResources(this.m_lbFontColor, "m_lbFontColor");
            this.m_lbFontColor.Name = "m_lbFontColor";
            // 
            // m_lbOpacity
            // 
            resources.ApplyResources(this.m_lbOpacity, "m_lbOpacity");
            this.m_lbOpacity.Name = "m_lbOpacity";
            // 
            // m_tbOpacity
            // 
            resources.ApplyResources(this.m_tbOpacity, "m_tbOpacity");
            this.m_tbOpacity.Name = "m_tbOpacity";
            // 
            // m_cbFontSize
            // 
            this.m_cbFontSize.FormattingEnabled = true;
            this.m_cbFontSize.Items.AddRange(new object[] {
            resources.GetString("m_cbFontSize.Items"),
            resources.GetString("m_cbFontSize.Items1"),
            resources.GetString("m_cbFontSize.Items2"),
            resources.GetString("m_cbFontSize.Items3"),
            resources.GetString("m_cbFontSize.Items4"),
            resources.GetString("m_cbFontSize.Items5"),
            resources.GetString("m_cbFontSize.Items6")});
            resources.ApplyResources(this.m_cbFontSize, "m_cbFontSize");
            this.m_cbFontSize.Name = "m_cbFontSize";
            // 
            // m_lbFont
            // 
            resources.ApplyResources(this.m_lbFont, "m_lbFont");
            this.m_lbFont.Name = "m_lbFont";
            // 
            // m_cbFont
            // 
            this.m_cbFont.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.m_cbFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbFont.FormattingEnabled = true;
            resources.ApplyResources(this.m_cbFont, "m_cbFont");
            this.m_cbFont.Name = "m_cbFont";
            // 
            // PanelSharedSettings
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelSharedSettings";
            this.m_pnSelf.ResumeLayout(false);
            this.m_pnSelf.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_tbOpacity)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private FontComboBox m_cbFont;
        private ColorPicker m_cbFontColor;
        private ColorPicker m_cbBackColor;
        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.ComboBox m_cbFontSize;
        private System.Windows.Forms.Label m_lbFont;
        private System.Windows.Forms.ColorDialog m_cdMoreColors;
        private System.Windows.Forms.Label m_lbOpacity;
        private System.Windows.Forms.TrackBar m_tbOpacity;
        private System.Windows.Forms.Label m_lbFontColor;
        private System.Windows.Forms.Label m_lbBackColor;
        private System.Windows.Forms.GroupBox m_gbPreview;
    }
}
