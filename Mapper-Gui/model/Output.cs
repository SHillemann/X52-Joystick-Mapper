using mvvm;
using System;
using System.Collections.Generic;
using System.Text;

namespace Mapper_Gui.model
{
    public class Output
    {
        int _joy=0;
        int _index=0;

        public int Joy { get { return _joy; } set { _joy = value; Messenger.Instance.Send("Change", "SetOutput"); } }
        public int Index
        {
            get { return _index; }
            set
            {
                _index = value; Messenger.Instance.Send("Change", "SetOutput");
            }
        }
        public string Description
        {
            get; set;
        }

        public Output() :  this(0,0)
        {

        }
        public Output(int joy, int index)
        {
            _joy = joy;
            _index = index;
            Description = "----";
        }
    }
}
