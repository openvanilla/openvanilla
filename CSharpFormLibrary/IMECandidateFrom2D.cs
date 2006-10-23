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
	public class IMECandidateForm2D : System.Windows.Forms.Form
	{
		//·í­¶ªºindex
		//int now_index;		
		//int baseSize;	//°ò¥»ªø«×(¨S¦³candidate®Éformªº¤j¤p)
		private System.ComponentModel.Container components = null;
		private System.UInt64 m_hwnd;
		private const int WM_MOUSEACTIVATE = 0x0021;
		private const int MA_ACTIVATE=0x0001;
		private const int MA_ACTIVATEANDEAT = 0x0002;
		private const int MA_NOACTIVATE=0x0003;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Panel panel3;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Panel panel4;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Panel panel5;
		private System.Windows.Forms.Button button5;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Panel panel6;
		private System.Windows.Forms.Button button6;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Panel panel7;
		private System.Windows.Forms.Button button7;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Panel panel8;
		private System.Windows.Forms.Button button8;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Panel panel9;
		private System.Windows.Forms.Button button9;
		private System.Windows.Forms.Label label9;
		private const int MA_NOACTIVATEANDEAT = 0x0004;
		private System.Windows.Forms.Button button10;
		private System.Windows.Forms.Button button11;
		private System.Windows.Forms.Button button12;
		private ArrayList buttonList ;
		
		public IMECandidateForm2D()
		{
			InitializeComponent();
			buttonList= new ArrayList();
			buttonList.Add(this.button1);
			buttonList.Add(this.button2);
			buttonList.Add(this.button3);
			buttonList.Add(this.button4);
			buttonList.Add(this.button5);
			buttonList.Add(this.button6);
			buttonList.Add(this.button7);
			buttonList.Add(this.button8);
			buttonList.Add(this.button9);			
			buttonList.Add(this.button12);		
			//baseSize = this.lbCandidates.Location.Y+100;
			//this.Opacity = 0.85;
		}
		/*public IMECandidateForm(string[] candidates)
		{
			InitializeComponent();
		}*/
		protected override void WndProc(ref Message m) 
		{
			
			if (m.Msg == WM_MOUSEACTIVATE) 
			{				
				//this.RaiseMouseEvent( button1,);								
				m.Result = (IntPtr)MA_NOACTIVATE;
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
			this.panel1 = new System.Windows.Forms.Panel();
			this.button1 = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.panel2 = new System.Windows.Forms.Panel();
			this.button2 = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.panel3 = new System.Windows.Forms.Panel();
			this.button3 = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.panel4 = new System.Windows.Forms.Panel();
			this.button4 = new System.Windows.Forms.Button();
			this.label4 = new System.Windows.Forms.Label();
			this.panel5 = new System.Windows.Forms.Panel();
			this.button5 = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.panel6 = new System.Windows.Forms.Panel();
			this.button6 = new System.Windows.Forms.Button();
			this.label6 = new System.Windows.Forms.Label();
			this.panel7 = new System.Windows.Forms.Panel();
			this.button7 = new System.Windows.Forms.Button();
			this.label7 = new System.Windows.Forms.Label();
			this.panel8 = new System.Windows.Forms.Panel();
			this.button8 = new System.Windows.Forms.Button();
			this.label8 = new System.Windows.Forms.Label();
			this.panel9 = new System.Windows.Forms.Panel();
			this.button9 = new System.Windows.Forms.Button();
			this.label9 = new System.Windows.Forms.Label();
			this.button10 = new System.Windows.Forms.Button();
			this.button11 = new System.Windows.Forms.Button();
			this.button12 = new System.Windows.Forms.Button();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.panel3.SuspendLayout();
			this.panel4.SuspendLayout();
			this.panel5.SuspendLayout();
			this.panel6.SuspendLayout();
			this.panel7.SuspendLayout();
			this.panel8.SuspendLayout();
			this.panel9.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.button1);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(72, 32);
			this.panel1.TabIndex = 6;
			// 
			// button1
			// 
			this.button1.Dock = System.Windows.Forms.DockStyle.Right;
			this.button1.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button1.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button1.Location = new System.Drawing.Point(16, 0);
			this.button1.Name = "button1";
			this.button1.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button1.Size = new System.Drawing.Size(56, 32);
			this.button1.TabIndex = 0;
			this.button1.Text = "¸â¸â";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// label1
			// 
			this.label1.BackColor = System.Drawing.Color.Pink;
			this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label1.Dock = System.Windows.Forms.DockStyle.Left;
			this.label1.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label1.Location = new System.Drawing.Point(0, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(16, 32);
			this.label1.TabIndex = 3;
			this.label1.Text = "W";
			this.label1.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.button2);
			this.panel2.Controls.Add(this.label2);
			this.panel2.Location = new System.Drawing.Point(72, 0);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(72, 32);
			this.panel2.TabIndex = 7;
			// 
			// button2
			// 
			this.button2.Dock = System.Windows.Forms.DockStyle.Right;
			this.button2.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button2.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button2.Location = new System.Drawing.Point(16, 0);
			this.button2.Name = "button2";
			this.button2.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button2.Size = new System.Drawing.Size(56, 32);
			this.button2.TabIndex = 0;
			this.button2.Text = "¸â¸â";
			// 
			// label2
			// 
			this.label2.BackColor = System.Drawing.Color.Pink;
			this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label2.Dock = System.Windows.Forms.DockStyle.Left;
			this.label2.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label2.Location = new System.Drawing.Point(0, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(16, 32);
			this.label2.TabIndex = 3;
			this.label2.Text = "E";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel3
			// 
			this.panel3.Controls.Add(this.button3);
			this.panel3.Controls.Add(this.label3);
			this.panel3.Location = new System.Drawing.Point(144, 0);
			this.panel3.Name = "panel3";
			this.panel3.Size = new System.Drawing.Size(72, 32);
			this.panel3.TabIndex = 8;
			// 
			// button3
			// 
			this.button3.Dock = System.Windows.Forms.DockStyle.Right;
			this.button3.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button3.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button3.Location = new System.Drawing.Point(16, 0);
			this.button3.Name = "button3";
			this.button3.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button3.Size = new System.Drawing.Size(56, 32);
			this.button3.TabIndex = 0;
			this.button3.Text = "¸â¸â";
			// 
			// label3
			// 
			this.label3.BackColor = System.Drawing.Color.Pink;
			this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label3.Dock = System.Windows.Forms.DockStyle.Left;
			this.label3.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label3.Location = new System.Drawing.Point(0, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(16, 32);
			this.label3.TabIndex = 3;
			this.label3.Text = "R";
			this.label3.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel4
			// 
			this.panel4.Controls.Add(this.button4);
			this.panel4.Controls.Add(this.label4);
			this.panel4.Location = new System.Drawing.Point(0, 32);
			this.panel4.Name = "panel4";
			this.panel4.Size = new System.Drawing.Size(72, 32);
			this.panel4.TabIndex = 9;
			// 
			// button4
			// 
			this.button4.Dock = System.Windows.Forms.DockStyle.Right;
			this.button4.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button4.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button4.Location = new System.Drawing.Point(16, 0);
			this.button4.Name = "button4";
			this.button4.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button4.Size = new System.Drawing.Size(56, 32);
			this.button4.TabIndex = 0;
			this.button4.Text = "¸â¸â";
			// 
			// label4
			// 
			this.label4.BackColor = System.Drawing.Color.Pink;
			this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label4.Dock = System.Windows.Forms.DockStyle.Left;
			this.label4.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label4.Location = new System.Drawing.Point(0, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(16, 32);
			this.label4.TabIndex = 3;
			this.label4.Text = "S";
			this.label4.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel5
			// 
			this.panel5.Controls.Add(this.button5);
			this.panel5.Controls.Add(this.label5);
			this.panel5.Location = new System.Drawing.Point(72, 32);
			this.panel5.Name = "panel5";
			this.panel5.Size = new System.Drawing.Size(72, 32);
			this.panel5.TabIndex = 10;
			// 
			// button5
			// 
			this.button5.Dock = System.Windows.Forms.DockStyle.Right;
			this.button5.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button5.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button5.Location = new System.Drawing.Point(16, 0);
			this.button5.Name = "button5";
			this.button5.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button5.Size = new System.Drawing.Size(56, 32);
			this.button5.TabIndex = 0;
			this.button5.Text = "¸â¸â";
			// 
			// label5
			// 
			this.label5.BackColor = System.Drawing.Color.Pink;
			this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label5.Dock = System.Windows.Forms.DockStyle.Left;
			this.label5.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label5.Location = new System.Drawing.Point(0, 0);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(16, 32);
			this.label5.TabIndex = 3;
			this.label5.Text = "D";
			this.label5.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel6
			// 
			this.panel6.Controls.Add(this.button6);
			this.panel6.Controls.Add(this.label6);
			this.panel6.Location = new System.Drawing.Point(144, 32);
			this.panel6.Name = "panel6";
			this.panel6.Size = new System.Drawing.Size(72, 32);
			this.panel6.TabIndex = 11;
			// 
			// button6
			// 
			this.button6.Dock = System.Windows.Forms.DockStyle.Right;
			this.button6.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button6.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button6.Location = new System.Drawing.Point(16, 0);
			this.button6.Name = "button6";
			this.button6.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button6.Size = new System.Drawing.Size(56, 32);
			this.button6.TabIndex = 0;
			this.button6.Text = "¸â¸â";
			// 
			// label6
			// 
			this.label6.BackColor = System.Drawing.Color.Pink;
			this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label6.Dock = System.Windows.Forms.DockStyle.Left;
			this.label6.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label6.Location = new System.Drawing.Point(0, 0);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(16, 32);
			this.label6.TabIndex = 3;
			this.label6.Text = "F";
			this.label6.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel7
			// 
			this.panel7.Controls.Add(this.button7);
			this.panel7.Controls.Add(this.label7);
			this.panel7.Location = new System.Drawing.Point(0, 64);
			this.panel7.Name = "panel7";
			this.panel7.Size = new System.Drawing.Size(72, 32);
			this.panel7.TabIndex = 12;
			// 
			// button7
			// 
			this.button7.Dock = System.Windows.Forms.DockStyle.Right;
			this.button7.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button7.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button7.Location = new System.Drawing.Point(16, 0);
			this.button7.Name = "button7";
			this.button7.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button7.Size = new System.Drawing.Size(56, 32);
			this.button7.TabIndex = 0;
			this.button7.Text = "¸â¸â";
			// 
			// label7
			// 
			this.label7.BackColor = System.Drawing.Color.Pink;
			this.label7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label7.Dock = System.Windows.Forms.DockStyle.Left;
			this.label7.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label7.Location = new System.Drawing.Point(0, 0);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(16, 32);
			this.label7.TabIndex = 3;
			this.label7.Text = "X";
			this.label7.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel8
			// 
			this.panel8.Controls.Add(this.button8);
			this.panel8.Controls.Add(this.label8);
			this.panel8.Location = new System.Drawing.Point(72, 64);
			this.panel8.Name = "panel8";
			this.panel8.Size = new System.Drawing.Size(72, 32);
			this.panel8.TabIndex = 13;
			// 
			// button8
			// 
			this.button8.Dock = System.Windows.Forms.DockStyle.Right;
			this.button8.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button8.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button8.Location = new System.Drawing.Point(16, 0);
			this.button8.Name = "button8";
			this.button8.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button8.Size = new System.Drawing.Size(56, 32);
			this.button8.TabIndex = 0;
			this.button8.Text = "¸â¸â";
			// 
			// label8
			// 
			this.label8.BackColor = System.Drawing.Color.Pink;
			this.label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label8.Dock = System.Windows.Forms.DockStyle.Left;
			this.label8.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label8.Location = new System.Drawing.Point(0, 0);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(16, 32);
			this.label8.TabIndex = 3;
			this.label8.Text = "C";
			this.label8.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// panel9
			// 
			this.panel9.Controls.Add(this.button9);
			this.panel9.Controls.Add(this.label9);
			this.panel9.Location = new System.Drawing.Point(144, 64);
			this.panel9.Name = "panel9";
			this.panel9.Size = new System.Drawing.Size(72, 32);
			this.panel9.TabIndex = 14;
			// 
			// button9
			// 
			this.button9.Dock = System.Windows.Forms.DockStyle.Right;
			this.button9.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button9.Font = new System.Drawing.Font("PMingLiU", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.button9.Location = new System.Drawing.Point(16, 0);
			this.button9.Name = "button9";
			this.button9.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.button9.Size = new System.Drawing.Size(56, 32);
			this.button9.TabIndex = 0;
			this.button9.Text = "¸â¸â";
			// 
			// label9
			// 
			this.label9.BackColor = System.Drawing.Color.Pink;
			this.label9.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.label9.Dock = System.Windows.Forms.DockStyle.Left;
			this.label9.Font = new System.Drawing.Font("PMingLiU", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(136)));
			this.label9.Location = new System.Drawing.Point(0, 0);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(16, 32);
			this.label9.TabIndex = 3;
			this.label9.Text = "V";
			this.label9.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// button10
			// 
			this.button10.BackColor = System.Drawing.Color.MistyRose;
			this.button10.Cursor = System.Windows.Forms.Cursors.PanNorth;
			this.button10.Location = new System.Drawing.Point(216, 0);
			this.button10.Name = "button10";
			this.button10.Size = new System.Drawing.Size(48, 32);
			this.button10.TabIndex = 15;
			this.button10.Text = "¡ô";
			// 
			// button11
			// 
			this.button11.BackColor = System.Drawing.Color.MistyRose;
			this.button11.Cursor = System.Windows.Forms.Cursors.PanSouth;
			this.button11.Location = new System.Drawing.Point(216, 64);
			this.button11.Name = "button11";
			this.button11.Size = new System.Drawing.Size(48, 32);
			this.button11.TabIndex = 16;
			this.button11.Text = "¡õ";
			// 
			// button12
			// 
			this.button12.BackColor = System.Drawing.Color.PeachPuff;
			this.button12.Cursor = System.Windows.Forms.Cursors.Hand;
			this.button12.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
			this.button12.Font = new System.Drawing.Font("Arial", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.button12.Location = new System.Drawing.Point(216, 32);
			this.button12.Name = "button12";
			this.button12.Size = new System.Drawing.Size(48, 32);
			this.button12.TabIndex = 17;
			this.button12.Text = "1/1";
			// 
			// IMECandidateForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 18);
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(264, 96);
			this.Controls.Add(this.button12);
			this.Controls.Add(this.button11);
			this.Controls.Add(this.button10);
			this.Controls.Add(this.panel9);
			this.Controls.Add(this.panel8);
			this.Controls.Add(this.panel7);
			this.Controls.Add(this.panel6);
			this.Controls.Add(this.panel5);
			this.Controls.Add(this.panel4);
			this.Controls.Add(this.panel3);
			this.Controls.Add(this.panel2);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Location = new System.Drawing.Point(199, 199);
			this.Name = "IMECandidateForm";
			this.ShowInTaskbar = false;
			this.Text = "Form1";
			this.Click += new System.EventHandler(this.IMECandidateForm_Click);
			this.Validating += new System.ComponentModel.CancelEventHandler(this.IMECandidateForm_Validating);
			this.Validated += new System.EventHandler(this.IMECandidateForm_Validated);
			this.Activated += new System.EventHandler(this.IMECandidateForm_Activated);
			this.Leave += new System.EventHandler(this.IMECandidateForm_Leave);
			this.Enter += new System.EventHandler(this.IMECandidateForm_Enter);
			this.Deactivate += new System.EventHandler(this.IMECandidateForm_Deactivate);
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.panel3.ResumeLayout(false);
			this.panel4.ResumeLayout(false);
			this.panel5.ResumeLayout(false);
			this.panel6.ResumeLayout(false);
			this.panel7.ResumeLayout(false);
			this.panel8.ResumeLayout(false);
			this.panel9.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		#region public methods

		public void SetCandidates(string inputs)
		{/*
			string[] a_inputs = inputs.Split(' ');
			if(a_inputs==null) return;			
			this.lbCandidates.Height=(a_inputs.Length+1)*15;
			this.ShowListView(a_inputs); // not show, only setstring*/
			string[] a_inputs = inputs.Split(' ');
			if(a_inputs==null) return;			
			//this.lbCandidates.Height=(a_inputs.Length+1)*15;
			this.ShowListView(a_inputs); 
		}

		public void SetLocation(int x, int y)
		{
			//System.Diagnostics.Debug.WriteLine("Screen.GetBounds().Width="+Screen.GetBounds(this).Width);
			//System.Diagnostics.Debug.WriteLine("Screen.GetBounds().Height="+Screen.GetBounds(this).Height);

			int screenW=Screen.GetBounds(this).Width;
			int screenH=Screen.GetBounds(this).Height;
			if (x+this.Width> screenW)
			{
				x=screenW-this.Width;
			}
			if (y+this.Height> screenH)
			{
				y=screenH-this.Height;
			}
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
			//this.lbCandidates.Items.Clear();
			System.Diagnostics.Debug.WriteLine("ClearCandidates");
			for(int i=0;i<=9;i++)
			{				
				((Button)this.buttonList[i]).Text="";
			}
		}
		/*public void DepthOfList(int number)
		{
			//14 = lbCandidate.Size.Height/9 ¶i¦ì - Height%9
			this.Size = new Size(this.Size.Width,baseSize+number*13);			
		}*/
		public void SetHWND(System.UInt64 hwnd)
		{
			System.Diagnostics.Debug.WriteLine("SetHWND");
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
			/*this.lbCandidates.Items.Clear();			
			this.lbCandidates.Items.AddRange(pageCandidates);
			this.lbCandidates.SelectedItem= this.lbCandidates.Items[0];*/	
			System.Diagnostics.Debug.WriteLine("ShowListView");
			for(int i=0;i<=9;i++)
			{
				
				if(i<pageCandidates.Length-1)
				{
					((Button)this.buttonList[i]).Text=pageCandidates[i].Substring(2);					
					System.Diagnostics.Debug.WriteLine(pageCandidates[i]);
				}
				else if (i==pageCandidates.Length-1)
				{
					//((Button)this.buttonList[9]).Text=pageCandidates[i];		
					this.button12.Text=pageCandidates[i];		
					System.Diagnostics.Debug.WriteLine(pageCandidates[i]);
				}
				else
				{
					//((Button)this.buttonList[i]).Text="";
				}
			}
			
		}			
		#endregion			
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
			//Debug.WriteLine("ShowNoActive cand ");
			UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
		}
		public void HideNoActive()
		{
			//Debug.WriteLine("HideNoActive cand ");
			UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
		}

		private void IMECandidateForm_Click(object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("Click");
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
			//System.Diagnostics.Debug.Write("Cand Active(); give back focus to ");
			//System.Diagnostics.Debug.WriteLine(m_hwnd);
			//	isTouched = true;
		}//else
			//	isTouched = false;
		}

		private void IMECandidateForm_Deactivate(object sender, System.EventArgs e)
		{
			//HideNoActive();
			//System.Diagnostics.Debug.WriteLine("Cand DeActive();");
		}

		private void lbCandidates_Enter(object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("lbCandidates_Enter");
		}

		private void lbCandidates_Leave(object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("lbCandidates_Leave");
		}

		private void IMECandidateForm_Enter(object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("Cand Enter");
		}

		private void IMECandidateForm_Leave(object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("Cand Leave");			
		}

		private void IMECandidateForm_Validated(object sender, System.EventArgs e)
		{
		
			//System.Diagnostics.Debug.WriteLine("Cand Validated");
		}
		private void IMECandidateForm_Validating(object sender, System.ComponentModel.CancelEventArgs e)
		{
			//System.Diagnostics.Debug.WriteLine("Cand Validating");
			
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			SendKeys.SendWait("1");
		}		
	}
}


