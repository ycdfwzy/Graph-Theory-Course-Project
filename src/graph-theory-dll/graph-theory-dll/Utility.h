#pragma once
#include <string>
using namespace std;

/*!
 * @brief 将string转为堆中的char*,方便javascript读取和删除
 */
const char* heap_str(const string& str);