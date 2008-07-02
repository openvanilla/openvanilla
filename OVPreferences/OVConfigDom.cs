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
        public string GetGlobalAttribute(
            string GlobalName, string attrName)
        {
            XmlNode nodeModule =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/Global[@name='" + GlobalName +
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
        public string GetGlobalAttribute(
            string GlobalName, string attrName, string defaultAttrValue)
        {
            if (null == GetGlobalAttribute(GlobalName, attrName))
                SetGlobalAttribute(GlobalName, attrName, defaultAttrValue);
            return GetGlobalAttribute(GlobalName, attrName);
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
        public void SetGlobalAttribute(
            string GlobalName, string attrName, string attrValue)
        {
            XmlNode globalModule =
                m_ovConfDOM.SelectSingleNode(
                "/OpenVanilla/Global[@name='" + GlobalName+ "']");

            if (globalModule == null)
            {
                XmlElement element =
                 m_ovConfDOM.CreateElement("Global");
                element.SetAttribute("name", GlobalName);
                

                globalModule =
                    m_ovConfDOM.SelectSingleNode(
                        "/OpenVanilla");
                globalModule.AppendChild(element);
            }

            XmlNode nodeModule =
                m_ovConfDOM.SelectSingleNode(
                    "/OpenVanilla/Global[@name='" + GlobalName +
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
                        "/OpenVanilla/Global[@name='" + GlobalName + "']");
                nodeModule.AppendChild(element);
            }
        }
    }
}
