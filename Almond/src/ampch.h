#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <array>

#include "Almond/Core/Log.h"
#include "Almond/Debug/Instrumentor.h"

#ifdef AM_PLATFORM_WINDOWS
	#include <Windows.h>
#endif