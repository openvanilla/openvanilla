

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
            this.m_lbOpacity = new System.Windows.Forms.Label();
            this.m_gpCandi = new System.Windows.Forms.GroupBox();
            this.m_cbCandiFontSize = new System.Windows.Forms.ComboBox();
            this.m_lbCandiBackColor = new System.Windows.Forms.Label();
            this.m_lbCandiFont = new System.Windows.Forms.Label();
            this.m_lbCandiFontColor = new System.Windows.Forms.Label();
            this.m_gpNotify = new System.Windows.Forms.GroupBox();
            this.m_cbFontSize = new System.Windows.Forms.ComboBox();
            this.m_lbBackColor = new System.Windows.Forms.Label();
            this.m_lbFont = new System.Windows.Forms.Label();
            this.m_lbFontColor = new System.Windows.Forms.Label();
            this.m_lbptOpacity = new System.Windows.Forms.Label();
            this.m_gbPreview = new System.Windows.Forms.GroupBox();
            this.panelPreview = new System.Windows.Forms.Panel();
            this.m_lbCandiPreview = new System.Windows.Forms.Label();
            this.m_lbNotifyPreview = new System.Windows.Forms.Label();
            this.m_tbOpacity = new System.Windows.Forms.TrackBar();
            this.m_cdMoreColors = new System.Windows.Forms.ColorDialog();
            this.m_cbEnableNotify = new System.Windows.Forms.CheckBox();
            this.m_cbCandiFont = new OVPreferences.FontComboBox();
            this.m_cbCandiBackColor = new OVPreferences.ColorButton();
            this.m_cbCandiFontColor = new OVPreferences.ColorButton();
            this.m_cbFont = new OVPreferences.FontComboBox();
            this.m_cbBackColor = new OVPreferences.ColorButton();
            this.m_cbFontColor = new OVPreferences.ColorButton();
            this.m_pnSelf.SuspendLayout();
            this.m_gpCandi.SuspendLayout();
            this.m_gpNotify.SuspendLayout();
            this.m_gbPreview.SuspendLayout();
            this.panelPreview.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_tbOpacity)).BeginInit();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.Controls.Add(this.m_lbOpacity);
            this.m_pnSelf.Controls.Add(this.m_gpCandi);
            this.m_pnSelf.Controls.Add(this.m_gpNotify);
            this.m_pnSelf.Controls.Add(this.m_lbptOpacity);
            this.m_pnSelf.Controls.Add(this.m_gbPreview);
            this.m_pnSelf.Controls.Add(this.m_tbOpacity);
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_lbOpacity
            // 
            resources.ApplyResources(this.m_lbOpacity, "m_lbOpacity");
            this.m_lbOpacity.Name = "m_lbOpacity";
            // 
            // m_gpCandi
            // 
            this.m_gpCandi.Controls.Add(this.m_cbCandiFont);
            this.m_gpCandi.Controls.Add(this.m_cbCandiBackColor);
            this.m_gpCandi.Controls.Add(this.m_cbCandiFontSize);
            this.m_gpCandi.Controls.Add(this.m_lbCandiBackColor);
            this.m_gpCandi.Controls.Add(this.m_cbCandiFontColor);
            this.m_gpCandi.Controls.Add(this.m_lbCandiFont);
            this.m_gpCandi.Controls.Add(this.m_lbCandiFontColor);
            resources.ApplyResources(this.m_gpCandi, "m_gpCandi");
            this.m_gpCandi.Name = "m_gpCandi";
            this.m_gpCandi.TabStop = false;
            // 
            // m_cbCandiFontSize
            // 
            this.m_cbCandiFontSize.FormattingEnabled = true;
            this.m_cbCandiFontSize.Items.AddRange(new object[] {
            resources.GetString("m_cbCandiFontSize.Items"),
            resources.GetString("m_cbCandiFontSize.Items1"),
            resources.GetString("m_cbCandiFontSize.Items2"),
            resources.GetString("m_cbCandiFontSize.Items3"),
            resources.GetString("m_cbCandiFontSize.Items4"),
            resources.GetString("m_cbCandiFontSize.Items5"),
            resources.GetString("m_cbCandiFontSize.Items6"),
            resources.GetString("m_cbCandiFontSize.Items7"),
            resources.GetString("m_cbCandiFontSize.Items8")});
            resources.ApplyResources(this.m_cbCandiFontSize, "m_cbCandiFontSize");
            this.m_cbCandiFontSize.Name = "m_cbCandiFontSize";
            this.m_cbCandiFontSize.SelectedIndexChanged += new System.EventHandler(this.m_cbCandiFontSize_SelectedIndexChanged);
            // 
            // m_lbCandiBackColor
            // 
            resources.ApplyResources(this.m_lbCandiBackColor, "m_lbCandiBackColor");
            this.m_lbCandiBackColor.Name = "m_lbCandiBackColor";
            // 
            // m_lbCandiFont
            // 
            resources.ApplyResources(this.m_lbCandiFont, "m_lbCandiFont");
            this.m_lbCandiFont.Name = "m_lbCandiFont";
            // 
            // m_lbCandiFontColor
            // 
            resources.ApplyResources(this.m_lbCandiFontColor, "m_lbCandiFontColor");
            this.m_lbCandiFontColor.Name = "m_lbCandiFontColor";
            // 
            // m_gpNotify
            // 
            this.m_gpNotify.Controls.Add(this.m_cbEnableNotify);
            this.m_gpNotify.Controls.Add(this.m_cbFont);
            this.m_gpNotify.Controls.Add(this.m_cbBackColor);
            this.m_gpNotify.Controls.Add(this.m_cbFontSize);
            this.m_gpNotify.Controls.Add(this.m_lbBackColor);
            this.m_gpNotify.Controls.Add(this.m_cbFontColor);
            this.m_gpNotify.Controls.Add(this.m_lbFont);
            this.m_gpNotify.Controls.Add(this.m_lbFontColor);
            resources.ApplyResources(this.m_gpNotify, "m_gpNotify");
            this.m_gpNotify.Name = "m_gpNotify";
            this.m_gpNotify.TabStop = false;
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
            resources.GetString("m_cbFontSize.Items6"),
            resources.GetString("m_cbFontSize.Items7"),
            resources.GetString("m_cbFontSize.Items8")});
            resources.ApplyResources(this.m_cbFontSize, "m_cbFontSize");
            this.m_cbFontSize.Name = "m_cbFontSize";
            this.m_cbFontSize.SelectedIndexChanged += new System.EventHandler(this.m_cbFontSize_SelectedIndexChanged);
            // 
            // m_lbBackColor
            // 
            resources.ApplyResources(this.m_lbBackColor, "m_lbBackColor");
            this.m_lbBackColor.Name = "m_lbBackColor";
            // 
            // m_lbFont
            // 
            resources.ApplyResources(this.m_lbFont, "m_lbFont");
            this.m_lbFont.Name = "m_lbFont";
            // 
            // m_lbFontColor
            // 
            resources.ApplyResources(this.m_lbFontColor, "m_lbFontColor");
            this.m_lbFontColor.Name = "m_lbFontColor";
            // 
            // m_lbptOpacity
            // 
            resources.ApplyResources(this.m_lbptOpacity, "m_lbptOpacity");
            this.m_lbptOpacity.Name = "m_lbptOpacity";
            // 
            // m_gbPreview
            // 
            this.m_gbPreview.BackColor = System.Drawing.SystemColors.Control;
            this.m_gbPreview.Controls.Add(this.panelPreview);
            resources.ApplyResources(this.m_gbPreview, "m_gbPreview");
            this.m_gbPreview.Name = "m_gbPreview";
            this.m_gbPreview.TabStop = false;
            // 
            // panelPreview
            // 
            this.panelPreview.BackColor = System.Drawing.SystemColors.Window;
            this.panelPreview.Controls.Add(this.m_lbCandiPreview);
            this.panelPreview.Controls.Add(this.m_lbNotifyPreview);
            resources.ApplyResources(this.panelPreview, "panelPreview");
            this.panelPreview.Name = "panelPreview";
            // 
            // m_lbCandiPreview
            // 
            resources.ApplyResources(this.m_lbCandiPreview, "m_lbCandiPreview");
            this.m_lbCandiPreview.Name = "m_lbCandiPreview";
            // 
            // m_lbNotifyPreview
            // 
            resources.ApplyResources(this.m_lbNotifyPreview, "m_lbNotifyPreview");
            this.m_lbNotifyPreview.Name = "m_lbNotifyPreview";
            // 
            // m_tbOpacity
            // 
            resources.ApplyResources(this.m_tbOpacity, "m_tbOpacity");
            this.m_tbOpacity.Maximum = 20;
            this.m_tbOpacity.Name = "m_tbOpacity";
            this.m_tbOpacity.Scroll += new System.EventHandler(this.m_tbOpacity_Scroll);
            // 
            // m_cbEnableNotify
            // 
            resources.ApplyResources(this.m_cbEnableNotify, "m_cbEnableNotify");
            this.m_cbEnableNotify.Name = "m_cbEnableNotify";
            this.m_cbEnableNotify.UseVisualStyleBackColor = true;
            
            // 
            // m_cbCandiFont
            // 
            this.m_cbCandiFont.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.m_cbCandiFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbCandiFont.FormattingEnabled = true;
            resources.ApplyResources(this.m_cbCandiFont, "m_cbCandiFont");
            this.m_cbCandiFont.Name = "m_cbCandiFont";
            this.m_cbCandiFont.SelectedIndexChanged += new System.EventHandler(this.m_cbCandiFont_SelectedIndexChanged);
            // 
            // m_cbCandiBackColor
            // 
            resources.ApplyResources(this.m_cbCandiBackColor, "m_cbCandiBackColor");
            this.m_cbCandiBackColor.Name = "m_cbCandiBackColor";
            this.m_cbCandiBackColor.Click += new System.EventHandler(this.m_cbCandiBackColor_Click);
            // 
            // m_cbCandiFontColor
            // 
            resources.ApplyResources(this.m_cbCandiFontColor, "m_cbCandiFontColor");
            this.m_cbCandiFontColor.Name = "m_cbCandiFontColor";
            this.m_cbCandiFontColor.Click += new System.EventHandler(this.m_cbCandiFontColor_Click);
            // 
            // m_cbFont
            // 
            this.m_cbFont.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.m_cbFont.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbFont.FormattingEnabled = true;
            resources.ApplyResources(this.m_cbFont, "m_cbFont");
            this.m_cbFont.Name = "m_cbFont";
            this.m_cbFont.SelectedIndexChanged += new System.EventHandler(this.m_cbFont_SelectedIndexChanged);
            // 
            // m_cbBackColor
            // 
            resources.ApplyResources(this.m_cbBackColor, "m_cbBackColor");
            this.m_cbBackColor.Name = "m_cbBackColor";
            this.m_cbBackColor.Click += new System.EventHandler(this.m_cbBackColor_Click);
            // 
            // m_cbFontColor
            // 
            resources.ApplyResources(this.m_cbFontColor, "m_cbFontColor");
            this.m_cbFontColor.Name = "m_cbFontColor";
            this.m_cbFontColor.Click += new System.EventHandler(this.m_cbFontColor_Click);
            // 
            // PanelSharedSettings
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelSharedSettings";
            this.m_pnSelf.ResumeLayout(false);
            this.m_pnSelf.PerformLayout();
            this.m_gpCandi.ResumeLayout(false);
            this.m_gpNotify.ResumeLayout(false);
            this.m_gpNotify.PerformLayout();
            this.m_gbPreview.ResumeLayout(false);
            this.panelPreview.ResumeLayout(false);
            this.panelPreview.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_tbOpacity)).EndInit();
            this.ResumeLayout(false);

        }


    
        
        #endregion
        private FontComboBox m_cbFont;
        private OVPreferences.ColorButton m_cbFontColor;
        private OVPreferences.ColorButton m_cbBackColor;
        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.ComboBox m_cbFontSize;
        private System.Windows.Forms.Label m_lbFont;
        private System.Windows.Forms.ColorDialog m_cdMoreColors;
        private System.Windows.Forms.TrackBar m_tbOpacity;
        private System.Windows.Forms.Label m_lbFontColor;
        private System.Windows.Forms.Label m_lbBackColor;
        private System.Windows.Forms.GroupBox m_gbPreview;
        private System.Windows.Forms.Label m_lbptOpacity;
        private System.Windows.Forms.GroupBox m_gpNotify;
        private System.Windows.Forms.Panel panelPreview;
        private System.Windows.Forms.GroupBox m_gpCandi;
        private FontComboBox m_cbCandiFont;
        private ColorButton m_cbCandiBackColor;
        private System.Windows.Forms.ComboBox m_cbCandiFontSize;
        private System.Windows.Forms.Label m_lbCandiBackColor;
        private ColorButton m_cbCandiFontColor;
        private System.Windows.Forms.Label m_lbCandiFont;
        private System.Windows.Forms.Label m_lbCandiFontColor;
        private System.Windows.Forms.Label m_lbOpacity;
        private System.Windows.Forms.Label m_lbNotifyPreview;
        private System.Windows.Forms.Label m_lbCandiPreview;
        private System.Windows.Forms.CheckBox m_cbEnableNotify;
        
    }
}
