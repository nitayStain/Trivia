
using System;
using System.Configuration;
using System.Windows;

namespace Client
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Window
    {
        public Login()
        {

            InitializeComponent();
            App.currentWindow = this;

            // parsing `App.Config` to fetch the ip used for the server
            try
            {
                var domain = ConfigurationManager.AppSettings.Get("SERVER_DOMAIN");
                if (domain == null)
                {
                    MessageBox.Show("Cannot open client connection.");
                    return;
                }


                string[] strings = domain.Split(':');
                ushort port = ushort.Parse(strings[1]);

                App.TriviaFunctions.Init(strings[0], port);
            } catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                App.Close();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // check if the inputs are empty, and display the error
            if (App.IsEmpty(iUsername.Text, "Username", LoginError) || App.IsEmpty(iPassword.Password, "Password", LoginError)) return;

            bool res = App.TriviaFunctions.Login(iUsername.Text, iPassword.Password, out var err);
            App.IsAuthenticated = res;
            if (res)
            {
                MessageBox.Show("Logged In.");

                App.OpenNext(new MainMenu());
            }
            else
            {
                LoginError.SetError(err);

            }
        }
        
        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            App.OpenNext(new SignUp());
        }
    }
}
