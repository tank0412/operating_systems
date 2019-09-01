using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
namespace OSLab1
{
    public class Generate
    {
        public void genValue(int count)
        {
            this.durations = RandomValuesGenerate(count, minDurationValue, maxDurationValue);
            this.intervals = RandomValuesGenerate(count, minIntervalValue, maxIntervalValue);
            this.intervals[0] = 0;
        }
        private int[] RandomValuesGenerate(int count, int minValue, int maxValue)
        {
            int[] GeneratedArray = new int[count];
            Random R = new Random();
            for (int i = 0; i < count; i++)
            {
                GeneratedArray[i] = R.Next(minValue, maxValue);
            }
            return GeneratedArray;
        }
        public int[] durations;
        public int[] intervals;
        private readonly int minDurationValue = 4;
        private readonly int maxDurationValue = 8;
        private readonly int minIntervalValue = 3;
        private readonly int maxIntervalValue = 9;
    }
}
