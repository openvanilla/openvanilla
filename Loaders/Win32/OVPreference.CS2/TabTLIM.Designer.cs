namespace OVPreference.CS2
{
    partial class TabTLIM
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
            this.tabPageTLIM = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.forcePOJStyleButton = new System.Windows.Forms.CheckBox();
            this.normalizeButton = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.diacriticEndButton = new System.Windows.Forms.RadioButton();
            this.diacriticFrontButton = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.outputPOJButton = new System.Windows.Forms.RadioButton();
            this.outputTLButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.inputDTButton = new System.Windows.Forms.RadioButton();
            this.inputTLPAButton = new System.Windows.Forms.RadioButton();
            this.inputPOJButton = new System.Windows.Forms.RadioButton();
            this.inputTLButton = new System.Windows.Forms.RadioButton();
            this.tabPageTLIM.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabPageTLIM
            // 
            this.tabPageTLIM.Controls.Add(this.groupBox4);
            this.tabPageTLIM.Controls.Add(this.groupBox3);
            this.tabPageTLIM.Controls.Add(this.groupBox2);
            this.tabPageTLIM.Controls.Add(this.groupBox1);
            this.tabPageTLIM.Location = new System.Drawing.Point(4, 22);
            this.tabPageTLIM.Name = "tabPageTLIM";
            this.tabPageTLIM.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTLIM.Size = new System.Drawing.Size(269, 301);
            this.tabPageTLIM.TabIndex = 1;
            this.tabPageTLIM.Text = "台羅輸入法";
            this.tabPageTLIM.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.forcePOJStyleButton);
            this.groupBox4.Controls.Add(this.normalizeButton);
            this.groupBox4.Location = new System.Drawing.Point(8, 232);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(260, 78);
            this.groupBox4.TabIndex = 15;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "調號與母音的輸入順序";
            // 
            // forcePOJStyleButton
            // 
            this.forcePOJStyleButton.AutoSize = true;
            this.forcePOJStyleButton.Location = new System.Drawing.Point(7, 47);
            this.forcePOJStyleButton.Name = "forcePOJStyleButton";
            this.forcePOJStyleButton.Size = new System.Drawing.Size(250, 17);
            this.forcePOJStyleButton.TabIndex = 3;
            this.forcePOJStyleButton.Text = "台羅輸出時傳統的（POJ式的）ou和nn符號";
            this.forcePOJStyleButton.UseVisualStyleBackColor = true;
            // 
            // normalizeButton
            // 
            this.normalizeButton.AutoSize = true;
            this.normalizeButton.Location = new System.Drawing.Point(7, 23);
            this.normalizeButton.Name = "normalizeButton";
            this.normalizeButton.Size = new System.Drawing.Size(158, 17);
            this.normalizeButton.TabIndex = 2;
            this.normalizeButton.Text = "送字前音節做正規化檢查";
            this.normalizeButton.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.diacriticEndButton);
            this.groupBox3.Controls.Add(this.diacriticFrontButton);
            this.groupBox3.Location = new System.Drawing.Point(8, 144);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(260, 78);
            this.groupBox3.TabIndex = 14;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "調號與母音的輸入順序";
            // 
            // diacriticEndButton
            // 
            this.diacriticEndButton.AutoSize = true;
            this.diacriticEndButton.Location = new System.Drawing.Point(6, 48);
            this.diacriticEndButton.Name = "diacriticEndButton";
            this.diacriticEndButton.Size = new System.Drawing.Size(167, 17);
            this.diacriticEndButton.TabIndex = 1;
            this.diacriticEndButton.TabStop = true;
            this.diacriticEndButton.Text = "先打母音再打調號 ( \' ` ^ = | )";
            this.diacriticEndButton.UseVisualStyleBackColor = true;
            // 
            // diacriticFrontButton
            // 
            this.diacriticFrontButton.AutoSize = true;
            this.diacriticFrontButton.Location = new System.Drawing.Point(7, 24);
            this.diacriticFrontButton.Name = "diacriticFrontButton";
            this.diacriticFrontButton.Size = new System.Drawing.Size(170, 17);
            this.diacriticFrontButton.TabIndex = 0;
            this.diacriticFrontButton.TabStop = true;
            this.diacriticFrontButton.Text = "先打調號 ( \' ` ^ = | ) 再打母音";
            this.diacriticFrontButton.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.outputPOJButton);
            this.groupBox2.Controls.Add(this.outputTLButton);
            this.groupBox2.Location = new System.Drawing.Point(141, 6);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(127, 131);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "輸出的音節種類";
            // 
            // outputPOJButton
            // 
            this.outputPOJButton.AutoSize = true;
            this.outputPOJButton.Location = new System.Drawing.Point(6, 80);
            this.outputPOJButton.Name = "outputPOJButton";
            this.outputPOJButton.Size = new System.Drawing.Size(105, 17);
            this.outputPOJButton.TabIndex = 13;
            this.outputPOJButton.TabStop = true;
            this.outputPOJButton.Text = "白話字（POJ）";
            this.outputPOJButton.UseVisualStyleBackColor = true;
            // 
            // outputTLButton
            // 
            this.outputTLButton.AutoSize = true;
            this.outputTLButton.Location = new System.Drawing.Point(6, 35);
            this.outputTLButton.Name = "outputTLButton";
            this.outputTLButton.Size = new System.Drawing.Size(86, 17);
            this.outputTLButton.TabIndex = 12;
            this.outputTLButton.TabStop = true;
            this.outputTLButton.Text = "台羅（TL）";
            this.outputTLButton.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.inputDTButton);
            this.groupBox1.Controls.Add(this.inputTLPAButton);
            this.groupBox1.Controls.Add(this.inputPOJButton);
            this.groupBox1.Controls.Add(this.inputTLButton);
            this.groupBox1.Location = new System.Drawing.Point(8, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(127, 131);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "輸入的音節種類";
            // 
            // inputDTButton
            // 
            this.inputDTButton.AutoSize = true;
            this.inputDTButton.Location = new System.Drawing.Point(6, 94);
            this.inputDTButton.Name = "inputDTButton";
            this.inputDTButton.Size = new System.Drawing.Size(49, 17);
            this.inputDTButton.TabIndex = 9;
            this.inputDTButton.TabStop = true;
            this.inputDTButton.Text = "通用";
            this.inputDTButton.UseVisualStyleBackColor = true;
            // 
            // inputTLPAButton
            // 
            this.inputTLPAButton.AutoSize = true;
            this.inputTLPAButton.Location = new System.Drawing.Point(6, 70);
            this.inputTLPAButton.Name = "inputTLPAButton";
            this.inputTLPAButton.Size = new System.Drawing.Size(52, 17);
            this.inputTLPAButton.TabIndex = 8;
            this.inputTLPAButton.TabStop = true;
            this.inputTLPAButton.Text = "TLPA";
            this.inputTLPAButton.UseVisualStyleBackColor = true;
            // 
            // inputPOJButton
            // 
            this.inputPOJButton.AutoSize = true;
            this.inputPOJButton.Location = new System.Drawing.Point(6, 47);
            this.inputPOJButton.Name = "inputPOJButton";
            this.inputPOJButton.Size = new System.Drawing.Size(105, 17);
            this.inputPOJButton.TabIndex = 7;
            this.inputPOJButton.TabStop = true;
            this.inputPOJButton.Text = "白話字（POJ）";
            this.inputPOJButton.UseVisualStyleBackColor = true;
            // 
            // inputTLButton
            // 
            this.inputTLButton.AutoSize = true;
            this.inputTLButton.Location = new System.Drawing.Point(6, 23);
            this.inputTLButton.Name = "inputTLButton";
            this.inputTLButton.Size = new System.Drawing.Size(86, 17);
            this.inputTLButton.TabIndex = 6;
            this.inputTLButton.TabStop = true;
            this.inputTLButton.Text = "台羅（TL）";
            this.inputTLButton.UseVisualStyleBackColor = true;
            // 
            // TabTLIM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "TabTLIM";
            this.tabPageTLIM.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.TabPage tabPageTLIM;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox forcePOJStyleButton;
        private System.Windows.Forms.CheckBox normalizeButton;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton diacriticEndButton;
        private System.Windows.Forms.RadioButton diacriticFrontButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton outputPOJButton;
        private System.Windows.Forms.RadioButton outputTLButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton inputDTButton;
        private System.Windows.Forms.RadioButton inputTLPAButton;
        private System.Windows.Forms.RadioButton inputPOJButton;
        private System.Windows.Forms.RadioButton inputTLButton;
    }
}
