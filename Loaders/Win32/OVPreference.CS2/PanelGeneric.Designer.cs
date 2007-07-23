namespace OVPreference.CS2
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
            this.m_pnSelf.Controls.Add(this.m_cbWarningBeep);
            this.m_pnSelf.Controls.Add(this.m_cbHitMaxAndCompose);
            this.m_pnSelf.Controls.Add(this.m_cbAutoCompose);
            this.m_pnSelf.Controls.Add(this.m_cbShiftSelectionKey);
            this.m_pnSelf.Controls.Add(this.m_tbMaxKeySequenceLength);
            this.m_pnSelf.Controls.Add(this.m_lbMaxKeySequenceLength);
            this.m_pnSelf.Location = new System.Drawing.Point(6, 12);
            this.m_pnSelf.Name = "m_pnSelf";
            this.m_pnSelf.Size = new System.Drawing.Size(163, 137);
            this.m_pnSelf.TabIndex = 0;
            // 
            // m_cbWarningBeep
            // 
            this.m_cbWarningBeep.AutoSize = true;
            this.m_cbWarningBeep.Location = new System.Drawing.Point(13, 81);
            this.m_cbWarningBeep.Name = "m_cbWarningBeep";
            this.m_cbWarningBeep.Size = new System.Drawing.Size(94, 17);
            this.m_cbWarningBeep.TabIndex = 5;
            this.m_cbWarningBeep.Text = "Warning Beep";
            this.m_cbWarningBeep.UseVisualStyleBackColor = true;
            this.m_cbWarningBeep.CheckedChanged += new System.EventHandler(this.m_cbWarningBeep_CheckedChanged);
            // 
            // m_cbHitMaxAndCompose
            // 
            this.m_cbHitMaxAndCompose.AutoSize = true;
            this.m_cbHitMaxAndCompose.Location = new System.Drawing.Point(13, 58);
            this.m_cbHitMaxAndCompose.Name = "m_cbHitMaxAndCompose";
            this.m_cbHitMaxAndCompose.Size = new System.Drawing.Size(130, 17);
            this.m_cbHitMaxAndCompose.TabIndex = 4;
            this.m_cbHitMaxAndCompose.Text = "Hit Max and Compose";
            this.m_cbHitMaxAndCompose.UseVisualStyleBackColor = true;
            this.m_cbHitMaxAndCompose.CheckedChanged += new System.EventHandler(this.m_cbHitMaxAndCompose_CheckedChanged);
            // 
            // m_cbAutoCompose
            // 
            this.m_cbAutoCompose.AutoSize = true;
            this.m_cbAutoCompose.Location = new System.Drawing.Point(13, 35);
            this.m_cbAutoCompose.Name = "m_cbAutoCompose";
            this.m_cbAutoCompose.Size = new System.Drawing.Size(95, 17);
            this.m_cbAutoCompose.TabIndex = 3;
            this.m_cbAutoCompose.Text = "Auto Compose";
            this.m_cbAutoCompose.UseVisualStyleBackColor = true;
            this.m_cbAutoCompose.CheckedChanged += new System.EventHandler(this.m_cbAutoCompose_CheckedChanged);
            // 
            // m_cbShiftSelectionKey
            // 
            this.m_cbShiftSelectionKey.AutoSize = true;
            this.m_cbShiftSelectionKey.Location = new System.Drawing.Point(13, 12);
            this.m_cbShiftSelectionKey.Name = "m_cbShiftSelectionKey";
            this.m_cbShiftSelectionKey.Size = new System.Drawing.Size(115, 17);
            this.m_cbShiftSelectionKey.TabIndex = 2;
            this.m_cbShiftSelectionKey.Text = "Shift Selection Key";
            this.m_cbShiftSelectionKey.UseVisualStyleBackColor = true;
            this.m_cbShiftSelectionKey.CheckedChanged += new System.EventHandler(this.m_cbShiftSelectionKey_CheckedChanged);
            // 
            // m_tbMaxKeySequenceLength
            // 
            this.m_tbMaxKeySequenceLength.Location = new System.Drawing.Point(107, 104);
            this.m_tbMaxKeySequenceLength.Name = "m_tbMaxKeySequenceLength";
            this.m_tbMaxKeySequenceLength.Size = new System.Drawing.Size(37, 20);
            this.m_tbMaxKeySequenceLength.TabIndex = 1;
            this.m_tbMaxKeySequenceLength.TextChanged += new System.EventHandler(this.m_tbMaxKeySequenceLength_TextChanged);
            // 
            // m_lbMaxKeySequenceLength
            // 
            this.m_lbMaxKeySequenceLength.AutoSize = true;
            this.m_lbMaxKeySequenceLength.Location = new System.Drawing.Point(10, 107);
            this.m_lbMaxKeySequenceLength.Name = "m_lbMaxKeySequenceLength";
            this.m_lbMaxKeySequenceLength.Size = new System.Drawing.Size(91, 13);
            this.m_lbMaxKeySequenceLength.TabIndex = 0;
            this.m_lbMaxKeySequenceLength.Text = "最大按鍵數目：";
            // 
            // PanelGeneric
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelGeneric";
            this.Size = new System.Drawing.Size(172, 172);
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
