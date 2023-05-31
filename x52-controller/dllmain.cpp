// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "AxisCurve.h"


#define DLL __declspec(dllexport)

DWORD _vendorId;
DWORD _productId;

const wchar_t* name = L"TestApp";
const wchar_t* pageDebugName = L"TestPage";
const int bit_10 = 1000;
const int bit_8 = 245;
std::vector<void*> devices;
std::vector<UCHAR> _buttonStates;
std::vector<int> _axisState;
std::vector<int> b_index;
std::vector<mapping> m_mode;
std::vector<std::string> m_text;
Mode _mode;
DWORD dwPage = 1;
bool _alt = false;
#define vJoy_1		1
#define vJoy_2		2

int ffb_direction = 0;
int ffb_strenght = 0;
int serial_result = 0;

int x_axis_range = 0;
int y_axis_range = 0;
int z_axis_range = 0;
int rx_axis_range = 0;
int ry_axis_range = 0;
int rz_axis_range = 0;
int delta_x = 0;
int delta_y = 0;
USHORT Z = 0;


JOYSTICK_POSITION_V2 iReport; // The structure that holds the full position data

extern "C"
{
    typedef void(__stdcall* ControlCallback)(x52_joystick stick);
    DLL void X52(ControlCallback controlCallback);
    DLL void ChangeMapping(int btn, int joy, int map_to, const char* desc);
    DLL void ChangeDeath(int axis_index, int range);
    DLL void Center(int x, int y);
    DLL void Halt();
    DLL void Reset();
}

ControlCallback callback;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
//Prototypes
void output(x52_joystick& x52);
void StartOutput();
void outputRawInputData(LPARAM lParam);
void initvJoy();
void initButton();
void RemovePage();
int updateAxis(int index, USHORT axis, int factor, int range);
bool AntiJitter(int axis, USHORT axis_new, int range);
UCHAR Invert(UCHAR input);
bool SpecialButton(UCHAR button);
void SetText(DWORD line, std::string text);
void SetLeds();
void SetLed(int index, int color);
void UpdateAxisLedMiddle(Leds led, UCHAR axis_value);
void UpdateAxisLedRange(Leds led, UCHAR axis_value);
void Lauflicht();
void LauflichtAnimationPower();
void LauflichtAnimationGear();
void SetProfile(std::string path);

void initMapping();

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INPUT) {
        outputRawInputData(lParam);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
void outputRawInputData(LPARAM lParam)
{
    UINT size = 0;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
    RAWINPUT* input = (RAWINPUT*)malloc(size);
    bool gotInput = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER)) > 0;
    if (gotInput)
    {
        x52_joystick x52{};
        GetRawInputDeviceInfo(input->header.hDevice, RIDI_DEVICEINFO, 0, &size);
        RID_DEVICE_INFO* info = (RID_DEVICE_INFO*)malloc(size);
        GetRawInputDeviceInfo(input->header.hDevice, RIDI_DEVICEINFO, info, &size);

        if (info->hid.dwProductId != 0x0762 || info->hid.dwVendorId != 0x06a3)
        {
            free(input);
            free(info);
            return;
        }
        GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, 0, &size);
        _HIDP_PREPARSED_DATA* data = (_HIDP_PREPARSED_DATA*)malloc(size);
        bool gotPreparsedData = GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, data, &size) > 0;
        if (gotPreparsedData)
        {
            HIDP_CAPS caps;
            HidP_GetCaps(data, &caps);

            HIDP_VALUE_CAPS* valueCaps = (HIDP_VALUE_CAPS*)malloc(caps.NumberInputValueCaps * sizeof(HIDP_VALUE_CAPS));
            HidP_GetValueCaps(HidP_Input, valueCaps, &caps.NumberInputValueCaps, data);

            ULONG value;
            HidP_GetUsageValue(HidP_Input, valueCaps[0].UsagePage, 0, valueCaps[0].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.y = (unsigned short)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[1].UsagePage, 0, valueCaps[1].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.x = (unsigned short)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[2].UsagePage, 0, valueCaps[2].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.rz = (unsigned short)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[3].UsagePage, 0, valueCaps[3].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.slider = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[4].UsagePage, 0, valueCaps[4].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.ry = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[5].UsagePage, 0, valueCaps[5].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.rx = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[6].UsagePage, 0, valueCaps[6].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.z = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[7].UsagePage, 0, valueCaps[7].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            unsigned char pov = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[8].UsagePage, 0, valueCaps[8].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.my = (unsigned char)value;
            HidP_GetUsageValue(HidP_Input, valueCaps[9].UsagePage, 0, valueCaps[9].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
            x52.mx = (unsigned char)value;
            free(valueCaps);

            HIDP_BUTTON_CAPS* buttonCaps = (HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
            HidP_GetButtonCaps(HidP_Input, buttonCaps, &caps.NumberInputButtonCaps, data);
            for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i)
            {
                ULONG usageCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
                USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * usageCount);
                HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages, &usageCount, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
                x52.size = (unsigned char)usageCount;
                for (ULONG usageIndex = 0; usageIndex < usageCount; ++usageIndex) {
                    x52.button[usageIndex] = usages[usageIndex];
                }
                if (pov != 0)
                {
                    x52.button[usageCount] = pov + 39;
                    x52.size++;
                }
                free(usages);
            }
            free(buttonCaps);
        }
        free(data);
        free(info);
        output(x52);
        callback(x52);
    }
    free(input);
}


std::atomic<bool> _msg(true);
void X52(ControlCallback controlCallback)
{
    callback = controlCallback;
    initButton();   
    initvJoy();
    StartOutput();
    WNDCLASS wnd = { 0 };
    wnd.hInstance = GetModuleHandle(0);
    wnd.lpfnWndProc = WindowProcedure;
    wnd.lpszClassName = TEXT("Raw input test");
    RegisterClass(&wnd);
    HWND hwnd = CreateWindow(wnd.lpszClassName, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, wnd.hInstance, 0);

    // Register devices

    RAWINPUTDEVICE deviceList[1];
    deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    deviceList[0].usUsage = HID_USAGE_GENERIC_JOYSTICK;
    deviceList[0].dwFlags = RIDEV_INPUTSINK;
    deviceList[0].hwndTarget = hwnd;

    UINT deviceCount = sizeof(deviceList) / sizeof(*deviceList);
    RegisterRawInputDevices(deviceList, deviceCount, sizeof(RAWINPUTDEVICE));

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0 && _msg) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ChangeDeath(int axis_index, int range)
{
    if (axis_index == 0)
    {
        x_axis_range = range;
    }
    if (axis_index == 1)
    {
        y_axis_range = range;
    }
}

void ChangeMapping(int btn, int joy, int map_to, const char* desc)
{
    if (m_mode.empty() || m_text.empty())
    {
        initMapping();
    }
    if (btn < 210)
    {
        m_mode[btn].joy = joy;
        m_mode[btn].button = map_to;
        m_mode[btn].description = desc;
    }
    else
    { //0 - 56
        int index = btn - 210;
        if (index >= 0 && index < 56)
        {
            m_text[index] = desc;
        }
    }
}

void initMapping() {

    //joy 1 1 - 120
    //joy 2 1 - 90
    int joy = 1;
    int button = 1;
    for (int i = 1; i <= 210; ++i)
    {
        if (button > 120)
        {
            joy = 2;
            button = 1;
        }
        m_mode.push_back({ "Y"+ std::to_string(joy) +"_B" + std::to_string(button),joy,button++});
    }
    for (int i = 1; i <= 7; ++i)
    {
        m_text.push_back("Pov " + std::to_string(i) + " Up");
        m_text.push_back("Pov " + std::to_string(i) + " UpRight");
        m_text.push_back("Pov " + std::to_string(i) + " Right");
        m_text.push_back("Pov " + std::to_string(i) + " DownRight");
        m_text.push_back("Pov " + std::to_string(i) + " Down");
        m_text.push_back("Pov " + std::to_string(i) + " DownLeft");
        m_text.push_back("Pov " + std::to_string(i) + " Left");
        m_text.push_back("Pov " + std::to_string(i) + " UpLeft");
    }
 
}

void initButton()
{

    for (int button = 0; button <= 47; ++button)
    {
        _buttonStates.push_back(0);        
    }
    for (int i = 0; i <= 8; ++i)
    {
        _axisState.push_back(16000);
    }
}

void AddMapping()
{

}
void Halt() 
{
    RelinquishVJD(vJoy_1);
    RelinquishVJD(vJoy_2);
    RemovePage();
    PostQuitMessage(0);
    _msg = false;
}

void Center(int x, int y)
{
    delta_x = x;
    delta_y = y;
}

void Reset()
{
    StartOutput();
}

void initvJoy()
{
    // Get the driver attributes (Vendor ID, Product ID, Version Number)
    if (!vJoyEnabled())
    {
        return;
    }

    // Get the status of the vJoy device before trying to acquire it
    VjdStat status1 = GetVJDStatus(vJoy_1);
    VjdStat status2 = GetVJDStatus(vJoy_2);

    switch (status1)
    {
    case VJD_STAT_OWN:
        break;
    case VJD_STAT_FREE:
        break;
    case VJD_STAT_BUSY:
        return;
    case VJD_STAT_MISS:
        return;
    default:
        return;
    };

    switch (status2)
    {
    case VJD_STAT_OWN:
        break;
    case VJD_STAT_FREE:
        break;
    case VJD_STAT_BUSY:
        return;
    case VJD_STAT_MISS:
        return;
    default:
        return;
    };

    // Acquire the vJoy device
    if (!AcquireVJD(vJoy_1))
    {
        return;
    }
    
    if (!AcquireVJD(vJoy_2))
    {
        return;
    }
    
    for (int i = 1; i <= 128; ++i)
    {
       SetBtn(false, vJoy_1, i);
    }
    for (int i = 1; i <= 128; ++i)
    {
       SetBtn(false, vJoy_2, i);
    }

}

UCHAR InvertMax(UCHAR input)
{
    return (UCHAR)((input - 15) * -1);
}


int SetPrez(int value, int factor)
{
    int range = 32000 / (factor * 2);
    int middle = 16000 - range;
    return (value / factor) + middle;
}

void output(x52_joystick &x52) 
{
    for (int i = 0; i < x52.size; ++i)
    {   //set
        UCHAR button = x52.button[i];
        _buttonStates[button] += 2;
    }
    for (int i = 1; i < _buttonStates.size(); ++i)
    {
        if (_buttonStates[i] > 0 && _buttonStates[i] < 3)
        {
            _buttonStates[i] -= 1;
            if (!SpecialButton(i))
            {
                if (i >= 40)
                {
                    int index = i - 40;
                    int joy = _alt ? vJoy_1 : vJoy_2;
                    if (_buttonStates[i] == 1)
                    {                
                      int alternativ = _alt ? 4 : 0;
                      int selc = (_mode + alternativ) % 7;
                      SetContPov(index * (36000 / 8), joy, _mode + 1);
                      SetText(2, m_text[index + (selc * 8)]);
                    }
                    else
                    {
                      SetContPov(-1, joy, _mode + 1);
                    }
                }

                else
                {
                    int alternativ = _alt ? 4 : 0;
                    int selc = (_mode + alternativ) % 7;
                    int btn = i - 1;
                    if(i > 6) // +1
                        btn = i - 2;
                    if(i > 17)// +2
                        btn = i - 4;
                    if(i > 28)// +3
                        btn = i - 7;
                    if(i > 32)// +1
                        btn = i - 8;
                    if(i > 35)// +2
                        btn = i - 10;
                    mapping map = m_mode[btn + (selc * 30)];

                    SetBtn(_buttonStates[i] == 1, map.joy, map.button);
                    SetText(2, map.description);
                }
            }
        }
        if (_buttonStates[i] == 3)
        {
            _buttonStates[i] = 1;
        }
    }

   
    //Mode 3 ohne alt
    if (_mode == Three && !_alt)
    {
        SetAxis(updateAxis(0, x52.x, bit_10, x_axis_range), vJoy_1, HID_USAGE_X);
        SetAxis(updateAxis(1, x52.y, bit_10, y_axis_range), vJoy_1, HID_USAGE_Y);
        SetAxis(updateAxis(0, 512, bit_10, 1), vJoy_2, HID_USAGE_X);
        SetAxis(updateAxis(1, 512, bit_10, 1), vJoy_2, HID_USAGE_Y);
        SetAxis(updateAxis(2, x52.rz, bit_10, 1), vJoy_1, HID_USAGE_RZ);
        SetAxis(updateAxis(3, Invert(x52.z), bit_8, 1), vJoy_1, HID_USAGE_SL1 );
    }
    //Mode 3 mit alt
    else if(_mode == Three && _alt)
    {
        SetAxis(updateAxis(0, x52.x, bit_10, x_axis_range), vJoy_1, HID_USAGE_RZ);
        SetAxis(updateAxis(1, x52.y, bit_10, y_axis_range), vJoy_1, HID_USAGE_Y);
        SetAxis(updateAxis(2, 512, bit_10, 1), vJoy_1, HID_USAGE_X);
        SetAxis(updateAxis(3, Invert(x52.z), bit_8, 1), vJoy_1, HID_USAGE_SL1 );
    }
    // Alle Anderen
    else
    {
        SetAxis(updateAxis(0, x52.x, bit_10, x_axis_range), vJoy_1, HID_USAGE_X);
        SetAxis(updateAxis(1, x52.y, bit_10, y_axis_range), vJoy_1, HID_USAGE_Y);
        SetAxis(updateAxis(0, 512, bit_10, 1), vJoy_2, HID_USAGE_X);
        SetAxis(updateAxis(1, 512, bit_10, 1), vJoy_2, HID_USAGE_Y);
        SetAxis(updateAxis(2, x52.rz, bit_10, 1), vJoy_1, HID_USAGE_RZ);
        SetAxis(updateAxis(3, Invert(x52.z), bit_8, 1), vJoy_1, HID_USAGE_Z);
        SetAxis(0, vJoy_1, HID_USAGE_SL1);
    }
    if (_alt)
    {   
        if (x52.mx == 8)
        {
            SetAxis(updateAxis(4, x52.rx, bit_8, 15), vJoy_1, HID_USAGE_RX);
        }
        else
        {
            SetAxis(SetPrez(updateAxis(7, x52.mx, 15, 0),6), vJoy_1, HID_USAGE_RX);
        }
        if (x52.my == 8)
        {
            SetAxis(updateAxis(5, x52.ry, bit_8, 15), vJoy_1, HID_USAGE_RY);
        }
        else
        {
            SetAxis(SetPrez(updateAxis(8, InvertMax(x52.my), 15, 0), 6), vJoy_1, HID_USAGE_RY);
        }
       
    }
    else
    {
        SetAxis(updateAxis(4, x52.rx, bit_8, 15), vJoy_1, HID_USAGE_RX);
        SetAxis(updateAxis(5, x52.ry, bit_8, 15), vJoy_1, HID_USAGE_RY);
        // Axis as Button
        if (x52.mx != 8)
        {
            SetBtn(x52.mx > 14, 2, 92);
            SetBtn(x52.mx < 1, 2, 94);
        }
        if (x52.my != 8)
        {
            SetBtn(x52.my < 1, 2, 91) ;
            SetBtn(x52.my > 14, 2, 93);
        }
    }
    SetAxis(updateAxis(6, x52.slider, bit_8, 1), vJoy_1, HID_USAGE_SL0);

    UpdateAxisLedMiddle(CLUTCH, x52.rx);
    UpdateAxisLedMiddle(FIRE_E, x52.ry);
    UpdateAxisLedRange(FIRE_D, x52.slider);
}

bool _mining_mode = false;
bool _quanta_mode = false;
 bool SpecialButton(UCHAR button)
{
    bool pressed = _buttonStates[button] == 0 ? false : true;
    switch (button)
    {
        //Missle & Mining
    case 3:
        if (pressed)
        {
            if (_mode == One)
            {
                _mode = Missle;
                SetText(0,  "Missle Operator");
                SetLeds();
                return false;
            }
            if (_mode == Missle)
            {
                _mode = One;
                SetText(0, "  Flight/Fight");
                SetLeds();
                return false;
            }
            if (_mode == Three && !_mining_mode)
            {
                _mining_mode = true;
                SetLed(FIRE_A, RED);
                SetText(0, " Mining Mode");
                return false;
            }
            else if (_mode == Three && _mining_mode)
            {
                _mining_mode = false;
                SetText(0, "     Mining");
                SetLeds();
                return false;
            }
        }
        break;
    case 4:
        if (pressed && _alt)
        {
            if (!_quanta_mode)
            {
                _quanta_mode = true;
                SetLed(FIRE_B, AMBER);
                return false;
            }
            else
            {
                _quanta_mode = false;
                SetLed(FIRE_B, OFF);
                return false;
            }
        }
        else if (pressed && !_alt)
        {
            if (!_quanta_mode)
            {
                SetLed(FIRE_B, RED);
                return false;
            }
            else
            {
                SetLed(FIRE_B, GREEN);
                return false;
            }
        }
        else if (!pressed && !_alt)
        {
            if (!_quanta_mode)
            {
                SetLed(FIRE_B, OFF);
                return false;
            }
            else
            {
                SetLed(FIRE_B, AMBER);
                return false;
            }
        }
        break;
    case 6:
        _alt = pressed;
        return true;
    case 10:
            if (pressed && _mode == One && !_alt)
            {
                std::thread licht(LauflichtAnimationGear);
                licht.detach();
            }
        return false;
    case 13:
        if (pressed && _mode == One && !_alt)
        {
            std::thread licht(LauflichtAnimationPower);
            licht.detach();
        }
        return false;
    case 28:
        _mode = One;
        SetText(0, "  Flight/Fight");
        ResetButtons(vJoy_1);
        ResetPovs(vJoy_1);
        ResetButtons(vJoy_2);
        ResetPovs(vJoy_2);
        SetLeds();
        return true;
    case 29:
        _mode = Two;
        SetText(0, " Turret/Ground");
        ResetButtons(vJoy_1);
        ResetPovs(vJoy_1);
        ResetButtons(vJoy_2);
        ResetPovs(vJoy_2);
        SetLeds();
        return true;
    case 30:
        _mode = Three;
        SetText(0, "     Mining");
        ResetButtons(vJoy_1);
        ResetPovs(vJoy_1);
        ResetButtons(vJoy_2);
        ResetPovs(vJoy_2);
        SetLeds();
        return true;
        //hat down
    case 26:
        if (pressed && _mode == Three && !_alt)
        {
            SetAxis(updateAxis(3, 128, 255, 1), vJoy_1, HID_USAGE_Z);
            return true;
        }
        else if (!pressed && _mode == Three && !_alt)
        { 
            SetAxis(0, vJoy_1, HID_USAGE_Z);
            return true;
        }
        return false;
    case 17:
    case 18:
    case 35:
    case 32:
    case 36:
        return true;
    }
    return false;
}

int clamp1024(int axis, int delta)
{
    int diff = axis - delta;
    if (diff <= 0 || axis == 0)
        return 0;
    if (diff >= 1023 || axis == 1023)
        return 1023;
    return diff;
}

int clamp255(int axis, int delta)
{
    int diff = axis - delta;
    if (diff <= 0 || axis == 0)
        return 0;
    if (diff >= 254 || axis == 254)
        return 254;
    return diff;
}

int updateAxis(int index, USHORT axis, int factor, int range)
{
    if (AntiJitter(_axisState[index], axis, range))
    {
        _axisState[index] = axis;
    }
    if (index == 0)
    {
        _axisState[index] = _xCurveMode[clamp1024(axis,delta_x)];
    }
    if (index == 1)
    {
        _axisState[index] = _yCurveMode[clamp1024(axis,delta_y)];
    }
    if (index == 4)
    {
        _axisState[index] = _rotCurveMode[axis];
    }
    if (index == 5)
    {
        _axisState[index] = _rotCurveMode[axis];
    }
    return (_axisState[index] * (32000 / factor));
}


 bool AntiJitter(int axis, USHORT axis_new, int range)
{
    return !(axis - range == axis_new || axis + range == axis_new || axis == axis_new);
}

 UCHAR Invert(UCHAR input)
 {
     return (UCHAR)~input;
 }

void __stdcall DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext) {
    if (bAdded) {
        devices.push_back(hDevice);
    }
}

void __stdcall DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext) {
    devices.push_back(hDevice);
}

void StartOutput()
{
    DirectOutput_Initialize(name);
    DirectOutput_RegisterDeviceCallback(*DirectOutput_Device_Callback, nullptr);
    DirectOutput_Enumerate(*DirectOutput_Enumerate_Callback, nullptr);
    DirectOutput_AddPage(devices[0], dwPage, pageDebugName, FLAG_SET_AS_ACTIVE);
    SetProfile("C:\\Users\\Public\\Documents\\Logitech\\X52 Professional\\empty.pr0");
    for (int i = 0; i < 24; ++i)
    {
        DirectOutput_SetLed(devices[0], dwPage, i, 0);
    }
}

void SetLeds()
{
    _mining_mode = false;
    _quanta_mode = false;
    if (_mode == One)
    {
        SetLed(FIRE, OFF);
        SetLed(THROTTLE, RED);
        SetLed(FIRE_A, GREEN);
        SetLed(FIRE_B, OFF);
        SetLed(POV, AMBER);
        SetLed(TOGGLE_1, GREEN);
        SetLed(TOGGLE_2, AMBER);
        SetLed(TOGGLE_3, RED);
    }
    if (_mode == Two)
    {
        SetLed(FIRE, OFF);
        SetLed(THROTTLE, RED);
        SetLed(FIRE_A, AMBER);
        SetLed(FIRE_B, OFF);
        SetLed(POV, RED);
        SetLed(TOGGLE_1, AMBER);
        SetLed(TOGGLE_2, AMBER);
        SetLed(TOGGLE_3, RED);
    }
    if (_mode == Three)
    {
        SetLed(FIRE, OFF);
        SetLed(THROTTLE, RED);
        SetLed(FIRE_A, GREEN);
        SetLed(FIRE_B, OFF);
        SetLed(POV, AMBER);
        SetLed(TOGGLE_1, GREEN);
        SetLed(TOGGLE_2, AMBER);
        SetLed(TOGGLE_3, RED);
    }
    if (_mode == Missle)
    {
        SetLed(FIRE, RED);
        SetLed(THROTTLE, RED);
        SetLed(FIRE_A, RED);
        SetLed(FIRE_B, OFF);
        SetLed(POV, AMBER);
        SetLed(TOGGLE_1, GREEN);
        SetLed(TOGGLE_2, AMBER);
        SetLed(TOGGLE_3, RED);
    }
}

int leds[11] = { 0,1,3,5,7,9,11,13,15,17,19 };
void SetLed(int index, int color)
{
    if (index > 10)
    {
        return;
    }
    bool onOff = false;
    int led_red = leds[index];
    int led_green = led_red + 1;
    if (index == 0 || index == 19)
    {
        onOff = true;
        color = color > 0 ? 1 : 0;
    }
    switch (color)
    {
    case 0:
        DirectOutput_SetLed(devices[0], dwPage, led_red, 0);
        if (!onOff)
        {
            DirectOutput_SetLed(devices[0], dwPage, led_green, 0);
        }
        break;
    case 1:
        DirectOutput_SetLed(devices[0], dwPage, led_red, 1);
        if (!onOff)
        {
            DirectOutput_SetLed(devices[0], dwPage, led_green, 0);
        }
        break;
    case 2:
        DirectOutput_SetLed(devices[0], dwPage, led_red, 0);
        DirectOutput_SetLed(devices[0], dwPage, led_green, 1);
        break;
    case 3:
        DirectOutput_SetLed(devices[0], dwPage, led_red, 1);
        DirectOutput_SetLed(devices[0], dwPage, led_green, 1);
        break;
    default:
        break;
    }
}

void UpdateAxisLedMiddle(Leds led, UCHAR axis_value)
{
    // 255 / 5 = 51
    if (axis_value >= 0 && axis_value <= 51)
    {
        SetLed(led, RED);
    }
    if (axis_value >= 52 && axis_value <= 102)
    {
        SetLed(led, AMBER);
    }
    if (axis_value >= 103 && axis_value <= 153)
    {
        SetLed(led, GREEN);
    }
    if (axis_value >= 154 && axis_value <= 204)
    {
        SetLed(led, AMBER);
    }
    if (axis_value >= 205 && axis_value <= 255)
    {
        SetLed(led, RED);
    }
}
void UpdateAxisLedRange(Leds led, UCHAR axis_value)
{
    // 255 / 3 = 85
    if (axis_value >= 0 && axis_value <= 85)
    {
        SetLed(led, RED);
    }
    if (axis_value >= 86 && axis_value <= 170)
    {
        SetLed(led, AMBER);
    }
    if (axis_value >= 171 && axis_value <= 255)
    {
        SetLed(led, GREEN);
    }
}

void RemovePage()
{
    DirectOutput_RemovePage(devices[0], dwPage);
    DirectOutput_Deinitialize();
}

void SetProfile(DWORD length, const char* string)
{
    const WCHAR* pwcsName;
    // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, 0);
    // allocate it
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, string, -1, (LPWSTR)pwcsName, nChars);
    DirectOutput_SetProfile(devices[0], length, pwcsName);
    delete[] pwcsName;
}

void SetText(DWORD line, std::string text)
{
    DWORD length = text.length();
    const char* string = text.c_str();
    const WCHAR* pwcsName;
    // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, 0);
    // allocate it
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, string, -1, (LPWSTR)pwcsName, nChars);
    DirectOutput_SetString(devices[0], dwPage, line, length, pwcsName);
    delete[] pwcsName;
}

void SetProfile(std::string path)
{
    const WCHAR* pwcsName;
    // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0);
    // allocate it
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, (LPWSTR)pwcsName, nChars);
    DirectOutput_SetProfile(devices[0], path.length(), pwcsName);
    delete[] pwcsName;
}

std::atomic<bool> _running(false);
void LauflichtAnimationPower()
{
    if (!_running.load())
    {
        _running = true;
        SetLed(TOGGLE_1, OFF);
        SetLed(TOGGLE_2, OFF);
        SetLed(TOGGLE_3, OFF);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_3, RED);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_2, RED);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_1, RED);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_3, AMBER);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_2, AMBER);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_1, AMBER);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_3, GREEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_2, GREEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_1, GREEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        SetLed(TOGGLE_1, OFF);
        SetLed(TOGGLE_2, OFF);
        SetLed(TOGGLE_3, OFF);
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
        SetLed(TOGGLE_1, GREEN);
        SetLed(TOGGLE_2, GREEN);
        SetLed(TOGGLE_3, GREEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
        SetLed(TOGGLE_1, OFF);
        SetLed(TOGGLE_2, OFF);
        SetLed(TOGGLE_3, OFF);
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
        SetLeds();
        _running = false;
    }
}

void LauflichtAnimationGear()
{
    if (!_running.load())
    {
        _running = true;
        SetLed(TOGGLE_2, OFF);
        for (int i = 0; i < 20; ++i)
        {
            SetLed(TOGGLE_1, OFF);
            SetLed(TOGGLE_3, AMBER);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            SetLed(TOGGLE_1, AMBER);
            SetLed(TOGGLE_3, OFF);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        SetLeds();
        _running = false;
    }
}