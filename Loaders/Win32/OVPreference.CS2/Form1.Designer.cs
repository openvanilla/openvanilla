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
            this.tcSelf = new System.Windows.Forms.TabControl();
            this.tabPageGeneric = new System.Windows.Forms.TabPage();
            this.tlSelf = new System.Windows.Forms.TableLayoutPanel();
            this.tlSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // saveButton
            // 
            this.saveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.saveButton.Location = new System.Drawing.Point(185, 342);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(94, 25);
            this.saveButton.TabIndex = 12;
            this.saveButton.Text = "儲存並離開";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // tcSelf
            // 
            this.tcSelf.Location = new System.Drawing.Point(3, 3);
            this.tcSelf.Name = "tcSelf";
            this.tcSelf.SelectedIndex = 0;
            this.tcSelf.Size = new System.Drawing.Size(276, 333);
            this.tcSelf.TabIndex = 13;
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
            // tlSelf
            // 
            this.tlSelf.AutoSize = true;
            this.tlSelf.ColumnCount = 1;
            this.tlSelf.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tlSelf.Controls.Add(this.saveButton, 0, 1);
            this.tlSelf.Controls.Add(this.tcSelf, 0, 0);
            this.tlSelf.Location = new System.Drawing.Point(14, 12);
            this.tlSelf.Name = "tlSelf";
            this.tlSelf.RowCount = 2;
            this.tlSelf.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tlSelf.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tlSelf.Size = new System.Drawing.Size(282, 372);
            this.tlSelf.TabIndex = 14;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(311, 397);
            this.Controls.Add(this.tlSelf);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "偏好設定";
            this.tlSelf.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.TabControl tcSelf;
        private System.Windows.Forms.TabPage tabPageGeneric;
        private System.Windows.Forms.TableLayoutPanel tlSelf;
    }
}

