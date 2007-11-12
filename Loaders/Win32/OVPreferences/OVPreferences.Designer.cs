namespace OVPreferences
{
    partial class OVPreferences
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OVPreferences));
            this.m_btSave = new System.Windows.Forms.Button();
            this.m_tcSelf = new System.Windows.Forms.TabControl();
            this.m_tlSelf = new System.Windows.Forms.TableLayoutPanel();
            this.listView1 = new System.Windows.Forms.ListView();
            this.m_tlSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_btSave
            // 
            resources.ApplyResources(this.m_btSave, "m_btSave");
            this.m_btSave.Name = "m_btSave";
            this.m_btSave.UseVisualStyleBackColor = true;
            this.m_btSave.Click += new System.EventHandler(this.m_btSave_Click);
            // 
            // m_tcSelf
            // 
            resources.ApplyResources(this.m_tcSelf, "m_tcSelf");
            this.m_tcSelf.Name = "m_tcSelf";
            this.m_tcSelf.SelectedIndex = 0;
            this.m_tcSelf.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            // 
            // m_tlSelf
            // 
            resources.ApplyResources(this.m_tlSelf, "m_tlSelf");
            this.m_tlSelf.Controls.Add(this.m_tcSelf, 1, 0);
            this.m_tlSelf.Controls.Add(this.m_btSave, 1, 1);
            this.m_tlSelf.Controls.Add(this.listView1, 0, 0);
            this.m_tlSelf.Name = "m_tlSelf";
            // 
            // listView1
            // 
            this.listView1.CheckBoxes = true;
            resources.ApplyResources(this.listView1, "listView1");
            this.listView1.Name = "listView1";
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.List;
            // 
            // OVPreferences
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_tlSelf);
            this.Name = "OVPreferences";
            this.m_tlSelf.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_btSave;
        private System.Windows.Forms.TabControl m_tcSelf;
        private System.Windows.Forms.TableLayoutPanel m_tlSelf;
        private System.Windows.Forms.ListView listView1;
    }
}

