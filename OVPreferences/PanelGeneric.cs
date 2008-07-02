using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Diagnostics;

namespace OVPreferences
{
    public partial class PanelGeneric : UserControl
    {
        protected OVConfig m_ovConf = null;
        protected XmlDocument m_ovConfDOM = null;
        protected OVConfigDom m_ovConfDomHandler = null;

        protected Hashtable htFamiliearName = new  Hashtable();

        private bool m_doAutoCompose;
        private bool m_doHitMaxAndCompose;
        private int m_maxKeySequenceLength;
        private bool m_doShiftSelectionKey;
        private bool m_doWarningBeep;
        private bool m_doWildcard;
        private string m_matchOneChar = "?";
        private string m_matchZeroOrMoreChar = "*";
        private string m_selectKey;
        private bool m_doAssociatedPhrase;
        private bool m_doOrderWordsByFreq;



        public PanelGeneric()
        {
            InitializeComponent();
        }

        public virtual void Init(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;
            m_ovConfDomHandler = new OVConfigDom(m_ovConfDOM);

            LoadSettings();
        }

        protected virtual void LoadSettings()
        {
            m_doAutoCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "autoCompose", "0")));
            m_doHitMaxAndCompose =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "hitMaxAndCompose", "0")));
            m_maxKeySequenceLength =
                Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "maxKeySequenceLength", "4"));
            m_doShiftSelectionKey =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "shiftSelectionKey", "1")));
            m_doWarningBeep =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "warningBeep", "0")));
            m_doWildcard =
                Convert.ToBoolean(Convert.ToInt32(
                    m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "wildcard", "1")));
            m_selectKey = m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "selectKey");
            if (m_selectKey==null)
            {
                m_tbselectKey.Visible = false;
                m_lbselectKey.Visible = false;
            }
            else
            {
                m_tbselectKey.Text = m_selectKey;    
            }

            m_doAssociatedPhrase = 
                Convert.ToBoolean(Convert.ToInt32(
                m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "associatedPhrase", "1")));
            m_doOrderWordsByFreq = 
                Convert.ToBoolean(Convert.ToInt32(
                m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "orderWordsByFreq","0")));

            m_matchOneChar = m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "matchOneChar", "?");
            m_matchZeroOrMoreChar = m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "matchZeroOrMoreChar", "*");


            m_cbAutoCompose.Checked = m_doAutoCompose;
            m_cbHitMaxAndCompose.Checked = m_doHitMaxAndCompose;
            m_tbmaxKeySequenceLength.Text = m_maxKeySequenceLength.ToString();
            m_cbShiftSelectionKey.Checked = m_doShiftSelectionKey;
            m_cbWarningBeep.Checked = m_doWarningBeep;
            m_cbWildCard.Checked = m_doWildcard;
            m_cbAssociatedPhrase.Checked = m_doAssociatedPhrase;
            m_cbOrderWordsByFreq.Checked = m_doOrderWordsByFreq;
            m_tbMatchOneChar.Text = m_matchOneChar.Substring(0, 1);
            m_tbMatchZeroOrMoreChar.Text = m_matchZeroOrMoreChar.Substring(0, 1);


            
        }

        private void m_cbShiftSelectionKey_CheckedChanged(object sender, EventArgs e)
        {
            m_doShiftSelectionKey = m_cbShiftSelectionKey.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "shiftSelectionKey",
                m_doShiftSelectionKey ? "1" : "0");
        }

        private void m_cbAutoCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doAutoCompose = m_cbAutoCompose.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "autoCompose",
                m_doAutoCompose ? "1" : "0");
        }

        private void m_cbHitMaxAndCompose_CheckedChanged(object sender, EventArgs e)
        {
            m_doHitMaxAndCompose = m_cbHitMaxAndCompose.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "hitMaxAndCompose",
                m_doHitMaxAndCompose ? "1" : "0");
        }

        private void m_cbWarningBeep_CheckedChanged(object sender, EventArgs e)
        {
            m_doWarningBeep = m_cbWarningBeep.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "warningBeep",
                m_doWarningBeep ? "1" : "0");
        }
        private void m_cbWildcard_CheckedChanged(object sender, EventArgs e)
        {
            m_doWildcard = m_cbWildCard.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "wildcard",
                m_doWildcard ? "1" : "0");
        }
        void m_tbmaxKeySequenceLength_TextChanged(object sender, System.EventArgs e)
        {
            if (m_tbmaxKeySequenceLength.Text.Length == 0) return;
            try
            {
                m_maxKeySequenceLength =
                    Convert.ToInt32(m_tbmaxKeySequenceLength.Text);
            }
            catch
            {
                m_maxKeySequenceLength = 0;
            }
            if (m_maxKeySequenceLength == 0)
            {
                MessageBox.Show("Positive Integer Please!");
                m_maxKeySequenceLength = 5;
                return;
            }

            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "maxKeySequenceLength",
                m_maxKeySequenceLength.ToString());
        }

        void m_tbselectKey_TextChanged(object sender, System.EventArgs e)
        {
            if (m_tbselectKey.Text.Length == 0) return;
                
            m_selectKey = m_tbselectKey.Text;
            
            
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "selectKey",
                m_selectKey);

        }

        private void m_cbAssociatedPhrase_CheckedChanged(object sender, EventArgs e)
        {
            m_doAssociatedPhrase = m_cbAssociatedPhrase.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "associatedPhrase",
                m_doAssociatedPhrase ? "1" : "0");

        }

        private void m_cbOrderWordsByFreq_CheckedChanged(object sender, EventArgs e)
        {
            m_doOrderWordsByFreq = m_cbOrderWordsByFreq.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "orderWordsByFreq",
                m_doOrderWordsByFreq ? "1" : "0");

        }

        void m_tbMatchZeroOrMoreChar_TextChanged(object sender, System.EventArgs e)
        {
            if ( m_tbMatchZeroOrMoreChar.Text.Length == 0) return;

            m_matchZeroOrMoreChar = m_tbMatchZeroOrMoreChar.Text;


            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "matchZeroOrMoreChar",
                m_matchZeroOrMoreChar);

        }

        void m_tbMatchOneChar_TextChanged(object sender, System.EventArgs e)
        {
            if (m_tbMatchOneChar.Text.Length == 0) return;

            m_matchOneChar = m_tbMatchOneChar.Text;


            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "matchOneChar",
                m_matchOneChar);
        }

 
       
    }
}