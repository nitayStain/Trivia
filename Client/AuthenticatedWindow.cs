using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    public partial class AuthenticatedWindow
    {
        // a static function that auto validates authentication for a used window, so it would return him to login page if not.
        public static void Authenticate(Window curr)
        {
            if (curr != null)
            {
                if(!App.IsAuthenticated)
                {
                    MessageBox.Show("Unauthenticated.");
                    
                    // end socket connection for good - catch anything that goes wrong
                    // maybe when socket is already disconnected etc.
                    try
                    {
                        Connection.GetInstance().Disconnect();
                    } catch { }

                    App.OpenNext(new Login());
                }
            }
        }
    }
}
