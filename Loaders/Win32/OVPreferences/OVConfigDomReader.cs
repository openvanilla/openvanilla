using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace OVPreference.CS2
{
    public class OVConfigDomReader
    {
        private XmlDocument m_ovConfDOM = null;
        public OVConfigDomReader(XmlDocument ovConfDOM)
        {
            m_ovConfDOM = ovConfDOM;
        }
        public void GetAttribute(
            string moduleName, string attrName, string attrValue)
        {
            XmlNodeList nodeList = m_ovConfDOM.GetElementsByTagName("key");
            
        }


    }
}
