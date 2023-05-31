// Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <WTypesbase.h>
#include <string>
#include <map>

enum Mode
{
    One, Two, Three, Missle
};
struct mapping
{
    std::string description;
    int joy;
    int button;
};
std::map<int, std::vector<mapping>> _mapping;
std::vector<UCHAR> _buttonStates;
bool _alt = true;
Mode _mode = One;
int main()
{
    for (int button = 0; button <= 47; ++button)
    {
        _buttonStates.push_back(0);
    }
    int setButton1 = 1;
    int setButton2 = 1;
    for (int button = 1; button <= 48; ++button)
    {
        std::vector<mapping> maps;
        for (int i = 0; i < 7; ++i)
        {
            if (button > 24)
            {
                maps.push_back({"B2_" + std::to_string(setButton2),2,setButton2++ });
                if (setButton2 > 128)
                {
                    setButton2 = 1;
                }
            }
            else
            {               
                maps.push_back({"B1_" + std::to_string(setButton1),1,setButton1++ });
                if (setButton1 > 128)
                {
                    setButton1 = 1;
                }
            }
        }
        //_mapping.insert(std::pair<int, std::vector<mapping>>(button,maps));
        _mapping[button] = maps;
    }

    for (int i = 1; i < _buttonStates.size(); ++i)
    {
           int alternativ = _alt ? 4 : 0;
           int selc = (_mode + alternativ)%7;           
           //std::vector<mapping> map = _mapping.find(i)->second
           std::vector<mapping> map = _mapping[i];
           std::cout << "Press: " << i << " desc: [" << map[selc].description << "] joy: [" << map[selc].joy << "] btn: " << map[selc].button << std::endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
