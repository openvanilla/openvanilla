namespace OVPreference.CS2
{
    partial class PanelModuleList
    {
        public void AddModule(object module, bool isChecked)
        {
            this.m_clModuleList.Items.Add(module, isChecked);
        }

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
            this.m_clModuleList = new System.Windows.Forms.CheckedListBox();
            this.m_pnSelf.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.AutoSize = true;
            this.m_pnSelf.Controls.Add(this.m_clModuleList);
            this.m_pnSelf.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pnSelf.Location = new System.Drawing.Point(0, 0);
            this.m_pnSelf.Name = "m_pnSelf";
            this.m_pnSelf.Size = new System.Drawing.Size(320, 260);
            this.m_pnSelf.TabIndex = 0;
            // 
            // m_clModuleList
            // 
            this.m_clModuleList.AllowDrop = true;
            this.m_clModuleList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_clModuleList.FormattingEnabled = true;
            this.m_clModuleList.Location = new System.Drawing.Point(0, 0);
            this.m_clModuleList.Name = "m_clModuleList";
            this.m_clModuleList.Size = new System.Drawing.Size(320, 259);
            this.m_clModuleList.TabIndex = 0;
            this.m_clModuleList.ItemCheck +=
                new System.Windows.Forms.ItemCheckEventHandler(
                    m_clModuleList_ItemCheck);
            this.m_clModuleList.MouseDown +=
                new System.Windows.Forms.MouseEventHandler(
                    m_clModuleList_MouseDown);
            this.m_clModuleList.MouseMove +=
                new System.Windows.Forms.MouseEventHandler(
                    m_clModuleList_MouseMove);
            this.m_clModuleList.MouseUp +=
                new System.Windows.Forms.MouseEventHandler(
                    m_clModuleList_MouseUp);
            this.m_clModuleList.DragDrop +=
                new System.Windows.Forms.DragEventHandler(
                    m_clModuleList_DragDrop);
            this.m_clModuleList.DragOver +=
                new System.Windows.Forms.DragEventHandler(
                    m_clModuleList_DragOver);
            // 
            // PanelModuleList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelModuleList";
            this.Size = new System.Drawing.Size(320, 260);
            this.m_pnSelf.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.CheckedListBox m_clModuleList;
    }
}
