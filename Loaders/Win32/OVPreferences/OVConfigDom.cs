using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace OVPreferences
{
    public class OVConfigDom
    {
        private XmlDocument m_ovConfDOM = null;
        public OVConfigDom(XmlDocument ovConfDOM)
        {
            m_ovConfDOM = ovConfDOM;
        }

        public string GetAttribute(
            string moduleName, string attrName)
        {
            XmlNode nodeModule =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/dict[@name='" + moduleName +
                        "']/key[@name='" + attrName + "']");
            if (nodeModule != null)
                return nodeModule.Attributes["value"].Value;
            else
                return null;
        }

        public string GetAttribute(
            string moduleName, string attrName, string defaultAttrValue)
        {
            if (null == GetAttribute(moduleName, attrName))
                SetAttribute(moduleName, attrName, defaultAttrValue);
            return GetAttribute(moduleName, attrName);
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
