#ifndef RESOURCELOADER_H_INCLUDED
#define RESOURCELOADER_H_INCLUDED

#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include "base/FileInputStream.h"

#if defined(_WIN32)
    #include <windows.h>
    extern "C" IMAGE_DOS_HEADER __ImageBase;
#else
    #include <dlfcn.h>
#endif

namespace ResourceLoader {

    inline std::string& getBasePath() {
        static std::string basePath = []() -> std::string {
            char buffer[1024];

        #if defined(_WIN32)
            HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
            GetModuleFileNameA(hModule, buffer, sizeof(buffer));
        #else
            Dl_info info;
            dladdr((void*)getBasePath, &info);
            strncpy(buffer, info.dli_fname, sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';
        #endif

            std::string fullPath(buffer);
            size_t slash = fullPath.find_last_of("/\\");
            return (slash != std::string::npos) ? fullPath.substr(0, slash) : ".";
        }();

        return basePath;
    }

    inline FileInputStream getResource(const std::string& name) {
        std::string sep =
        #if defined(_WIN32)
            "\\";
        #else
            "/";
        #endif

        return FileInputStream(getBasePath() + sep + name);
    }
}


#endif
