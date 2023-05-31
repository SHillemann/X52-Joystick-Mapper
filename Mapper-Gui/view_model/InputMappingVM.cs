using Mapper_Gui.model;
using mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Button = Mapper_Gui.model.Button;

namespace Mapper_Gui.view_model
{
    class InputMappingVM : BaseVM
    {

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void ControlCallback(X52_Joystick x52_Joystick);

        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void X52([MarshalAs(UnmanagedType.FunctionPtr)] ControlCallback callbackPointer);
        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void Halt();
        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void Reset();
        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void ChangeMapping(int btn, int joy, int map_to, string desc);
        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void ChangeDeath(int axis_index, int range);
        [DllImport("x52-controller.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void Center(int x, int y);

        Thread _x52;
        Mapping mapping;
        public InputMappingVM()
        {
            mapping = new Mapping();
            Messenger.Instance.Register<List<TextBox>>(this, "InputControl_Pressed", UpdateUI);
            Messenger.Instance.Register<List<TextBox>>(this, "InputControl_Pressed_Copy", UpdateUI_Copy);
            Messenger.Instance.Register<string>(this, "MainControl", Choose);
            Messenger.Instance.Register<string>(this, "SetOutput", PropChanged);
            Messenger.Instance.Send(mapping.NextFreeButton(), "InputMapping");
            ControlCallback call = Callback;
            SendToJoy();
            _x52 = new Thread(() => X52(call));
            _x52.Priority = ThreadPriority.Highest;
            _x52.Start();        
        }

        private int _raw_x = 0;
        private int _raw_y = 0;

        void Callback(X52_Joystick x52_Joystick)
        {
            _raw_x = x52_Joystick.x;
            _raw_y = x52_Joystick.y;
            Messenger.Instance.Send(x52_Joystick, "RawControl");
        }

        private void PropChanged(string obj)
        {
            Messenger.Instance.Send(mapping.NextFreeButton(), "InputMapping");
        }

        ~InputMappingVM()
        {
            Messenger.Instance.Unregister<string>(this, "MainControl");
        }

        public ObservableCollection<Button> Mapping => mapping.Map;

        private void Choose(string function)
        {
            switch (function)
            {
                case "save":
                    Save();
                    break;
                case "stop":                    
                    Halt();
                    _x52.Join();
                    break;
                case "send":
                    SendToJoy();
                    break;
                case "reset":
                    ResetJoy();
                    break;
            }
        }

        private void ResetJoy()
        {
            Center(512 - _raw_x, 512 - _raw_y);
        }

        private void SendToJoy()
        {
            foreach (var button in Mapping)
            {
                int index = button.Id - 1;
                if (button.Id > 30)
                {
                    index += 180;
                    //Mode1
                    ChangeMapping(index, button.Mode1.Joy, button.Mode1.Index, button.Mode1.Description);
                    //Mode2
                    ChangeMapping(index + 8, button.Mode2.Joy, button.Mode2.Index, button.Mode2.Description);
                    //Mode3
                    ChangeMapping(index + 16, button.Mode3.Joy, button.Mode3.Index, button.Mode3.Description);
                    //Missle
                    ChangeMapping(index + 24, button.Mode_missle.Joy, button.Mode_missle.Index, button.Mode_missle.Description);
                    //Mode1_alt
                    ChangeMapping(index + 32, button.Mode1_alt.Joy, button.Mode1_alt.Index, button.Mode1_alt.Description);
                    //Mode2_alt
                    ChangeMapping(index + 40, button.Mode2_alt.Joy, button.Mode2_alt.Index, button.Mode2_alt.Description);
                    //Mode3_alt
                    ChangeMapping(index + 48, button.Mode3_alt.Joy, button.Mode3_alt.Index, button.Mode3_alt.Description);
                    continue;
                }

                    //Mode1
                    ChangeMapping(index, button.Mode1.Joy, button.Mode1.Index, button.Mode1.Description);
                    //Mode2
                    ChangeMapping(index + 30, button.Mode2.Joy, button.Mode2.Index, button.Mode2.Description);
                    //Mode3
                    ChangeMapping(index + 60, button.Mode3.Joy, button.Mode3.Index, button.Mode3.Description);
                    //Missle
                    ChangeMapping(index + 90, button.Mode_missle.Joy, button.Mode_missle.Index, button.Mode_missle.Description);
                    //Mode1_alt
                    ChangeMapping(index + 120, button.Mode1_alt.Joy, button.Mode1_alt.Index, button.Mode1_alt.Description);
                    //Mode2_alt
                    ChangeMapping(index + 150, button.Mode2_alt.Joy, button.Mode2_alt.Index, button.Mode2_alt.Description);
                    //Mode3_alt
                    ChangeMapping(index + 180, button.Mode3_alt.Joy, button.Mode3_alt.Index, button.Mode3_alt.Description);
                    //X_Axis
                    ChangeDeath(0,22);
                    //Y_AXIS
                    ChangeDeath(1,48);
            }
        }

        TextBox _joy;
        TextBox _index;
        TextBox _description;
        TextBox _joy_copy;
        TextBox _index_copy;
        TextBox _description_copy;
        private void UpdateUI(List<TextBox> obj)
        {
            _joy = obj[0];
            _index = obj[1];
            _description = obj[2];
        }

        private void UpdateUI_Copy(List<TextBox> obj)
        {
            _joy_copy = obj[0];
            _index_copy = obj[1];
            _description_copy = obj[2];
        }

        private ICommand _nextCommand;
        private ICommand _copyCommand;
        private ICommand _toCommand;
        private ICommand _switchCommand;

        public ICommand NextCommand
        {
            get
            {
                return _nextCommand ?? (_nextCommand = new CommandHandler((input) =>
                {
                    var next = mapping.NextFreeButton();
                    var item = input as Output;
                    item.Joy = next.Key;
                    item.Index = next.Value;
                    if (_joy != null)
                        _joy.Text = item.Joy.ToString();
                    if (_index != null)
                        _index.Text = item.Index.ToString();
                }, () => true));
            }
        }
        Output _saved;
        public ICommand CopyCommand
        {
            get
            {
                return _copyCommand ?? (_copyCommand = new CommandHandler((input) =>
                {
                    var item = input as Output;
                    _saved = item;
                }, () => true));
            }
        }

        public ICommand SwitchCommand
        {
            get
            {
                return _switchCommand ?? (_switchCommand = new CommandHandler((input) =>
                {
                    var item = input as Output;
                    var copy = new Output(item.Joy, item.Index);
                    copy.Description = item.Description;
                    item.Joy = _saved.Joy;
                    item.Index = _saved.Index;
                    item.Description = _saved.Description;
                    if (_joy != null)
                        _joy.Text = item.Joy.ToString();
                    if (_index != null)
                        _index.Text = item.Index.ToString();
                    if (_description != null)
                        _description.Text = item.Description;
                    _saved.Joy = copy.Joy;
                    _saved.Index = copy.Index;
                    _saved.Description = copy.Description;
                    if (_joy_copy != null)
                        _joy_copy.Text = _saved.Joy.ToString();
                    if (_index_copy != null)
                        _index_copy.Text = _saved.Index.ToString();
                    if (_description_copy != null)
                        _description_copy.Text = _saved.Description;

                }, () =>
                {
                    return _saved != null;
                }));
            }
        }

        public ICommand ToCommand
        {
            get
            {
                return _toCommand ?? (_toCommand = new CommandHandler((input) =>
                {
                    var item = input as Output;
                    item.Joy = _saved.Joy;
                    item.Index = _saved.Index;
                    item.Description = _saved.Description;
                    if (_joy != null)
                        _joy.Text = item.Joy.ToString();
                    if (_index != null)
                        _index.Text = item.Index.ToString();
                    if (_description != null)
                        _description.Text = item.Description;
                }, () =>
                {
                    return _saved != null;
                }));
            }
        }

        private void Save()
        {
            Json.WriteJson(Mapping);
        }
    }
}
