using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CSharpFormLibrary
{
    public class IMESymbolForm : System.Windows.Forms.Form
    {
        //private System.Windows.Forms.ListView m_listViews;
        
        private int currentPage = 0; //目前頁次 
        private int totalPage = 0; //傳入的 Symbol 共有幾頁                 
        private ArrayList m_candArray; //傳入的 candi array
        private string[] m_inputs;
        private int m_mode = 0; //0:直式  1:展開

        //當頁的index
        //int now_index;		
        //int baseSize;	//基本長度(沒有Symbol時form的大小)
        private IntPtr m_hwnd;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ImageList imageList1;
        private Label label1;
        private Panel panel1;
        private Panel panel2;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private Point mouseOffset;
        private IMEListView m_listViews;
        private TabControl tabControl1;
        private TabPage tabPage1;
        private TabPage tabPage2;
        private TabPage tabPage3;
        private IContainer components;
        private Panel panel3;
        private IMEButton buttonUp;
        private IMEButton buttonDown;
        private bool isMouseDown = false;

        public IMESymbolForm()
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        public IMESymbolForm(string[] Symbols)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_MOUSEACTIVATE)
            {
                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;
                return;
            }            
        }

        protected override void Dispose(bool disposing)
        {        
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
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.panel3 = new System.Windows.Forms.Panel();
            this.buttonUp = new CSharpFormLibrary.IMEButton();
            this.buttonDown = new CSharpFormLibrary.IMEButton();
            this.m_listViews = new CSharpFormLibrary.IMEListView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 25;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Width = 80;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.label1.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.label1.Location = new System.Drawing.Point(3, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "符號表(施工中)";
            this.label1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseDown);
            this.label1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseMove);
            this.label1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseUp);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(236, 21);
            this.panel1.TabIndex = 1;
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseDown);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseMove);
            this.panel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMESymbolForm_MouseUp);
            // 
            // panel2
            // 
            this.panel2.AutoSize = true;
            this.panel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel2.Controls.Add(this.tabControl1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 21);
            this.panel2.Margin = new System.Windows.Forms.Padding(0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(236, 187);
            this.panel2.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.Padding = new System.Drawing.Point(0, 0);
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(236, 187);
            this.tabControl1.TabIndex = 1;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // tabPage1
            //             
            this.tabPage1.Controls.Add(this.m_listViews);
            this.tabPage1.Location = new System.Drawing.Point(4, 24);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(0);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(228, 159);
            this.tabPage1.TabIndex = 2;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.buttonUp);
            this.panel3.Controls.Add(this.buttonDown);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel3.Location = new System.Drawing.Point(184, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(44, 159);
            this.panel3.TabIndex = 4;
            // 
            // buttonUp
            // 
            this.buttonUp.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonUp.Location = new System.Drawing.Point(0, 0);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Size = new System.Drawing.Size(44, 100);
            this.buttonUp.TabIndex = 5;
            this.buttonUp.Text = "上";
            this.buttonUp.UseVisualStyleBackColor = true;
            this.buttonUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonUp_MouseUp);
            // 
            // buttonDown
            // 
            this.buttonDown.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonDown.Location = new System.Drawing.Point(0, 59);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Size = new System.Drawing.Size(44, 100);
            this.buttonDown.TabIndex = 4;
            this.buttonDown.Text = "下";
            this.buttonDown.UseVisualStyleBackColor = true;
            this.buttonDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonDown_MouseUp);
            // 
            // m_listViews
            // 
            this.m_listViews.Alignment = System.Windows.Forms.ListViewAlignment.SnapToGrid;
            this.m_listViews.BackColor = System.Drawing.Color.GhostWhite;
            this.m_listViews.Font = new System.Drawing.Font("PMingLiU", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.m_listViews.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_listViews.GridLines = true;
            this.m_listViews.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.m_listViews.HideSelection = false;
            this.m_listViews.Location = new System.Drawing.Point(0, 0);
            this.m_listViews.Margin = new System.Windows.Forms.Padding(0);
            this.m_listViews.Name = "m_listViews";
            this.m_listViews.Scrollable = false;
            this.m_listViews.Size = new System.Drawing.Size(121, 97);
            this.m_listViews.TabIndex = 1;
            this.m_listViews.UseCompatibleStateImageBehavior = false;
            this.m_listViews.View = System.Windows.Forms.View.Details;
            // 
            // tabPage2
            // 
            this.tabPage2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabPage2.Location = new System.Drawing.Point(4, 24);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(0);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(228, 173);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 24);
            this.tabPage3.Margin = new System.Windows.Forms.Padding(0);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(228, 173);
            this.tabPage3.TabIndex = 3;
            this.tabPage3.Text = "tabPage3";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // IMESymbolForm
            // 
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(236, 208);
            this.ControlBox = false;
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "IMESymbolForm";
            this.ShowInTaskbar = false;
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }
        #endregion

        #region public methods

        public void SetSymbols(string inputs)
        {
            //input
            string[] a_inputs = inputs.Split(' ');
            if (a_inputs == null) return;
            string[] fakeInputs ={  "1.一", "2.二", "3.三", "4.四", "5.五", "6.六", "7.七", "8.八", "9.九","(1/9)", 
              "1.十一", "2.十二", "3.十三", "4.十四", "5.十五", "6.十六", "7.十七", "8.十八", "9.十九","(2/9)" };

            //this.ShowListView(a_inputs); // not show, only setstring		            
            
            //m_inputs = a_inputs;                              
            m_inputs = fakeInputs;
            //label2.Visible=buttonUp.Visible = buttonDown.Visible = false;

            string fakeInputs2 = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";            
            m_listViews.SetCapacity(6, 6);
            m_listViews.SetContent(fakeInputs2);
           // Debug.WriteLine("this.panel2.Location.X=" + this.panel2.Location.X);
            //Debug.WriteLine("this.m_listViews.Location.X=" + this.m_listViews.Location.X);
            
            this.panel2.Margin = Padding.Empty;
            this.panel2.Padding = Padding.Empty;
            this.panel2.Width = m_listViews.Width;
            this.buttonUp.Height = panel3.Height / 2;
            this.buttonDown.Height = panel3.Height / 2;
            Debug.WriteLine("panel2 width=" + this.panel2.Width.ToString());
            Debug.WriteLine("form width=" + this.Width.ToString());
            //this.Width = this.panel2.Width;
            //this.Width = m_listViews.Width;
            //this.Height = panel1.Height+tabControl1.Height+ m_listViews.Height;
            //set form size
           /* int width = 0;// this.vScrollBar1.Width;
            int height = this.panel1.Height;
            width += this.m_listViews.Items[0].Bounds.Right;            
            height += this.m_listViews.Items[this.m_listViews.Items.Count - 1].Bounds.Bottom;
            this.Width = width;
            Debug.WriteLine("right="+this.m_listViews.Items[0].Bounds.Right.ToString());
            Debug.WriteLine("width=" + Width);
            this.Height= height + 5;   //+5為了好看
            Debug.WriteLine("listview visible?"+m_listViews.Visible.ToString());
            */            
            //m_mode = 1;    
            //this.vScrollBar1.Visible = true;
            //this.ShowListView(m_mode); // not show, only setstring                       
        }

        public void SwitchCandiMode() //expand Symbol window if possible
        {
            if (m_mode == 0)
            {
                if (totalPage > 1) //expandable
                {
                    m_mode = 1;
                }
            }
            else if (m_mode == 1)
            {
                m_mode = 0;
            }
            this.ShowListView(m_mode);
        }

        public void ResetCandi()
        {
            this.m_listViews.Columns.Clear();

        }
        public void SetLocation(int x, int y)
        {
            this.Location = new Point(x, y);
        }

        public void DisposeForm()
        {
            this.Dispose(true);
        }

        /*public int GetValue()
        {
            return this.m_listViews.SelectedIndex;
        }*/
        public void ClearSymbols()
        {
            this.m_listViews.Items.Clear();
        }

        public void SetHWND(IntPtr hwnd)
        {
            m_hwnd = hwnd;
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

        private void IMESymbolForm_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
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

        private void IMESymbolForm_MouseMove(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (isMouseDown)
            {
                Point mousePos = Control.MousePosition;
                mousePos.Offset(mouseOffset.X, mouseOffset.Y);
                Location = mousePos;
            }
        }

        private void IMESymbolForm_MouseUp(object sender,
            System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                isMouseDown = false;
            }
        }
        private void ShowListView(int mode) //mode 0:直式 1:展開
        {
            ListViewItem li;
            totalPage = 0;

            //parse & save to arraylist
            m_candArray = new ArrayList();
            for (int i = 0; i < m_inputs.Length; i++)
            {
                string cand = m_inputs[i];
                if (i == m_inputs.Length - 1)
                    m_candArray.Add(cand);
                else
                {
                    string no = cand.Substring(0, 1);
                    string candText = cand.Substring(2);
                    m_candArray.Add(candText);
                }
            }

            //page info
            string pageInfo = m_inputs[m_inputs.Length - 1];
            currentPage = Int32.Parse(pageInfo.Substring(1, pageInfo.IndexOf('/') - 1));
            string foo = pageInfo.Substring(pageInfo.IndexOf('/') + 1);
            totalPage = Int32.Parse(foo.Substring(0, foo.Length - 1));

            //set scrollbar
            //this.vScrollBar1.Enabled = (totalPage > 1);

            //init listView
            this.m_listViews.Columns.Clear();
            this.m_listViews.Items.Clear();

            switch (mode)
            {
                case 0:
                    {
                        //columns
                        this.m_listViews.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});

                        //set scrollbar                        
                        //this.vScrollBar1.Maximum = totalPage * 9;
                        //this.vScrollBar1.TabIndex = totalPage;
                        //this.vScrollBar1.Value = (currentPage - 1) * 9 + 1;

                        //new & set items
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            string cand = m_inputs[i];
                            li = new ListViewItem();
                            li.UseItemStyleForSubItems = false;
                            string no = cand.Substring(0, 1);
                            string candText = cand.Substring(2);
                            if (i == m_candArray.Count - 1)
                                li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", SystemColors.WindowText, Color.GhostWhite, this.m_listViews.Font);
                            else
                                li.SubItems[0] = new ListViewItem.ListViewSubItem(li, no, SystemColors.WindowText, Color.GhostWhite, this.m_listViews.Font);
                            li.SubItems.Add(m_candArray[i].ToString(), SystemColors.WindowText, SystemColors.Window, this.m_listViews.Font);
                            this.m_listViews.Items.Add(li);
                        }
                        break;
                    }
                case 1:
                    {
                        //set columns
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            if (i % 9 == 0)
                            {
                                this.m_listViews.Columns.Add("no", 25, HorizontalAlignment.Left);
                                this.m_listViews.Columns.Add("cand", 60, HorizontalAlignment.Left);
                            }
                        }

                        //new items                        
                        for (int i = 0; i < 9; i++)
                        {
                            if (this.m_listViews.Items.Count <= i)
                            {
                                this.m_listViews.Items.Add(new ListViewItem());
                            }
                            else
                            {
                                this.m_listViews.Items[i] = new ListViewItem();
                            }
                            this.m_listViews.Items[i].UseItemStyleForSubItems = false;
                        }

                        //set scrollbar
                        //this.vScrollBar1.Enabled = false;

                        //set items
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            int columnIndex = (i / 9) * 2; //放編號的那些 Column 的 index
                            li = this.m_listViews.Items[i % 9];
                            if (li.SubItems.Count == 1) //第一個subitem No.
                            {
                                if (columnIndex == (currentPage - 1) * 2)
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, (i % 9 + 1).ToString(), SystemColors.WindowText, Color.GhostWhite, this.m_listViews.Font);
                                else
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", SystemColors.WindowText, SystemColors.Window, this.m_listViews.Font);
                            }
                            else //其他subitem No.
                            {
                                if (columnIndex == (currentPage - 1) * 2)
                                    li.SubItems.Add((i % 9 + 1).ToString(), SystemColors.WindowText, Color.GhostWhite, this.m_listViews.Font);
                                else
                                    li.SubItems.Add("", SystemColors.WindowText, SystemColors.Window, this.m_listViews.Font);
                            }
                            li.SubItems.Add(m_candArray[i].ToString(), SystemColors.WindowText, SystemColors.Window, this.m_listViews.Font);
                        }
                        break;
                    }

                default:
                    {
                        break;
                    }
            }

            //set form size
            int width = 0;// this.vScrollBar1.Width;
            int height=  this.panel1.Height;            
            width += this.m_listViews.Items[0].Bounds.Right;
            height += this.m_listViews.Items[this.m_listViews.Items.Count-1].Bounds.Bottom;                                    
            this.Width = width ;
            this.Height = height + 5;   //+5為了好看
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
            if (!this.Visible)
                UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
        }

        public void HideNoActive()
        {
            if (this.Visible)
                UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
        }

        private void buttonUp_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv=(IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }


        private void buttonDown_MouseUp(object sender, MouseEventArgs e)
        {
            IMEListView lv = (IMEListView)this.tabControl1.SelectedTab.Controls[0];
            lv.ShowNextPage();
        }

        private void m_listViews_KeyDown(object sender, KeyEventArgs e)
        {
            /* if (totalPage > 1 && e.KeyCode==Keys.Right) //超過一頁才有展開功能
             {            
             }*/
        }

        private void m_listViews_KeyPress(object sender, KeyPressEventArgs e)
        {
            /*    if (totalPage > 1)              
                {
                    e.Handled = true;
                }*/
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            if (this.tabControl1.SelectedTab.Controls.Count==0)
            {
                IMEListView lv = new IMEListView();
                this.tabControl1.SelectedTab.Controls.Add(lv);
                string foo = "";                
                switch (this.tabControl1.SelectedIndex)
                {
                    case 0:
                        foo = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                        lv.SetCapacity(6, 6);
                        lv.SetContent(foo);
                        break;
                    case 1:
                        foo = "1 2 3 4 5 6 7";
                        lv.SetCapacity(6, 6);
                        lv.SetContent(foo);
                        break;
                    case 2:
                        foo = "ㄅ ㄆ ㄇ ㄈ";
                        lv.SetCapacity(6, 6);
                        lv.SetContent(foo);
                        break;
                    default:
                        break;
                }         
            }
            //m_listViews.ShowNextPage();
           
        }

        private void buttonUp_Click(object sender, EventArgs e)
        {

        }

        private void buttonDown_Click(object sender, EventArgs e)
        {

        }
    }
}


