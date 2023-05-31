using Mapper_Gui.model;
using Mapper_Gui.view_model;
using mvvm;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
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
    /// Interaktionslogik für InputCurveControl.xaml
    /// </summary>
    public partial class InputCurveControl : UserControl
    {
        private readonly BackgroundWorker worker = new BackgroundWorker();
        public InputCurveControl()
        {
            DataContext = new InputCurveVM();
            InitializeComponent();
            worker.DoWork += worker_DoWork;
            worker.RunWorkerCompleted += worker_RunWorkerCompleted;
            worker.RunWorkerAsync();
        }
        private void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            // run all background tasks here
            Thread.Sleep(1000);
        }
        private void worker_RunWorkerCompleted(object sender,
                                           RunWorkerCompletedEventArgs e)
        {
            //update ui once worker complete his work
            Time.Content = DateTime.Now.ToString("dddd, dd MMMM yyyy HH:mm:ss");
            worker.RunWorkerAsync();
        }
    }
}