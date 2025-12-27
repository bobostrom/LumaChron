
#include <signal.h>
#include <vector>
#include <thread>
#include <fstream>
#include <gpiod.h>
#include <algorithm>
#include <map>
#include <filesystem>
// #include "colorConstants.hpp"
#include "Display.hpp"
#include "fdComm.hpp"
#include "regNum.hpp"
#include "GPIOaccess.hpp"
#include "MessageBuffer.hpp"
// #include "stdioComm.hpp"
// #include "Buttons.hpp"
#include "json.hpp"
using json = nlohmann::json;


using namespace std;
void alarmWakeup(int sig_num);
int i;
int file;
char buf[10];
std::chrono::steady_clock::time_point currTime; 
char timeString[std::size("HHMMSS")];
