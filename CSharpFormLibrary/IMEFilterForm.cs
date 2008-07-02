using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace OVManagedUI
{
    public partial class IMEFilterForm : Form
    {
        private ArrayList list=new ArrayList();        

        public IMEFilterForm()
        {
            InitializeComponent();
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

        public void SetSymbols(string[] inputs)
        {            
            lv.BackColor = Color.GhostWhite;
            lv.BorderStyle = BorderStyle.Fixed3D;
            lv.SetCapacity(7, 7);
            lv.SetContent(inputs[0]);                            
        }

        private void IMEFilterForm_Load(object sender, EventArgs e)
        {

          
        }

        public void SetPossibleList(string inputs)
        {
            list.AddRange(inputs.Split(' '));
            listBox1.Items.Clear();
            listBox1.Items.AddRange(inputs.Split(' '));
            SetFilterOptions();
        }

        public void SetFilterOptions()
        {
            string group0 = "£t£u£v£w£x£y£z£{£|£}£~£¡£¢£££¤£¥£¦£§£¨£©£ª";
            string group1 = "£¸£¹£º";
            string group2 = "£«£¬£­£®£¯£°£±£²£³£´£µ£¶";
            string group3 = "£¾£¿£½£»";
                        
            for (int j=0 ; j<1;j++)            
            {
                string spell=(string)list[j];
                for(int i=0;i< spell.Length;i++)
                {                    
                    if (group0.IndexOf(spell[i])!=-1)
                    {
                        //if (j == 0)
                        {
                            checkBoxPho0.Text = spell[i].ToString();
                        }
                        checkBoxPho0.Visible = true;                                            
                    }
                    else if (group1.IndexOf(spell[i]) != -1)
                    {
                        //if (j == 0)
                        {
                            checkBoxPho1.Text = spell[i].ToString();
                        }
                        checkBoxPho1.Visible = true;                    
                    }
                    else if (group2.IndexOf(spell[i]) != -1)
                    {
                        //if (j == 0)
                        {
                            checkBoxPho2.Text = spell[i].ToString();
                        }
                        checkBoxPho2.Visible = true;                    
                    }
                    
                    if (j == 0)
                    {
                        if (i == (spell.Length - 1)) //possible be tone
                        {
                            if (spell[i] == '£½')
                            {
                                checkBoxTone2.Visible = true;
                            }
                            else if (spell[i] == '£¾')
                            {
                                checkBoxTone3.Visible = true;
                            }
                            else if (spell[i] == '£¿')
                            {
                                checkBoxTone4.Visible = true;
                            }
                            else if (spell[i] == '£»')
                            {
                                checkBoxTone7.Visible = true;
                            
                            }
                            else // £¸Án
                            {
                                checkBoxTone1.Visible = true;
                            }
                        }
                    }
                }
            }

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBoxTone1_CheckedChanged(object sender, EventArgs e)
        {

        }        
    }
}