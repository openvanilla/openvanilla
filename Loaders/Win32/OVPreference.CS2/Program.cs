using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Resources;
using System.Globalization;

namespace OVPreference.CS2
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();

            //It would be much better to initialize the thread¡¦s UICulture and Culture 
            //when the thread is first created, and decouple it from a specific form¡¦s code. 
            //This can be handled in the Main() entry point
            //System.Threading.Thread.CurrentThread.CurrentUICulture = 
            //    new System.Globalization.CultureInfo(Properties.Settings.Default.UICulture);

            //System.Threading.Thread.CurrentThread.CurrentCulture =
            //System.Globalization.CultureInfo.CreateSpecificCulture
            //      (Properties.Settings.Default.Culture);

            //ResourceManager resourceMgr = Resource_ch_TW.ResourceManager;
            //ResourceManager resourceMgr = 
            //    ResourceManager.CreateFileBasedResourceManager("Resource",@"C:\Program Files\ResourceSet", null);

            //CultureInfo ci = Resource_ch_TW.Culture;// new CultureInfo("ch-TW");

            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new OVPrefrence());
        }
    }
}