/*
 * @filepath: 
 * @Descripttion: 
 * @version: 
 * @Author: ruixue.huang@yunzhou-tech.com
 * @Date: 2022-09-22 19:27:01
 * @LastEditors: huangruixue
 * @LastEditTime: 2022-09-26 18:55:44
 */
#ifndef _FILENAME_H_
#define _FILENAME_H_
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <cstring>  // 如果strcmp()函数报错，则使用<cstring>


int GetFileNames(std::string path,std::vector<std::string>& filenames);
#endif