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
using System.Xml;

namespace Client
{
    /// <summary>
    /// Interaction logic for GameResults.xaml
    /// </summary>
    public partial class GameResults : Window
    {
        PlayerResults[] playersRes;

        public GameResults()
        {
            InitializeComponent();

            App.TriviaFunctions.getGameResults(getResults);
            for (int i = 0; i < playersRes.Length; i++)
            {
                TextBlock textBlock = new TextBlock();

                textBlock.Text = playersRes[i].username;
                textBlock.HorizontalAlignment = HorizontalAlignment.Center;
                textBlock.FontSize = 20;
                textBlock.Foreground = new SolidColorBrush(Colors.White);
                textBlock.TextWrapping = TextWrapping.Wrap;

                users.Children.Add(textBlock);

                textBlock.Text = playersRes[i].correctAnsCount.ToString();
                correctAns.Children.Add(textBlock);

                textBlock.Text = playersRes[i].avgAnsTime.ToString();
                avgAnsTime.Children.Add(textBlock);
            }
        }

        private void getResults(getGameResultsResponse resp)
        {
            playersRes = resp.results;
        }

        private void backToMenu(object sender, RoutedEventArgs e)
        {
            App.OpenNext(new MainMenu());
        }
    }
}
