/*
 * @filepath: 
 * @Descripttion: 
 * @version: 
 * @Author: ruixue.huang@yunzhou-tech.com
 * @Date: 2022-09-22 16:16:19
 * @LastEditors: huangruixue
 * @LastEditTime: 2022-09-23 09:34:45
 */
#ifndef DRV_SET_QUERY_H
#define DRV_SET_QUERY_H

#include <iostream>
#include "dev_json.h"

#define BUF 1024
#define BUFSIZE 64

class Set_Query{
public:
        Set_Query();
        void showAllFiles( const char * dir_name );              //遍历文件
        void creat_file(char buf0[BUF],std::string json);      //根据配置文件创建目录文件
        int all_read(char buff[BUF]);         //读
        int all_write(char buf[BUF]);        //写

        int set_gpio(char buf0[BUF]);         //设置gpio文件的值
        int query_gpio(char buf0[BUF]);       //查询gpio文件的内容

        int set_adc(char buf0[BUF]);          //设置adc文件的值
        int query_adc(char buf0[BUF]);        //查询adc文件的内容

public:
        device_Data dev;
        gpio_data s;            //得gpio数据
        serial_data k;          //得usart can 数据
        char sg[BUFSIZE];
};

#endif // DRV_SET_QUERY_H