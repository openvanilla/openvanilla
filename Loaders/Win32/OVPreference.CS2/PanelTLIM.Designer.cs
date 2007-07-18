namespace OVPreference.CS2
{
    partial class PanelTLIM
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
            this.m_gbSpellCheck = new System.Windows.Forms.GroupBox();
            this.m_lbDiacritics = new System.Windows.Forms.Label();
            this.m_lbToneMark = new System.Windows.Forms.Label();
            this.m_cbForcePOJStyle = new System.Windows.Forms.CheckBox();
            this.m_cbNormalize = new System.Windows.Forms.CheckBox();
            this.m_gbTypingOrder = new System.Windows.Forms.GroupBox();
            this.m_rbDiacriticEnd = new System.Windows.Forms.RadioButton();
            this.m_rbDiacriticFront = new System.Windows.Forms.RadioButton();
            this.m_gbSyllableTypeOutput = new System.Windows.Forms.GroupBox();
            this.m_rbOutputPOJ = new System.Windows.Forms.RadioButton();
            this.m_rbOutputTL = new System.Windows.Forms.RadioButton();
            this.m_gbSyllableTypeInput = new System.Windows.Forms.GroupBox();
            this.m_rbInputDT = new System.Windows.Forms.RadioButton();
            this.m_rbInputTLPA = new System.Windows.Forms.RadioButton();
            this.m_rbInputPOJ = new System.Windows.Forms.RadioButton();
            this.m_rbInputTL = new System.Windows.Forms.RadioButton();
            this.m_pnSelf.SuspendLayout();
            this.m_gbSpellCheck.SuspendLayout();
            this.m_gbTypingOrder.SuspendLayout();
            this.m_gbSyllableTypeOutput.SuspendLayout();
            this.m_gbSyllableTypeInput.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.AutoSize = true;
            this.m_pnSelf.Controls.Add(this.m_gbSpellCheck);
            this.m_pnSelf.Controls.Add(this.m_gbTypingOrder);
            this.m_pnSelf.Controls.Add(this.m_gbSyllableTypeOutput);
            this.m_pnSelf.Controls.Add(this.m_gbSyllableTypeInput);
            this.m_pnSelf.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pnSelf.Location = new System.Drawing.Point(0, 0);
            this.m_pnSelf.Name = "m_pnSelf";
            this.m_pnSelf.Padding = new System.Windows.Forms.Padding(3);
            this.m_pnSelf.Size = new System.Drawing.Size(287, 459);
            this.m_pnSelf.TabIndex = 1;
            this.m_pnSelf.Text = "台羅輸入法";
            // 
            // m_gbSpellCheck
            // 
            this.m_gbSpellCheck.Controls.Add(this.m_lbDiacritics);
            this.m_gbSpellCheck.Controls.Add(this.m_lbToneMark);
            this.m_gbSpellCheck.Controls.Add(this.m_cbForcePOJStyle);
            this.m_gbSpellCheck.Controls.Add(this.m_cbNormalize);
            this.m_gbSpellCheck.Location = new System.Drawing.Point(8, 297);
            this.m_gbSpellCheck.Name = "m_gbSpellCheck";
            this.m_gbSpellCheck.Size = new System.Drawing.Size(242, 141);
            this.m_gbSpellCheck.TabIndex = 11;
            this.m_gbSpellCheck.TabStop = false;
            this.m_gbSpellCheck.Text = "拼寫與調符自動檢查";
            // 
            // m_lbDiacritics
            // 
            this.m_lbDiacritics.Location = new System.Drawing.Point(24, 97);
            this.m_lbDiacritics.Name = "m_lbDiacritics";
            this.m_lbDiacritics.Size = new System.Drawing.Size(212, 33);
            this.m_lbDiacritics.TabIndex = 5;
            this.m_lbDiacritics.Text = "使用右上方有一點的o來表示oo，以及用上標n來表示nn";
            // 
            // m_lbToneMark
            // 
            this.m_lbToneMark.Location = new System.Drawing.Point(24, 39);
            this.m_lbToneMark.Name = "m_lbToneMark";
            this.m_lbToneMark.Size = new System.Drawing.Size(212, 35);
            this.m_lbToneMark.TabIndex = 4;
            this.m_lbToneMark.Text = "依照台羅的標調規則，將調號標在正確的母音上。";
            // 
            // m_cbForcePOJStyle
            // 
            this.m_cbForcePOJStyle.AutoSize = true;
            this.m_cbForcePOJStyle.Location = new System.Drawing.Point(7, 77);
            this.m_cbForcePOJStyle.Name = "m_cbForcePOJStyle";
            this.m_cbForcePOJStyle.Size = new System.Drawing.Size(206, 17);
            this.m_cbForcePOJStyle.TabIndex = 3;
            this.m_cbForcePOJStyle.Text = "使用台羅傳統版的符號來輸出台羅";
            this.m_cbForcePOJStyle.UseVisualStyleBackColor = true;
            // 
            // m_cbNormalize
            // 
            this.m_cbNormalize.AutoSize = true;
            this.m_cbNormalize.Location = new System.Drawing.Point(7, 19);
            this.m_cbNormalize.Name = "m_cbNormalize";
            this.m_cbNormalize.Size = new System.Drawing.Size(134, 17);
            this.m_cbNormalize.TabIndex = 2;
            this.m_cbNormalize.Text = "輸入後做正規化檢查";
            this.m_cbNormalize.UseVisualStyleBackColor = true;
            // 
            // m_gbTypingOrder
            // 
            this.m_gbTypingOrder.Controls.Add(this.m_rbDiacriticEnd);
            this.m_gbTypingOrder.Controls.Add(this.m_rbDiacriticFront);
            this.m_gbTypingOrder.Location = new System.Drawing.Point(8, 210);
            this.m_gbTypingOrder.Name = "m_gbTypingOrder";
            this.m_gbTypingOrder.Size = new System.Drawing.Size(242, 71);
            this.m_gbTypingOrder.TabIndex = 14;
            this.m_gbTypingOrder.TabStop = false;
            this.m_gbTypingOrder.Text = "調號與母音的輸入順序";
            // 
            // m_rbDiacriticEnd
            // 
            this.m_rbDiacriticEnd.AutoSize = true;
            this.m_rbDiacriticEnd.Location = new System.Drawing.Point(7, 42);
            this.m_rbDiacriticEnd.Name = "m_rbDiacriticEnd";
            this.m_rbDiacriticEnd.Size = new System.Drawing.Size(167, 17);
            this.m_rbDiacriticEnd.TabIndex = 1;
            this.m_rbDiacriticEnd.TabStop = true;
            this.m_rbDiacriticEnd.Text = "先打母音再打調號 ( \' ` ^ = | )";
            this.m_rbDiacriticEnd.UseVisualStyleBackColor = true;
            // 
            // m_rbDiacriticFront
            // 
            this.m_rbDiacriticFront.AutoSize = true;
            this.m_rbDiacriticFront.Location = new System.Drawing.Point(7, 19);
            this.m_rbDiacriticFront.Name = "m_rbDiacriticFront";
            this.m_rbDiacriticFront.Size = new System.Drawing.Size(170, 17);
            this.m_rbDiacriticFront.TabIndex = 0;
            this.m_rbDiacriticFront.TabStop = true;
            this.m_rbDiacriticFront.Text = "先打調號 ( \' ` ^ = | ) 再打母音";
            this.m_rbDiacriticFront.UseVisualStyleBackColor = true;
            // 
            // m_gbSyllableTypeOutput
            // 
            this.m_gbSyllableTypeOutput.Controls.Add(this.m_rbOutputPOJ);
            this.m_gbSyllableTypeOutput.Controls.Add(this.m_rbOutputTL);
            this.m_gbSyllableTypeOutput.Location = new System.Drawing.Point(8, 128);
            this.m_gbSyllableTypeOutput.Name = "m_gbSyllableTypeOutput";
            this.m_gbSyllableTypeOutput.Size = new System.Drawing.Size(242, 63);
            this.m_gbSyllableTypeOutput.TabIndex = 13;
            this.m_gbSyllableTypeOutput.TabStop = false;
            this.m_gbSyllableTypeOutput.Text = "輸出的音節種類";
            // 
            // m_rbOutputPOJ
            // 
            this.m_rbOutputPOJ.AutoSize = true;
            this.m_rbOutputPOJ.Location = new System.Drawing.Point(7, 42);
            this.m_rbOutputPOJ.Name = "m_rbOutputPOJ";
            this.m_rbOutputPOJ.Size = new System.Drawing.Size(105, 17);
            this.m_rbOutputPOJ.TabIndex = 13;
            this.m_rbOutputPOJ.TabStop = true;
            this.m_rbOutputPOJ.Text = "白話字（POJ）";
            this.m_rbOutputPOJ.UseVisualStyleBackColor = true;
            // 
            // m_rbOutputTL
            // 
            this.m_rbOutputTL.AutoSize = true;
            this.m_rbOutputTL.Location = new System.Drawing.Point(7, 19);
            this.m_rbOutputTL.Name = "m_rbOutputTL";
            this.m_rbOutputTL.Size = new System.Drawing.Size(86, 17);
            this.m_rbOutputTL.TabIndex = 12;
            this.m_rbOutputTL.TabStop = true;
            this.m_rbOutputTL.Text = "台羅（TL）";
            this.m_rbOutputTL.UseVisualStyleBackColor = true;
            // 
            // m_gbSyllableTypeInput
            // 
            this.m_gbSyllableTypeInput.Controls.Add(this.m_rbInputDT);
            this.m_gbSyllableTypeInput.Controls.Add(this.m_rbInputTLPA);
            this.m_gbSyllableTypeInput.Controls.Add(this.m_rbInputPOJ);
            this.m_gbSyllableTypeInput.Controls.Add(this.m_rbInputTL);
            this.m_gbSyllableTypeInput.Location = new System.Drawing.Point(8, 6);
            this.m_gbSyllableTypeInput.Name = "m_gbSyllableTypeInput";
            this.m_gbSyllableTypeInput.Size = new System.Drawing.Size(242, 107);
            this.m_gbSyllableTypeInput.TabIndex = 12;
            this.m_gbSyllableTypeInput.TabStop = false;
            this.m_gbSyllableTypeInput.Text = "輸入的音節種類";
            // 
            // m_rbInputDT
            // 
            this.m_rbInputDT.AutoSize = true;
            this.m_rbInputDT.Location = new System.Drawing.Point(7, 88);
            this.m_rbInputDT.Name = "m_rbInputDT";
            this.m_rbInputDT.Size = new System.Drawing.Size(205, 17);
            this.m_rbInputDT.TabIndex = 9;
            this.m_rbInputDT.TabStop = true;
            this.m_rbInputDT.Text = "通用：輸入通用拼音，輸出為台羅";
            this.m_rbInputDT.UseVisualStyleBackColor = true;
            // 
            // m_rbInputTLPA
            // 
            this.m_rbInputTLPA.AutoSize = true;
            this.m_rbInputTLPA.Location = new System.Drawing.Point(7, 65);
            this.m_rbInputTLPA.Name = "m_rbInputTLPA";
            this.m_rbInputTLPA.Size = new System.Drawing.Size(187, 17);
            this.m_rbInputTLPA.TabIndex = 8;
            this.m_rbInputTLPA.TabStop = true;
            this.m_rbInputTLPA.Text = "TLPA：輸入TLPA，輸出為台羅";
            this.m_rbInputTLPA.UseVisualStyleBackColor = true;
            // 
            // m_rbInputPOJ
            // 
            this.m_rbInputPOJ.AutoSize = true;
            this.m_rbInputPOJ.Location = new System.Drawing.Point(7, 42);
            this.m_rbInputPOJ.Name = "m_rbInputPOJ";
            this.m_rbInputPOJ.Size = new System.Drawing.Size(233, 17);
            this.m_rbInputPOJ.TabIndex = 7;
            this.m_rbInputPOJ.TabStop = true;
            this.m_rbInputPOJ.Text = "白話字（POJ）：輸入POJ，輸出為台羅";
            this.m_rbInputPOJ.UseVisualStyleBackColor = true;
            // 
            // m_rbInputTL
            // 
            this.m_rbInputTL.AutoSize = true;
            this.m_rbInputTL.Location = new System.Drawing.Point(7, 19);
            this.m_rbInputTL.Name = "m_rbInputTL";
            this.m_rbInputTL.Size = new System.Drawing.Size(86, 17);
            this.m_rbInputTL.TabIndex = 6;
            this.m_rbInputTL.TabStop = true;
            this.m_rbInputTL.Text = "台羅（TL）";
            this.m_rbInputTL.UseVisualStyleBackColor = true;
            // 
            // PanelTLIM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelTLIM";
            this.Size = new System.Drawing.Size(287, 459);
            this.m_pnSelf.ResumeLayout(false);
            this.m_gbSpellCheck.ResumeLayout(false);
            this.m_gbSpellCheck.PerformLayout();
            this.m_gbTypingOrder.ResumeLayout(false);
            this.m_gbTypingOrder.PerformLayout();
            this.m_gbSyllableTypeOutput.ResumeLayout(false);
            this.m_gbSyllableTypeOutput.PerformLayout();
            this.m_gbSyllableTypeInput.ResumeLayout(false);
            this.m_gbSyllableTypeInput.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.GroupBox m_gbSpellCheck;
        private System.Windows.Forms.Label m_lbToneMark;
        private System.Windows.Forms.Label m_lbDiacritics;
        private System.Windows.Forms.CheckBox m_cbForcePOJStyle;
        private System.Windows.Forms.CheckBox m_cbNormalize;
        private System.Windows.Forms.GroupBox m_gbTypingOrder;
        private System.Windows.Forms.RadioButton m_rbDiacriticEnd;
        private System.Windows.Forms.RadioButton m_rbDiacriticFront;
        private System.Windows.Forms.GroupBox m_gbSyllableTypeOutput;
        private System.Windows.Forms.RadioButton m_rbOutputPOJ;
        private System.Windows.Forms.RadioButton m_rbOutputTL;
        private System.Windows.Forms.GroupBox m_gbSyllableTypeInput;
        private System.Windows.Forms.RadioButton m_rbInputDT;
        private System.Windows.Forms.RadioButton m_rbInputTLPA;
        private System.Windows.Forms.RadioButton m_rbInputPOJ;
        private System.Windows.Forms.RadioButton m_rbInputTL;
    }
}
