using System;
using System.Collections.Generic;

namespace OVPreference.CS2
{
    public class OVConfig
    {
        private string m_moduleName;
        public string moduleName
        {
            get { return m_moduleName; }
            set { m_moduleName = value; }
        }

        public Dictionary<string, string> settings =
            new Dictionary<string, string>();
    }
}
