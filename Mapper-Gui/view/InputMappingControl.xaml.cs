using mvvm;
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

namespace Mapper_Gui.view
{
    /// <summary>
    /// Interaction logic for InputMappingControl.xaml
    /// </summary>
    public partial class InputMappingControl : UserControl
    {
        public InputMappingControl()
        {
            DataContext = new view_model.InputMappingVM();
            InitializeComponent();
        }

        private void Next_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.Button button = (System.Windows.Controls.Button)sender;
            System.Windows.Controls.StackPanel panel12 = (System.Windows.Controls.StackPanel)button.Parent;
            System.Windows.Controls.StackPanel panel1 = (System.Windows.Controls.StackPanel)panel12.Parent;
            System.Windows.Controls.StackPanel panel11 = (System.Windows.Controls.StackPanel)panel1.Children[0];

            List<TextBox> _box = new List<TextBox>();
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[0]);
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[1]);
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[2]);

            Messenger.Instance.Send(_box, "InputControl_Pressed");
        }

        private void Copy_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.Button button = (System.Windows.Controls.Button)sender;
            System.Windows.Controls.StackPanel panel12 = (System.Windows.Controls.StackPanel)button.Parent;
            System.Windows.Controls.StackPanel panel1 = (System.Windows.Controls.StackPanel)panel12.Parent;
            System.Windows.Controls.StackPanel panel11 = (System.Windows.Controls.StackPanel)panel1.Children[0];

            List<TextBox> _box = new List<TextBox>();
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[0]);
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[1]);
            _box.Add((System.Windows.Controls.TextBox)panel11.Children[2]);

            Messenger.Instance.Send(_box, "InputControl_Pressed_Copy");
        }
    }
}
