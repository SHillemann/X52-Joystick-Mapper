using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using mvvm;

namespace Mapper_Gui.view
{
    /// <summary>
    /// Interaction logic for MainControl.xaml
    /// </summary>
    public partial class MainControl : UserControl
    {
        public MainControl()
        {
            Messenger.Instance.Register<KeyValuePair<int, int>>(this, "InputMapping", SetLable);
            InitializeComponent();
        }
        private void SetLable(KeyValuePair<int, int> button)
        {
            ButtonInfo.Content = "Next Free: Button " + button.Value + " on Joy " + button.Key;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Messenger.Instance.Send("save", "MainControl");
        }

        internal void Stop()
        {
            Messenger.Instance.Send("stop", "MainControl");
        }

        private void Button_Click_Send(object sender, RoutedEventArgs e)
        {
            Messenger.Instance.Send("send", "MainControl");
        }
        private void Button_Click_Reset(object sender, RoutedEventArgs e)
        {
            Messenger.Instance.Send("reset", "MainControl");
        }
        
    }
}