#pragma once

enum Color {
    pink,
    red,
    green,
    blue,
    yellow,
    white,
    black,
    orange,
    purple,
    cyan,
    unknown
};

struct Config {
    int RDeviation     = 10;
    int GDeviation     = 10;
    int BDeviation     = 10;
    Color runningColor = Color::pink;
    Color idleColor    = Color::black;
    int runningRGB[3]  = {255, 0, 255};
    int idleRGB[3]     = {0, 0, 0};
    bool RGBoutput     = true;
};

class variables
{
public:
    variables(/* args */);
    char *printColors(Color theColor);
private:
    /* data */
};
