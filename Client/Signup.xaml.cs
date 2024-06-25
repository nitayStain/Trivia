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
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class SignUp : Window
    {
        public SignUp()
        {
            InitializeComponent();

            username.Text = password.Password = email.Text = string.Empty;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {            
            if (App.IsEmpty(username.Text, "Username", Error) || App.IsEmpty(email.Text, "Email", Error) || App.IsEmpty(password.Password, "Password", Error))
            {
                return;
            }

            bool res = App.TriviaFunctions.Signup(username.Text, password.Password, email.Text, out var err);
            App.IsAuthenticated = res;
            if(res)
            {
                MessageBox.Show("Logged In.");

                App.OpenNext(new MainMenu());
            }
            else
            {
                Error.SetError(err);

            }
        }
    }
}
