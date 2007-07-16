namespace OVPreference.CS2
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.m_btSave = new System.Windows.Forms.Button();
            this.m_tcSelf = new System.Windows.Forms.TabControl();
            this.m_tlSelf = new System.Windows.Forms.TableLayoutPanel();
            this.m_tlSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_btSave
            // 
            this.m_btSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_btSave.Location = new System.Drawing.Point(185, 342);
            this.m_btSave.Name = "m_btSave";
            this.m_btSave.Size = new System.Drawing.Size(94, 25);
            this.m_btSave.TabIndex = 12;
            this.m_btSave.Text = "儲存並離開";
            this.m_btSave.UseVisualStyleBackColor = true;
            this.m_btSave.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // m_tcSelf
            // 
            this.m_tcSelf.Location = new System.Drawing.Point(3, 3);
            this.m_tcSelf.Name = "m_tcSelf";
            this.m_tcSelf.SelectedIndex = 0;
            this.m_tcSelf.Size = new System.Drawing.Size(276, 333);
            this.m_tcSelf.TabIndex = 13;
            // 
            // m_tlSelf
            // 
            this.m_tlSelf.AutoSize = true;
            this.m_tlSelf.ColumnCount = 1;
            this.m_tlSelf.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.m_tlSelf.Controls.Add(this.m_btSave, 0, 1);
            this.m_tlSelf.Controls.Add(this.m_tcSelf, 0, 0);
            this.m_tlSelf.Location = new System.Drawing.Point(14, 12);
            this.m_tlSelf.Name = "m_tlSelf";
            this.m_tlSelf.RowCount = 2;
            this.m_tlSelf.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.m_tlSelf.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.m_tlSelf.Size = new System.Drawing.Size(282, 372);
            this.m_tlSelf.TabIndex = 14;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(311, 397);
            this.Controls.Add(this.m_tlSelf);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "偏好設定";
            this.m_tlSelf.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_btSave;
        private System.Windows.Forms.TabControl m_tcSelf;
        private System.Windows.Forms.TableLayoutPanel m_tlSelf;
    }
}

