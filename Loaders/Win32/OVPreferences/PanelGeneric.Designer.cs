namespace OVPreferences
{
    partial class PanelGeneric
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelGeneric));
            this.m_pnSelf = new System.Windows.Forms.Panel();
            this.m_cbWarningBeep = new System.Windows.Forms.CheckBox();
            this.m_cbHitMaxAndCompose = new System.Windows.Forms.CheckBox();
            this.m_cbAutoCompose = new System.Windows.Forms.CheckBox();
            this.m_cbShiftSelectionKey = new System.Windows.Forms.CheckBox();
            this.m_tbMaxKeySequenceLength = new System.Windows.Forms.TextBox();
            this.m_lbMaxKeySequenceLength = new System.Windows.Forms.Label();
            this.m_pnSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.AccessibleDescription = null;
            this.m_pnSelf.AccessibleName = null;
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.BackgroundImage = null;
            this.m_pnSelf.Controls.Add(this.m_cbWarningBeep);
            this.m_pnSelf.Controls.Add(this.m_cbHitMaxAndCompose);
            this.m_pnSelf.Controls.Add(this.m_cbAutoCompose);
            this.m_pnSelf.Controls.Add(this.m_cbShiftSelectionKey);
            this.m_pnSelf.Controls.Add(this.m_tbMaxKeySequenceLength);
            this.m_pnSelf.Controls.Add(this.m_lbMaxKeySequenceLength);
            this.m_pnSelf.Font = null;
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_cbWarningBeep
            // 
            this.m_cbWarningBeep.AccessibleDescription = null;
            this.m_cbWarningBeep.AccessibleName = null;
            resources.ApplyResources(this.m_cbWarningBeep, "m_cbWarningBeep");
            this.m_cbWarningBeep.BackgroundImage = null;
            this.m_cbWarningBeep.Font = null;
            this.m_cbWarningBeep.Name = "m_cbWarningBeep";
            this.m_cbWarningBeep.UseVisualStyleBackColor = true;
            this.m_cbWarningBeep.CheckedChanged += new System.EventHandler(this.m_cbWarningBeep_CheckedChanged);
            // 
            // m_cbHitMaxAndCompose
            // 
            this.m_cbHitMaxAndCompose.AccessibleDescription = null;
            this.m_cbHitMaxAndCompose.AccessibleName = null;
            resources.ApplyResources(this.m_cbHitMaxAndCompose, "m_cbHitMaxAndCompose");
            this.m_cbHitMaxAndCompose.BackgroundImage = null;
            this.m_cbHitMaxAndCompose.Font = null;
            this.m_cbHitMaxAndCompose.Name = "m_cbHitMaxAndCompose";
            this.m_cbHitMaxAndCompose.UseVisualStyleBackColor = true;
            this.m_cbHitMaxAndCompose.CheckedChanged += new System.EventHandler(this.m_cbHitMaxAndCompose_CheckedChanged);
            // 
            // m_cbAutoCompose
            // 
            this.m_cbAutoCompose.AccessibleDescription = null;
            this.m_cbAutoCompose.AccessibleName = null;
            resources.ApplyResources(this.m_cbAutoCompose, "m_cbAutoCompose");
            this.m_cbAutoCompose.BackgroundImage = null;
            this.m_cbAutoCompose.Font = null;
            this.m_cbAutoCompose.Name = "m_cbAutoCompose";
            this.m_cbAutoCompose.UseVisualStyleBackColor = true;
            this.m_cbAutoCompose.CheckedChanged += new System.EventHandler(this.m_cbAutoCompose_CheckedChanged);
            // 
            // m_cbShiftSelectionKey
            // 
            this.m_cbShiftSelectionKey.AccessibleDescription = null;
            this.m_cbShiftSelectionKey.AccessibleName = null;
            resources.ApplyResources(this.m_cbShiftSelectionKey, "m_cbShiftSelectionKey");
            this.m_cbShiftSelectionKey.BackgroundImage = null;
            this.m_cbShiftSelectionKey.Font = null;
            this.m_cbShiftSelectionKey.Name = "m_cbShiftSelectionKey";
            this.m_cbShiftSelectionKey.UseVisualStyleBackColor = true;
            this.m_cbShiftSelectionKey.CheckedChanged += new System.EventHandler(this.m_cbShiftSelectionKey_CheckedChanged);
            // 
            // m_tbMaxKeySequenceLength
            // 
            this.m_tbMaxKeySequenceLength.AccessibleDescription = null;
            this.m_tbMaxKeySequenceLength.AccessibleName = null;
            resources.ApplyResources(this.m_tbMaxKeySequenceLength, "m_tbMaxKeySequenceLength");
            this.m_tbMaxKeySequenceLength.BackgroundImage = null;
            this.m_tbMaxKeySequenceLength.Font = null;
            this.m_tbMaxKeySequenceLength.Name = "m_tbMaxKeySequenceLength";
            this.m_tbMaxKeySequenceLength.TextChanged += new System.EventHandler(this.m_tbMaxKeySequenceLength_TextChanged);
            // 
            // m_lbMaxKeySequenceLength
            // 
            this.m_lbMaxKeySequenceLength.AccessibleDescription = null;
            this.m_lbMaxKeySequenceLength.AccessibleName = null;
            resources.ApplyResources(this.m_lbMaxKeySequenceLength, "m_lbMaxKeySequenceLength");
            this.m_lbMaxKeySequenceLength.Font = null;
            this.m_lbMaxKeySequenceLength.Name = "m_lbMaxKeySequenceLength";
            // 
            // PanelGeneric
            // 
            this.AccessibleDescription = null;
            this.AccessibleName = null;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = null;
            this.Controls.Add(this.m_pnSelf);
            this.Font = null;
            this.Name = "PanelGeneric";
            this.m_pnSelf.ResumeLayout(false);
            this.m_pnSelf.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.CheckBox m_cbWarningBeep;
        private System.Windows.Forms.CheckBox m_cbHitMaxAndCompose;
        private System.Windows.Forms.CheckBox m_cbAutoCompose;
        private System.Windows.Forms.CheckBox m_cbShiftSelectionKey;
        private System.Windows.Forms.TextBox m_tbMaxKeySequenceLength;
        private System.Windows.Forms.Label m_lbMaxKeySequenceLength;
    }
}
