using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
    /// <summary>
    /// Interaction logic for RoomCreationPage.xaml
    /// </summary>
    /// 
    public interface INavPage
    {
        void Close();
    }


    public partial class RoomCreationPage : Page, INavPage
    {
        private bool closed = false;

        public RoomCreationPage()
        {
            InitializeComponent();

            room_name.Text = $"{App.Username}'s Room";
            max_players.Text = "12";
            time_for_question.Text = "10";
        }

        public void Close() => closed = true;

        private void CreateButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string roomName = room_name.Text;
                uint maxPlayers = uint.Parse(max_players.Text);
                uint timePerQuestion = uint.Parse(time_for_question.Text);

                if(App.TriviaFunctions.CreateRoom(roomName, maxPlayers, timePerQuestion, out var err))
                {
                    if (err != "") MessageBox.Show(err);
                    else
                    {
                        App.OpenNext(new GameLobby(roomName, true));
                    }
                }
                else
                {
                    if (err != "") MessageBox.Show(err);
                }
            }
            catch
            {
                MessageBox.Show("The input you registered is not the requested type.");
            }
        }
    }
}
