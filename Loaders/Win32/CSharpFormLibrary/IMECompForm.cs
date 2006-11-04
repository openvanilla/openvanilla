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
		private System.Windows.Forms.Label label1;
		private const int MA_NOACTIVATEANDEAT = 0x0004;

		private int caretWidth = 0;
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
				m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
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
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// textBox1
			// 
			this.textBox1.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(255)), ((System.Byte)(255)), ((System.Byte)(192)));
			this.textBox1.Dock = System.Windows.Forms.DockStyle.Left;
			this.textBox1.Font = new System.Drawing.Font("新細明體", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.textBox1.Location = new System.Drawing.Point(0, 0);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(404, 27);
			this.textBox1.TabIndex = 0;
			this.textBox1.Text = "textBox1";
			this.textBox1.WordWrap = false;
			// 
			// label1
			// 
			this.label1.BackColor = System.Drawing.SystemColors.WindowFrame;
			this.label1.Location = new System.Drawing.Point(0, 20);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(23, 5);
			this.label1.TabIndex = 1;
			this.label1.Text = "label1";
			this.label1.Visible = false;
			// 
			// IMECompForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(7, 20);
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(404, 27);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBox1);
			this.Font = new System.Drawing.Font("新細明體", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "IMECompForm";
			this.Opacity = 0.75;
			this.ShowInTaskbar = false;
			this.Text = "IMECompForm";
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

			if(inputs==null || textBox1.Text==inputs) return;
			
			Graphics g = this.textBox1.CreateGraphics();

			int width = (int)g.MeasureString(inputs, this.textBox1.Font).Width;
//			caretWidth = width;

			g.Dispose();
			
			//string[] a_inputs = inputs.Split(' ');			

			
			this.Width = width;
			
			//this.Height = this.textBox1.PreferredHeight;
			this.textBox1.Text = inputs;
			//this.textBox1.Focus();
		}

		public void SetCaretX(int x)
		{
#if false
			this.label1.Width = 10;
			this.label1.Left = (caretWidth)*(x-1);
#endif
		}
	}
}
