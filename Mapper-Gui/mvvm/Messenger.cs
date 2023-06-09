﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace mvvm
{
    public class Messenger : IMessenger
    {
        private static Messenger instance;
        private static object lockObject = new object();

        private Dictionary<Type, List<ActionIdentifier>> _references = new Dictionary<Type, List<ActionIdentifier>>();

        private Messenger() { }

        public static Messenger Instance
        {
            get
            {
                lock (lockObject)
                {
                    if (instance == null)
                        instance = new Messenger();
                    return instance;
                }
            }
        }

        #region IMessenger Members

        public void Register<TNotification>(object recipient, Action<TNotification> action)
        {
            Register<TNotification>(recipient, null, action);
        }

        public void Register<TNotification>(object recipient, string identCode, Action<TNotification> action)
        {
            Type messageType = typeof(TNotification);

            if (!_references.ContainsKey(messageType))
                _references.Add(messageType, new List<ActionIdentifier>());

            ActionIdentifier actionIdent = new ActionIdentifier();
            actionIdent.Action = new WeakReferenceAction<TNotification>(recipient, action);
            actionIdent.IdentificationCode = identCode;

            _references[messageType].Add(actionIdent);
        }

        public void Send<TNotification>(TNotification notification)
        {
            Type type = typeof(TNotification);
            if (_references.ContainsKey(type))
            {
                List<ActionIdentifier> typeActionIdentifiers = _references[type];
                foreach (ActionIdentifier ai in typeActionIdentifiers)
                {
                    IActionParameter actionParameter = ai.Action as IActionParameter;
                    if (actionParameter != null)
                        actionParameter.ExecuteWithParameter(notification);
                    else
                        ai.Action.Execute();
                }
            }
        }

        public void Send<TNotification>(TNotification notification, string identCode)
        {
            Type type = typeof(TNotification);
            if (_references.ContainsKey(type))
            {
                List<ActionIdentifier> typeActionIdentifiers = _references[type];
                foreach (ActionIdentifier ai in typeActionIdentifiers)
                {
                    if (ai.IdentificationCode == identCode)
                    {
                        IActionParameter actionParameter = ai.Action as IActionParameter;
                        if (actionParameter != null)
                            actionParameter.ExecuteWithParameter(notification);
                        else
                            ai.Action.Execute();
                    }
                }
            }
        }

        public void Unregister<TNotification>(object recipient)
        {
            Unregister<TNotification>(recipient, null);
        }

        public void Unregister<TNotification>(object recipient, string identCode)
        {
            bool lockTaken = false;

            try
            {
                Monitor.Enter(_references, ref lockTaken);
                foreach (Type targetType in _references.Keys)
                {
                    foreach (ActionIdentifier wra in _references[targetType])
                    {
                        if (wra.Action != null && wra.Action.Target != null && wra.Action.Target.Target == recipient)
                            if (String.IsNullOrEmpty(identCode) || (!String.IsNullOrEmpty(identCode) && !String.IsNullOrEmpty(wra.IdentificationCode) && wra.IdentificationCode.Equals(identCode)))
                                wra.Action.Unload();
                    }
                }
            }
            finally
            {
                if (lockTaken)
                    Monitor.Exit(_references);
            }
        }

        #endregion
    }
}
