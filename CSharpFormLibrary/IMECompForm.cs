using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
	/// <summary>
	/// Summary description for IMECompForm.
	/// </summary>
	public class IMECompForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_NOACTIVATE=0x0003;
		private const int MA_NOACTIVATEANDEAT = 0x0004;
		public IMECompForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}
		protected override void WndProc(ref Message m) 
		{
			if (m.Msg == WM_MOUSEACTIVATE) 
			{
				m.Result = (IntPtr)MA_NOACTIVATE;
				return;
			}
			base.WndProc(ref m);
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// textBox1
			// 
			this.textBox1.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(255)), ((System.Byte)(255)), ((System.Byte)(192)));
			this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textBox1.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.textBox1.Location = new System.Drawing.Point(0, 0);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(408, 27);
			this.textBox1.TabIndex = 0;
			this.textBox1.Text = "textBox1";
			
            //
            // IMECompForm
            //
            //<comment author='b6s'>This makes DragDrop registration fail.
            //this.AllowDrop = true;
            //</comment>
            this.AutoScaleBaseSize = new System.Drawing.Size(7, 20);
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(408, 24);
            this.Controls.Add(this.textBox1);
			this.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "IMECompForm";
			this.ShowInTaskbar = false;
			this.Text = "IMECompForm";
			this.Activated += new System.EventHandler(this.IMECompForm_Activated);
			this.Deactivate += new System.EventHandler(this.IMECompForm_Deactivate);
			this.ResumeLayout(false);

		}
		#endregion
		public void ShowNoActive()
		{
			UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}
		public void HideNoActive()
		{
			UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}
		public void ClearComp()
		{
			this.textBox1.Clear();
		}
		public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}
		public void SetComp(string inputs)
		{
			//string[] a_inputs = inputs.Split(' ');			
			if(inputs==null || textBox1.Text==inputs) return;
			
			this.Width = (inputs.Length)*16+7;
			//this.Height = this.textBox1.PreferredHeight;
			this.textBox1.Text = inputs;
			//this.textBox1.Focus();

		}

		private void textBox1_TextChanged(object sender, System.EventArgs e)
		{
		
		}

		private void IMECompForm_Activated(object sender, System.EventArgs e)
		{
			//ShowNoActive();
			//System.Diagnostics.Debug.WriteLine(this.textBox1.Text);
			//System.Diagnostics.Debug.WriteLine("Comp Active();");
		}

		private void IMECompForm_Deactivate(object sender, System.EventArgs e)
		{
			//HideNoActive();
			//System.Diagnostics.Debug.WriteLine("Comp DeActive();");
		}
	}
}
