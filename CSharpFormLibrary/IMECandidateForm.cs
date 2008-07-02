using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace OVManagedUI
{
    public class IMECandidateForm : System.Windows.Forms.Form
    {
        //private System.Windows.Forms.ListView lbCandidates;
        private OVManagedUI.IMEListView lbCandidates;
        private IContainer components;
        private int m_currentPage = 0; //目前頁次 
        private int m_totalPage = 0; //傳入的 candidate 共有幾頁                 
        private ArrayList m_candArray; //傳入的 candi array
        private string[] m_candidates;
        private int m_formStyle = 0; //0:直式  1:展開
        private int m_compHeight = 0;
        private int basewidth = 0;
        private int selected_Y = -1;
        private int offset_X = 0;
        static object candilocker = new object();
        static object selectItemlocker = new object();



        private Color m_borderColor =        
        Color.FromArgb(255,
                ((int)(((byte)(80)))),
                ((int)(((byte)(80)))),
                ((int)(((byte)(80)))));
        private Color m_topColor =
            Color.FromArgb(255,
                ((int)(((byte)(169)))),
                ((int)(((byte)(169)))),
                ((int)(((byte)(169)))));
        
        private Color m_backColor =
                Color.FromArgb(255,
                ((int)(((byte)(100)))),
                ((int)(((byte)(100)))),
                ((int)(((byte)(100)))));

        private Color m_textColor = Color.White;
                
        private Color l_textColor =  SystemColors.WindowText;
        private Color l_backColor = SystemColors.Window;


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
        private Label m_lbTitle;
        private Panel panel1;
        private Panel panel2;
        private const int MA_NOACTIVATEANDEAT = 0x0004;

        private bool m_isFormSizeSet = false;

        private bool inOnMouse=false;

        protected void MyOnMouseDown(Point pt)
        {
            //this.SelectDown();
            //MessageBox.Show("original pt=" + pt.ToString());
            if (!inOnMouse)
            {
                inOnMouse = true;
                this.SelectItem(this.PointToClient(pt));
                inOnMouse = false;
            }
        }

        protected override void WndProc(ref Message m)
        {

            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE )// WM_MOUSEACTIVATE)
            {
                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;
                MyOnMouseDown(Control.MousePosition);
                
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
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.m_lbTitle = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.lbCandidates = new OVManagedUI.IMEListView();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(1, 25);
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "1";
            this.columnHeader1.Width = -2;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "一二三四";
            this.columnHeader2.Width = -2;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.Location = new System.Drawing.Point(131, 0);
            this.vScrollBar1.Maximum = 11;
            this.vScrollBar1.Minimum = 1;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(24, 229);
            this.vScrollBar1.TabIndex = 1;
            this.vScrollBar1.Value = 1;
            this.vScrollBar1.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.vScrollBar_MouseWhell);
            this.vScrollBar1.ValueChanged += new System.EventHandler(this.vScrollBar_Valuechanged);
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar_Scroll);
            // 
            // m_lbTitle
            // 
            this.m_lbTitle.AutoSize = true;
            this.m_lbTitle.BackColor = System.Drawing.Color.Transparent;
            this.m_lbTitle.Font = new System.Drawing.Font("Microsoft JhengHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_lbTitle.ForeColor = System.Drawing.SystemColors.Window;
            this.m_lbTitle.Location = new System.Drawing.Point(59, 4);
            this.m_lbTitle.Name = "m_lbTitle";
            this.m_lbTitle.Size = new System.Drawing.Size(73, 20);
            this.m_lbTitle.TabIndex = 0;
            this.m_lbTitle.Text = "候選字詞";
            this.m_lbTitle.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.InfoText;
            this.panel1.Controls.Add(this.m_lbTitle);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Font = new System.Drawing.Font("Microsoft JhengHei", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.panel1.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(155, 29);
            this.panel1.TabIndex = 1;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // panel2
            // 
            this.panel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel2.Controls.Add(this.lbCandidates);
            this.panel2.Controls.Add(this.vScrollBar1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 29);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(155, 229);
            this.panel2.TabIndex = 0;
            // 
            // lbCandidates
            // 
            this.lbCandidates.Activation = System.Windows.Forms.ItemActivation.TwoClick;
            this.lbCandidates.Alignment = System.Windows.Forms.ListViewAlignment.Default;
            this.lbCandidates.BackColor = System.Drawing.Color.GhostWhite;
            this.lbCandidates.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbCandidates.Font = new System.Drawing.Font("Microsoft JhengHei", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.lbCandidates.ForeColor = System.Drawing.SystemColors.WindowText;
            this.lbCandidates.FullRowSelect = true;
            this.lbCandidates.GridLines = true;
            this.lbCandidates.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lbCandidates.Location = new System.Drawing.Point(0, 0);
            this.lbCandidates.Margin = new System.Windows.Forms.Padding(0);
            this.lbCandidates.MultiSelect = false;
            this.lbCandidates.Name = "lbCandidates";
            this.lbCandidates.Scrollable = false;
            this.lbCandidates.Size = new System.Drawing.Size(131, 229);
            this.lbCandidates.SmallImageList = this.imageList1;
            this.lbCandidates.TabIndex = 0;
            this.lbCandidates.UseCompatibleStateImageBehavior = false;
            this.lbCandidates.View = System.Windows.Forms.View.Details;
            this.lbCandidates.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.lbCandidates_ItemSelectionChanged);
            // 
            // IMECandidateForm
            // 
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(155, 258);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "IMECandidateForm";
            this.ShowInTaskbar = false;
            this.Deactivate += new System.EventHandler(this.IMECandidateForm_Deactivate);
            this.Activated += new System.EventHandler(this.IMECandidateForm_Activated);
            this.Enter += new System.EventHandler(this.IMECandidateForm_Enter);
            this.Leave += new System.EventHandler(this.IMECandidateForm_Leave);
            this.Validating += new System.ComponentModel.CancelEventHandler(this.IMECandidateForm_Validating);
            this.Validated += new System.EventHandler(this.IMECandidateForm_Validated);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        
                
        void lbCandidates_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("selection item changed:" + e.ItemIndex);
            if (e.ItemIndex == selected_Y)
            {
                // item deselected
                this.lbCandidates.Items[e.ItemIndex].SubItems[0].ForeColor = l_textColor;
                this.lbCandidates.Items[e.ItemIndex].SubItems[1].ForeColor = l_textColor;
                this.lbCandidates.Items[e.ItemIndex].SubItems[0].BackColor = l_backColor;
                this.lbCandidates.Items[e.ItemIndex].SubItems[1].BackColor = l_backColor;
            }
            else
            {  // new item selected
                selected_Y = e.ItemIndex;
                this.lbCandidates.Items[e.ItemIndex].SubItems[0].ForeColor = SystemColors.HighlightText;
                this.lbCandidates.Items[e.ItemIndex].SubItems[1].ForeColor = SystemColors.HighlightText;
                this.lbCandidates.Items[e.ItemIndex].SubItems[0].BackColor = SystemColors.Highlight;
                this.lbCandidates.Items[e.ItemIndex].SubItems[1].BackColor = SystemColors.Highlight;
            }
            
        }

        
        #endregion

        #region public methods
        public IMECandidateForm()
        {
            InitializeComponent();
            //baseSize = this.lbCandidates.Location.Y+100;
            this.Opacity = 0.95;
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();

            setbasewidth();         
                        

        }

        public IMECandidateForm(string[] candidates)
        {
            InitializeComponent();
            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);

            setbasewidth();
            
        }

        public void SetColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
        {
            /*
            m_textColor = Color.FromArgb(titlecolor);
            m_backColor = Color.FromArgb(titlebackcolor);
            m_topColor = Color.FromArgb(
                (int)m_backColor.A,
                ((int)m_backColor.R) * 169 / 100,
                ((int)m_backColor.G) * 169 / 100,
                ((int)m_backColor.B) * 169 / 100);
            m_borderColor = Color.FromArgb(
                (int)m_backColor.A,
                ((int)m_backColor.R) * 80 / 100,
                ((int)m_backColor.G) * 80 / 100,
                ((int)m_backColor.B) * 80 / 100);
            l_backColor = Color.FromArgb(backcolor);
            l_textColor = Color.FromArgb(forecolor);
            System.Diagnostics.Debug.WriteLine("Candicolors" + m_textColor.ToArgb().ToString());
             */
        }
        public void SetFont(string ff, int fontsize)
        {
            System.Diagnostics.Debug.Write("SetCandiFont facename:" + ff);
            lbCandidates.Font = new Font(ff, fontsize);
            m_lbTitle.Font = new Font(ff, fontsize);
            
        }


        private void setbasewidth()
        {
            this.lbCandidates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
                            this.columnHeader1, this.columnHeader2});

            ListViewItem li = new ListViewItem();
            li.SubItems[0] = new ListViewItem.ListViewSubItem(li,"&", l_textColor, l_backColor, this.lbCandidates.Font);
            li.SubItems.Add("一二一二", l_textColor, l_backColor, this.lbCandidates.Font);
            this.lbCandidates.Items.Add(li);

            this.columnHeader1.Width = (int)(this.lbCandidates.Font.GetHeight()*1.3f);
            this.columnHeader2.Width = -1;

            basewidth = this.lbCandidates.Items[0].Bounds.Right + this.vScrollBar1.Width;
            this.Width = basewidth;
            //ShowNoActive();

            System.Diagnostics.Debug.WriteLine("basewidth:" + basewidth.ToString());

        }

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

        public void SetCandidates(string candidateSequence)
        {
            lock (candilocker)
            {
                System.Diagnostics.Debug.WriteLine(candidateSequence);
                if (this.lbCandidates.Items.Count > 0) this.ClearCandidates();  // clear candi pages first.
                selected_Y = -1;

                string[] candidates;
                if ((candidateSequence.IndexOf('\t')) > 0)
                    candidates =
                        candidateSequence.Split(
                            new char[] { '\t' },
                            StringSplitOptions.RemoveEmptyEntries);
                else
                {
                    //<comment author='b6s'>
                    // A temporary patch of the special case of
                    // "shift selection key" by space.
                    bool isStartWithSpace = false;
                    if (candidateSequence.StartsWith(" "))//."))
                    {
                        isStartWithSpace = true;
                        candidateSequence = candidateSequence.TrimStart();
                    }
                    //</comment>

                    candidates =
                        candidateSequence.Split(
                            new char[] { ' ' },
                            StringSplitOptions.RemoveEmptyEntries);

                    //<comment author='b6s'>
                    // Appends the heading space back for the spacial case of
                    // "shift selection key."
                    if (isStartWithSpace &&
                        candidates != null && candidates.Length > 0)
                        candidates[0] = " " + candidates[0];
                    //</comment>
                }

                if (candidates == null) return;
                m_formStyle = 0;

                m_candidates = candidates;
                

                this.ShowListView(m_formStyle); // not show, only setstring                       

            }


        }

        public void SwitchCandiMode() //expand candidate window if possible
        {
            if (m_formStyle == 0)
            {
                if (m_totalPage > 1) //expandable
                {
                    m_formStyle = 1;
                }
            }
            else if (m_formStyle == 1)
            {
                m_formStyle = 0;
            }
            this.ShowListView(m_formStyle);
        }

        public void ResetCandi()
        {
            this.lbCandidates.Columns.Clear();

        }

        public void SetLocation(int x, int y, int compHeight)
        {
            
                m_compHeight = compHeight;

                //this.Location = new Point(x, y + m_compHeight *3/2);

                //@warning A damn state flag, needs refactoring ASAP.
                //if (m_isFormSizeSet)
               // {
                    
                    this.Location = new Point(x-offset_X, y + m_compHeight * 3 / 2);

                 //   this.panel1.Refresh();
                 //   this.panel2.Refresh();
                 //   m_isFormSizeSet = false;

                //}
                //else this.Location = new Point(x, y + m_compHeight * 3 / 2);
                //duckScreenBoundry();
            

        }

        public void SetOpacity(int o)
        {
            this.Opacity = o / 100f;
        }

        public void selectNextItem(int i)
        {
            lock (candilocker)    // wait unitl the new candi page is updated
            {


                if (i > 0)  // advance one item
                {
                    if (selected_Y < this.lbCandidates.Items.Count - 1)  //selected_Y++;
                        this.lbCandidates.Items[selected_Y + 1].Selected = true;
                    else // end of page. go to next page.
                    {
                        SendKeys.SendWait("{RIGHT}");
                        selected_Y = -1;
                    }

                }
                else  // go back one item
                {
                    if (selected_Y > 0)
                        this.lbCandidates.Items[selected_Y - 1].Selected = true;
                    else  // beginning of page. goto previous page.
                    {
                       SendKeys.SendWait("{LEFT}");
                       selected_Y = this.lbCandidates.Items.Count;
                    }
                }
               
           }


        }
        public int getSelectedItemKey()
        {

            if (selected_Y > -1 && selected_Y < this.lbCandidates.Items.Count)
            {
                char key = m_candidates[selected_Y].ToCharArray()[1];
                if (key == '.') return (int)m_candidates[selected_Y].ToString().ToCharArray()[0];
                else return (int) key;

            }
            else
                return 0;
            

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
                //const int WS_EX_NOACTIVATE = 0x08000000;
                //CreateParams cp = base.CreateParams;
                //cp.ExStyle = cp.ExStyle | WS_EX_NOACTIVATE;
                //return cp;//
            }
        }

        #endregion

        #region private methods

        private void ShowListView(int mode) //mode 0:直式 1:展開
        {            
            //l_textColor = m_textColor;
            //l_backColor = m_backColor;
            this.lbCandidates.ForeColor = l_textColor;
            this.lbCandidates.BackColor = l_backColor;
            

            ListViewItem li;
            m_totalPage = 0;
            

            //parse & save to arraylist
            m_candArray = new ArrayList();
            for (int i = 0; i < m_candidates.Length; i++)
            {
                string cand = m_candidates[i];
                if (i == m_candidates.Length - 1)
                    m_candArray.Add(cand);
                else
                {
                    string no = cand.Substring(0, 1);
                    string candText = cand.Substring(2);
                    m_candArray.Add(candText);
                }
            }

            //page info
            
            string pageInfo = m_candidates[m_candidates.Length - 1];

            System.Diagnostics.Debug.WriteLine(pageInfo + pageInfo.IndexOf("(").ToString());
            pageInfo = pageInfo.Substring(pageInfo.IndexOf("("));
            System.Diagnostics.Debug.WriteLine(pageInfo);
            
            m_currentPage = Int32.Parse(pageInfo.Substring(1, pageInfo.IndexOf('/') - 1));
            string foo = pageInfo.Substring(pageInfo.IndexOf('/') + 1);
            
            //m_totalPage = Int32.Parse(foo.Substring(0, foo.Length - 1));
            m_totalPage = Int32.Parse(foo.Substring(0, foo.IndexOf(")")));

            //set scrollbar
            this.vScrollBar1.Enabled = (m_totalPage > 1);

            //init listView
            this.lbCandidates.Columns.Clear();
            this.lbCandidates.Items.Clear();

            switch (mode)
            {
                case 0:
                    {
                        //columns
                        this.lbCandidates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
                            this.columnHeader1, this.columnHeader2});
            
                        //set scrollbar                        
                        this.vScrollBar1.Maximum = m_totalPage * 9;
                        //this.vScrollBar1.LargeChange = 9;
                        this.vScrollBar1.SmallChange = 3;
                        //this.vScrollBar1.TabIndex = m_totalPage;
                        this.vScrollBar1.Value = (m_currentPage - 1) * 9 + 1;
                        

                        //new & set items
                        
                        for (int i = 0; i < m_candArray.Count; i++)
                        {
                            string cand = m_candidates[i];
                            li = new ListViewItem();
                            li.UseItemStyleForSubItems = false;
                            string no = cand.Substring(0, 1);
                            string candText = cand.Substring(2);
                            if (i == m_candArray.Count - 1)
                            {
                                //li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", l_textColor, l_backColor, this.lbCandidates.Font);
                                this.m_lbTitle.Text = m_candArray[i].ToString();
                                this.m_lbTitle.Location = new Point(
                                    (this.Width-this.m_lbTitle.Width)*8/10                                   
                                    , this.m_lbTitle.Location.Y);

                                //  li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", SystemColors.InfoText, Color.GhostWhite, this.lbCandidates.Font);
                            }
                            else
                            {
                                li.SubItems[0] = new ListViewItem.ListViewSubItem(li, no, l_textColor, l_backColor, this.lbCandidates.Font);
                                //                     li.SubItems[0] = new ListViewItem.ListViewSubItem(li, no, SystemColors.InfoText, Color.GhostWhite, this.lbCandidates.Font);

                                li.SubItems.Add(m_candArray[i].ToString(), l_textColor, l_backColor, this.lbCandidates.Font);
                                this.lbCandidates.Items.Add(li);
                            }
                            //li.SubItems.Add(m_candArray[i].ToString(), l_textColor, l_backColor, this.lbCandidates.Font);
           //                 li.SubItems.Add(m_candArray[i].ToString(), SystemColors.InfoText, Color.GhostWhite, this.lbCandidates.Font);
                            //this.lbCandidates.Items.Add(li);
                            
                        }
                        this.columnHeader1.Width = (int) (this.lbCandidates.Font.GetHeight() *1.3f);
                        //this.columnHeader2.Width = basewidth;
                        this.columnHeader2.Width = -2;
                        this.lbCandidates.Columns[0].TextAlign = HorizontalAlignment.Center;
                        this.lbCandidates.Columns[1].TextAlign = HorizontalAlignment.Left;
                                            
                        
                        
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
                                this.lbCandidates.Columns.Add("cand", 120, HorizontalAlignment.Left);
                                
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
                                if (columnIndex == (m_currentPage - 1) * 2)
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, (i % 9 + 1).ToString(), l_textColor, l_backColor, this.lbCandidates.Font);
                                else
                                    li.SubItems[0] = new ListViewItem.ListViewSubItem(li, "", l_textColor, l_backColor, this.lbCandidates.Font);
                            }
                            else //其他subitem No.
                            {
                                if (columnIndex == (m_currentPage - 1) * 2)
                                    li.SubItems.Add((i % 9 + 1).ToString(), l_textColor, l_backColor, this.lbCandidates.Font);
                                else
                                    li.SubItems.Add("", l_textColor, l_backColor, this.lbCandidates.Font);
                            }
                            li.SubItems.Add(m_candArray[i].ToString(), l_textColor, l_backColor, this.lbCandidates.Font);
                        }
                        break;
                    }

                default:
                    {
                        break;
                    }
            }


            

            //set form size
            
            int width = this.lbCandidates.Items[0].Bounds.Right;
            int height=  this.panel1.Height;

            width += this.vScrollBar1.Width;
            height += this.lbCandidates.Items[this.lbCandidates.Items.Count-1].Bounds.Bottom;

            offset_X = width - basewidth;
            System.Diagnostics.Debug.WriteLine("NewWidth:" + width.ToString() + ";basewidth:" + basewidth.ToString() + ";offset_X:" + offset_X.ToString());

            this.Width = width;           
            this.Height = height +5  ;   //+5為了好看
                     
            
            //@warning A damn state flag, needs refactoring ASAP.
            m_isFormSizeSet = true;
            //duckScreenBoundry();

              this.panel2.Refresh();
              this.panel1.Refresh();



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

        
        private void duckScreenBoundry()
        {
            if (this.Bottom >
                //Screen.PrimaryScreen.WorkingArea.Bottom)
                Screen.GetWorkingArea(this).Bottom)
                this.Location =
                    new Point(
                        this.Location.X,
                        this.Location.Y - this.Height - m_compHeight * 2);
            if (this.Right >
                //Screen.PrimaryScreen.WorkingArea.Right)
                Screen.GetWorkingArea(this).Right)
                this.Location =
                    new Point(this.Location.X - this.Width, this.Location.Y);
            
        }
        private void panel1_Paint(object sender, PaintEventArgs pea)
        {

            //Text = "候選字詞";
            //Font = new Font("Microsoft JhengHei", 12.0f);
            //SizeF stringSize = new SizeF();
            //SizeF charSize = new SizeF();
            //stringSize = pea.Graphics.MeasureString(Text, Font);
            //charSize = pea.Graphics.MeasureString("A", Font);

            this.panel1.Height = m_lbTitle.Height + 6;
            //this.panel1.Height = this.panel3.Height;

            //this.panel1.BringToFront();

            //this.panel1.Width = (int)(stringSize.Width + 4);

            
           // ButtonBorderStyle m_buttonBorderStyle = ButtonBorderStyle.Outset;

            SizeF sizeF =
                new SizeF(
                    (float)this.Width,
                    (float)this.panel1.Height / 5.0f);

            RectangleF rectF = new RectangleF(new PointF(0.0f, 0.0f), sizeF);
            Brush b = new LinearGradientBrush(
                rectF, m_topColor, m_backColor,LinearGradientMode.Vertical);
            pea.Graphics.FillRectangle(b, rectF);

            rectF = new RectangleF(
                new PointF(0.0f, sizeF.Height), new SizeF(sizeF.Width, sizeF.Height * 4));

            b = new System.Drawing.SolidBrush(m_backColor);
            pea.Graphics.FillRectangle(b, rectF);

            /*
            //draw text
            b = new SolidBrush(m_textColor);

            PointF pt = new PointF //((Width - stringSize.Width) * 0.5f, 0);
                  ((Width - stringSize.Width) * 0.2f, (this.panel1.Height - stringSize.Height) * 0.7f);
            pea.Graphics.DrawString(Text, Font, b, pt);
             */

            b.Dispose();
            //ControlPaint.DrawBorder(
            //    pea.Graphics, ClientRectangle, m_backColor, m_buttonBorderStyle);

           

        }

        private void vScrollBar_Valuechanged(object sender, EventArgs e)
        {


            //MessageBox.Show("value changed"+ this.vScrollBar1.Value.ToString());
        }


        private void vScrollBar_Scroll(object sender, EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("vscroll value:" + vScrollBar1.Value.ToString() + ";currentpage:" + m_currentPage.ToString());
            if (vScrollBar1.Value == m_currentPage || vScrollBar1.Value == (m_currentPage - 1) * 9) return;
            if (vScrollBar1.Value > (m_currentPage - 1) * 9)
            {
                //for (int i = 0; i < (vScrollBar1.Value - (m_currentPage - 1) * 9) / 9; i++)
                for (int i = 0; i < (vScrollBar1.Value/9 - (m_currentPage - 1) ); i++)
                    SendKeys.SendWait("{RIGHT}");
            }
            //if ((vScrollBar1.Value -1)/9 < m_currentPage  )
            else
            {
                for (int i = 0; i < (m_currentPage - ((vScrollBar1.Value) / 9)); i++)
                {
                    System.Diagnostics.Debug.WriteLine("Scroll up:" + i.ToString());
                    SendKeys.SendWait("{LEFT}");
                }
            }

            //MessageBox.Show("scroll" + this.vScrollBar1.ToString());
        }

        private void vScrollBar_MouseWhell(object sender, EventArgs e)
        {


            //MessageBox.Show("mousewhell" + this.vScrollBar1.ToString());
        }
        /*
        private void panel3_Paint(object sender, PaintEventArgs pea)
        {


            ButtonBorderStyle m_buttonBorderStyle = ButtonBorderStyle.Outset;


            RectangleF rectF = new RectangleF(new PointF(0.0f, 0.0f),
                    new SizeF((float)this.Width, (float)this.panel3.Height));


            Brush b = new System.Drawing.SolidBrush(m_backColor);
            pea.Graphics.FillRectangle(b, rectF);

            b.Dispose();
            ControlPaint.DrawBorder(
                pea.Graphics, ClientRectangle,
                    m_backColor, m_buttonBorderStyle);

            this.panel2.BringToFront();
        }
         */

        private void SelectItem(Point pt)
        {
            
            // Get the item at the mouse pointer.
            pt.Y -= this.panel1.Height;
            ListViewHitTestInfo info = this.lbCandidates.HitTest(pt.X, pt.Y);
            System.Diagnostics.Debug.WriteLine("clicked at X:" + pt.X + "clicked at Y:" + pt.Y+"panel1 height"+this.panel1.Height);
            ListViewItem.ListViewSubItem subItem = null;
            if (info != null)
                if (info.Item != null)
                    subItem = info.Item.GetSubItemAt(pt.X, pt.Y);

            if (subItem != null)
            {
                int i = this.lbCandidates.Items.IndexOf(info.Item);
                                
                if (i > -1 && i == selected_Y) // item double clicked.
                {
                    int ret = UtilFuncs.SendMessage(
                    new IntPtr((long)m_hwnd),
                    (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                    0xE, //IMN_PRIVATE
                    12);
                    
                }

                if (i >= 0 && i < this.lbCandidates.Items.Count ) // select the item.
                    this.lbCandidates.Items[i].Selected = true;
               
            }
        }

        
        #endregion
       


    }


}