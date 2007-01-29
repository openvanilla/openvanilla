using System;
using System.EnterpriseServices.Internal;
namespace GacUtil
{
    public class Program
    {        
        static void Main(string[] args)
        {
            Publish p = new Publish();
            if(args[0] == "install")
            {
                p.GacInstall(args[1]);
            }
            else if (args[0] == "uninstall")
            {                
                p.GacRemove(args[1]);
            }            
        }
    }
}
