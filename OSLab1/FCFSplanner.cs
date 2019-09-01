using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSLab1
{
    class FCFSplanner : Planner
    {
        public override void Planning(int count, int[] durations, int[] intervals)
        {
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
            // время до завершения активного процесса
            int currentDuration = 0;
            // время до поступления в очередь следующего процесса
            int currentInterval = 0;
            // очередь
            List<int> TurnList = new List<int>();
            for (int t = 0; indexOfNextProcess < count || currentDuration > 0 || TurnList.Count > 0; ++t)
            {
                // если подоспел новый процесс
                if (currentInterval == 0 && indexOfNextProcess < count)
                {
                    // добавляем его в очередь
                    TurnList.Add(indexOfNextProcess);
                    this.maxQueueLength = Math.Max(this.maxQueueLength, TurnList.Count);
                    // помечаем его как "ожидающий"
                    ProcessStatus[indexOfNextProcess] = 1;
                    // и ожидаем следующего
                    if (++indexOfNextProcess < count)
                    {
                        currentInterval = this.intervals[indexOfNextProcess];
                    }
                }
                // если закончился активный процесс
                if (currentDuration == 0)
                {
                    // помечаем его как "завершенный"
                    ProcessStatus[indexOfActiveProcess] = 0;
                    // если очередь не пуста
                    if (TurnList.Count > 0)
                    {
                        // берем следующий процесс из очереди 
                        indexOfActiveProcess = GetNextFromTurn(TurnList);
                        // удаляем из очереди
                        TurnList.Remove(indexOfActiveProcess);
                        // запоминаем его длительность
                        currentDuration = this.durations[indexOfActiveProcess];
                        // помечаем его как "запущенный"
                        ProcessStatus[indexOfActiveProcess] = 2;
                    }
                }
                // уменьшаем время до завершения активного процесса
                currentDuration = Math.Max(0, --currentDuration);
                // уменьшаем время до поступления нового процесса в очередь
                currentInterval = Math.Max(0, --currentInterval);
                // заполняем информационную часть новыми состояниями
                timelineDiagram.Add(new int[count]);
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
