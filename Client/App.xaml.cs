using System;
using System.Diagnostics;
using System.Windows;

namespace Client
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static Window currentWindow = null;

        public readonly static int ROOM_FETCH_INTERVAL = 4500;

        // global static instance for the trivia functionality
        public readonly static TriviaManager TriviaFunctions = new TriviaManager();
        public static string Username;

        // a global bool of is the app is authenticated (connected or logged in)
        public static bool IsAuthenticated { get; set; }

        // changes the error box according to input's emptiness
        public static bool IsEmpty(string str, string label, ErrorBox err)
        {
            if(str == null || str == "" || str == string.Empty)
            {
                err.SetError($"{label} must not be empty.");
                return true;
            }

            return false;
        }

        public static void Close()
        {
            TriviaFunctions.Shutdown();
            Environment.Exit(0);
        }

        // open another window, and close the current
        public static void OpenNext(Window next)
        {
            next.Show();
            if(currentWindow != null) currentWindow.Close();
            currentWindow = next;
        }

        private void Application_Exit(object sender, ExitEventArgs e)
        {
            try
            {
                // close/leave current room when app is closed
                if(!TriviaFunctions.CloseRoom(out var err))
                {
                    TriviaFunctions.LeaveRoom(out err);
                }

                TriviaFunctions.Shutdown();
                Environment.Exit(0);
            } catch { }
        }
    }
}
