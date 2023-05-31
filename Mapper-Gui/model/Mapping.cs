using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace Mapper_Gui.model
{
    class Mapping
    {
        List<Button> _buttons = new List<Button>();
        ObservableCollection<Button> _mappingItems;


        public Mapping()
        {
            _buttons = Json.ReadJson();
            if (_buttons.Count == 0)
            {
                for (int i = 1; i <= 38; i++)
                {
                    _buttons.Add(new Button(i));
                }
            }
        }



        public ObservableCollection<Button> Map
        {
            get
            {
                if (_mappingItems == null)
                {
                    _mappingItems = new ObservableCollection<Button>();
                    foreach (var button in _buttons)
                    {
                        _mappingItems.Add(button);
                    }
                }
                return _mappingItems;
            }
        }

        public KeyValuePair<int, int> NextFreeButton()
        {
            bool found = false;
            for (int joy = 1; joy <= 2; joy++)
            {
                for (int index = 1; index <= 120; index++)
                {
                    foreach (var button in _buttons)
                    {
                        if (button.Mode1.Index == index && button.Mode1.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode1_alt.Index == index && button.Mode1_alt.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode2.Index == index && button.Mode2.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode2_alt.Index == index && button.Mode2_alt.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode3.Index == index && button.Mode3.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode3_alt.Index == index && button.Mode3_alt.Joy == joy)
                        {
                            found = true;
                            break;
                        }
                        if (button.Mode_missle.Index == index && button.Mode_missle.Joy == joy)
                        {
                            found = true;
                            break;
                        }

                    }
                    if (!found)
                    {
                        return new KeyValuePair<int, int>(joy, index);
                    }
                    found = false;
                }
            }
            return new KeyValuePair<int, int>(0, 0);
        }
    }
}
