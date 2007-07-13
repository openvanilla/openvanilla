namespace OVPreference.CS2
{
    partial class PanelGeneric
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
            this.plSelf = new System.Windows.Forms.Panel();
            this.lbMaxKeySequenceLength = new System.Windows.Forms.Label();
            this.tbMaxKeySequenceLength = new System.Windows.Forms.TextBox();
            this.cbShiftSelectionKey = new System.Windows.Forms.CheckBox();
            this.cbAutoCompose = new System.Windows.Forms.CheckBox();
            this.cbHitMaxAndCompose = new System.Windows.Forms.CheckBox();
            this.cbWarningBeep = new System.Windows.Forms.CheckBox();
            this.plSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // plSelf
            // 
            this.plSelf.Controls.Add(this.cbWarningBeep);
            this.plSelf.Controls.Add(this.cbHitMaxAndCompose);
            this.plSelf.Controls.Add(this.cbAutoCompose);
            this.plSelf.Controls.Add(this.cbShiftSelectionKey);
            this.plSelf.Controls.Add(this.tbMaxKeySequenceLength);
            this.plSelf.Controls.Add(this.lbMaxKeySequenceLength);
            this.plSelf.Location = new System.Drawing.Point(6, 12);
            this.plSelf.Name = "plSelf";
            this.plSelf.Size = new System.Drawing.Size(160, 148);
            this.plSelf.TabIndex = 0;
            // 
            // lbMaxKeySequenceLength
            // 
            this.lbMaxKeySequenceLength.AutoSize = true;
            this.lbMaxKeySequenceLength.Location = new System.Drawing.Point(13, 123);
            this.lbMaxKeySequenceLength.Name = "lbMaxKeySequenceLength";
            this.lbMaxKeySequenceLength.Size = new System.Drawing.Size(91, 13);
            this.lbMaxKeySequenceLength.TabIndex = 0;
            this.lbMaxKeySequenceLength.Text = "最大按鍵數目：";
            // 
            // tbMaxKeySequenceLength
            // 
            this.tbMaxKeySequenceLength.Location = new System.Drawing.Point(110, 120);
            this.tbMaxKeySequenceLength.Name = "tbMaxKeySequenceLength";
            this.tbMaxKeySequenceLength.Size = new System.Drawing.Size(37, 20);
            this.tbMaxKeySequenceLength.TabIndex = 1;
            this.tbMaxKeySequenceLength.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // cbShiftSelectionKey
            // 
            this.cbShiftSelectionKey.AutoSize = true;
            this.cbShiftSelectionKey.Location = new System.Drawing.Point(16, 8);
            this.cbShiftSelectionKey.Name = "cbShiftSelectionKey";
            this.cbShiftSelectionKey.Size = new System.Drawing.Size(115, 17);
            this.cbShiftSelectionKey.TabIndex = 2;
            this.cbShiftSelectionKey.Text = "Shift Selection Key";
            this.cbShiftSelectionKey.UseVisualStyleBackColor = true;
            this.cbShiftSelectionKey.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // cbAutoCompose
            // 
            this.cbAutoCompose.AutoSize = true;
            this.cbAutoCompose.Location = new System.Drawing.Point(16, 31);
            this.cbAutoCompose.Name = "cbAutoCompose";
            this.cbAutoCompose.Size = new System.Drawing.Size(95, 17);
            this.cbAutoCompose.TabIndex = 3;
            this.cbAutoCompose.Text = "Auto Compose";
            this.cbAutoCompose.UseVisualStyleBackColor = true;
            this.cbAutoCompose.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // cbHitMaxAndCompose
            // 
            this.cbHitMaxAndCompose.AutoSize = true;
            this.cbHitMaxAndCompose.Location = new System.Drawing.Point(16, 54);
            this.cbHitMaxAndCompose.Name = "cbHitMaxAndCompose";
            this.cbHitMaxAndCompose.Size = new System.Drawing.Size(130, 17);
            this.cbHitMaxAndCompose.TabIndex = 4;
            this.cbHitMaxAndCompose.Text = "Hit Max and Compose";
            this.cbHitMaxAndCompose.UseVisualStyleBackColor = true;
            this.cbHitMaxAndCompose.CheckedChanged += new System.EventHandler(this.checkBox3_CheckedChanged);
            // 
            // cbWarningBeep
            // 
            this.cbWarningBeep.AutoSize = true;
            this.cbWarningBeep.Location = new System.Drawing.Point(16, 77);
            this.cbWarningBeep.Name = "cbWarningBeep";
            this.cbWarningBeep.Size = new System.Drawing.Size(94, 17);
            this.cbWarningBeep.TabIndex = 5;
            this.cbWarningBeep.Text = "Warning Beep";
            this.cbWarningBeep.UseVisualStyleBackColor = true;
            this.cbWarningBeep.CheckedChanged += new System.EventHandler(this.checkBox4_CheckedChanged);
            // 
            // PanelGeneric
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.plSelf);
            this.Name = "PanelGeneric";
            this.Size = new System.Drawing.Size(172, 172);
            this.plSelf.ResumeLayout(false);
            this.plSelf.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel plSelf;
        private System.Windows.Forms.CheckBox cbWarningBeep;
        private System.Windows.Forms.CheckBox cbHitMaxAndCompose;
        private System.Windows.Forms.CheckBox cbAutoCompose;
        private System.Windows.Forms.CheckBox cbShiftSelectionKey;
        private System.Windows.Forms.TextBox tbMaxKeySequenceLength;
        private System.Windows.Forms.Label lbMaxKeySequenceLength;
    }
}
