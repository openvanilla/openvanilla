using System;
using System.Collections.Generic;
using System.Text;

namespace OVPreference.CS2
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
                    diff =
                        Convert.ToInt32(x.settings["priority"]) -
                        Convert.ToInt32(y.settings["priority"]);
                    break;
                case EnumSortBy.Enabled:
                    diff =
                        Convert.ToInt32(x.settings["enabled"]) -
                        Convert.ToInt32(y.settings["enabled"]);
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
