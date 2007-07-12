namespace OVPreference.CS2
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.saveButton = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageGeneric = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(183, 330);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(94, 25);
            this.saveButton.TabIndex = 12;
            this.saveButton.Text = "儲存並離開";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // tabControl1
            //             
            this.tabControl1.Location = new System.Drawing.Point(0, 1);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(277, 327);
            this.tabControl1.TabIndex = 13;
            // 
            // tabPageGeneric
            // 
            this.tabPageGeneric.Location = new System.Drawing.Point(4, 22);
            this.tabPageGeneric.Name = "tabPageGeneric";
            this.tabPageGeneric.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageGeneric.Size = new System.Drawing.Size(269, 301);
            this.tabPageGeneric.TabIndex = 2;
            this.tabPageGeneric.Text = "泛用模組";
            this.tabPageGeneric.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(279, 358);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.saveButton);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "偏好設定";
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        #endregion

        protected void AddTabGeneric()
        {
            this.tabControl1.Controls.Add(this.tabPageGeneric);
        }

        protected void AddTabTLIM()
        {
            //this.tabControl1.Controls.Add(this.tabPageTLIM);
        }

        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageGeneric;
    }
}

