using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Globalization;

namespace OVPreferences
{
    public partial class PanelModuleList : UserControl
    {
        
        private XmlDocument m_ovConfDOM = null;
        private bool m_isMouseDown = false;
        private int m_draggingIndex = -1;
        private OVConfigDom m_ovConfDomHandler = null;
        private Point m_previousLineBegin;
        private Point m_previousLineEnd;
        private Panel m_panelOptions;
        private Label m_labelOptions;
        private List<OVConfig> m_ovConfList = null;
        private int imCount=0;
        private int kpCount=0;
        private int ofCount=0;

        public PanelModuleList(XmlDocument ovConfDOM, List<OVConfig> ovConfList , Panel panelOptions, Label labelOptions)
        {
            m_ovConfDOM = ovConfDOM;
            m_ovConfList = ovConfList;
            m_ovConfDomHandler = new OVConfigDom(m_ovConfDOM);
            m_panelOptions = panelOptions;
            m_labelOptions = labelOptions;

            InitializeComponent();
        }

        private void SetModulePriority()
        {
            //int priority = m_ovConfList.Count; // m_clModuleList.Items.Count;
            int priority = imCount;
            for (int i = 0; i < imCount; i++)
            {
                
                m_ovConfDomHandler.SetAttribute(
                    m_ovConfList[i].moduleName, "priority", priority.ToString());
                priority--;
            }

            priority = kpCount;
            for (int i = imCount; i < imCount+kpCount; i++)
            {
                m_ovConfDomHandler.SetAttribute(
                    m_ovConfList[i].moduleName, "priority", priority.ToString());
                priority--;
            }
            priority = ofCount;
            for (int i = imCount+ kpCount; i < imCount+kpCount+ofCount; i++)
            {
                m_ovConfDomHandler.SetAttribute(
                    m_ovConfList[i].moduleName, "priority", priority.ToString());
                priority--;
            }

            /*
            foreach (OVConfig item in m_ovConfList) //m_clModuleList.Items)
            {

                string moduleName = item.moduleName;   //item.ToString();
                m_ovConfDomHandler.SetAttribute(
                    moduleName, "priority", priority.ToString());
                priority--;
            }
             */
        }

        
        private void m_clModuleList_ItemCheck(object sender, ItemCheckEventArgs e)
        {     

        
            if (e.Index < 0) return;
            string checkedModuleName = m_ovConfList[e.Index].moduleName;

                
            if (checkedModuleName != "OVOFFullWidthCharacter")
                 m_ovConfDomHandler.SetAttribute(
                    checkedModuleName,
                    "enable",
                    e.NewValue.Equals(CheckState.Checked) ? "1" : "0");

            
        }

        private void m_clModuleList_MouseDown(object sender, MouseEventArgs e)
        {
            m_isMouseDown = true;
        }

        private void m_clModuleList_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_isMouseDown) return;
            if (m_clModuleList.Items.Count == 0) return;

            m_draggingIndex = m_clModuleList.IndexFromPoint(e.X, e.Y);
            if (m_draggingIndex < 0)
            {
                m_isMouseDown = false;
                return;
            }
                m_clModuleList.DoDragDrop(
                m_clModuleList.Items[m_draggingIndex], DragDropEffects.Move);
        }

        private void m_clModuleList_MouseUp(object sender, MouseEventArgs e)
        {
            m_isMouseDown = false;
        }

        private void m_clModuleList_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.StringFormat))
            {
                int targetIndex =
                    m_clModuleList.IndexFromPoint(
                        m_clModuleList.PointToClient(new Point(e.X, e.Y)));

                if (targetIndex == m_draggingIndex)
                {
                    m_draggingIndex = -1;
                    m_isMouseDown = false;
                    m_clModuleList.Refresh();
                    return;
                }

                if(m_draggingIndex< imCount&& (targetIndex>=imCount|| targetIndex==-1))
                {
                    if (targetIndex == -1) targetIndex = 0; // move IM to the fist IM position
                    if (targetIndex >= imCount) targetIndex = imCount-1; // move IM to last IM position
                }
                if (m_draggingIndex < (imCount+kpCount) && (m_draggingIndex >= imCount)
                    && (targetIndex < imCount || targetIndex>= imCount+kpCount))
                {
                    if (targetIndex < imCount) targetIndex = imCount; // move KP to the fist KP position
                    if (targetIndex >= imCount+kpCount) targetIndex = imCount+kpCount-1; // move KP to last KP position
                }
                if (m_draggingIndex < (imCount + kpCount + ofCount) && (m_draggingIndex >= (imCount+kpCount))
                    && (targetIndex < (imCount+kpCount)))
                {
                    if (targetIndex < imCount+kpCount) targetIndex = imCount+kpCount; // move OF to the fist OF position
                    
                }


                object item = m_clModuleList.Items[m_draggingIndex];
                OVConfig conf = m_ovConfList[m_draggingIndex];
                
                bool isItemChecked =
                    m_clModuleList.GetItemChecked(m_draggingIndex);

                try { m_clModuleList.Items.RemoveAt(m_draggingIndex); }
                catch  {  }
                try { m_ovConfList.RemoveAt(m_draggingIndex); }
                catch { }


                //<comment author='b6s'> It seems to be the bottom
                if (targetIndex == -1)
                {
                    m_clModuleList.Items.Add(item);
                    m_ovConfList.Add(conf);
                    targetIndex = m_clModuleList.Items.Count - 1;
                }
                //</comment>
                else
                {
                    m_clModuleList.Items.Insert(targetIndex, item);
                    m_ovConfList.Insert(targetIndex, conf);
                }

                SetModulePriority();
                m_clModuleList.SetItemChecked(targetIndex, isItemChecked);

                m_draggingIndex = -1;
                m_isMouseDown = false;
                m_clModuleList.Refresh();
            }
        }

        private void m_clModuleList_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.Text))
            {
                e.Effect = DragDropEffects.Move;
                int overIndex =
                    m_clModuleList.IndexFromPoint(
                        m_clModuleList.PointToClient(new Point(e.X, e.Y)));

                Rectangle r = m_clModuleList.GetItemRectangle(overIndex);
                Graphics g = m_clModuleList.CreateGraphics();
                if(!m_previousLineBegin.IsEmpty && !m_previousLineEnd.IsEmpty)
                    g.DrawLine(
                        new Pen(m_clModuleList.BackColor),
                        m_previousLineBegin,
                        m_previousLineEnd);
                m_previousLineBegin =
                    new Point(
                        r.Left,
                        overIndex > m_draggingIndex? r.Bottom: r.Top);
                m_previousLineEnd =
                    new Point(r.Right,
                        overIndex > m_draggingIndex? r.Bottom: r.Top);
                g.DrawLine(
                    new Pen(m_clModuleList.ForeColor),
                    m_previousLineBegin,
                    m_previousLineEnd);
            }
            else
                e.Effect = DragDropEffects.None;
        }

        private void showPanel()
        {
            if (m_clModuleList.SelectedIndex > -1)
            {
                m_labelOptions.Text = m_ovConfList[m_clModuleList.SelectedIndex].zh_TWName;
                //    m_ovConfDomHandler.GetAttribute(m_clModuleList.Items[m_clModuleList.SelectedIndex].ToString(), "LocalizedName-zh_TW");
                System.Diagnostics.Debug.WriteLine("show panel:" + m_clModuleList.SelectedIndex.ToString());
                System.Diagnostics.Debug.WriteLine("show panel:" + m_clModuleList.Items[m_clModuleList.SelectedIndex].ToString());
                System.Diagnostics.Debug.WriteLine("modulename:" + m_ovConfList[m_clModuleList.SelectedIndex].moduleName);
            }

            if (m_ovConfList[m_clModuleList.SelectedIndex].moduleName == "OVOFFullWidthCharacter")
                m_clModuleList.SetItemChecked(m_clModuleList.SelectedIndex, true);

            
                

            //m_labelOptions.Text = m_clModuleList.Items[m_clModuleList.SelectedIndex].ToString();
            //m_labelOptions.Text = "";
            ControlCollection c = m_panelOptions.Controls;
            
            foreach (Control item in c)
            {

                if (m_clModuleList.SelectedIndex > -1 
                    && item.Name.Equals( m_ovConfList[m_clModuleList.SelectedIndex].moduleName))
                {
                    item.Visible = true;
                    item.BringToFront();
                }
                else
                {
                    item.Visible = false;
                }
            }
        }

        private void m_clModuleList_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            showPanel();
        }

        public void start()
        {
            if (m_clModuleList.Items.Count > 0 && m_clModuleList.SelectedIndex==-1)
             m_clModuleList.SelectedIndex = 0;
            showPanel();
        }
        void m_clModuleList_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            
            if (e.KeyCode == Keys.Delete)
            {
                System.Diagnostics.Debug.WriteLine("Key-UP:del selected_index:"+ m_clModuleList.SelectedIndex.ToString());
                int i = m_clModuleList.SelectedIndex;
                if (i>-1 &&
                    MessageBox.Show("Are you sure you want to delete this item", "OVPreference", MessageBoxButtons.OKCancel) == DialogResult.OK)
                {
                    
                    XmlNode currentnode = m_ovConfDOM.SelectSingleNode(
                                "/OpenVanilla/dict[@name='" + m_ovConfList[i].moduleName +"']");
                    
                    XmlNode root = m_ovConfDOM.DocumentElement;
                    root.RemoveChild(currentnode);
                    
                    m_clModuleList.Items.RemoveAt(i);
                    m_ovConfList.RemoveAt(i);

                    m_clModuleList.SelectedIndex = 0;
                }
            }
        }
    }
}
