#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <csignal>
#include <unistd.h>

#include "main.h"

//#define DEBUG

class Curve
{
    public:

    std::vector<int> temperatures;
    std::vector<int> pwmValues;
};

Curve activeCurve;

const std::string tempPath = "/sys/class/drm/card0/device/hwmon/hwmon1/temp1_input";
const std::string pwmTogglePath = "/sys/class/drm/card0/device/hwmon/hwmon1/pwm1_enable";
const std::string pwmPath = "/sys/class/drm/card0/device/hwmon/hwmon1/pwm1";

const std::string configPath = "/etc/fancurve/curve";

bool halt = false;

void signal(int sig)
{
    halt = true;
}

int main()
{
    signal(SIGINT, signal);
    start();

    return 0;
}

void start()
{
    std::cout << "Started fancurve" << std::endl;

    readCurve();
    setPWMToggle(1);

    loop();
}

void end()
{
    std::cout << "Stopped fancurve" << std::endl;

    setPWMToggle(2);
    writeCurve();
}

void loop()
{
    while(!halt)
    {
        float temp = getTemperature();

        unsigned long stepCount = activeCurve.temperatures.size();
        for (int i = 0; i < stepCount; i++)
        {
            int nextTemp = activeCurve.temperatures[i];
            int nextPWM = activeCurve.pwmValues[i];

            if (i < stepCount)
            {
                nextTemp = activeCurve.temperatures[i + 1];
                nextPWM = activeCurve.pwmValues[i + 1];
            }

            if (temp <= nextTemp)
            {
                float inverseTempLerp = inverseLerp(activeCurve.temperatures[i], nextTemp, temp);
                float PWMLerp = std::lerp(activeCurve.pwmValues[i], nextPWM, inverseTempLerp);

                if (getPWM() == PWMLerp)
                    break;

#ifdef DEBUG
                std::cout << "Temp:" << temp << "C Fan:" << PWMLerp << "%" << std::endl;
#endif
                setPWM(PWMLerp);
                break;
            }
        }

        sleep(1);
    }

    end();
}

void readCurve()
{
    std::vector<std::string> vector = getFileString(configPath);

    int length = vector.size();

    for (int i = 0; i < length; i++)
    {
        int separator = vector[i].find(' ');
        if (separator == -1)
            continue;

        int temp = atoi(vector[i].substr(0, separator).c_str());
        int pwm = atoi(vector[i].substr(separator + 1).c_str());

        if ((temp == 0 || pwm == 0) && i > 0)
            continue;

        activeCurve.temperatures.push_back(temp);
        activeCurve.pwmValues.push_back(pwm);
    }
}

void writeCurve()
{
    std::ofstream file(configPath);

    if (!file.is_open())
        return;

    for (int i = 0; i < activeCurve.temperatures.size(); i++)
        file << activeCurve.temperatures[i] << " " << activeCurve.pwmValues[i] << std::endl;

    file.close();
}

void setPWM(int value)
{
    std::ofstream file(pwmPath);

    if (!file.is_open())
        return;

    int multiplied = value * 2.55;

    file << std::clamp(multiplied, 0, 255);
    file.close();
}

void setPWMToggle(int value)
{
    std::ofstream file(pwmTogglePath);

    if (!file.is_open())
        return;

    file << std::clamp(value, 0, 2);
    file.close();
}

float getTemperature()
{
    int value = getFileInt(tempPath);
    if (value < 0)
        return -1;

    return (float)value / 1000;
}

int getPWM()
{
    int value = getFileInt(pwmPath);
    if (value < 0)
        return -1;

    return (int)(value / 2.55);
}

int getPWMToggle()
{
    int value = getFileInt(pwmTogglePath);
    if (value < 0)
        return -1;

    return value;
}

int getFileInt(std::string filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
        return -1;

    std::string content;
    std::getline(file, content);

    file.close();

    return std::atoi(content.c_str());
}

std::vector<std::string> getFileString(std::string filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
        return {};

    std::string line;
    std::vector<std::string> content;

    while(std::getline(file, line))
        content.push_back(line);

    file.close();

    return content;
}

float inverseLerp(float x, float y, float value)
{
    return (value - x)/(y - x);
}