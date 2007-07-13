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
            this.lbDiacritics = new System.Windows.Forms.Label();
            this.lbToneMark = new System.Windows.Forms.Label();
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
            // pnSelf
            // 
            this.pnSelf.AutoSize = true;
            this.pnSelf.Controls.Add(this.gbSpellCheck);
            this.pnSelf.Controls.Add(this.gbTypingOrder);
            this.pnSelf.Controls.Add(this.gbSyllableTypeOutput);
            this.pnSelf.Controls.Add(this.gbSyllableTypeInput);
            this.pnSelf.Location = new System.Drawing.Point(15, 10);
            this.pnSelf.Name = "pnSelf";
            this.pnSelf.Padding = new System.Windows.Forms.Padding(3);
            this.pnSelf.Size = new System.Drawing.Size(257, 438);
            this.pnSelf.TabIndex = 1;
            this.pnSelf.Text = "台羅輸入法";
            // 
            // gbSpellCheck
            // 
            this.gbSpellCheck.Controls.Add(this.lbDiacritics);
            this.gbSpellCheck.Controls.Add(this.lbToneMark);
            this.gbSpellCheck.Controls.Add(this.cbForcePOJStyle);
            this.gbSpellCheck.Controls.Add(this.cbNormalize);
            this.gbSpellCheck.Location = new System.Drawing.Point(8, 297);
            this.gbSpellCheck.Name = "gbSpellCheck";
            this.gbSpellCheck.Size = new System.Drawing.Size(242, 133);
            this.gbSpellCheck.TabIndex = 11;
            this.gbSpellCheck.TabStop = false;
            this.gbSpellCheck.Text = "拼寫與調符自動檢查";
            // 
            // lbDiacritics
            // 
            this.lbDiacritics.Location = new System.Drawing.Point(24, 97);
            this.lbDiacritics.Name = "lbDiacritics";
            this.lbDiacritics.Size = new System.Drawing.Size(212, 33);
            this.lbDiacritics.TabIndex = 5;
            this.lbDiacritics.Text = "使用右上方有一點的o來表示oo，以及用上標n來表示nn";
            // 
            // lbToneMark
            // 
            this.lbToneMark.Location = new System.Drawing.Point(24, 39);
            this.lbToneMark.Name = "lbToneMark";
            this.lbToneMark.Size = new System.Drawing.Size(212, 35);
            this.lbToneMark.TabIndex = 4;
            this.lbToneMark.Text = "依照台羅的標調規則，將調號標在正確的母音上。";
            // 
            // cbForcePOJStyle
            // 
            this.cbForcePOJStyle.AutoSize = true;
            this.cbForcePOJStyle.Location = new System.Drawing.Point(7, 77);
            this.cbForcePOJStyle.Name = "cbForcePOJStyle";
            this.cbForcePOJStyle.Size = new System.Drawing.Size(206, 17);
            this.cbForcePOJStyle.TabIndex = 3;
            this.cbForcePOJStyle.Text = "使用台羅傳統版的符號來輸出台羅";
            this.cbForcePOJStyle.UseVisualStyleBackColor = true;
            // 
            // cbNormalize
            // 
            this.cbNormalize.AutoSize = true;
            this.cbNormalize.Location = new System.Drawing.Point(7, 19);
            this.cbNormalize.Name = "cbNormalize";
            this.cbNormalize.Size = new System.Drawing.Size(134, 17);
            this.cbNormalize.TabIndex = 2;
            this.cbNormalize.Text = "輸入後做正規化檢查";
            this.cbNormalize.UseVisualStyleBackColor = true;
            // 
            // gbTypingOrder
            // 
            this.gbTypingOrder.Controls.Add(this.rbDiacriticEnd);
            this.gbTypingOrder.Controls.Add(this.rbDiacriticFront);
            this.gbTypingOrder.Location = new System.Drawing.Point(8, 210);
            this.gbTypingOrder.Name = "gbTypingOrder";
            this.gbTypingOrder.Size = new System.Drawing.Size(242, 71);
            this.gbTypingOrder.TabIndex = 14;
            this.gbTypingOrder.TabStop = false;
            this.gbTypingOrder.Text = "調號與母音的輸入順序";
            // 
            // rbDiacriticEnd
            // 
            this.rbDiacriticEnd.AutoSize = true;
            this.rbDiacriticEnd.Location = new System.Drawing.Point(7, 42);
            this.rbDiacriticEnd.Name = "rbDiacriticEnd";
            this.rbDiacriticEnd.Size = new System.Drawing.Size(167, 17);
            this.rbDiacriticEnd.TabIndex = 1;
            this.rbDiacriticEnd.TabStop = true;
            this.rbDiacriticEnd.Text = "先打母音再打調號 ( \' ` ^ = | )";
            this.rbDiacriticEnd.UseVisualStyleBackColor = true;
            // 
            // rbDiacriticFront
            // 
            this.rbDiacriticFront.AutoSize = true;
            this.rbDiacriticFront.Location = new System.Drawing.Point(7, 19);
            this.rbDiacriticFront.Name = "rbDiacriticFront";
            this.rbDiacriticFront.Size = new System.Drawing.Size(170, 17);
            this.rbDiacriticFront.TabIndex = 0;
            this.rbDiacriticFront.TabStop = true;
            this.rbDiacriticFront.Text = "先打調號 ( \' ` ^ = | ) 再打母音";
            this.rbDiacriticFront.UseVisualStyleBackColor = true;
            // 
            // gbSyllableTypeOutput
            // 
            this.gbSyllableTypeOutput.Controls.Add(this.rbOutputPOJ);
            this.gbSyllableTypeOutput.Controls.Add(this.rbOutputTL);
            this.gbSyllableTypeOutput.Location = new System.Drawing.Point(8, 128);
            this.gbSyllableTypeOutput.Name = "gbSyllableTypeOutput";
            this.gbSyllableTypeOutput.Size = new System.Drawing.Size(242, 63);
            this.gbSyllableTypeOutput.TabIndex = 13;
            this.gbSyllableTypeOutput.TabStop = false;
            this.gbSyllableTypeOutput.Text = "輸出的音節種類";
            // 
            // rbOutputPOJ
            // 
            this.rbOutputPOJ.AutoSize = true;
            this.rbOutputPOJ.Location = new System.Drawing.Point(7, 42);
            this.rbOutputPOJ.Name = "rbOutputPOJ";
            this.rbOutputPOJ.Size = new System.Drawing.Size(105, 17);
            this.rbOutputPOJ.TabIndex = 13;
            this.rbOutputPOJ.TabStop = true;
            this.rbOutputPOJ.Text = "白話字（POJ）";
            this.rbOutputPOJ.UseVisualStyleBackColor = true;
            // 
            // rbOutputTL
            // 
            this.rbOutputTL.AutoSize = true;
            this.rbOutputTL.Location = new System.Drawing.Point(7, 19);
            this.rbOutputTL.Name = "rbOutputTL";
            this.rbOutputTL.Size = new System.Drawing.Size(86, 17);
            this.rbOutputTL.TabIndex = 12;
            this.rbOutputTL.TabStop = true;
            this.rbOutputTL.Text = "台羅（TL）";
            this.rbOutputTL.UseVisualStyleBackColor = true;
            // 
            // gbSyllableTypeInput
            // 
            this.gbSyllableTypeInput.Controls.Add(this.rbInputDT);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputTLPA);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputPOJ);
            this.gbSyllableTypeInput.Controls.Add(this.rbInputTL);
            this.gbSyllableTypeInput.Location = new System.Drawing.Point(8, 6);
            this.gbSyllableTypeInput.Name = "gbSyllableTypeInput";
            this.gbSyllableTypeInput.Size = new System.Drawing.Size(242, 107);
            this.gbSyllableTypeInput.TabIndex = 12;
            this.gbSyllableTypeInput.TabStop = false;
            this.gbSyllableTypeInput.Text = "輸入的音節種類";
            // 
            // rbInputDT
            // 
            this.rbInputDT.AutoSize = true;
            this.rbInputDT.Location = new System.Drawing.Point(7, 88);
            this.rbInputDT.Name = "rbInputDT";
            this.rbInputDT.Size = new System.Drawing.Size(205, 17);
            this.rbInputDT.TabIndex = 9;
            this.rbInputDT.TabStop = true;
            this.rbInputDT.Text = "通用：輸入通用拼音，輸出為台羅";
            this.rbInputDT.UseVisualStyleBackColor = true;
            // 
            // rbInputTLPA
            // 
            this.rbInputTLPA.AutoSize = true;
            this.rbInputTLPA.Location = new System.Drawing.Point(7, 65);
            this.rbInputTLPA.Name = "rbInputTLPA";
            this.rbInputTLPA.Size = new System.Drawing.Size(187, 17);
            this.rbInputTLPA.TabIndex = 8;
            this.rbInputTLPA.TabStop = true;
            this.rbInputTLPA.Text = "TLPA：輸入TLPA，輸出為台羅";
            this.rbInputTLPA.UseVisualStyleBackColor = true;
            // 
            // rbInputPOJ
            // 
            this.rbInputPOJ.AutoSize = true;
            this.rbInputPOJ.Location = new System.Drawing.Point(7, 42);
            this.rbInputPOJ.Name = "rbInputPOJ";
            this.rbInputPOJ.Size = new System.Drawing.Size(233, 17);
            this.rbInputPOJ.TabIndex = 7;
            this.rbInputPOJ.TabStop = true;
            this.rbInputPOJ.Text = "白話字（POJ）：輸入POJ，輸出為台羅";
            this.rbInputPOJ.UseVisualStyleBackColor = true;
            // 
            // rbInputTL
            // 
            this.rbInputTL.AutoSize = true;
            this.rbInputTL.Location = new System.Drawing.Point(7, 19);
            this.rbInputTL.Name = "rbInputTL";
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
            this.AutoSize = true;
            this.Controls.Add(this.pnSelf);
            this.Name = "PanelTLIM";
            this.Size = new System.Drawing.Size(287, 459);
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
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel pnSelf;
        private System.Windows.Forms.GroupBox gbSpellCheck;
        private System.Windows.Forms.Label lbToneMark;
        private System.Windows.Forms.Label lbDiacritics;
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
