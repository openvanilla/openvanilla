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
		private System.Windows.Forms.ListView lbCandidates;
		//當頁的index
		//int now_index;		
		//int baseSize;	//基本長度(沒有candidate時form的大小)
		private System.ComponentModel.Container components = null;
		private System.UInt64 m_hwnd;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_ACTIVATE=0x0001;
		private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private TextBox textBox1;
        private VScrollBar vScrollBar1;
        private ColumnHeader columnHeader1;
		private const int MA_NOACTIVATEANDEAT = 0x0004;
		
		public IMECandidateForm()
		{
			InitializeComponent();
//			baseSize = this.lbCandidates.Location.Y+100;
			//this.Opacity = 1;
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
            this.lbCandidates = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.SuspendLayout();
            // 
            // lbCandidates
            // 
            this.lbCandidates.BackColor = System.Drawing.SystemColors.Window;
            this.lbCandidates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.lbCandidates.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.lbCandidates.ForeColor = System.Drawing.SystemColors.WindowText;
            this.lbCandidates.GridLines = true;
            this.lbCandidates.Location = new System.Drawing.Point(0, 28);
            this.lbCandidates.Name = "lbCandidates";
            this.lbCandidates.Scrollable = false;
            this.lbCandidates.Size = new System.Drawing.Size(104, 217);
            this.lbCandidates.TabIndex = 0;
            this.lbCandidates.UseCompatibleStateImageBehavior = false;
            this.lbCandidates.View = System.Windows.Forms.View.List;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 93;
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.textBox1.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBox1.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(126, 28);
            this.textBox1.TabIndex = 2;
            this.textBox1.Text = "候選字";
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.Location = new System.Drawing.Point(106, 28);
            this.vScrollBar1.Maximum = 11;
            this.vScrollBar1.Minimum = 1;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(20, 220);
            this.vScrollBar1.TabIndex = 0;
            this.vScrollBar1.Value = 1;
            // 
            // IMECandidateForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
            this.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.ClientSize = new System.Drawing.Size(126, 248);
            this.Controls.Add(this.vScrollBar1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.lbCandidates);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Location = new System.Drawing.Point(199, 220);
            this.Name = "IMECandidateForm";
            this.ShowInTaskbar = false;
            this.Deactivate += new System.EventHandler(this.IMECandidateForm_Deactivate);
            this.Enter += new System.EventHandler(this.IMECandidateForm_Enter);
            this.Activated += new System.EventHandler(this.IMECandidateForm_Activated);
            this.Validated += new System.EventHandler(this.IMECandidateForm_Validated);
            this.Click += new System.EventHandler(this.IMECandidateForm_Click);
            this.Validating += new System.ComponentModel.CancelEventHandler(this.IMECandidateForm_Validating);
            this.Leave += new System.EventHandler(this.IMECandidateForm_Leave);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		#region public methods

		public void SetCandidates(string inputs)
		{
			string[] a_inputs = inputs.Split(' ');
			if(a_inputs==null) return;
            //int newHeight = (a_inputs.Length + 1) * this.lbCandidates.ItemHeight;
            /*if (newHeight != this.lbCandidates.Height)
            {
                this.lbCandidates.Height = (a_inputs.Length + 1) * this.lbCandidates.ItemHeight;
            }*/
            
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

		/*public int GetValue()
		{
			return this.lbCandidates.SelectedIndex;
		}*/
		public void ClearCandidates()
		{
			this.lbCandidates.Items.Clear();
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
            ListViewItem li;
            foreach (string cand in pageCandidates)
            {
                li = new ListViewItem(cand);
                this.lbCandidates.Items.Add(li);
            }
			//this.lbCandidates.SelectedItem= this.lbCandidates.Items[0];            
            this.lbCandidates.Items[0].Selected = true;
            string lastItem = pageCandidates[pageCandidates.Length-1];
            int currentPage = Int32.Parse(lastItem.Substring(1,lastItem.IndexOf('/')-1));
            string foo = lastItem.Substring(lastItem.IndexOf('/')+1);            
            int pagesCount = Int32.Parse(foo.Substring(0, foo.Length - 1));
          
            this.vScrollBar1.Enabled=(pagesCount>1);
          //  this.vScrollBar1.Height = this.lbCandidates.Height;
            this.vScrollBar1.Maximum=pagesCount*9;
            this.vScrollBar1.TabIndex = pagesCount;
            this.vScrollBar1.Value = (currentPage-1)*9+1;
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
            if (!this.Visible)
			    UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}
		public void HideNoActive()
		{
            if (this.Visible)
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


