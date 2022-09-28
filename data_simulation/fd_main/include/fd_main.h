/*
 * @filepath: 
 * @Descripttion: 
 * @version: 
 * @Author: ruixue.huang@yunzhou-tech.com
 * @Date: 2022-09-22 19:27:01
 * @LastEditors: huangruixue
 * @LastEditTime: 2022-09-26 14:52:06
 */
#ifndef _FD_MAIN
#define _FD_MAIN

#include <iostream>
#include <fstream>  
#include <string>
#include <vector>
#include <sys/io.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>

#include "dev_json.h"
#include "player.h"
#include "drv_set_query.h"
extern char *s_path;



class find_directory : public device_Data{
// public:
//     find_directory();
//     ~find_directory();
public:
    void show();
    void show_2();
    void show_3();
    void gpio_set();
    void serial_set(const char *serial);
    void adc_set();
private:
    DIR * dp;

};



#endif