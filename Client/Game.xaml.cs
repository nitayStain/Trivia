using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        PlayerResults playerRes;
        uint numOfQuestions;
        uint answerTimeOut;
        uint remainingRime;

        public Game()
        {
            InitializeComponent();

            playerRes = new PlayerResults();
            numOfQuestions = 10;
            setAnswerTimeOut();
            remainingRime = answerTimeOut;

            Thread refreshThread = new Thread(new ThreadStart(() =>
            {
                while (true)
                {
                    timeLeft.Text = remainingRime.ToString() + "/" + answerTimeOut.ToString();
                    Thread.Sleep(1000);
                    remainingRime--;
                }
            }));
            refreshThread.Start();
        }

        private void onSubmitAnswer(uint answerId)
        {
            if (App.TriviaFunctions.CheckAnswer(answerId))
            {
                playerRes.correctAnsCount++;
                correctAnsCount.Text = "Correct Answers: " + playerRes.correctAnsCount.ToString();
            }
            else
            {
                playerRes.wrongAnsCount++;
            }
            remainingRime = answerTimeOut;
        }

        private void getQuestion()
        {
            App.TriviaFunctions.GetQuestion((getQuestionResponse resp) =>
            {
                if (resp.success == 1)
                {
                    question.Text = resp.question;
                    answer1.Content = resp.answers[0];
                    answer2.Content = resp.answers[1];
                    answer3.Content = resp.answers[2];
                    answer4.Content = resp.answers[3];
                    questionsLeft.Text = (numOfQuestions - (playerRes.correctAnsCount + playerRes.wrongAnsCount)).ToString() + "/" + numOfQuestions.ToString();
                }
                else
                {
                    App.OpenNext(new GameResults());
                }
            });
        }

        private void chooseAnswer(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            switch (btn.Name[-1])
            {
                case '1':
                    onSubmitAnswer(1);
                    break;
                case '2':
                    onSubmitAnswer(2);
                    break;
                case '3':
                    onSubmitAnswer(3);
                    break;
                case '4':
                    onSubmitAnswer(4);
                    break;
            }
            getQuestion();
        }

        private void OnLeaveGame(object sender, RoutedEventArgs e)
        {
            if (App.TriviaFunctions.LeaveGame(out var err))
            {
                App.OpenNext(new MainMenu());
            }
            else
            {
                if (err != null) MessageBox.Show(err);
            }
        }

        private void setAnswerTimeOut()
        {
            App.TriviaFunctions.GetRoomData((GetRoomStateResponse resp) =>
            {
                if (resp == null) return;
                answerTimeOut = resp.answerTimeOut;
            });
        }
    }
}
