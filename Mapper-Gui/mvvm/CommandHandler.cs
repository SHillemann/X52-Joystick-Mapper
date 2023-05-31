using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;

namespace mvvm
{
    public class CommandHandler : ICommand
    {
        private Action<object> _action;
        private Func<bool> _canExecute;

        public CommandHandler(Action<object> action, Func<bool> canExecute)
        {
            _action = action;
            _canExecute = canExecute;
        }

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object parameter)
        {
            return _canExecute.Invoke();
        }

        public void Execute(object parameter)
        {
            _action(parameter);
        }
    }
}
