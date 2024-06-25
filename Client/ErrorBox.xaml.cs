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
    /// Interaction logic for ErrorBox.xaml
    /// </summary>
    public partial class ErrorBox : UserControl
    {
        public ErrorBox()
        {
            InitializeComponent();

            ErrorPanel.Visibility = Visibility.Hidden;
        }

        public void SetError(string message)
        {
            ErrorLabel.Text = message;
            ErrorPanel.Visibility = Visibility.Visible;
        }
    }
}
