#ifndef _PTY_H_
#define _PTY_H_
#include <iostream>

extern "C"{

#include <stdio.h>
#include <sys/types.h>
#include <strings.h>
#include <sys/stat.h>
#include <pty.h>
#include <getopt.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
//int main_pty(int argc, char *const argv[]);
}

#define DEFAULT_TIME 1000

/*
每打开一个终端，/dev/pts/生成一个从设备文件「0,1,2....」,输入tty可以查看当前所在的终端

grantpt函数用于更改从设备的节点的权限为用户读写，组写。

unlockpt函数用于准予对伪终端从设备的访问，从而允许应用程序打开该设备。阻止其他进程打开从设备后，建立该设备的应用程序有机会在使用主，从设备之间正确地初始化这些设备。
*/


void get_time(char *buf);  //把输进来字符串内的时间戳转成标准时间 (未使用)
unsigned int count_diff(char *buf, long* time);  //记录时间差值
int log_buf_to_hex(uint8_t * des_buf, char *src_buf);  //把字符串转成16进制提取
uint8_t char_to_int(char c);  //把字符转成int
int main_pty(int argc, char * argv[]);
#endif