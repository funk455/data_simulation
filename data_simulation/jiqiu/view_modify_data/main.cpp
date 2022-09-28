/*
 * @filepath: 
 * @Descripttion: 
 * @version: 
 * @Author: ruixue.huang@yunzhou-tech.com
 * @Date: 2022-09-22 16:16:19
 * @LastEditors: huangruixue
 * @LastEditTime: 2022-09-22 16:42:16
 */
#include "drv_set_query.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "dev_json.h"
using namespace std;

int main(int argc, const char** argv) {
    Set_Query sq;                           //创建对象调用各个函数
    sq.creat_file();
    char str[]={0};
    while(1){
        cout<<"输入你的操作: setgpio /querygpio /setadc /queryadc"<<endl;
        cin>>str;                              
        if(strcmp(str,"setgpio")==0){
            int len_sg = sq.set_gpio();                             
            cout<<"写入字节数："<<len_sg<<endl;
        }
        else if(strcmp(str,"querygpio")== 0){
            int len_qg = sq.query_gpio();                           
            cout<<"读取字节数："<<len_qg<<endl;
        }
        else if(strcmp(str,"setadc") == 0){
            int len_sa = sq.set_adc();                               
            cout<<"写入字节数："<<len_sa<<endl;
        }
        else if(strcmp(str,"queryadc") ==0 ){
            int len_qa = sq.query_adc();                             
            cout<<"读取字节数："<<len_qa<<endl;
        // }
        cout<<"\n"<<endl;
    }
    }
        return 0;
}
