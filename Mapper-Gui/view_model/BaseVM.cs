using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace Mapper_Gui.view_model
{
    public class BaseVM : INotifyPropertyChanged
    {

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
