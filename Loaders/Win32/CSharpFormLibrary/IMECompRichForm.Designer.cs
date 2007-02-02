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
            this.components = new System.ComponentModel.Container();
            //this.richTextBox1 = new System.Windows.Forms.TextBox();
            //this.label1 = new System.Windows.Forms.Label();
            //this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // label1
            /*// 
            this.label1.BackColor = System.Drawing.SystemColors.WindowText;
            this.label1.Font = new System.Drawing.Font("PMingLiU", 1F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.label1.Location = new System.Drawing.Point(4, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(1, 22);
            this.label1.TabIndex = 1;
            this.label1.Text = " ";
            this.label1.Visible = false;*/
            // 
            // richTextBox1
            // 
 /*           this.richTextBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.richTextBox1.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.richTextBox1.Location = new System.Drawing.Point(5, 0);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(0);
            this.richTextBox1.Multiline = false;
            this.richTextBox1.Name = "richTextBox1";
            //this.richTextBox1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            //this.richTextBox1.SelectionAlignment = CSharpFormLibrary.TextAlign.Left;
            this.richTextBox1.Size = new System.Drawing.Size(915, 22);
            this.richTextBox1.TabIndex = 2;
            this.richTextBox1.Text = "";
            this.richTextBox1.WordWrap = false;
            this.richTextBox1.Visible = false; 
        */ 
            // 
            // timer1
            // 
            //this.timer1.Tick += new System.EventHandler(this.timer2_Tick);
            //this.timer1.Interval = 1000;            
            // 
            // IMECompRichForm
            //             
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(formInitWidth, 22);
            this.ControlBox = false;
            //this.Controls.Add(this.label1); 
            //this.Controls.Add(this.richTextBox1);                      
            this.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            //this.MaximumSize = new System.Drawing.Size(600, 25);
            this.Name = "IMECompRichForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.ResumeLayout(false);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.IMECompRichForm_Paint);

        }

        #endregion

        private System.Windows.Forms.Label label1;        
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox richTextBox1;
       
    }
}