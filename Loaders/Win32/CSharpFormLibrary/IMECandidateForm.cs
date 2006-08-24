using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace CSharpFormLibrary
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class IMECandidateForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lbCandidates;
		private System.Windows.Forms.TextBox tbHeadLine;
		//當頁的index
		int now_index;
		//基本長度(沒有candidate時form的大小)
		int baseSize;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public IMECandidateForm()
		{
			InitializeComponent();
			baseSize = this.lbCandidates.Location.Y;
		}

		public IMECandidateForm(string[] candidates)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}
		
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
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
			this.lbCandidates = new System.Windows.Forms.ListBox();
			this.tbHeadLine = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// lbCandidates
			// 
			this.lbCandidates.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.lbCandidates.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.lbCandidates.ImeMode = System.Windows.Forms.ImeMode.NoControl;
			this.lbCandidates.ItemHeight = 15;
			this.lbCandidates.Location = new System.Drawing.Point(0, 29);
			this.lbCandidates.Name = "lbCandidates";
			this.lbCandidates.Size = new System.Drawing.Size(125, 135);
			this.lbCandidates.TabIndex = 0;
			this.lbCandidates.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.lbCandidates_KeyPress);
			// 
			// tbHeadLine
			// 
			this.tbHeadLine.AllowDrop = true;
			this.tbHeadLine.Dock = System.Windows.Forms.DockStyle.Top;
			this.tbHeadLine.Location = new System.Drawing.Point(0, 0);
			this.tbHeadLine.Name = "tbHeadLine";
			this.tbHeadLine.ReadOnly = true;
			this.tbHeadLine.Size = new System.Drawing.Size(124, 25);
			this.tbHeadLine.TabIndex = 1;
			this.tbHeadLine.Text = "Candidates";
			// 
			// IMECandidateForm
			// 
			this.AllowDrop = true;
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.ClientSize = new System.Drawing.Size(124, 163);
			this.Controls.Add(this.tbHeadLine);
			this.Controls.Add(this.lbCandidates);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "IMECandidateForm";
			this.Text = "Form1";
			this.ResumeLayout(false);

		}


		#endregion

		#region public methods
		/*
		public void ShowCandidates(string[] inputs)
		{
			//this.Size = new Size(200,100);
			if(inputs==null) return;
			this.ShowListView(inputs);
			this.ShowDialog();
		}
		*/

		public void ShowCandidates(string inputs)
		{
			string[] a_inputs = inputs.Split(' ');
			if(a_inputs==null) return;
			this.ShowListView(a_inputs);
			this.baseSize = this.lbCandidates.Location.Y;
			this.ShowDialog();
			
		}

		public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}

		public void DisposeForm()
		{
			this.Dispose(true);
		}

		public int GetValue()
		{
			return this.lbCandidates.SelectedIndex;
		}

		#endregion

		#region private methods

		private void ShowListView(string[] pageCandidates)
		{
			this.lbCandidates.Items.Clear();
			//this.lbCandidates.Items.AddRange(pageCandidates);
			int order =1;
			foreach(string pageCandidate in pageCandidates)
			{
				this.lbCandidates.Items.Add(pageCandidate);
				order++;
			}
			this.lbCandidates.SelectedItem= this.lbCandidates.Items[0];
			DepthOfList(pageCandidates.Length);
		}

		public void DepthOfList(int number)
		{
			//14 = lbCandidate.Size.Height/9 進位 - Height%9
			this.Size = new Size(this.Size.Width,baseSize+number*13);
		}

		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			CSharpFormLibrary.IMECandidateForm form = new CSharpFormLibrary.IMECandidateForm();
			Application.Run(form);
		}

		private void lbCandidates_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			
		}	
	}
}
