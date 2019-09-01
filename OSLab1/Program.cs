using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSLab1
{
    class Program
    {
        static void Main(string[] args)
        {
            int n = 10;
            Generate gen = new Generate();
            gen.genValue(n);
            int[] durations = gen.durations;
            int[] intervals = gen.intervals; // Это случайные значения
            Console.WriteLine("Durations:");
            for (int i = 0; i < durations.Length; i++)
            {
                Console.Write(durations[i] + " ");
            }
            Console.WriteLine();
            Console.WriteLine("Intervals:");
            for (int i = 0; i < intervals.Length; i++)
            {
                Console.Write(intervals[i] + " ");
            }
            Console.WriteLine();
            Planner planner1 = new FCFSplanner();
            planner1.Planning(n, durations, intervals);
            Planner planner2 = new RRplanner();
            planner2.Planning(n, durations, intervals);
            Console.WriteLine("FCFS:\nAverage time - " + planner1.AverageTurnroundTime + "\n" + "Waiting - " + planner1.AverageWaitTime + "\n" +
                "Max count process in queue - " + planner1.MaxQueueLength + "\n" +
                "RR2:\nAverage time - " + planner2.AverageTurnroundTime + "\n" + "Waiting - " + planner2.AverageWaitTime + "\n" +
                "Max count process in queue - " + planner2.MaxQueueLength + "\n");
            Console.ReadKey();
        }
    }
}
