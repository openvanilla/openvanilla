using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace OVPreferences
{
    public class OVConfigDomWriter
    {
        private XmlDocument m_ovConfDOM = null;
        public OVConfigDomWriter(XmlDocument ovConfDOM)
        {
            m_ovConfDOM = ovConfDOM;
        }

        public void SetAttribute(
            string moduleName, string attrName, string attrValue)
        {
            XmlNode nodeModule =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" + moduleName +
                        "']/key[@name='" + attrName + "']");
            if (nodeModule != null)
                nodeModule.Attributes["value"].Value = attrValue;
            else
            {
                XmlElement element =
                    m_ovConfDOM.CreateElement("key");
                element.SetAttribute("name", attrName);
                element.SetAttribute("value", attrValue);

                nodeModule =
                    m_ovConfDOM.SelectSingleNode(
                        "/OpenVanilla/dict[@name='" + moduleName + "']");
                nodeModule.AppendChild(element);
            }
        }
    }
}
