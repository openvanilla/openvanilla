using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
	public class IMECandidateForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lbCandidates;
		//當頁的index
		//int now_index;		
		int baseSize;	//基本長度(沒有candidate時form的大小)
		private System.ComponentModel.Container components = null;
		private System.UInt64 m_hwnd;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_ACTIVATE=0x0001;
		private const int MA_ACTIVATEANDEAT = 0x0002;
		private const int MA_NOACTIVATE=0x0003;
		private const int MA_NOACTIVATEANDEAT = 0x0004;
		
		public IMECandidateForm()
		{
			InitializeComponent();
			baseSize = this.lbCandidates.Location.Y+100;
			this.Opacity = 0.85;
		}
		public IMECandidateForm(string[] candidates)
		{
			InitializeComponent();
		}
		protected override void WndProc(ref Message m) 
		{
			if (m.Msg == WM_MOUSEACTIVATE) 
			{
				m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
				//m.Result = (IntPtr)MA_NOACTIVATE;
				return;
			}

			base.WndProc(ref m);
		}
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
			this.SuspendLayout();
			// 
			// lbCandidates
			// 
			this.lbCandidates.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.lbCandidates.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.lbCandidates.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.lbCandidates.ItemHeight = 15;
			this.lbCandidates.Location = new System.Drawing.Point(0, 0);
			this.lbCandidates.Name = "lbCandidates";
			this.lbCandidates.Size = new System.Drawing.Size(120, 90);
			this.lbCandidates.TabIndex = 0;
			// 
			// IMECandidateForm
			// 
			//this.ControlBox=false;
			//this.AllowDrop = true;
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(80, 160);
			this.Controls.Add(this.lbCandidates);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Location = new System.Drawing.Point(199, 199);
			this.Name = "IMECandidateForm";
			this.Opacity = 0;
			this.ShowInTaskbar = false;
			this.Text = "Form1";
			this.Click += new System.EventHandler(this.IMECandidateForm_Click);
			this.Validating += new System.ComponentModel.CancelEventHandler(this.IMECandidateForm_Validating);
			this.Validated += new System.EventHandler(this.IMECandidateForm_Validated);
			this.Activated += new System.EventHandler(this.IMECandidateForm_Activated);
			this.Leave += new System.EventHandler(this.IMECandidateForm_Leave);
			this.Enter += new System.EventHandler(this.IMECandidateForm_Enter);
			this.Deactivate += new System.EventHandler(this.IMECandidateForm_Deactivate);
			this.ResumeLayout(false);

		}

		#endregion

		#region public methods

		public void SetCandidates(string inputs)
		{
			string[] a_inputs = inputs.Split(' ');
			if(a_inputs==null) return;			
			this.lbCandidates.Height=(a_inputs.Length+1)*15;
			this.ShowListView(a_inputs); // not show, only setstring
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
		public void ClearCandidates()
		{
			this.lbCandidates.Items.Clear();
		}
		public void DepthOfList(int number)
		{
			//14 = lbCandidate.Size.Height/9 進位 - Height%9
			this.Size = new Size(this.Size.Width,baseSize+number*13);			
		}
		public void SetHWND(System.UInt64 hwnd)
		{
			m_hwnd=hwnd;
		}

		#endregion

		#region a protected override method for WS_POPUP

		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams cp = base.CreateParams;
				cp.Style = unchecked((int)0x80000000); //WS_POPUP
				return cp;
				/*const int WS_EX_NOACTIVATE = 0x08000000;
				CreateParams cp = base.CreateParams;
				cp.ExStyle = cp.ExStyle | WS_EX_NOACTIVATE;
				return cp;*/
			}
		}

		#endregion

		#region private methods

		private void ShowListView(string[] pageCandidates)
		{
			this.lbCandidates.Items.Clear();
			this.lbCandidates.Items.AddRange(pageCandidates);
			this.lbCandidates.SelectedItem= this.lbCandidates.Items[0];
			

			//DepthOfList(pageCandidates.Length);
		}
	
		
		#endregion

		/*
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			CSharpFormLibrary.IMECandidateForm form = new CSharpFormLibrary.IMECandidateForm();
			Application.Run(form);
		}
		*/

		
		public int GetWidth()
		{
			return this.Width;
		}
		public int GetHeight()
		{
			return this.Height;
		}
		public void ShowNoActive()
		{
			Debug.WriteLine("ShowNoActive cand ");
			UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}
		public void HideNoActive()
		{
			Debug.WriteLine("HideNoActive cand ");
			UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}

		private void IMECandidateForm_Click(object sender, System.EventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("Click");
		}

		private void lbCandidates_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			//MessageBox.Show(this.lbCandidates.Parent.ToString());
			//SendKeys.SendWait((this.lbCandidates.SelectedIndex+1).ToString());
		}

		private void IMECandidateForm_Activated(object sender, System.EventArgs e)
		{
			//ShowNoActive();
			//if(!isTouched) 
			{
				//UtilFuncs.GiveBackFocus(m_hwnd);
				System.Diagnostics.Debug.Write("Cand Active(); give back focus to ");
				System.Diagnostics.Debug.WriteLine(m_hwnd);
			//	isTouched = true;
			}//else
			//	isTouched = false;
		}

		private void IMECandidateForm_Deactivate(object sender, System.EventArgs e)
		{
			//HideNoActive();
			System.Diagnostics.Debug.WriteLine("Cand DeActive();");
		}

		private void lbCandidates_Enter(object sender, System.EventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("lbCandidates_Enter");
		}

		private void lbCandidates_Leave(object sender, System.EventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("lbCandidates_Leave");
		}

		private void IMECandidateForm_Enter(object sender, System.EventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("Cand Enter");
		}

		private void IMECandidateForm_Leave(object sender, System.EventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("Cand Leave");			
		}

		private void IMECandidateForm_Validated(object sender, System.EventArgs e)
		{
		
			System.Diagnostics.Debug.WriteLine("Cand Validated");
		}
		private void IMECandidateForm_Validating(object sender, System.ComponentModel.CancelEventArgs e)
		{
			System.Diagnostics.Debug.WriteLine("Cand Validating");
			
		}		
	}
}


