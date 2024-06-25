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
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        public MainMenu()
        {
            Trace.WriteLine(App.IsAuthenticated);
            InitializeComponent();

            AuthenticatedWindow.Authenticate(this);
        }

        //
        private void OnExitClicked(object sender, RoutedEventArgs e)
        {
            App.Close();
        }

        private void OnRoomsClicked(object sender, RoutedEventArgs e)
        {
            App.OpenNext(new Rooms());
        }

        private void OnStatsClicked(object sender, RoutedEventArgs e)
        {
            App.OpenNext(new StatisticsMenu());
        }
    }
}
