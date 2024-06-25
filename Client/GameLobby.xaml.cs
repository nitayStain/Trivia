using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for GameLobby.xaml
    /// </summary>
    /// 

    public class GameLobbyState : StateObject
    {
        private ObservableCollection<string> _players;

        public ObservableCollection<string> Players
        {
            get => _players;
            set => SetProperty(ref _players, value);
        }
    }

    public partial class GameLobby : Window
    {
        private GameLobbyState gameState;

        public GameLobby(string roomName, bool isAdmin)
        {
            InitializeComponent();

            room_name.Text = roomName;

            if (isAdmin)
            {
                btn1.Click += OnStartGame;
                btn2.Click += OnCloseGame;
            }
            else
            {
                btn1.Click += OnLeaveGame;
                btn1.Content = "Leave Game";
                btns.Children.Remove(btn2);
            }

            DataContext = new GameLobbyState();
            gameState = (GameLobbyState)DataContext;

            Thread refreshThread = new Thread(new ThreadStart(() =>
            {
                while(true)
                {
                    FetchLobbyData();
                    Thread.Sleep(500);
                }
            }));
            refreshThread.Start();
        }

        private void FetchLobbyData()
        {
            App.TriviaFunctions.GetRoomData((GetRoomStateResponse resp) =>
            {
                if (resp == null || resp.players == null) return;

                gameState.Players = new ObservableCollection<string>(resp.players);
            });
        }

        // button handlers
        private void OnCloseGame(object sender, RoutedEventArgs e)
        {
            if(App.TriviaFunctions.CloseRoom(out var err))
            {
                App.OpenNext(new MainMenu());
            }
            else
            {
                if (err != null) MessageBox.Show(err);
            }
        }

        private void OnStartGame(object sender, RoutedEventArgs e)
        {
            if (App.TriviaFunctions.StartGame(out var err))
            {
                App.OpenNext(new Game());
            }
            else
            {
                if (err != null) MessageBox.Show(err);
            }
        }

        private void OnLeaveGame(object sender, RoutedEventArgs e)
        {
            if (App.TriviaFunctions.CloseRoom(out var err))
            {
                App.OpenNext(new MainMenu());
            }
            else
            {
                if (err != null) MessageBox.Show(err);
            }
        }
    }
}
