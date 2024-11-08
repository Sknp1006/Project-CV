#ifndef _H_UTILS_UTILS
#define _H_UTILS_UTILS

#include <string>

extern "C" {
    #include "whereami.h"
}

extern std::string g_ExecutablePath;
extern std::string g_ModulePath;

/// @brief 获取可执行文件运行路径
/// @param Path 
static void GetExecutablePath(std::string& Path)
{
    char *path = nullptr;
    int length, dirname_length;

    length = WAI_PREFIX(getExecutablePath)(nullptr, 0, &dirname_length);
    if (length > 0)
    {
        path = (char*)malloc(length + 1);
        WAI_PREFIX(getExecutablePath)(path, length, &dirname_length);
        path[length] = '\0';

        printf("executable path: %s\n", path);
        path[dirname_length] = '\0';
        printf("  dirname: %s\n", path);
        printf("  basename: %s\n", path + dirname_length + 1);
        Path = std::string(path);
        free(path);
    }
}
/// @brief 获取模块运行路径
/// @param Path 
static void GetModulePath(std::string& Path)
{
    char *path = nullptr;
    int length, dirname_length;

    length = WAI_PREFIX(getModulePath)(nullptr, 0, &dirname_length);
    if (length > 0)
    {
        path = (char*)malloc(length + 1);
        WAI_PREFIX(getModulePath)(path, length, &dirname_length);
        path[length] = '\0';

        printf("module path: %s\n", path);
        path[dirname_length] = '\0';
        printf("  dirname: %s\n", path);
        printf("  basename: %s\n", path + dirname_length + 1);
        Path = std::string(path);
        free(path);
    }
}

#endif // _H_UTILS_UTILS