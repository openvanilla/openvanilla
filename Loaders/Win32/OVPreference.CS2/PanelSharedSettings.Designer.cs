namespace OVPreference.CS2
{
    partial class PanelSharedSettings
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        FontComboBox fontComboBox1;
        ColorPicker fontColorPicker;
        ColorPicker bgColorPicker;
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.button1 = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.bgColorPicker = new OVPreference.CS2.ColorPicker();
            this.fontColorPicker = new OVPreference.CS2.ColorPicker();
            this.fontComboBox1 = new OVPreference.CS2.FontComboBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.bgColorPicker);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.fontColorPicker);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.trackBar1);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.comboBox2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.fontComboBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(285, 317);
            this.panel1.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(23, 122);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(93, 12);
            this.label4.TabIndex = 9;
            this.label4.Text = "Background Color";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(23, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "Font Color";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 223);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "Opacity";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(71, 214);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(173, 42);
            this.trackBar1.TabIndex = 4;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(25, 188);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 21);
            this.button1.TabIndex = 3;
            this.button1.Text = "Color";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "8",
            "9",
            "10",
            "11",
            "12",
            "14",
            "16"});
            this.comboBox2.Location = new System.Drawing.Point(205, 34);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(51, 20);
            this.comboBox2.TabIndex = 2;
            this.comboBox2.Text = "8";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Font";
            // 
            // bgColorPicker
            // 
            this.bgColorPicker.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.bgColorPicker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.bgColorPicker.FormattingEnabled = true;
            this.bgColorPicker.Items = null;
            this.bgColorPicker.Location = new System.Drawing.Point(25, 138);
            this.bgColorPicker.Name = "bgColorPicker";
            this.bgColorPicker.Size = new System.Drawing.Size(162, 23);
            this.bgColorPicker.TabIndex = 10;
            // 
            // fontColorPicker
            // 
            this.fontColorPicker.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.fontColorPicker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fontColorPicker.FormattingEnabled = true;
            this.fontColorPicker.Items = null;
            this.fontColorPicker.Location = new System.Drawing.Point(24, 85);
            this.fontColorPicker.Name = "fontColorPicker";
            this.fontColorPicker.Size = new System.Drawing.Size(163, 23);
            this.fontColorPicker.TabIndex = 8;
            this.fontColorPicker.SelectedIndexChanged += new System.EventHandler(this.fontColorPickerNotiFunction);
            // 
            // fontComboBox1
            // 
            this.fontComboBox1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.fontComboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fontComboBox1.FormattingEnabled = true;
            this.fontComboBox1.IntegralHeight = false;
            this.fontComboBox1.Location = new System.Drawing.Point(24, 34);
            this.fontComboBox1.MaxDropDownItems = 20;
            this.fontComboBox1.Name = "fontComboBox1";
            this.fontComboBox1.Size = new System.Drawing.Size(163, 23);
            this.fontComboBox1.TabIndex = 6;
            // 
            // PanelSharedSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PanelSharedSettings";
            this.Size = new System.Drawing.Size(285, 317);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar trackBar1;
        //private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label3;
        //private System.Windows.Forms.ComboBox comboBox3;
        private System.Windows.Forms.Label label4;
        //private System.Windows.Forms.ComboBox comboBox1;
    }
}
