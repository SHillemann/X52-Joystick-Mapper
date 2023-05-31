using System;
using System.Collections.Generic;
using System.Text;

namespace Mapper_Gui.model
{
    public class Button
    {
        public string Name { get { return ButtonNames[Id]; } }
        //Joy_1
        public Output Mode1 { get; set; }       //1-30
        public Output Mode2 { get; set; }       //31-60
        public Output Mode3 { get; set; }       //61-90
        public Output Mode_missle { get; set; } //94-120
                                                // Joy_2
        public Output Mode1_alt { get; set; }   //1-30
        public Output Mode2_alt { get; set; }   //31-60
        public Output Mode3_alt { get; set; }   //61-90
        public int Id { get; set;}

        public Button() { }
        public Button(int id)
        {
            Id = id;
            if (id > 30)
            {
                Mode1 = new Output();
                Mode2 = new Output();
                Mode3 = new Output();
                Mode_missle = new Output();
                Mode1_alt = new Output();
                Mode2_alt = new Output();
                Mode3_alt = new Output();
            }
            else
            {
                Mode1 = new Output(1, id);
                Mode2 = new Output(1, id + 30);
                Mode3 = new Output(1, id + 60);
                Mode_missle = new Output(1, id + 90);
                Mode1_alt = new Output(2, id);
                Mode2_alt = new Output(2, id + 30);
                Mode3_alt = new Output(2, id + 60);
            }
        }

        private readonly Dictionary<int, string> ButtonNames = new Dictionary<int, string>
        {
            {1, "Trigger" },
            {2, "Launch"},
            {3, "Fire A"},
            {4, "Fire B"},
            {5, "Fire C"},
            {6, "Fire D"},
            {7, "Fire E"},
            {8, "Toggle 1"},
            {9, "Toggle 2"},
            {10, "Toggle 3" },
            {11, "Toggle 4"},
            {12, "Toggle 5"},
            {13, "Toggle 6"},
            {14, "Second Trigger"},
            {15, "Mouse Fire"},
            {16, "Wheel Button"},
            {17, "POV 2 Up"},
            {18, "POV 2 Right"},
            {19, "POV 2 Down"},
            {20, "POV 2 Left"},
            {21, "Throttle Hat Up"},
            {22, "Throttle Hat Right"},
            {23, "Throttle Hat Down"},
            {24, "Throttle Hat Left"},
            {25, "Clutch Button"},
            {26, "Start/Stop"},
            {27, "Reset"},
            {28, "Soft Up"},
            {29, "Soft Down"},
            {30, "Soft Button"},
            {31, "POV 1 Up"},//--
            {32, "POV 1 UpRight"},//--
            {33, "POV 1 Right"},//--
            {34, "POV 1 DownRight"},//--
            {35, "POV 1 Down"},//--
            {36, "POV 1 DownLeft"},//--
            {37, "POV 1 Left"},//--
            {38, "POV 1 UpLeft"},//--
        };
    }
}
