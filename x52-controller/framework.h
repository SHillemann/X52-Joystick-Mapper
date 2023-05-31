#pragma once

#define NOMINMAX                        // exlude winbdow.h min max macros and use std c++
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <thread>
//std
#include <string>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>
//X52 Output
#include "include/DirectOutput.h"
//vJoy
#include "winioctl.h"
#include "include/public.h"
#include "include/vjoyinterface.h"
#include "x52-joystick.h"
