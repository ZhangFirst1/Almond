#pragma once

#include <string>

namespace Almond {

	class FileDialogs {
	public:
		// 如果取消返回空字符串
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}