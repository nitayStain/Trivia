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
    /// Interaction logic for HighScoresPage.xaml
    /// </summary>
    public partial class HighScoresPage : Page, INavPage
    {
        private bool closed = false;
        private string[] _scores;
        public HighScoresPage()
        {
            InitializeComponent();
            App.TriviaFunctions.GetHighScores(setScores);
            if (_scores == null)
            {
                MessageBox.Show("No high scores :(");
                score_first.Text = "No information";
                score_second.Text = "No information";
                score_third.Text = "No information";
            }
            else
            {
                score_first.Text = _scores[0];
                score_second.Text = _scores[1];
                score_third.Text = _scores[2];
            }
        }

        public void Close() => closed = true;

        private void setScores(string[] scores)
        {
            _scores = scores;
        }
    }
}
