using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
	/// <summary>
	/// Summary description for IMEStatusForm.
	/// </summary>
	public class IMEStatusForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button6;
		private System.Windows.Forms.ContextMenu contextMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;

		int currentMenu = 0;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_ACTIVATE=1;		
		private const int MA_ACTIVATEANDEAT = 2;
		private const int MA_NOACTIVATE=0x0003;		
		private const int MA_NOACTIVATEANDEAT = 0x0004;

		public IMEStatusForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			this.SetStyle(ControlStyles.StandardClick,false);
			this.SetStyle(ControlStyles.EnableNotifyMessage,false);
			this.SetStyle(ControlStyles.UserMouse,false);
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}
		protected override void WndProc(ref Message m) 
		{							
			if (m.Msg == WM_MOUSEACTIVATE)// && m.Result==(IntPtr) MA_ACTIVATE) 
			{				
				//m.Result = (IntPtr)MA_NOACTIVATEANDEAT;				
				
				m.Result = (IntPtr)MA_NOACTIVATE;
				/*
				System.Diagnostics.Debug.WriteLine("==================");
				System.Diagnostics.Debug.WriteLine("hwnd->"+m.HWnd);
				System.Diagnostics.Debug.WriteLine("Msg->"+m.Msg);
				System.Diagnostics.Debug.WriteLine("Result->"+m.Result);
				System.Diagnostics.Debug.WriteLine("WM_MOUSEACTIVATE");*/
				//return;
			}
				/*System.Diagnostics.Debug.WriteLine("==================");
				System.Diagnostics.Debug.WriteLine("hwnd->"+m.HWnd);
				System.Diagnostics.Debug.WriteLine("Msg->"+m.Msg);
				System.Diagnostics.Debug.WriteLine("Result->"+m.Result);*/
			else
			{
				base.WndProc(ref m);
			}
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
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.contextMenu1 = new System.Windows.Forms.ContextMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.button3 = new System.Windows.Forms.Button();
			this.button6 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.BackColor = System.Drawing.SystemColors.Desktop;
			this.button1.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(255)), ((System.Byte)(255)), ((System.Byte)(128)));
			this.button1.Location = new System.Drawing.Point(5, 5);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(96, 29);
			this.button1.TabIndex = 0;
			this.button1.Text = "自然輸入法";
			// 
			// button2
			// 
			this.button2.BackColor = System.Drawing.SystemColors.Desktop;
			this.button2.ContextMenu = this.contextMenu1;
			this.button2.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button2.ForeColor = System.Drawing.SystemColors.HighlightText;
			this.button2.Location = new System.Drawing.Point(101, 5);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(48, 29);
			this.button2.TabIndex = 1;
			this.button2.Text = "button2";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// contextMenu1
			// 
			this.contextMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.menuItem1,
																						 this.menuItem2,
																						 this.menuItem3});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.Text = "";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 1;
			this.menuItem2.Text = "test2";
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 2;
			this.menuItem3.Text = "test3";
			// 
			// button3
			// 
			this.button3.BackColor = System.Drawing.SystemColors.Desktop;
			this.button3.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button3.ForeColor = System.Drawing.SystemColors.HighlightText;
			this.button3.Location = new System.Drawing.Point(149, 5);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(48, 29);
			this.button3.TabIndex = 2;
			this.button3.Text = "button3";
			// 
			// button6
			// 
			this.button6.BackColor = System.Drawing.SystemColors.Desktop;
			this.button6.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button6.ForeColor = System.Drawing.SystemColors.HighlightText;
			this.button6.Location = new System.Drawing.Point(197, 5);
			this.button6.Name = "button6";
			this.button6.Size = new System.Drawing.Size(48, 29);
			this.button6.TabIndex = 5;
			this.button6.Text = "設定";
			this.button6.Visible = false;
			// 
			// IMEStatusForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.BackColor = System.Drawing.SystemColors.ActiveCaption;
			this.ClientSize = new System.Drawing.Size(249, 38);
			this.Controls.Add(this.button6);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "IMEStatusForm";
			this.ShowInTaskbar = false;
			this.Text = "IMEStatusForm";
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
			this.ResumeLayout(false);
			
			//this.button1.
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
		public void SetLocation(int x, int y)
		{
			this.Location = new Point(x,y);
		}
		public void SetChiEng(bool isChinese)
		{
			if(isChinese)
			{
				this.button3.Text = "中";
			}else
				this.button3.Text = "英";
		}
		public void SetModString(string inputs)
		{
			if(inputs == null) return;

			if(inputs.Length >10)
			{
				this.button2.Width = (inputs.Length)*10;
			}
			else
			{
				this.button2.Width = (inputs.Length)*2*10;
			}
			this.button3.Left = this.button2.Right;
			this.button6.Left = this.button3.Right;
			this.Width = this.button1.Width+ this.button2.Width 
				+ this.button3.Width +/*this.button6.Width*/ + 8;
			//inputs = inputs.Length + inputs;
			this.button2.Text = inputs;
			//button2.ContextMenu.Show(button2, new System.Drawing.Point(0, button2.Height));
			
		}

		public void SetMenuModString(string inputs)
		{
			if(inputs == null) return;

			if(currentMenu == 0)
				this.menuItem1.Text = inputs;
			if(currentMenu == 1)
				this.menuItem2.Text = inputs;
			if(currentMenu == 2)
				this.menuItem3.Text = inputs;			
			currentMenu++;			
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			this.ShowNoActive();
//			button2.ContextMenu.Show(button2, new System.Drawing.Point(0, (-2)*(button2.Height)));			
		}

		private Point mouseOffset;
		private bool isMouseDown = false;

		private void IMEStatusForm_MouseDown(object sender,System.Windows.Forms.MouseEventArgs e)
		{
			int xOffset;
			int yOffset;
			if (e.Button == MouseButtons.Left)
			{
				xOffset = -e.X - SystemInformation.FrameBorderSize.Width;
				yOffset = -e.Y;
				mouseOffset = new Point(xOffset, yOffset);
				isMouseDown = true;
			}
		}
		private void IMEStatusForm_MouseMove(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (isMouseDown)
			{				
				Point mousePos = Control.MousePosition;
				mousePos.Offset(mouseOffset.X, mouseOffset.Y);
				Location = mousePos;
			}
		}

		private void IMEStatusForm_MouseUp(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				isMouseDown = false;
			}
		}
	}
}
