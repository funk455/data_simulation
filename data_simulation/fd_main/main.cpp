#include "fd_main.h"

class data_set : public Set_Query
{
};
char *s_path;
find_directory m_fd;
int j_exsit;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("缺少命令行参数 现已有 %s\n", argv[0]);
        return -1;
    }

    
    data_set data;

    char number;
    char add_flog;
    DIR *dp;
    DIR *dp1;
    //printf("22222222\n");
    dp = opendir(argv[1]);

    if (dp == NULL)
    {
        s_path = getenv("HOME");
        s_path = strcat(s_path, "/sim_root");
        // printf("%s",s_path);

        dp1 = opendir(s_path);
        if (dp1 == NULL)
        {
            perror("open sim_root error");
            return -1;
        }
        else
        {
            printf("find sim_root sunccess\n");
        }
    }
    else
    {
        // printf("open sim_root sunccess\n");
        s_path = argv[1];
    }
    m_fd.json_file = std::string(s_path) + "/out.json";
     j_exsit = m_fd.load_from_json();
    char buf0[1024]={0};
    strcpy(buf0,s_path);
    if (dp)
    {
        char s_path_1[1024];
        strcpy(s_path_1, argv[1]);
        change_allpath(argv[1]);

        if( j_exsit == 1 )
        {
            serial_data can_t, usart_t; 
            m_fd.get_can_data(0,can_t);
            change_canpath(can_t.path);
            m_fd.get_usart_data(0,usart_t);
            change_usartpath(usart_t.path);
            
        }
        else
        {
            s_path = strcat(argv[1], "/can");
            change_canpath(argv[1]);      
            change_usartpath( std::string(s_path) + "/usart");      
        }


        //s_path = strcat(s_path_1, "/usart");
        
    }
    else if (dp1)
    {
        char s_path_2[1024];
        strcpy(s_path_2, s_path);
        change_allpath(s_path);
        
        if( j_exsit == 1 )
        {
            serial_data can_t, usart_t; 
            m_fd.get_can_data(0,can_t);
            change_canpath(can_t.path);
            m_fd.get_usart_data(0,usart_t);
            change_usartpath(usart_t.path);
            // std::cout << t.path << std::endl;
            // pause();
        }
        else
        {
            //strcat(s_path_2, "/can");
            change_canpath(std::string(s_path) + "/can"); 
            change_usartpath(std::string(s_path) + "/usart");             
        }
        //strcat(s_path_2, "/usart");
        //change_usartpath(s_path_2);
       
    }
    

    device_Data dev;
    serial_data s_d;
    dev.get_usart_data(2, s_d);

    std::cout << s_d.path << std::endl;
    while (1)
    {
        system("clear");
        if (dp)
        {
            printf("当前目录处于%s\n", argv[1]);
        }
        else if (dp1)
        {
            printf("当前目录处于%s\n", s_path);
        }
        m_fd.show();
        m_fd.load_from_json();
        scanf("%c", &number);

        if (number == '1')
        {
            printf("数据配置功能\n");
            system("clear");
            while (1)
            {
                m_fd.show_2();
                std::cin >> add_flog;
                if (add_flog == '1')
                {
                    m_fd.gpio_set();
                }
                else if (add_flog == '2')
                {
                    m_fd.serial_set("can");
                }
                else if (add_flog == '3')
                {
                    m_fd.serial_set("usart");
                }
                else if (add_flog == '4')
                {
                    m_fd.adc_set();
                }
                else if (add_flog == '5')
                {
                    break;
                }
                else
                {
                    printf("有效数字:1~4,5:结束添加\n");
                    continue;
                }
            }
        }
        else if (number == '2')
        {
            system("clear");
            printf("查看/修改回放数据文件的功能\n");

            char a = 0;
            while (1)
            {
                m_fd.show_3();
                std::cout << "请输入要操作的功能：" << std::endl;
                std::cin >> a;
                if (a == '1')
                {
                    data.creat_file(buf0,m_fd.json_file);
                }
                else if (a == '2')
                {
                    int len = data.query_gpio(buf0);
                    std::cout << "读取字节数：" << len << std::endl;
                }
                else if (a == '3')
                {
                    int len = data.set_gpio(buf0);
                    std::cout << "读取字节数：" << len << std::endl;
                }
                else if (a == '4')
                {
                    int len = data.query_adc(buf0);
                    std::cout << "读取字节数：" << len << std::endl;
                }
                else if (a == '5')
                {
                    int len = data.set_adc(buf0);
                    std::cout << "读取字节数：" << len << std::endl;
                }
                else if (a == '6')
                {
                    break;
                }
            }
        }
        else if (number == '3')
        {
            system("clear");
            printf("回放数据功能\n");
           
            playback_data();
        }
        else if (number == '4')
        {
            break;
        }
        else
        {
            continue;
        }
    }

    return 0;
}