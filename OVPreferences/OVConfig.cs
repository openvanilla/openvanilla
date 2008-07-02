using System;
using System.Collections.Generic;

namespace OVPreferences
{
    public class OVConfig
    {
        private string m_moduleName;
        private string m_moduleType;
        private string m_zh_TWName;
        private string m_zh_CNName;
        private string m_enName;
        
        public string moduleName
        {
            get { return m_moduleName; }
            set { m_moduleName = value; }
        }
        public string moduleType
        {
            get { return m_moduleType; }
            set { m_moduleType = value; }
        }
        public string zh_TWName
        {
            get { return m_zh_TWName; }
            set { m_zh_TWName = value; }
        }
        public string zh_CNName
        {
            get { return m_zh_CNName; }
            set { m_zh_CNName = value; }
        }
        public string enName
        {
            get { return m_enName; }
            set { m_enName = value; }
        }

        public Dictionary<string, string> settings =
            new Dictionary<string, string>();
    }
}
