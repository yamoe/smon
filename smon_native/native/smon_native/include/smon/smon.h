#pragma once

#if defined(_WIN32)
#	define WINDOWS
# define _CRT_SECURE_NO_WARNINGS
# define WIN32_LEAN_AND_MEAN
# include <SDKDDKVer.h>
# include <windows.h>
#elif defined(__linux__)
#	define LINUX
# include <cstdarg>
#elif defined(__unix__)
#	error not implemented
#elif defined(__APPLE__)
#	error not implemented
#else
#	error not implemented
#endif

#include <ctime>
#include <stdint.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>

#include "util/util.h"
#include "util/stack_trace.h"
#include "util/log.h"
#include "util/char_array.h"

#include "mon/data.h"
#include "mon/interface.h"
#if defined(WINDOWS)
# include "mon/win/manager.h"
#else
# include "mon/linux/manager.h"
#endif



namespace smon {
  static Manager& get_manager()
  {
#if defined(WINDOWS)
    static smon::Manager* inst_ = new win::Manager();
#else
    static smon::Manager* inst_ = new linux::Manager();
#endif
    return *inst_;
  }

}

