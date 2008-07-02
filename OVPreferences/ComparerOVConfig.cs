using System;
using System.Collections.Generic;
using System.Text;

namespace OVPreferences
{
    public enum EnumSortBy { Priority, Enabled }
    public enum EnumSortOrder { Ascending, Descending }

    public class ComparerOVConfig : IComparer<OVConfig>
    {
        private EnumSortBy m_sortBy = EnumSortBy.Priority;
        public EnumSortBy SortBy
        {
            get { return m_sortBy; }
            set { m_sortBy = value; }
        }
        
        private EnumSortOrder m_sortOrder = EnumSortOrder.Descending;
        public EnumSortOrder SortOrder
        {
            get { return m_sortOrder; }
            set { m_sortOrder = value; }
        }

        public int Compare(OVConfig x, OVConfig y)
        {
            int diff = 0;
            switch (m_sortBy)
            {
                case EnumSortBy.Priority:
                    int priorityX = 0;
                    int priorityY = 0;
                    if (x.settings.ContainsKey("priority"))
                        priorityX = Convert.ToInt32(x.settings["priority"]);
                    if (x.moduleType.Equals( "OVInputMethod"))
                        priorityX += 2000;
                    if (x.moduleType.Equals("OVKeyPreprocessor"))
                        priorityX += 1000;

                    if (y.settings.ContainsKey("priority"))
                        priorityY = Convert.ToInt32(y.settings["priority"]);
                    if (y.moduleType.Equals( "OVInputMethod"))
                        priorityY += 2000;
                    if (y.moduleType.Equals( "OVKeyPreprocessor"))
                        priorityY += 1000;
                    
                    diff = priorityX - priorityY;                        
                    break;
                case EnumSortBy.Enabled:
                    int enabledX = 0;
                    int enabledY = 0;
                    if (x.settings.ContainsKey("enable"))
                        enabledX = Convert.ToInt32(x.settings["enable"]);
                    if (y.settings.ContainsKey("enable"))
                        enabledY = Convert.ToInt32(y.settings["enable"]);

                    diff = enabledX - enabledY;
                    break;
                default:
                    break;
            }

            if (m_sortOrder.Equals(EnumSortOrder.Descending))
                return -diff;
            else
                return diff;
        }
    }
}
