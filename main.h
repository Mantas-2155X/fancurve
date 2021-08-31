#ifndef FANCURVE_MAIN_H
#define FANCURVE_MAIN_H

void start();
void end();
void loop();
void readCurve();
void writeCurve();
void setPWM(int);
void setPWMToggle(int);
float getTemperature();
int getPWM();
int getPWMToggle();
int getFileInt(std::string);
std::vector<std::string> getFileString(std::string);
float inverseLerp(float, float, float);

#endif