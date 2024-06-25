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
    /// Interaction logic for PersonalStatisticsPage.xaml
    /// </summary>
    public partial class PersonalStatisticsPage : Page, INavPage
    {
        private bool closed = false;
        private string[] _stats;

        public PersonalStatisticsPage()
        {
            InitializeComponent();
            App.TriviaFunctions.GetStats(setStats);
            if (_stats == null)
            {
                MessageBox.Show("Something went wrong :(");
                avgAnsTime.Text += "No information";
                numOfCorrectAns.Text += "No information";
                numOfAns.Text += "No information";
                numOfGames.Text += "No information";
            }
            else
            {
                avgAnsTime.Text += _stats[0];
                numOfCorrectAns.Text += _stats[1];
                numOfAns.Text += _stats[2];
                numOfGames.Text += _stats[3];
            }
        }

        public void Close() => closed = true;

        private void setStats(string[] stats)
        {
            _stats = stats;
        }
    }
}
