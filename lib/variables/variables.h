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
    unsigned long RDeviation    = 40;
    unsigned long GDeviation    = 40;
    unsigned long BDeviation    = 40;
    Color runningColor          = Color::pink;
    Color idleColor             = Color::black;
    Color warningColor          = Color::orange;
    Color alarmColor            = Color::red;
    unsigned long runningRGB[3] = {255, 0, 255};
    unsigned long idleRGB[3]    = {0, 0, 0};
    unsigned long warningRGB[3] = {255, 165, 0};
    unsigned long alarmRGB[3]   = {255, 0, 0};
};

struct ColorValues {
    // store rgb values for different colors
    int pink[3]   = {255, 0, 255};
    int red[3]    = {255, 0, 0};
    int green[3]  = {0, 255, 0};
    int blue[3]   = {0, 0, 255};
    int yellow[3] = {255, 255, 0};
    int white[3]  = {255, 255, 255};
    int black[3]  = {0, 0, 0};
    int orange[3] = {255, 165, 0};
    int purple[3] = {128, 0, 128};
    int cyan[3]   = {0, 255, 255};
};

class variables {
  public:
    variables(/* args */);
    const char *printColors(Color theColor);
    void printConfig(Config &aConfig);

  private:
    /* data */
};
