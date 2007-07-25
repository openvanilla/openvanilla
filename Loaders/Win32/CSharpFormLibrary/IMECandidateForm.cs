using System;
using System.Diagnostics;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace CSharpFormLibrary
{
    public class IMECandidateForm : System.Windows.Forms.Form
    {
        //private System.Windows.Forms.ListView lbCandidates;
        private IMEListView lbCandidates;
        private IContainer components;
        private int currentPage = 0; //目前頁次 
        private int totalPage = 0; //傳入的 candidate 共有幾頁                 
        private ArrayList m_candArray; //傳入的 candi array
        private string[] m_inputs;
        private int m_mode = 0; //0:直式  1:展開

        //當頁的index
        //int now_index;		
        //int baseSize;	//基本長度(沒有candidate時form的大小)
        private IntPtr m_hwnd;
        private const int WM_MOUSEACTIVATE = 0x0021;
        private const int MA_ACTIVATE = 0x0001;
        private const int MA_ACTIVATEANDEAT = 0x0002;
        private const int MA_NOACTIVATE = 0x0003;
        private VScrollBar vScrollBar1;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ImageList imageList1;
        private Label label1;
        private Panel panel1;
        private Panel panel2;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        public IMECandidateForm()
        {
            InitializeComponent();
            //baseSize = this.lbCandidates.Location.Y+100;
            //this.Opacity = 1;
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        public IMECandidateForm(string[] candidates)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
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

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
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
            //this.lbCandidates = new System.Windows.Forms.ListView();
            this.lbCandidates = new IMEListView();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbCandidates
            // 
            this.lbCandidates.Alignment = System.Windows.Forms.ListViewAlignment.SnapToGrid;
            this.lbCandidates.BackColor = System.Drawing.SystemColors.Window;
            this.lbCandidates.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbCandidates.Font = new System.Drawing.Font("PMingLiU", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.lbCandidates.ForeColor = System.Drawing.SystemColors.WindowText;
            this.lbCandidates.GridLines = true;
            this.lbCandidates.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lbCandidates.HideSelection = false;
            this.lbCandidates.Name = "lbCandidates";
            this.lbCandidates.Bounds = new Rectangle(0, 0, 105, 266);
            this.lbCandidates.Scrollable = false;
            this.lbCandidates.SmallImageList = this.imageList1;
            this.lbCandidates.View = System.Windows.Forms.View.Details;
            this.lbCandidates.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbCandidates_KeyDown);
            this.lbCandidates.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.lbCandidates_KeyPress);
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(1, 25);
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // columnHeader1
            // 
            //this.columnHeader1.Width = 25;
            this.columnHeader1.Width = -1;
            // 
            // columnHeader2
            //             
            //this.columnHeader2.Width = 80;
            this.columnHeader2.Width = -1;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.Maximum = 11;
            this.vScrollBar1.Minimum = 1;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Bounds = new Rectangle(105, 0, 24, 266);
            this.vScrollBar1.Value = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.label1.Font = new System.Drawing.Font("PMingLiU", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.label1.Name = "label1";
            this.label1.Bounds = new Rectangle(3, 4, 52, 15);
            this.label1.Text = "候選字";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Name = "panel1";
            this.panel1.Bounds = new Rectangle(0, 0, 129, 21);
            // 
            // panel2
            // 
            this.panel2.AutoSize = true;
            this.panel2.Controls.Add(this.lbCandidates);
            this.panel2.Controls.Add(this.vScrollBar1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Name = "panel2";
            this.panel2.Bounds = new Rectangle(0, 21, 129, 266);
            // 
            // IMECandidateForm
            // 
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "IMECandidateForm";
            this.ShowInTaskbar = false;
            this.Deactivate += new System.EventHandler(this.IMECandidateForm_Deactivate);
            this.Enter += new System.EventHandler(this.IMECandidateForm_Enter);
            this.Activated += new System.EventHandler(this.IMECandidateForm_Activated);
            this.Validated += new System.EventHandler(this.IMECandidateForm_Validated);
            this.Click += new System.EventHandler(this.IMECandidateForm_Click);
            this.Validating += new System.ComponentModel.CancelEventHandler(this.IMECandidateForm_Validating);
            this.Leave += new System.EventHandler(this.IMECandidateForm_Leave);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();
        }
        #endregion

        #region public methods

        public void SetCandidates(string inputs)
        {
            string[] a_inputs;
            if ((inputs.IndexOf('\t')) > 0)
                a_inputs = inputs.Split('\t');
            else
            {
                //<comment author='b6s'>
                // A temporary patch of the special case of
                // "shift selection key" by space.
                bool isStartWithSpace = false;
                if (inputs.StartsWith(" ."))
                {
                    isStartWithSpace = true;
                    inputs = inputs.TrimStart();
                }
                //</comment>

                a_inputs = inputs.Split(' ');

                //<comment author='b6s'>
                // Appends the heading space back for the spacial case of
                // "shift selection key."
                if (isStartWithSpace &&
                    a_inputs != null && a_inputs.Length > 0)
                    a_inputs[0] = " " + a_inputs[0];
                //</comment>
            }
            
            if (a_inputs == null) return;
            //string[] fakeInputs =
                //{"1.一", "2.二", "3.三", "4.四", "5.五", "6.六", "7.七", "8.八", "9.九","(1/9)", 
                //"1.十一", "2.十二", "3.十三", "4.十四", "5.十五", "6.十六", "7.十七", "8.十八", "9.十九","(2/9)" };

            //this.ShowListView(a_inputs); // not show, only setstring		            
            m_mode = 0;

            //傳入cand List
            //m_inputs = fakeInputs;
            m_inputs = a_inputs;
            this.ShowListView(m_mode); // not show, only setstring                       
        }

        public void SwitchCandiMode() //expand candidate window if possible
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
            this.lbCandidates.Columns.Clear();

        }

        public void SetLocation(int x, int y, int compHeight)
        {
            this.Location = new Point(x, y + compHeight);
            if (this.Bottom >
                Screen.PrimaryScreen.WorkingArea.Bottom)
                this.Location =
                    new Point(
                        this.Location.X,
                        this.Location.Y - this.Height - compHeight);
            if (this.Right >
                Screen.PrimaryScreen.WorkingArea.Right)
                this.Location =
                    new Point(this.Location.X - this.Width, this.Location.Y);             
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
            this.vScrollBar1.Enabled = (totalPage > 1);

            //init listView
            this.lbCandidates.Columns.Clear();
            this.lbCandidates.Items.Clear();

            switch (mode)
            {
                case 0:
                    {
                        //columns
                        this.lbCandidates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});

                        //set scrollbar                        
                        this.vScrollBar1.Maximum = totalPage * 9;
                        this.vScrollBar1.TabIndex = totalPage;
                        this.vScrollBar1.Value = (currentPage - 1) * 9 + 1;

                        //new & set items
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            string cand = m_inputs[i];
                            li = new ListViewItem();
                            li.UseItemStyleForSubItems = false;
                            string no = cand.Substring(0, 1);
                            string candText = cand.Substring(2);
                            if (i == m_candArray.Count - 1)
                                li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", SystemColors.WindowText, Color.GhostWhite, this.lbCandidates.Font);
                            else
                                li.SubItems[0] = new ListViewItem.ListViewSubItem(li, no, SystemColors.WindowText, Color.GhostWhite, this.lbCandidates.Font);
                            li.SubItems.Add(m_candArray[i].ToString(), SystemColors.WindowText, SystemColors.Window, this.lbCandidates.Font);
                            this.lbCandidates.Items.Add(li);
                        }
                        this.columnHeader1.Width = -1;
                        this.columnHeader2.Width = -1;
                        break;
                    }
                case 1:
                    {
                        //set columns
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            if (i % 9 == 0)
                            {
                                this.lbCandidates.Columns.Add("no", 25, HorizontalAlignment.Left);
                                this.lbCandidates.Columns.Add("cand", 60, HorizontalAlignment.Left);
                            }
                        }

                        //new items                        
                        for (int i = 0; i < 9; i++)
                        {
                            if (this.lbCandidates.Items.Count <= i)
                            {
                                this.lbCandidates.Items.Add(new ListViewItem());
                            }
                            else
                            {
                                this.lbCandidates.Items[i] = new ListViewItem();
                            }
                            this.lbCandidates.Items[i].UseItemStyleForSubItems = false;
                        }

                        //set scrollbar
                        this.vScrollBar1.Enabled = false;

                        //set items
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            int columnIndex = (i / 9) * 2; //放編號的那些 Column 的 index
                            li = this.lbCandidates.Items[i % 9];
                            if (li.SubItems.Count == 1) //第一個subitem No.
                            {
                                if (columnIndex == (currentPage - 1) * 2)
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, (i % 9 + 1).ToString(), SystemColors.WindowText, Color.GhostWhite, this.lbCandidates.Font);
                                else
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", SystemColors.WindowText, SystemColors.Window, this.lbCandidates.Font);
                            }
                            else //其他subitem No.
                            {
                                if (columnIndex == (currentPage - 1) * 2)
                                    li.SubItems.Add((i % 9 + 1).ToString(), SystemColors.WindowText, Color.GhostWhite, this.lbCandidates.Font);
                                else
                                    li.SubItems.Add("", SystemColors.WindowText, SystemColors.Window, this.lbCandidates.Font);
                            }
                            li.SubItems.Add(m_candArray[i].ToString(), SystemColors.WindowText, SystemColors.Window, this.lbCandidates.Font);
                        }
                        break;
                    }

                default:
                    {
                        break;
                    }
            }

            //set form size
            int width = this.vScrollBar1.Width;
            int height=  this.panel1.Height;            
            width += this.lbCandidates.Items[0].Bounds.Right;
            height += this.lbCandidates.Items[this.lbCandidates.Items.Count-1].Bounds.Bottom;                                    
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

        private void lbCandidates_KeyDown(object sender, KeyEventArgs e)
        {
            /* if (totalPage > 1 && e.KeyCode==Keys.Right) //超過一頁才有展開功能
             {
                 //expandable = true;                
                 MessageBox.Show("fuck");
             }*/
        }

        private void lbCandidates_KeyPress(object sender, KeyPressEventArgs e)
        {
            /*    if (totalPage > 1)              
                {
                    e.Handled = true;
                    MessageBox.Show("fuck");
                }*/
        }
    }
}


