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
                    ProcessStatus[indexOfNextProcess] = 1; //пометить процесс как ожидающий
                    indexOfNextProcess++; 
                }
                periodEnds = periodleft == 0; //проверка закончился ли квант
                durationEnds = durations[indexOfActiveProcess] == 0; //проверка закончилась ли длительность процесса
                if (periodEnds || durationEnds) //если да
                {
                    if (!durationEnds) //длительность не закончилась
                    {
                        TurnList.Add(indexOfActiveProcess);
                        ProcessStatus[indexOfActiveProcess] = 1; //пометить процесс как ожидающий
                    }
                    else
                    {
                        ProcessStatus[indexOfActiveProcess] = 0; //пометить процесс как не выполнящийся
                        finishedcount++;
                    }
                    TurnList.Remove(indexOfActiveProcess); //удаляет процесс
                    periodleft = period; //сохранения оставшегося кванта 
                    if (TurnList.Count > 0) //если есть еще процессы
                    {
                        indexOfActiveProcess = GetNextFromTurn(TurnList); //берем новый процесс из списка (его индекс)
                        ProcessStatus[indexOfActiveProcess] = 2; //помечаем его как выполняющийся
                    }
                }


                if (indexOfNextProcess < intervals.Length) //если индекс след. процесса меньше кол-ва интервалов
                {
                    --intervals[indexOfNextProcess]; //уменьшаем интервал след процесса
                }
                if (TurnList.Count > 0) //если в очереди есть процессы
                {
                    --durations[indexOfActiveProcess]; //уменьшаем длительность текущего процесса 
                }
                periodleft--;
                // заполняем информационную часть новыми состояниями                
                for (int i = 0; i < count; ++i)
                {
                    if (ProcessStatus[i] == 1) ++waitTime[i]; //если процесс ожидает увеличиваем время ожидания
                    if (ProcessStatus[i] == 2) ++activeTime[i]; //если процесс выполняется увеличиваем время выполнения
                }
            }
        }
        public override int GetNextFromTurn(List<int> Turnlist)
        {
            return Turnlist.First();
        }
    }
}

