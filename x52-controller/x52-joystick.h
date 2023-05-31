#pragma once
struct x52_joystick
{
    unsigned short x;
    unsigned short y;
    unsigned short rz;
    unsigned char  z;
    unsigned char  rx;
    unsigned char  ry;
    unsigned char  slider;
    unsigned char  mx;
    unsigned char  my;
    unsigned char  size;
    unsigned char  button[12];
};

struct mapping
{
    std::string description;
    int joy;
    int button;
};

enum Mode
{
    One, Two, Three, Missle
};

enum Leds
{
    FIRE,
    FIRE_A,
    FIRE_B,
    FIRE_D,
    FIRE_E,
    TOGGLE_1,
    TOGGLE_2,
    TOGGLE_3,
    POV,
    CLUTCH,
    THROTTLE
};

enum Leds_Light
{
    OFF, RED, GREEN, AMBER
};