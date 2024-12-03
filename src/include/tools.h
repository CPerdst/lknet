//
// Created by l1Akr on 2024/12/3.
//

#ifndef LKNET_TOOLS_H
#define LKNET_TOOLS_H

#if defined(_WIN32) || defined(_WIN64)  // Windows ƽ̨
#ifdef EXPORTING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else  // �� Windows ƽ̨ (Linux, macOS ��)
#ifdef EXPORTING_DLL
#define DLL_API __attribute__ ((visibility ("default")))
#else
#define DLL_API
#endif
#endif

DLL_API void printClass();

#endif //LKNET_TOOLS_H
