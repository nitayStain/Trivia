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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    public class PagelistView : StateObject
    {
        private ObservableCollection<RoomData> rooms;

        public ObservableCollection<RoomData> Rooms
        {
            get => rooms;
            set => SetProperty(ref rooms, value);
        }
    }

    /// <summary>
    /// Interaction logic for RoomlistPage.xaml
    /// </summary>
    public partial class RoomlistPage : Page, INavPage
    {

        private bool closed;
        private PagelistView viewModel;

        private string currentRoomId = "";
        private Button currentButton = null;

        public RoomlistPage()
        {
            InitializeComponent();

            closed = false;

            DataContext = new PagelistView();
            viewModel = (PagelistView)DataContext;

            Thread refreshThread = new Thread(new ThreadStart(RefreshCurrentRooms));
            refreshThread.Start();

        }

        private void RefreshCurrentRooms()
        {
            while(!closed)
            {
                App.TriviaFunctions.GetRooms(SetRooms);
                Thread.Sleep(App.ROOM_FETCH_INTERVAL);
            }
        }

        private void SetRooms(RoomData[] rooms)
        {
            if (rooms == null)
            {
                return;
            }

            viewModel.Rooms = new ObservableCollection<RoomData>(rooms);
        }

        public void Close() => closed = true;
        private void OnRoomSelect(object sender, RoutedEventArgs e)
        {
            var btn = e.Source as Button;
            if (btn == null) return;
            var id = btn.Tag as string;
            if (id == "" || id == null) return;
            
            if(currentButton != null)
            {
                currentButton.Background = Application.Current.Resources["PrimaryDarkBrush"] as Brush;
            }
            currentRoomId = id;

            btn.Background = Application.Current.Resources["SecondaryDarkBrush"] as Brush;
            currentButton = btn;
        }

        private void OnRoomJoin(object sender, RoutedEventArgs e)
        {
            if (currentRoomId == "") return;

            if (App.TriviaFunctions.JoinRoom(currentRoomId, out var err))
            {
                if (err != "") MessageBox.Show(err);
                else
                {
                    App.OpenNext(new GameLobby(viewModel.Rooms.ToList().Find(m => m.id == currentRoomId).name, false));
                }
            }
            else
            {
                if (err != "") MessageBox.Show(err);
            }
        }
    }
}
