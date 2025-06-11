#pragma once

#define ENABLE_LOGGING 1

#if ENABLE_LOGGING
#include <fstream>
inline std::ofstream logFile("wndwalk.log", std::ios::app);
#define LOG(msg) do { logFile << msg << std::endl; } while (0)
#else
#define LOG(msg) do {} while (0)
#endif
