using System;

namespace mvvm
{
    interface IActionParameter
    {
        void ExecuteWithParameter(object notification);
    }
}