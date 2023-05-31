using System;
using System.Collections.Generic;
using System.Text;

namespace mvvm
{
    public class WeakReferenceAction
    {
        private WeakReference _target;
        private Action _action;

        public WeakReference Target { get { return _target; } }

        public WeakReferenceAction(object target, Action action)
        {
            _target = new WeakReference(target);
            _action = action;
        }
        
        public void Execute()
        {
            if (_action != null && _target != null && _target.IsAlive)
            {
                _action.Invoke();
            }

        }
        public void Unload()
        {
            _target = null;
            _action = null;
        }
    }
    public class WeakReferenceAction<T> : WeakReferenceAction, IActionParameter
    {
        private Action<T> action;
        public WeakReferenceAction(object target, Action<T> action)
            : base(target, null)
        {
            this.action = action;
        }
        public new void Execute()
        {
            if (action != null && Target != null && Target.IsAlive)
                action(default(T));
        }
        public void Execute(T parameter)
        {
            if (action != null && Target != null && Target.IsAlive)
                this.action(parameter);
        }
        public Action<T> Action
        {
            get
            {
                return action;
            }
        }
        #region IActionParameter Members
        public void ExecuteWithParameter(object parameter)
        {
            this.Execute((T)parameter);
        }
        #endregion
    }

}
