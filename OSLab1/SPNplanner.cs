using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSLab1
{
    class SPNplanner : Planner
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
            int currentDuration = -1;
            // время до поступления в очередь следующего процесса
            int currentInterval = 0;
            // очередь
            List<int> TurnList = new List<int>();
            int minIndex = 0;
            while (checkIsProcessLeft() == true)
            {
                if (currentDuration == 0)
                {
                    // помечаем его как "завершенный"
                    ProcessStatus[indexOfActiveProcess] = 0;
                    durations[minIndex] = 0; //ставит длит 0, т.е. как бы удаляем его из массива. TODO: исправить костыль
                    currentDuration = -1; //костыль, чтобы дать понять программе, когда нужно брать след процесс с мин длительностью 
                }

                if (currentDuration == -1)
                {
                    minIndex = 0;
                    for (int z = 1; z < durations.Length; ++z)
                    { // получаем индекс процесса с минимальной длительностью выполнения
                        if (durations[minIndex] == 0) {
                            minIndex++; // to prevent infinite loop
                        }
                        if (durations[minIndex] > durations[z])
                        {
                            if (z != minIndex)
                            {
                                if (durations[z] != 0)
                                {
                                    minIndex = z;
                                }
                            }
                        }
                    }
                    // добавляем процесс с мин. длительностью в очередь
                    TurnList.Add(minIndex);
                    indexOfActiveProcess = minIndex;
                    // запоминаем его длительность
                    currentDuration = this.durations[indexOfActiveProcess];
                    // помечаем его как "запущенный"
                    ProcessStatus[minIndex] = 2;

                    currentDuration = this.durations[indexOfActiveProcess];

                    // и ожидаем следующего
                    if (++indexOfNextProcess < count)
                    {
                        int minNextIndex = 0;
                        for (int z = 1; z < durations.Length; ++z)
                        { // получаем индекс процесса с минимальной длительностью выполнения
                            if (durations[minNextIndex] > durations[z])
                            {
                                if (z != minIndex)
                                {
                                    minNextIndex = z;
                                }
                            }
                        }
                        indexOfNextProcess = minNextIndex;
                        currentInterval = this.intervals[indexOfNextProcess];
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
        public bool checkIsProcessLeft()
        {
            for (int a = 0; a < durations.Length; ++a)
            {
                if (durations[a] != 0) return true;
            }
            return false;
        }
    }

}