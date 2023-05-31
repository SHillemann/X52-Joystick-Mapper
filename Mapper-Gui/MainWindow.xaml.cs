using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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


namespace Mapper_Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            StateChanged += MainWindowStateChangeRaised;
        }


        // Can execute
        private void CommandBinding_CanExecute(object sender, System.Windows.Input.CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        // Minimize
        private void CommandBinding_Executed_Minimize(object sender, System.Windows.Input.ExecutedRoutedEventArgs e)
        {
            SystemCommands.MinimizeWindow(this);
        }

        // Maximize
        private void CommandBinding_Executed_Maximize(object sender, System.Windows.Input.ExecutedRoutedEventArgs e)
        {
            SystemCommands.MaximizeWindow(this);
        }

        // Restore
        private void CommandBinding_Executed_Restore(object sender, System.Windows.Input.ExecutedRoutedEventArgs e)
        {
            SystemCommands.RestoreWindow(this);
        }

        // Close
        private void CommandBinding_Executed_Close(object sender, System.Windows.Input.ExecutedRoutedEventArgs e)
        {
            MainControl.Stop();
            SystemCommands.CloseWindow(this);
        }


        // State change
        private void MainWindowStateChangeRaised(object sender, EventArgs e)
        {
            if (WindowState == WindowState.Maximized)
            {
                MainWindowBorder.BorderThickness = new Thickness(8);
                RestoreButton.Visibility = Visibility.Visible;
                MaximizeButton.Visibility = Visibility.Collapsed;
            }
            else
            {
                MainWindowBorder.BorderThickness = new Thickness(0);
                RestoreButton.Visibility = Visibility.Collapsed;
                MaximizeButton.Visibility = Visibility.Visible;
            }
        }
    }
}
