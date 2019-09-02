using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
namespace OSLab1
{
    public class Generate
    {
        bool isInterval;
        public void genValue(int count)
        {
            isInterval = false;
            this.durations = RandomValuesGenerate(count, minDurationValue, maxDurationValue);
            isInterval = true;
            this.intervals = RandomValuesGenerate(count, minIntervalValue, maxIntervalValue);
            this.intervals[0] = 0;
        }
        private int[] RandomValuesGenerate(int count, int minValue, int maxValue)
        {
            int[] GeneratedArray = new int[count];
            Random R = new Random();
            for (int i = 0; i < count; i++)
            {
                if (isInterval != true)
                {
                    GeneratedArray[i] = R.Next(minValue, maxValue);
                }
                else {
                    int Temp = R.Next(minValue, maxValue); // потому что запрещены временные парадоксы
                    minValue = Temp;
                    GeneratedArray[i] = Temp;
                }
            }
            return GeneratedArray;
        }
        public int[] durations;
        public int[] intervals;
        private readonly int minDurationValue = 3; //Длительность
        private readonly int maxDurationValue = 9;
        private readonly int minIntervalValue = 1; //Интервал (ТОЛЬКО по возрастанию)
        private readonly int maxIntervalValue = 7;
    }
}
