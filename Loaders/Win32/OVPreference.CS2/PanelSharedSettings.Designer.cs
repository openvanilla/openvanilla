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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelSharedSettings));
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
            this.panel1.AccessibleDescription = null;
            this.panel1.AccessibleName = null;
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.BackgroundImage = null;
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
            this.panel1.Font = null;
            this.panel1.Name = "panel1";
            // 
            // label4
            // 
            this.label4.AccessibleDescription = null;
            this.label4.AccessibleName = null;
            resources.ApplyResources(this.label4, "label4");
            this.label4.Font = null;
            this.label4.Name = "label4";
            // 
            // label3
            // 
            this.label3.AccessibleDescription = null;
            this.label3.AccessibleName = null;
            resources.ApplyResources(this.label3, "label3");
            this.label3.Font = null;
            this.label3.Name = "label3";
            // 
            // label2
            // 
            this.label2.AccessibleDescription = null;
            this.label2.AccessibleName = null;
            resources.ApplyResources(this.label2, "label2");
            this.label2.Font = null;
            this.label2.Name = "label2";
            // 
            // trackBar1
            // 
            this.trackBar1.AccessibleDescription = null;
            this.trackBar1.AccessibleName = null;
            resources.ApplyResources(this.trackBar1, "trackBar1");
            this.trackBar1.BackgroundImage = null;
            this.trackBar1.Font = null;
            this.trackBar1.Name = "trackBar1";
            // 
            // button1
            // 
            this.button1.AccessibleDescription = null;
            this.button1.AccessibleName = null;
            resources.ApplyResources(this.button1, "button1");
            this.button1.BackgroundImage = null;
            this.button1.Font = null;
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.AccessibleDescription = null;
            this.comboBox2.AccessibleName = null;
            resources.ApplyResources(this.comboBox2, "comboBox2");
            this.comboBox2.BackgroundImage = null;
            this.comboBox2.Font = null;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            resources.GetString("comboBox2.Items"),
            resources.GetString("comboBox2.Items1"),
            resources.GetString("comboBox2.Items2"),
            resources.GetString("comboBox2.Items3"),
            resources.GetString("comboBox2.Items4"),
            resources.GetString("comboBox2.Items5"),
            resources.GetString("comboBox2.Items6")});
            this.comboBox2.Name = "comboBox2";
            // 
            // label1
            // 
            this.label1.AccessibleDescription = null;
            this.label1.AccessibleName = null;
            resources.ApplyResources(this.label1, "label1");
            this.label1.Font = null;
            this.label1.Name = "label1";
            // 
            // bgColorPicker
            // 
            this.bgColorPicker.AccessibleDescription = null;
            this.bgColorPicker.AccessibleName = null;
            resources.ApplyResources(this.bgColorPicker, "bgColorPicker");
            this.bgColorPicker.BackgroundImage = null;
            this.bgColorPicker.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.bgColorPicker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.bgColorPicker.Font = null;
            this.bgColorPicker.FormattingEnabled = true;
            this.bgColorPicker.Items = null;
            this.bgColorPicker.Name = "bgColorPicker";
            this.bgColorPicker.SelectedIndexChanged += new System.EventHandler(this.bgColorPickerNotiFunction);
            // 
            // fontColorPicker
            // 
            this.fontColorPicker.AccessibleDescription = null;
            this.fontColorPicker.AccessibleName = null;
            resources.ApplyResources(this.fontColorPicker, "fontColorPicker");
            this.fontColorPicker.BackgroundImage = null;
            this.fontColorPicker.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.fontColorPicker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fontColorPicker.Font = null;
            this.fontColorPicker.FormattingEnabled = true;
            this.fontColorPicker.Items = null;
            this.fontColorPicker.Name = "fontColorPicker";
            this.fontColorPicker.SelectedIndexChanged += new System.EventHandler(this.fontColorPickerNotiFunction);
            // 
            // fontComboBox1
            // 
            this.fontComboBox1.AccessibleDescription = null;
            this.fontComboBox1.AccessibleName = null;
            resources.ApplyResources(this.fontComboBox1, "fontComboBox1");
            this.fontComboBox1.BackgroundImage = null;
            this.fontComboBox1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.fontComboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fontComboBox1.Font = null;
            this.fontComboBox1.FormattingEnabled = true;
            this.fontComboBox1.Name = "fontComboBox1";
            // 
            // PanelSharedSettings
            // 
            this.AccessibleDescription = null;
            this.AccessibleName = null;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = null;
            this.Controls.Add(this.panel1);
            this.Font = null;
            this.Name = "PanelSharedSettings";
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
