namespace OVPreference.CS2
{
    partial class PanelTLIM
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
            this.pnSelf = new System.Windows.Forms.Panel();

            this.gbSpellCheck = new System.Windows.Forms.GroupBox();
            this.cbForcePOJStyle = new System.Windows.Forms.CheckBox();
            this.cbNormalize = new System.Windows.Forms.CheckBox();

            this.gbTypingOrder = new System.Windows.Forms.GroupBox();
            this.rbDiacriticEnd = new System.Windows.Forms.RadioButton();
            this.rbDiacriticFront = new System.Windows.Forms.RadioButton();

            this.gbSyllableTypeOutput = new System.Windows.Forms.GroupBox();
            this.rbOutputPOJ = new System.Windows.Forms.RadioButton();
            this.rbOutputTL = new System.Windows.Forms.RadioButton();

            this.gbSyllableTypeInput = new System.Windows.Forms.GroupBox();
            this.rbInputDT = new System.Windows.Forms.RadioButton();
            this.rbInputTLPA = new System.Windows.Forms.RadioButton();
            this.rbInputPOJ = new System.Windows.Forms.RadioButton();
            this.rbInputTL = new System.Windows.Forms.RadioButton();


            this.pnSelf.SuspendLayout();
            this.gbSpellCheck.SuspendLayout();
            this.gbTypingOrder.SuspendLayout();
            this.gbSyllableTypeOutput.SuspendLayout();
            this.gbSyllableTypeInput.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelSelf
            // 
            this.pnSelf.Controls.Add(this.gbSpellCheck);
            this.pnSelf.Controls.Add(this.gbTypingOrder);
            this.pnSelf.Controls.Add(this.gbSyllableTypeOutput);
            this.pnSelf.Controls.Add(this.gbSyllableTypeInput);
            this.pnSelf.Location = new System.Drawing.Point(3, 0);
            this.pnSelf.Name = "panelSelf";
            this.pnSelf.Padding = new System.Windows.Forms.Padding(3);
            this.pnSelf.Size = new System.Drawing.Size(272, 308);
            this.pnSelf.TabIndex = 1;
            this.pnSelf.Text = "台羅輸入法";
            // 
            // groupBoxSpellCheck
            // 
            this.gbSpellCheck.Controls.Add(this.cbForcePOJStyle);
            this.gbSpellCheck.Controls.Add(this.cbNormalize);
            this.gbSpellCheck.Location = new System.Drawing.Point(8, 234);
            this.gbSpellCheck.Name = "groupBox4";
            this.gbSpellCheck.Size = new System.Drawing.Size(258, 65);
            this.gbSpellCheck.TabIndex = 15;
            this.gbSpellCheck.TabStop = false;
            this.gbSpellCheck.Text = "拼寫與調符自動檢查";
            // 
            // forcePOJStyleButton
            // 
            this.cbForcePOJStyle.AutoSize = true;
            this.cbForcePOJStyle.Location = new System.Drawing.Point(7, 47);
            this.cbForcePOJStyle.Name = "forcePOJStyleButton";
            this.cbForcePOJStyle.Size = new System.Drawing.Size(250, 17);
            this.cbForcePOJStyle.TabIndex = 3;
            this.cbForcePOJStyle.Text = "台羅輸出時傳統的（POJ式的）ou和nn符號";
            this.cbForcePOJStyle.UseVisualStyleBackColor = true;
            // 
            // normalizeButton
            // 
            this.cbNormalize.AutoSize = true;
            this.cbNormalize.Location = new System.Drawing.Point(7, 23);
            this.cbNormalize.Name = "normalizeButton";
            this.cbNormalize.Size = new System.Drawing.Size(158, 17);
            this.cbNormalize.TabIndex = 2;
            this.cbNormalize.Text = "送字前音節做正規化檢查";
            this.cbNormalize.UseVisualStyleBackColor = true;
            // 
            // groupBoxTypingOrder
            // 
            this.gbTypingOrder.Controls.Add(this.rbDiacriticEnd);
            this.gbTypingOrder.Controls.Add(this.rbDiacriticFront);
            this.gbTypingOrder.Location = new System.Drawing.Point(8, 144);
            this.gbTypingOrder.Name = "groupBox3";
            this.gbTypingOrder.Size = new System.Drawing.Size(260, 78);
            this.gbTypingOrder.TabIndex = 14;
            this.gbTypingOrder.TabStop = false;
            this.gbTypingOrder.Text = "調號與母音的輸入順序";
            // 
            // diacriticEndButton
            // 
            this.rbDiacriticEnd.AutoSize = true;
            this.rbDiacriticEnd.Location = new System.Drawing.Point(6, 48);
            this.rbDiacriticEnd.Name = "diacriticEndButton";
            this.rbDiacriticEnd.Size = new System.Drawing.Size(167, 17);
            this.rbDiacriticEnd.TabIndex = 1;
            this.rbDiacriticEnd.TabStop = true;
            this.rbDiacriticEnd.Text = "先打母音再打調號 ( \' ` ^ = | )";
            this.rbDiacriticEnd.UseVisualStyleBackColor = true;
            // 
            // diacriticFrontButton
            // 
            this.rbDiacriticFront.AutoSize = true;
            this.rbDiacriticFront.Location = new System.Drawing.Point(7, 24);
            this.rbDiacriticFront.Name = "diacriticFrontButton";
            this.rbDiacriticFront.Size = new System.Drawing.Size(170, 17);
            this.rbDiacriticFront.TabIndex = 0;
            this.rbDiacriticFront.TabStop = true;
            this.rbDiacriticFront.Text = "先打調號 ( \' ` ^ = | ) 再打母音";
            this.rbDiacriticFront.UseVisualStyleBackColor = true;
            // 
            // groupBoxSyllableTypeOutput
            // 
            this.gbSyllableTypeOutput.Controls.Add(this.rbOutputPOJ);
            this.gbSyllableTypeOutput.Controls.Add(this.rbOutputTL);
            this.gbSyllableTypeOutput.Location = new System.Drawing.Point(141, 6);
            this.gbSyllableTypeOutput.Name = "groupBox2";
            this.gbSyllableTypeOutput.Size = new System.Drawing.Size(127, 131);
            this.gbSyllableTypeOutput.TabIndex = 13;
            this.gbSyllableTypeOutput.TabStop = false;
            this.gbSyllableTypeOutput.Text = "輸出的音節種類";
            // 
            // outputPOJButton
            // 
            this.rbOutputPOJ.AutoSize = true;
            this.rbOutputPOJ.Location = new System.Drawing.Point(6, 80);
            this.rbOutputPOJ.Name = "outputPOJButton";
            this.rbOutputPOJ.Size = new System.Drawing.Size(105, 17);
            this.rbOutputPOJ.TabIndex = 13;
            this.rbOutputPOJ.TabStop = true;
            this.rbOutputPOJ.Text = "白話字（POJ）";
            this.rbOutputPOJ.UseVisualStyleBackColor = true;
            // 
            // outputTLButton
            // 
            this.rbOutputTL.AutoSize = true;
            this.rbOutputTL.Location = new System.Drawing.Point(6, 35);
            this.rbOutputTL.Name = "outputTLButton";
            this.rbOutputTL.Size = new System.Drawing.Size(86, 17);
            this.rbOutputTL.TabIndex = 12;
            this.rbOutputTL.TabStop = true;
            this.rbOutputTL.Text = "台羅（TL）";
            this.rbOutputTL.UseVisualStyleBackColor = true;
            // 
            // groupBoxSyllableTypeInput
            // 
            this.gbSyllableTypeInput.Controls.Add(this.rbInputDT);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputTLPA);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputPOJ);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputTL);
            this.gbSyllableTypeInput.Location = new System.Drawing.Point(8, 6);
            this.gbSyllableTypeInput.Name = "groupBox1";
            this.gbSyllableTypeInput.Size = new System.Drawing.Size(127, 131);
            this.gbSyllableTypeInput.TabIndex = 12;
            this.gbSyllableTypeInput.TabStop = false;
            this.gbSyllableTypeInput.Text = "輸入的音節種類";
            // 
            // inputDTButton
            // 
            this.rbInputDT.AutoSize = true;
            this.rbInputDT.Location = new System.Drawing.Point(6, 94);
            this.rbInputDT.Name = "inputDTButton";
            this.rbInputDT.Size = new System.Drawing.Size(49, 17);
            this.rbInputDT.TabIndex = 9;
            this.rbInputDT.TabStop = true;
            this.rbInputDT.Text = "通用";
            this.rbInputDT.UseVisualStyleBackColor = true;
            // 
            // inputTLPAButton
            // 
            this.rbInputTLPA.AutoSize = true;
            this.rbInputTLPA.Location = new System.Drawing.Point(6, 70);
            this.rbInputTLPA.Name = "inputTLPAButton";
            this.rbInputTLPA.Size = new System.Drawing.Size(52, 17);
            this.rbInputTLPA.TabIndex = 8;
            this.rbInputTLPA.TabStop = true;
            this.rbInputTLPA.Text = "TLPA";
            this.rbInputTLPA.UseVisualStyleBackColor = true;
            // 
            // inputPOJButton
            // 
            this.rbInputPOJ.AutoSize = true;
            this.rbInputPOJ.Location = new System.Drawing.Point(6, 47);
            this.rbInputPOJ.Name = "inputPOJButton";
            this.rbInputPOJ.Size = new System.Drawing.Size(105, 17);
            this.rbInputPOJ.TabIndex = 7;
            this.rbInputPOJ.TabStop = true;
            this.rbInputPOJ.Text = "白話字（POJ）";
            this.rbInputPOJ.UseVisualStyleBackColor = true;
            // 
            // inputTLButton
            // 
            this.rbInputTL.AutoSize = true;
            this.rbInputTL.Location = new System.Drawing.Point(6, 23);
            this.rbInputTL.Name = "inputTLButton";
            this.rbInputTL.Size = new System.Drawing.Size(86, 17);
            this.rbInputTL.TabIndex = 6;
            this.rbInputTL.TabStop = true;
            this.rbInputTL.Text = "台羅（TL）";
            this.rbInputTL.UseVisualStyleBackColor = true;
            // 
            // PanelTLIM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pnSelf);
            this.Name = "PanelTLIM";
            this.Size = new System.Drawing.Size(279, 312);
            this.pnSelf.ResumeLayout(false);
            this.gbSpellCheck.ResumeLayout(false);
            this.gbSpellCheck.PerformLayout();
            this.gbTypingOrder.ResumeLayout(false);
            this.gbTypingOrder.PerformLayout();
            this.gbSyllableTypeOutput.ResumeLayout(false);
            this.gbSyllableTypeOutput.PerformLayout();
            this.gbSyllableTypeInput.ResumeLayout(false);
            this.gbSyllableTypeInput.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnSelf;
        private System.Windows.Forms.GroupBox gbSpellCheck;
        private System.Windows.Forms.CheckBox cbForcePOJStyle;
        private System.Windows.Forms.CheckBox cbNormalize;
        private System.Windows.Forms.GroupBox gbTypingOrder;
        private System.Windows.Forms.RadioButton rbDiacriticEnd;
        private System.Windows.Forms.RadioButton rbDiacriticFront;
        private System.Windows.Forms.GroupBox gbSyllableTypeOutput;
        private System.Windows.Forms.RadioButton rbOutputPOJ;
        private System.Windows.Forms.RadioButton rbOutputTL;
        private System.Windows.Forms.GroupBox gbSyllableTypeInput;
        private System.Windows.Forms.RadioButton rbInputDT;
        private System.Windows.Forms.RadioButton rbInputTLPA;
        private System.Windows.Forms.RadioButton rbInputPOJ;
        private System.Windows.Forms.RadioButton rbInputTL;
    }
}
