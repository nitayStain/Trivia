using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for Rooms.xaml
    /// </summary>
    public partial class Rooms : Window
    {
        private string currentPage = "roomlist";

        public Rooms()
        {
            InitializeComponent();

            Main.Content = new RoomlistPage();

            // TODO: create the roomlist page using those room metadatas
            App.TriviaFunctions.GetRooms((RoomData[] data) =>
            {
                if (data == null)
                    return;


            });
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            INavPage? page = Main.Content as INavPage;
            if(page != null) page.Close();

            App.OpenNext(new MainMenu());
        }

        private void MovePage(object sender, RoutedEventArgs e)
        {
            currentPage = currentPage == "roomlist" ? "roomcreation" : "roomlist";
            INavPage? page = Main.Content as INavPage;
            if (page != null) page.Close();

            if (currentPage == "roomlist")
            {
                Main.Content = new RoomlistPage();
            }
            else
            {
                Main.Content = new RoomCreationPage();
            }
        }
    }
}
