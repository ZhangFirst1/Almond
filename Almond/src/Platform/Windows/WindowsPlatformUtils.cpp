#include "ampch.h"
#include "Almond/Utils/PlatformUtils.h"
#include "Almond/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Almond {

	std::string FileDialogs::OpenFile(const char* filter)
	{
		// 定义并初始化 Windows API 用于打开文件对话框的结构体
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };					// 存储最终选中文件的完整路径（最多 260 个字符）
		CHAR currentDir[256] = { 0 };				// 获取当前目录路径
		ZeroMemory(&ofn, sizeof(OPENFILENAME));		// 清空结构体内存
		ofn.lStructSize = sizeof(OPENFILENAME);		// 设置结构体大小

		// 设置拥有窗口句柄（用于让对话框在应用窗口上弹出）
		// 这里使用了 GLFW + Win32 的混合方式获取本地窗口句柄
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		// 设置输出文件路径缓冲区及其大小
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		// 获取当前工作目录，作为对话框初始打开路径
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		// 设置文件类型过滤器，例如：
		// "Almond Scene (*.almond)\0*.almond\0"
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		// 设置打开文件对话框的标志：路径必须存在、文件必须存在、不更改当前工作目录
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		// 调用 Windows API 打开文件对话框，如果用户选择了文件就返回路径
		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		// 否则返回空字符串（用户取消、或出错）
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		// ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

}