using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace CSharpFormLibrary
{            
    public class IMEListView : System.Windows.Forms.ListView
    {
        private int m_maxRowNum = 6;
        private int m_maxColumnNum = 9;
        private int m_columnNum =0;
        private int m_rowNum = 0;
        private int selectedX=-1;
        private int selectedY=-1;
        private int pageNo_all = 0;
        private int pageNo_now = 0;
        private int pageCapacity = 0;
        private string[] content;
        private int contentNumInPage=0;
        private ImageList imageList1;
        private IContainer components;

        private const int MA_NOACTIVATEANDEAT = 0x0004;

        public IMEListView()
        {
            
            this.SetStyle(
               // ControlStyles.UserPaint |
                //ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();

            //components
            this.components = new System.ComponentModel.Container();
            // 
            // imageList1
            // 
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            //this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(1, 25);
            //this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            //
            //listview
            //
            Alignment = System.Windows.Forms.ListViewAlignment.Default;
            BackColor = System.Drawing.Color.GhostWhite;
            ForeColor = System.Drawing.SystemColors.WindowText;
            GridLines = true;
            //HideSelection = false;
            //Location = new System.Drawing.Point(0, 0);
            Margin = new System.Windows.Forms.Padding(0);
            Padding = new System.Windows.Forms.Padding(0);
            Scrollable = false;
            Font = new System.Drawing.Font("PMingLiU", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            SmallImageList = this.imageList1;
            //TabIndex = 1;
            //Size = new System.Drawing.Size(284, 211);
            UseCompatibleStateImageBehavior = false;
            View = System.Windows.Forms.View.Details;
        }

        #region protected methods

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
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                return cp;
            }
        }
        protected void MyOnMouseDown(Point pt)
        {
            //this.SelectDown();
            //MessageBox.Show("original pt=" + pt.ToString());
            this.SelectItem(this.PointToClient(pt));
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                MyOnMouseDown(MousePosition);
            }
        }

        private void SelectItem(Point pt)
        {
            // Get the item at the mouse pointer.            
            ListViewHitTestInfo info = this.HitTest(pt.X, pt.Y);            
            ListViewItem.ListViewSubItem subItem = null;            
            if (info != null)
                if (info.Item != null)
                    subItem = info.Item.GetSubItemAt(pt.X , pt.Y);

            // Show the text of the subitem, if found.
            if (subItem != null)
            {
                //MessageBox.Show(subItem.Text);
                int tmpY = this.Items.IndexOf(info.Item);
                int tmpX = info.Item.SubItems.IndexOf(subItem);                
                if (tmpY >= 0 && tmpX >= 0)
                {
                    UnMarkSelected();
                    selectedY = tmpY;
                    selectedX = tmpX;
                    MarkSelected();
                }
            }
        }

        private void MarkSelected()
        {
            this.Items[selectedY].SubItems[selectedX].BackColor = Color.DimGray;
            this.Items[selectedY].SubItems[selectedX].ForeColor = Color.White;
        }

        private void UnMarkSelected()
        {
            this.Items[selectedY].SubItems[selectedX].BackColor = Color.GhostWhite;
            this.Items[selectedY].SubItems[selectedX].ForeColor = SystemColors.WindowText;
        }
        private void ShowPage(int pageNo)
        {
            ClearPage();
            pageNo_now = pageNo;
            if (pageNo >= 0 && pageNo < pageNo_all) //合理的page
            {
                int itemIndex;
                int subItemIndex;
                ListViewItem li;
                if (pageNo == pageNo_all - 1) //last page
                {
                    contentNumInPage = content.Length - pageNo * pageCapacity;
                }
                else // filled page 
                {
                    contentNumInPage = pageCapacity;
                }
                for (int i = 0; i < pageCapacity; i++)
                {
                    subItemIndex = i / m_maxColumnNum; //放編號的那些 Column 的 index
                    itemIndex = i % m_maxRowNum;
                    li = this.Items[itemIndex];
                    if (i < contentNumInPage) //有值
                    {
                        if (i < m_maxRowNum) //第一個用new的方式
                        {
                            li.SubItems[0] = new ListViewItem.ListViewSubItem(li, content[i + pageNo * pageCapacity], SystemColors.WindowText, Color.GhostWhite, Font);
                        }
                        else //其他用add的方式
                        {
                            li.SubItems.Add(content[i + pageNo * pageCapacity].ToString(), SystemColors.WindowText, Color.GhostWhite, Font);
                        }
                    }
                    /*
                                        else //清空
                                        {
                                            if (i < m_maxRowNum) //第一個用new的方式
                                            {
                                                li.SubItems.RemoveAt(0);
                                            }
                                            else //其他用add的方式
                                            {
                                                li.SubItems.RemoveAt();
                                            }
                                        }*/

                }

                selectedX = selectedY = 0;
                MarkSelected();
            }
        }
        #endregion
        #region public methods
        public string GetSelectedItem()
        {
            return this.Items[selectedY].SubItems[selectedX].Text;
        }

        public void SetCapacity(int maxRowNum, int maxColumnNum)
        {
            pageCapacity = maxRowNum * maxColumnNum;
            m_maxRowNum = maxRowNum;
            m_maxColumnNum = maxColumnNum;

            ClearPage();
             
            this.Width = 30 * maxColumnNum;
            this.Height = this.Items[0].Bounds.Height* maxRowNum;
        }

        public void ClearPage()
        {
            Columns.Clear();
            Items.Clear();

            //init columns            
            for (int i = 0; i < m_maxColumnNum; i++)
            {
                this.Columns.Add("", 30, HorizontalAlignment.Center);
            }

            //init rows                       
            for (int i = 0; i < m_maxRowNum; i++)
            {
                this.Items.Add("");
                this.Items[i].UseItemStyleForSubItems = false;
            }
        }

        public void SetContent(string inputs)
        {
            content = inputs.Split(' ');
            if (content.Length == 0) return;
            pageNo_all = ((content.Length - 1) / pageCapacity )+1;
            ShowPage(0);            
        }

        public void ShowNextPage()
        {
            if (pageNo_now + 1 == pageNo_all)
                ShowPage(0);
            else
                ShowPage(pageNo_now+1);
        }

        public void SelectDown()  //向下
        {
            UnMarkSelected();
            if (selectedY + 1 >= m_maxRowNum)
            {
                if (selectedX + 1 >= m_maxColumnNum)
                {
                    selectedX = selectedY = 0;
                }
                else
                {
                    selectedY = 0;
                    selectedX++;
                }
            }
            else
            {                
                selectedY++;                
                    
            }
            if (selectedX * m_maxRowNum + selectedY >= contentNumInPage) //不合法規零
                selectedX = selectedY = 0;
            MarkSelected();
        }
        #endregion

    }
}

