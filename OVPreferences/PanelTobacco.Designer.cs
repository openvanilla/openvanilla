namespace OVPreferences
{
    partial class PanelTobacco
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelTobacco));
            this.pnTobacco = new System.Windows.Forms.Panel();
            this.m_cbChooseInFront = new System.Windows.Forms.CheckBox();
            this.m_cbClearOnError = new System.Windows.Forms.CheckBox();
            this.pnTobacco.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnTobacco
            // 
            this.pnTobacco.Controls.Add(this.m_cbChooseInFront);
            this.pnTobacco.Controls.Add(this.m_cbClearOnError);
            resources.ApplyResources(this.pnTobacco, "pnTobacco");
            this.pnTobacco.Name = "pnTobacco";
            // 
            // m_cbChooseInFront
            // 
            resources.ApplyResources(this.m_cbChooseInFront, "m_cbChooseInFront");
            this.m_cbChooseInFront.Name = "m_cbChooseInFront";
            this.m_cbChooseInFront.UseVisualStyleBackColor = true;
            this.m_cbChooseInFront.CheckedChanged += new System.EventHandler(this.m_cbChooseInFront_CheckedChanged);
            // 
            // m_cbClearOnError
            // 
            resources.ApplyResources(this.m_cbClearOnError, "m_cbClearOnError");
            this.m_cbClearOnError.Name = "m_cbClearOnError";
            this.m_cbClearOnError.UseVisualStyleBackColor = true;
            this.m_cbClearOnError.CheckedChanged += new System.EventHandler(this.m_cbClearOnError_CheckedChanged);
            // 
            // PanelTobacco
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.pnTobacco);
            this.Name = "PanelTobacco";
            this.Controls.SetChildIndex(this.pnTobacco, 0);
            this.pnTobacco.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnTobacco;
        private System.Windows.Forms.CheckBox m_cbChooseInFront;
        private System.Windows.Forms.CheckBox m_cbClearOnError;
    }
}
