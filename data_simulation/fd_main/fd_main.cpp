#include "fd_main.h"

// #define PATH "/home/oem/Desktop/project/sim_root"

void find_directory::show()
{
    printf("请输入功能前面对应的数字\n");
    printf("1. 数据配置功能\n");
    printf("2. 查看/修改回放数据文件的功能\n");
    printf("3. 回放数据功能\n");
    printf("4. 退出\n");
}

void find_directory::show_2()
{
    printf("1. gpio配置\n");
    printf("2. can配置\n");
    printf("3. usart配置\n");
    printf("4. adc配置\n");
    printf("5. 退出配置\n");
}

void find_directory::show_3()
{
    printf("1.创建并显示目录下文件\n");
    printf("2.查看gpio数据\n");
    printf("3.修改gpio数据\n");
    printf("4.查看adc数据\n");
    printf("5.修改adc数据\n");
    printf("6.返回\n");
}

void find_directory::gpio_set()
{
    int dev_index = -1;
    gpio_data g_data;
    int gpio_rule = 0;
    system("clear");
    while(1){
        system("clear");
        int a;
        printf("1. gpio索引: %d\n2. gpio输入输出 : %s\n3. gpio值 %d \n4. 生成配置\n", dev_index,g_data.direction.c_str(),g_data.value);
        std::cin >> a;
        switch(a)
        {
            case 1:
                std::cout << "gpio 索引：";
                std::cin >> dev_index;
                break;
            case 2:
                gpio_rule |= D;
                std::cout << "gpio 输入“in” 输出“out”:";
                std::cin >> g_data.direction;
                break;
            case 3:
                gpio_rule |= V;
                std::cout << "gpio 值：";
                std::cin >> g_data.value;
                break;
            case 4:
                system("clear");
                goto gpio_save;
            default :
                continue;
        }
    }
    gpio_save:
    set_gpio_data(dev_index,g_data,gpio_rule);
    save_to_json();
    gpio_rule = 0;
}

void find_directory::serial_set(const char *serial)
{
    int serial_rule = 0;
    int dev_index = -1;
    serial_data s_data;
    // if(strcmp(serial,"usart") == 0)
    // {
    //     s_data.path = strcat(s_path,"/usart/");
    // }
    // if(strcmp(serial,"can") == 0 )
    // {
    //     s_data.path = strcat(s_path,"/can/");
    //     const char* str = "a.log";
    //     if(strstr(str,"log")!=NULL)
    //     {
    //         strcat(const_cast<char*>(s_data.path.c_str()),str);
    //     }
    // }    
    system("clear");
    while(1){
        system("clear");
        int a;
        printf("1. %s索引: %d\n2. %s路径 : %s\n3. %s波特率 %s \n4. %s循环次数%d\n5. 生成配置\n", 
            serial, dev_index, serial, s_data.path.c_str(), serial, s_data.baud.c_str(), serial, s_data.looptime);
        std::cin >> a;
        switch(a)
        {
            case 1:
                std::cout << serial << "索引:";
                std::cin >> dev_index;
                break;
            case 2:
                serial_rule |= P;
                std::cout << serial << "路径:";
                std::cin >> s_data.path;
                break;
            case 3:
                serial_rule |= B;
                std::cout << serial << "波特率:";
                std::cin >> s_data.baud;
                break;
            case 4:
                serial_rule |= L;
                std::cout << serial << "循环次数:";
                std::cin >> s_data.looptime;
                break;
            case 5:
                system("clear");
                goto save;
            default :
                continue;
        }
    }
    save:
    if(strcmp(serial,"usart") == 0)
    {
        set_usart_data(dev_index,s_data,serial_rule);
        change_usartpath(s_data.path);
    }
    if(strcmp(serial,"can") == 0 )
    {
        set_can_data(dev_index,s_data,serial_rule);
        change_canpath(s_data.path);
    }
    save_to_json();
    
    serial_rule = 0;
}

void find_directory::adc_set()
{
    int dev_index = -1;
    int a_data = 0;
    system("clear");
    while(1)
    {
        system("clear");
        int a;
        printf("1. adc的索引:%d\n2. adc的值%d\n3. 生成配置\n",dev_index,a_data);
        std::cin >> a;
        switch(a)
        {
            case 1:
                std::cout << "adc的索引:";
                std::cin >> dev_index;
                break;
            case 2:
                std::cout << "adc的值:";
                std::cin >> a_data;
                break;
            case 3:
                system("clear");
                goto adc_save;
            default :
                continue;
        }
    }
    adc_save:
    set_adc_data(dev_index,a_data);
    save_to_json();
}

#if 0
int main(int argc,char **argv)
{
    find_directory fd_dir;
    fd_dir.json_file = "../../out.json";
    int number ,dev_index = -1;
    int a_data = 1;
    char add_flog;
    gpio_data g_data;
    serial_data s_data; 
    int gpio_rule = 0;
    uint8_t serial_rule = 0;


    if(argc >= 2){
        fd_dir.IsDirectory(argv[1]);
    }
    else{
        fd_dir.find_sim_root();
    }


    
    fd_dir.show();
    fd_dir.load_from_json();
    scanf("%d",&number);
    if(number == 1){
        system("clear");
        while(1){
            fd_dir.show_2();
            std::cin >> add_flog;
            if(add_flog == '1'){
                // fd_dir.gpio_set();
                #if 1
                while(1){
                    system("clear");
                    int a;
                    printf("1. gpio索引: %d\n2. gpio输入输出 : %s\n3. gpio值 %d \n4. 生成配置\n", dev_index,g_data.direction.c_str(),g_data.value);
                    std::cin >> a;
                    switch(a)
                    {
                        case 1:
                            std::cout << "gpio 索引：";
                            std::cin >> dev_index;
                            break;
                        case 2:
                            gpio_rule |= D;
                            std::cout << "gpio 输入“in” 输出“out”:";
                            std::cin >> g_data.direction;
                            break;
                        case 3:
                            gpio_rule |= V;
                            std::cout << "gpio 值：";
                            std::cin >> g_data.value;
                            break;
                        case 4:
                            system("clear");
                            goto gpio_save;
                        default :
                            continue;
                    }
                }
                gpio_save:
                fd_dir.set_gpio_data(dev_index,g_data,gpio_rule);
                fd_dir.save_to_json();
                gpio_rule = 0;
                #endif
            }else if(add_flog == '2'){
                system("clear");
                printf("can配置:\n");
                // fd_dir.serial_set();
                #if 1
                while(1){
                    system("clear");
                    int a;
                    printf("1. can索引: %d\n2. can路经 : %s\n3. can波特率 %s \n4. 循环次数%d\n5. 生成配置\n", dev_index,s_data.path.c_str(),s_data.baud.c_str(),s_data.looptime);
                    std::cin >> a;
                    switch(a)
                    {
                        case 1:
                            std::cout << "can 索引:";
                            std::cin >> dev_index;
                            break;
                        case 2:
                            serial_rule |= P;
                            std::cout << "can路径:";
                            std::cin >> s_data.path;
                            break;
                        case 3:
                            serial_rule |= B;
                            std::cout << "can 波特率：";
                            std::cin >> s_data.baud;
                            break;
                        case 4:
                            serial_rule |= L;
                            std::cout << "can 循环次数:";
                            std::cin >> s_data.looptime;
                            break;
                        case 5:
                            system("clear");
                            goto can_save;
                        default :
                            continue;
                    }
                }
                can_save:
                fd_dir.set_can_data(dev_index,s_data,serial_rule);
                fd_dir.save_to_json();
                serial_rule = 0;
                #endif
            }else if(add_flog == '3'){
                system("clear");
                printf("usart 配置:\n");
                // fd_dir.serial_set();
                #if 1
                while(1){
                    system("clear");
                    int a;
                    printf("1. usart索引: %d\n2. usart路经 : %s\n3. usart波特率 %s \n4. usart循环次数%d\n5. 生成配置\n", dev_index,s_data.path.c_str(),s_data.baud.c_str(),s_data.looptime);
                    std::cin >> a;
                    switch(a)
                    {
                        case 1:
                            std::cout << "usart 索引:";
                            std::cin >> dev_index;
                            break;
                        case 2:
                            serial_rule |= P;
                            std::cout << "usart路径:";
                            std::cin >> s_data.path;
                            break;
                        case 3:
                            serial_rule |= B;
                            std::cout << "usart 波特率：";
                            std::cin >> s_data.baud;
                            break;
                        case 4:
                            serial_rule |= L;
                            std::cout << "usart 循环次数:";
                            std::cin >> s_data.looptime;
                            break;
                        case 5:
                            system("clear");
                            goto usart_save;
                        default :
                            continue;
                    }
                }
                usart_save:
                fd_dir.set_usart_data(dev_index,s_data,serial_rule);
                fd_dir.save_to_json();
                serial_rule = 0;
                #endif
            }else if(add_flog == '4'){
                system("clear");
                // fd_dir.adc_set();
                #if 1
                while(1)
                {
                    system("clear");
                    int a;
                    printf("1. adc的索引:%d\n2. adc的值%d\n3. 生成配置\n",dev_index,a_data);
                    std::cin >> a;
                    switch(a)
                    {
                        case 1:
                            std::cout << "adc的索引:";
                            std::cin >> dev_index;
                            break;
                        case 2:
                            std::cout << "adc的值:";
                            std::cin >> a_data;
                            break;
                        case 3:
                            system("clear");
                            goto adc_save;
                        default :
                            continue;
                    }
                }
                adc_save:
                fd_dir.set_adc_data(dev_index,a_data);
                fd_dir.save_to_json();
                #endif
            }else if(add_flog == '5'){
                break;
            }else{
                printf("有效数字:1~4,5:结束添加\n");
                continue;
            }
            
        }


    }else if(number == 2){
        printf("查看/修改\n");
    }else if(number == 3){
        printf("回放\n");
    }else{
        fd_dir.show();
    }
    return 0;

}
#endif
