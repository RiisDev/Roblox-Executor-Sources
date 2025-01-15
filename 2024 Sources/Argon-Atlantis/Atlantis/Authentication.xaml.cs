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

namespace Atlantis
{
    /// <summary>
    /// Interaction logic for Authentication.xaml
    /// </summary>
    public partial class Authentication : Window
    {
        public Authentication()
        {
            InitializeComponent();
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
                DragMove();
        }

        private void ButtonHandler(object sender, RoutedEventArgs e)
        {
            switch (((Button)sender).Name)
            {
                case "Exit":
                    Environment.Exit(0);
                    break;
                case "Minimise":
                    WindowState = WindowState.Minimized;
                    break;
                case "ContinueButton":
                    // TO DO
                    // TEMP CODE BELOW // 
                    var Executor = new MainWindow();
                    Executor.Show();
                    Hide();
                    break;
                case "GetKeyButton":
                    // TO DD
                    break;
            }
        }
    }
}
