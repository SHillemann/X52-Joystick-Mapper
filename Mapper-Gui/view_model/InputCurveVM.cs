using Mapper_Gui.model;
using mvvm;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace Mapper_Gui.view_model
{
    class InputCurveVM : BaseVM
    {
        double joy_x = 0;
        double joy_y = 0;

        public InputCurveVM()
        {
            Messenger.Instance.Register<X52_Joystick>(this, "RawControl", UpdateView);
        }

        private void UpdateView(X52_Joystick joystick)
        {
            joy_x = joystick.x;
            joy_y = joystick.y;
            OnPropertyChanged("JoyX");
            OnPropertyChanged("JoyY");
            OnPropertyChanged("JoyRaw");
        }

        public double JoyX
        {
            get
            {
                return to256(joy_x,1024);
            }
        }

        public double JoyY
        {
            get
            {
                return to256(joy_y, 1024);
            }
        }

        public string JoyRaw
        {
            get
            {
                return joy_x+", "+joy_y;
            }
        }
     
        private double to256(double input, double rate)
        {
            return input * (247 / rate);
        }
    }
}
