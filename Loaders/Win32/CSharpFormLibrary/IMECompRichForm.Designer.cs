namespace CSharpFormLibrary
{
    partial class IMECompRichForm
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
            this.SuspendLayout();
            // 
            // IMECompRichForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(240, 20);
            this.ControlBox = false;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "IMECompRichForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            //this.TransparencyKey = this.BackColor;
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.IMECompRichForm_Paint);
            this.ResumeLayout(false);

        }

        #endregion

        /*private System.Windows.Forms.Label label1;        
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox richTextBox1;*/
       
    }
}