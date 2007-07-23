using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreference.CS2
{
    public partial class PanelModuleList : UserControl
    {
        private XmlDocument m_ovConfDOM = null;
        private bool m_isMouseDown = false;
        private int m_draggingIndex = -1;
        private OVConfigDomWriter m_ovConfDomWriter = null;
        private Point m_previousLineBegin;
        private Point m_previousLineEnd;

        public PanelModuleList(XmlDocument ovConfDOM)
        {
            m_ovConfDOM = ovConfDOM;
            m_ovConfDomWriter = new OVConfigDomWriter(m_ovConfDOM);

            InitializeComponent();
        }

        private void SetModulePriority()
        {
            int priority = m_clModuleList.Items.Count;
            foreach (object item in m_clModuleList.Items)
            {
                string moduleName = item.ToString();
                m_ovConfDomWriter.SetAttribute(
                    moduleName, "priority", priority.ToString());
                priority--;
            }
        }

        private void m_clModuleList_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (e.Index < 0) return;
            string checkedModuleName =
                m_clModuleList.Items[e.Index].ToString();
            m_ovConfDomWriter.SetAttribute(
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
                    return;
                }

                object item = m_clModuleList.Items[m_draggingIndex];
                bool isItemChecked =
                    m_clModuleList.GetItemChecked(m_draggingIndex);

                m_clModuleList.Items.RemoveAt(m_draggingIndex);

                //<comment author='b6s'> It seems to be the bottom
                if (targetIndex == -1)
                {
                    m_clModuleList.Items.Add(item);
                    targetIndex = m_clModuleList.Items.Count - 1;
                }
                //</comment>
                else
                    m_clModuleList.Items.Insert(targetIndex, item);

                SetModulePriority();
                m_clModuleList.SetItemChecked(targetIndex, isItemChecked);

                m_draggingIndex = -1;
                m_isMouseDown = false;
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
    }
}
