using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

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

        private const int MA_NOACTIVATEANDEAT = 0x0004;

        public IMEListView()
        {
            this.BackColor = Color.GhostWhite;
            this.SetStyle(
            //    ControlStyles.UserPaint |
                //ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
        }

        #region protected methods
     
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle = 0x00000004; //WS_EX_NOPARENTNOTIFY
                return cp;
            }
        }

        protected void MyOnMouseDown()
        {
            this.SelectDown();
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                MyOnMouseDown();
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

        #endregion
        #region public methods
        public void SetCapacity(int maxRowNum, int maxColumnNum)
        {
            
            m_maxRowNum = maxRowNum;
            m_maxColumnNum = maxColumnNum;

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
            string[] content = inputs.Split(' ');
            int itemIndex;
            int subItemIndex;
            ListViewItem li;            
            for (int i = 0; i < content.Length; i++)
            {                
                subItemIndex = i / m_maxColumnNum; //放編號的那些 Column 的 index
                itemIndex = i % m_maxRowNum;
                li = this.Items[itemIndex];
                if (i < m_maxRowNum) //第一個用new的方式
                {                    
                    li.SubItems[0]=new ListViewItem.ListViewSubItem(li,content[i],SystemColors.WindowText,Color.GhostWhite,Font);
                }
                else //其他用add的方式
                {                    
                    li.SubItems.Add(content[i].ToString(), SystemColors.WindowText, Color.GhostWhite, Font);
                }
            }
            selectedX = selectedY = 0;
            MarkSelected();
        }
        public void SelectDown()  //向下
        {
            UnMarkSelected();
            if(selectedY+1 >= m_maxRowNum)
            {
                if (selectedX+1 >= m_maxColumnNum)
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
                selectedY++;
            MarkSelected();
        }
        #endregion
    }
}

