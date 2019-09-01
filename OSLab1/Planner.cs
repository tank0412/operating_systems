using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSLab1
{
    abstract class Planner
    {
        protected List<int[]> timelineDiagram;
        protected int[] waitTime;
        protected int[] activeTime;
        protected int[] durations;
        protected int[] intervals;
        protected int maxQueueLength;
        public Planner()
        {
            timelineDiagram = new List<int[]>();
            waitTime = new int[0];
            activeTime = new int[0];
        }
        public int MaxQueueLength
        {
            get
            {
                return this.maxQueueLength;
            }
        }
        public double AverageTurnroundTime
        {
            get
            {
                return Math.Round(this.waitTime.Average() + this.activeTime.Average(), 2);
            }
        }
        public double AverageWaitTime
        {
            get
            {
                return Math.Round(this.waitTime.Average(), 2);
            }
        }
        public abstract void Planning(int count, int[] durations, int[] intervals);
        public abstract int GetNextFromTurn(List<int> Turnlist);
    }
}
