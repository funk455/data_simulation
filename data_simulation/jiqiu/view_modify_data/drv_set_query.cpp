#include "drv_set_query.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 
#include <iostream>
#include <sys/stat.h>
using namespace std;

char buf[BUF]={0};                                             //读取文件内容的缓存区
char buff[BUF]={0};                                        //存储路径 (创建目录，创建文件，读写文件
int const MAX_STR_LEN = 200;

Set_Query::Set_Query(){
    this->sg[BUFSIZE]={0};
}
void Set_Query::showAllFiles(const char * dir_name){
if( NULL == dir_name )
	{
		cout<<" dir_name is null ! "<<endl;
		return;
	}
	struct stat s;
	lstat( dir_name , &s );
	if( ! S_ISDIR( s.st_mode ) )
	{
		cout<<"dir_name is not a valid directory !"<<endl;
		return;
	}
	
	struct dirent * filename;    
 	DIR * dir;                   
	dir = opendir( dir_name );
	if( NULL == dir )
	{
		cout<<"Can not open dir "<<dir_name<<endl;
		return;
	}
	cout<<"Successfully opened the dir !"<<endl;
	while( ( filename = readdir(dir) ) != NULL )
	{
		if( strcmp( filename->d_name , "." ) == 0 || 
			strcmp( filename->d_name , "..") == 0    )
			continue;
		cout<<filename ->d_name <<endl;
	}
}
/*给一个路径，就能去修改和查询
根据配置文件，来创建总的目录文件，open或者程序里面的命令来创建*/                  
void Set_Query::creat_file(char buf0[1024],std::string json){
    dev.json_file = json;
    dev.load_from_json();
    sprintf(buff,"%s%s",buf0,"/adc");
    cout<<buff<<"路径下有adc文件: "<<endl;
    showAllFiles(buff);
    memset(buff,0,BUFSIZE);
    for (auto it = dev.adc.adc_map.begin();it != dev.adc.adc_map.end();it++)
    {
        sprintf(buff,"%s%s",buf0,"/adc");
        if(NULL==opendir(buff)){
            int ret = mkdir(buff,0775);
            if(ret < 0){
                perror("创建目录失败: ");
                break;
            }
        }
        memset(buff,0,BUF);
        sprintf(buff,"%s%s%s",buf0,"/adc/",it->first.c_str());
        int fd = open(buff,O_RDWR | O_CREAT,0777);
        if(fd < 0){
            perror("创建文件失败: ");
        }
        const char *p = it->first.c_str();
        int a;
        dev.get_adc_data(atoi(&p[2]),a);
        char b[BUFSIZE]={0};
        sprintf(b,"%d",a);
        write(fd,b,strlen(b));
        memset(buff,0,BUFSIZE);
        p=NULL;
        close(fd);
    }
    // cout<<"这里有can文件: "<<endl;cout<<"in0_input,in1_input, in2_input, in3_input, in4_input, in5_input, in6_input,in7_input"<<endl;
    //             perror("创建目录失败: ");
    //             break;
    //         }   
    //     }
    //     cout<< it->first <<".log"<<endl;
    //     sprintf(buff,"%s%s%s",s2,it->first.c_str(),".log");
    //     int fd = open(buff,O_RDWR | O_CREAT,0777);
    //     if(fd < 0){
    //             perror("创建文件失败: ");
    //     }
    //     const char *p = it->first.c_str();
    //     dev.get_can_data(atoi(&p[3]),k);
    //     write(fd,k.path.c_str(),strlen(k.path.c_str()));
    //     p=NULL;
    //     memset(buff,0,BUFSIZE);
    //     close(fd);
    // }
    // cout<<"这里有usart文件: "<<endl;
    // for (auto it = dev.usart.serial_map.begin();it != dev.usart.serial_map.end();it++)
    // {
    //     if(NULL==opendir(s3)){
    //         int ret = mkdir(s3,0775);
    //         if(ret < 0){
    //             perror("创建目录失败: ");
    //             break;
    //         }
    //     }
    //     cout<< it->first <<endl;
    //     sprintf(buff,"%s%s",s3,it->first.c_str());
    //     int fd = open(buff,O_RDWR | O_CREAT,0777);
    //     if(fd < 0){
    //             perror("创建文件失败: ");
    //     }
    //     const char *p = it->first.c_str();
    //     dev.get_usart_data(atoi(&p[4]),k);
    //     write(fd,k.path.c_str(),strlen(k.path.c_str()));
    //     p=NULL;
    //     memset(buff,0,BUFSIZE);
    //     close(fd);
    // }
    sprintf(buff,"%s%s",buf0,"/gpio");
    cout<<buff<<"路径下有gpio文件: "<<endl;
    showAllFiles(buff);
    for (auto it = dev.gpio.gpio_map.begin();it != dev.gpio.gpio_map.end();it++)
    {
        sprintf(buff,"%s%s%s",buf0,"/gpio/",it->first.c_str());
        if(NULL==opendir(buff)){
            int ret = mkdir(buff,0775);
            if(ret < 0){
                perror("创建目录失败: ");
                break;
            }
        }
        memset(buff,0,BUFSIZE);
        sprintf(buff,"%s%s%s%s",buf0,"/gpio/",it->first.c_str(),"/value");
        int fd = open(buff,O_RDWR | O_CREAT,0777);
        if(fd < 0){
                perror("创建文件失败: ");
        }
        const char *p = it->first.c_str();
        dev.get_gpio_data(atoi(&p[4]),s);
        char b[BUFSIZE]={0};
        sprintf(b,"%d",s.value);
        write(fd,b,strlen(b));
        p=NULL;
        memset(buff,0,BUFSIZE);
        close(fd);
        sprintf(buff,"%s%s%s%s",buf0,"/gpio/",it->first.c_str(),"/direction");
        int fd1 = open(buff,O_RDWR | O_CREAT,0777);
        if(fd1 < 0){
                perror("创建文件失败: ");
        }
        const char *q = it->first.c_str();
        dev.get_gpio_data(atoi(&q[4]),s);
        write(fd1,s.direction.c_str(),strlen(s.direction.c_str()));
        memset(buff,0,BUFSIZE);
        close(fd1);
    }
}
int Set_Query::all_write(char buff[BUF]){
    int fd =open(buff,O_RDWR | O_TRUNC);
    char info[BUF] = {0};
    if(fd < 0){
        perror("open error:");
        return -1;
    }
    else{
    cout<<"请输入想要写入的内容: "<<endl;
    cin>>info;
    int size = write(fd,info,strlen(info));
    if(size < 0){
        perror("write error: ");
    }
    memset(buff,0,BUFSIZE);
    memset(sg,0,BUFSIZE);
    lseek(fd,0,SEEK_SET);
    close(fd);
    return size;
    }
}
int Set_Query::all_read(char buff[BUF]){
    int fd =open(buff,O_RDWR);
    if(fd < 0){
        perror("open error:");
        return -1;
    }
    else{
    int size = read(fd,buf,BUF);
    if(size < 0){
        perror("read error:");
    }
    cout<<"文件信息内容是: "<<buf<<endl;
    memset(buf,0,BUFSIZE);
    memset(buff,0,BUFSIZE);
    memset(sg,0,BUFSIZE);
    lseek(fd,0,SEEK_SET);
    close(fd);
    return size;
    }
}

int Set_Query::set_gpio(char buf0[BUF]){
    cout<<"请输入你要修改的gpio文件编号:"<<endl;
    cin>>sg;
    sprintf(buff,"%s%s%s%s",buf0,"/gpio/",sg,"/value");
    int writelen = all_write(buff);
    return writelen;
}
int Set_Query::query_gpio(char buf0[BUF]){
    cout<<"请输入你要查询的gpio文件编号:"<<endl;
    cin>>sg;
    sprintf(buff,"%s%s%s%s",buf0,"/gpio/",sg,"/value");
    int readlen = all_read(buff);
    return readlen;
}

int Set_Query::set_adc(char buf0[BUF]){
    cout<<"请输入你要修改的adc文件编号:"<<endl;
    cin>>sg;
    sprintf(buff,"%s%s%s",buf0,"/adc/",sg);
    int writelen = all_write(buff); 
    return writelen;
}
int Set_Query::query_adc(char buf0[BUF]){
    cout<<"请输入你要查询的adc文件编号:"<<endl;
    cin>>sg;
    sprintf(buff,"%s%s%s",buf0,"/adc/",sg); 
    int readlen = all_read(buff);
    return readlen;
}

