using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    // this class manages carefully every task the client creates
    // it's managed in order to send/receive messages that basically can freeze the program by waiting
    // running send or receive tasks on a thread would be much more efficient.
    public class ThreadManager
    {
        // runs the update function as a thread
        public void Run()
        {
            new Thread(new ThreadStart(Update)).Start();
        }

        private readonly List<Action> executeOnThread = new List<Action>();
        private readonly List<Action> executeOnThreadCopy = new List<Action>();
        private bool actionToExecuteOnThread = false;

        // a function that appends an action to be done on the executing thread.
        public void ExecuteOnThread(Action action)
        {
            if(action == null)
            {
                throw new Exception("No action to execute on this thread.");
            }
            else
            {
                lock(executeOnThread)
                {
                    executeOnThread.Add(action);
                    actionToExecuteOnThread = true;
                }
            }
        }

        // the thread function
        private void Update()
        {
            while(true)
            {
                RunCurrentExecutables();
                Thread.Sleep(100);
            }
        }

        // runs every current executable action
        private void RunCurrentExecutables()
        {
            if(actionToExecuteOnThread)
            {
                executeOnThreadCopy.Clear();

                lock(executeOnThread)
                {
                    executeOnThreadCopy.AddRange(executeOnThread);
                    executeOnThread.Clear();

                    actionToExecuteOnThread = false;
                }

                for(int i = 0; i < executeOnThreadCopy.Count; i++)
                {
                    executeOnThreadCopy[i].Invoke();
                }
            }
        }
    }
}
