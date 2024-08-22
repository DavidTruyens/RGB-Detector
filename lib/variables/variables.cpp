#include "variables.h"
#include "logging.h"

extern uLog theLog;


variables::variables(/* args */)
{
}

const char *variables::printColors(Color theColor){
    switch (theColor) {
        case Color::pink:
            return "pink";
        case Color::red:
            return "red";
        case Color::green:
            return "green";
        case Color::blue:
            return "blue";
        case Color::yellow:
            return "yellow";
        case Color::white:
            return "white";
        case Color::black:
            return "black";
        case Color::orange:
            return "orange";
        case Color::purple:
            return "purple";
        case Color::cyan:
            return "cyan";
        default:
            return "unknown color";
    }
}

void variables::printConfig(Config &aConfig){
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Running color: %s", printColors(aConfig.runningColor));
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Idle color: %s", printColors(aConfig.idleColor));
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Warning color: %s", printColors(aConfig.warningColor));
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Alarm color: %s", printColors(aConfig.alarmColor));
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Running RGB: %d, %d, %d", aConfig.runningRGB[0], aConfig.runningRGB[1], aConfig.runningRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Idle RGB: %d, %d, %d", aConfig.idleRGB[0], aConfig.idleRGB[1], aConfig.idleRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Warning RGB: %d, %d, %d", aConfig.warningRGB[0], aConfig.warningRGB[1], aConfig.warningRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Alarm RGB: %d, %d, %d", aConfig.alarmRGB[0], aConfig.alarmRGB[1], aConfig.alarmRGB[2]);
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Deviations: R: %d, G: %d, B: %d", aConfig.RDeviation, aConfig.GDeviation, aConfig.BDeviation);
    theLog.snprintf(subSystem::general, loggingLevel::Info, "Brightness: %d", aConfig.brightness);
}