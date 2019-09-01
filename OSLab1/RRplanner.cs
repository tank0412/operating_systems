using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSLab1
{
    class RRplanner : Planner
    {
        public override void Planning(int count, int[] durations, int[] intervals)
        {// сброс предыдущих показаний
            this.activeTime = new int[count];
            this.waitTime = new int[count];
            this.maxQueueLength = 0;
            this.durations = durations;
            this.intervals = intervals;
            // инициализация новых переменных
            // регистр состояний процессов (0 - не выполняется, 1 - ожидает, 2 - выполняется)
            int[] ProcessStatus = new int[count];
            // указатель на активный (выполняющийся) процесс
            int indexOfActiveProcess = 0;
            // указатель на следующий поступающий в очередь процесс
            int indexOfNextProcess = 0;
            // время до поступления в очередь следующего процессаint currentInterval = 0;
            int period = 2;
            int periodleft = period;
            int finishedcount = 0;
            // очередь
            List<int> TurnList = new List<int>();
            bool periodEnds;
            bool durationEnds;
            ProcessStatus[indexOfActiveProcess] = 2;
            for (int t = 0; finishedcount < count; ++t)
            {
                if (indexOfNextProcess < intervals.Length && intervals[indexOfNextProcess] == 0)
                {
                    TurnList.Add(indexOfNextProcess);
                    this.maxQueueLength = Math.Max(this.maxQueueLength, TurnList.Count);
                    ProcessStatus[indexOfNextProcess] = 1;
                    indexOfNextProcess++;
                }
                periodEnds = periodleft == 0;
                durationEnds = durations[indexOfActiveProcess] == 0;
                if (periodEnds || durationEnds)
                {
                    if (!durationEnds)
                    {
                        TurnList.Add(indexOfActiveProcess);
                        ProcessStatus[indexOfActiveProcess] = 1;
                    }
                    else
                    {
                        ProcessStatus[indexOfActiveProcess] = 0;
                        finishedcount++;
                    }
                    TurnList.Remove(indexOfActiveProcess);
                    periodleft = period;
                    if (TurnList.Count > 0)
                    {
                        indexOfActiveProcess = GetNextFromTurn(TurnList);
                        ProcessStatus[indexOfActiveProcess] = 2;
                    }
                }


                if (indexOfNextProcess < intervals.Length)
                {
                    --intervals[indexOfNextProcess];
                }
                if (TurnList.Count > 0)
                {
                    --durations[indexOfActiveProcess];
                }
                periodleft--;
                // заполняем информационную часть новыми состояниями                
                for (int i = 0; i < count; ++i)
                {
                    if (ProcessStatus[i] == 1) ++waitTime[i];
                    if (ProcessStatus[i] == 2) ++activeTime[i];
                }
            }
        }
        public override int GetNextFromTurn(List<int> Turnlist)
        {
            return Turnlist.First();
        }
    }
}

