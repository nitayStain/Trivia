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
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for StatisticsMenu.xaml
    /// </summary>
    public partial class StatisticsMenu : Window
    {
        public StatisticsMenu()
        {
            InitializeComponent();
            frame.Content = null;
        }

        private void back(object sender, RoutedEventArgs e)
        {
            INavPage? page = frame.Content as INavPage;
            if (page != null) page.Close();
            App.OpenNext(new MainMenu());
        }

        private void showStatistics(object sender, RoutedEventArgs e)
        {
            movePage(new PersonalStatisticsPage());
        }

        private void showHighScores(object sender, RoutedEventArgs e)
        {
            movePage(new HighScoresPage());
        }

        private void movePage(INavPage page)
        {
            if (frame.Content == page)
            {
                page.Close();
                frame.Content = null;
            }
            else
            {
                frame.Content = page;
                frame.Navigate(page);
            }
        }
    }
}
 