using System;
using System.Collections;
using System.Text;
using System.Drawing;

namespace OVPreferences
{
 
    public enum KnownColorFilter
    {
        System,
        Web,
        All
    };

    public class KnownColorCollection : ColorCollection
    {

        protected readonly int COUNT;
        protected readonly int FIRST;
        protected readonly int LAST;

        public KnownColorCollection(KnownColorFilter filter)
        {
            switch (filter)
            {
                case KnownColorFilter.All: COUNT = 167; FIRST = 1; break;
                case KnownColorFilter.System: COUNT = 27; FIRST = 1; break;
                case KnownColorFilter.Web: COUNT = 140; FIRST = 28; break;
            }
            LAST = FIRST + COUNT;
        }

        public int Count
        {
            get { return COUNT; }
        }

        public IEnumerator GetEnumerator()
        {
            return new KnownColorEnumerator(this);
        }

        public Color this[int iColor]
        {
            get
            {
                //+2 is because preventing the exception due to "More Colors" option!
                if (iColor < 0 || iColor >= Count +2) throw new ArgumentOutOfRangeException();
                return Color.FromKnownColor((KnownColor)(iColor + FIRST));
            }
        }

        public Color this[string szColor]
        {
            get
            {
                if (szColor.Length == 0) throw new ArgumentNullException();
                return Color.FromName(szColor);
            }
        }

        public int IndexOf(string ColorName)
        {
            for (int i = FIRST; i < LAST; i++)
            {
                if (Color.FromKnownColor((KnownColor)i).Name.Equals(ColorName)) return i - FIRST;
            }
            return -1;
        }

        #region KnownColorEnumerator

        private class KnownColorEnumerator : System.Collections.IEnumerator
        {
            private int m_Location;
            private ColorCollection m_ColorCollection;

            public KnownColorEnumerator(ColorCollection colors)
            {
                m_ColorCollection = colors;
                m_Location = -1;
            }

            public bool MoveNext()
            {
                return (++m_Location < m_ColorCollection.Count);
            }

            public Object Current
            {
                get
                {
                    if (m_Location < 0 || m_Location > m_ColorCollection.Count) throw new InvalidOperationException();
                    return m_ColorCollection[m_Location];
                }
            }

            public void Reset()
            {
                m_Location = -1;
            }
        }
        #endregion KnownColorEnumerator
    }
}
