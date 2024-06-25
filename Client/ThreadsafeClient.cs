using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    class ThreadsafeClient : Connection
    {
        private readonly ThreadManager _threadManager = new ThreadManager();

        private static ThreadsafeClient instance;

        public ThreadsafeClient()
        {
            _threadManager.Run();
        }

        public static ThreadsafeClient GetInstance()
        {
            if(instance == null) instance = new ThreadsafeClient();
            return instance;
        }

        // uses the thread manager to safely run a send task in the background
        public void Send(RequestCode code, IRequest data)
        {
            _threadManager.ExecuteOnThread(() => base.Send(code, data));
        }

        // uses the thread manager to safely run a receive task, that gets an action that handles this data receivement and runs on the executor thread
        public void HandleData(Action<ResponseCode, string> action)
        {
            _threadManager.ExecuteOnThread(() =>
            {
                var resCode = GetCurrentResponse();
                var payload = Protocol.GetString(ClientStream);

                try
                {
                    action(resCode, payload);
                } catch { }
            });
        }
    }
}
