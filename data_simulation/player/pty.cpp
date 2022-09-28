#include "pty.h"


extern char devname[100];
int main_pty(int argc, char * argv[])
{
    //system("clear");
    int c;
    int read_flag = 0;
    int loop_num = 1;
    int loop_alway = 0;
    int log_open_flag = 0;
    char log_file_name[50]; 
    int option_index=0;
    optind=0;
    static struct option longOpts[] = {
      { "input", required_argument, NULL, 'I' },
      { 0, 0, 0, 0 }
    };

    while ( (c = getopt_long(argc, argv, "I:l::o", longOpts, &option_index)) != -1)
    {

        switch(c)
        {
            case '?':
                return 0;
            case 'I':
                log_open_flag = 1;
                strcpy(log_file_name,optarg);  //获取文件路径
                break;
            case 'l':
                if(optarg == NULL)
                {
                    loop_alway = 1;  //无限循环
                }
                else
                {
                    loop_num = atoi(optarg);
                }
                break;
            case 'o':
                {
                    read_flag = 1;
                }

        }
    }

    if(log_open_flag == 0)
    {
        printf("要指定文件路径...\n");
        return 0;
    }

    //打开主终端，并生成1个从终端文件。
    int fd_m = open("/dev/ptmx", O_RDWR | O_NOCTTY); //O_NOCTTY ：该参数不会使打开的文件成为该进程的控制终端。如果没有指定这个标志，那么任何一个 输入都将会影响用户的进程。
    //printf("ptm: %d\n",fd_m );

    const char *pts_name;
    pts_name = (const char *)ptsname(fd_m);

    std::cout << pts_name << std::endl;  //打印从终端文件名字
    printf("devname %s \n",devname);
    int devnamefd=open(devname,O_RDWR|O_TRUNC);
    if ( devnamefd < 0 )
    {
        std::cout<<"errno"<<std::endl;
    }
    
    write(devnamefd,pts_name,strlen(pts_name));
    close(devnamefd);
    //grantpt, unlockpt: 在每次打开pseudoterminal slave的时候，必须传递对应的PTM的文件描述符。grantpt以获得权限，然后调用unlockpt解锁 
    //grantpt(fd_m);
    unlockpt(fd_m);

    int fd_s;
    if(read_flag == 1)
        fd_s = open(pts_name, O_RDONLY | O_NOCTTY); //通过「从终端文件名字」获得从终端的「文件描述符」,如没有unlockpt会失败
    

    char    log_buf[256]; //从log提取到的一行字符串
    uint8_t buf_send[256];  //存有8位的数据
    uint8_t buf_read[256];
    int len = 0;

    bzero(buf_send, 256);
    bzero(buf_read, 256);

    FILE *f_log =  fopen(log_file_name, "r"); //只读打开microcom.log
    if(f_log == NULL)
    {
        perror( "文件打开失败" );
        return 0;
    }
    
    /*  读取1行   */
    //fscanf(f_log,"%[^\n]", buf_read);  //方法1
    //fgets(buf_read, sizeof(buf_read), f_log);  //方法2
    //cout << buf_read << endl;

    long last_time = 0; //记录上一次的时间戳
    int i;

    //若loop_alway为1，loop_num--不执行，则进行无限循环
    while (loop_alway || loop_num--)
    {
        rewind(f_log);   //把输进来字符串内的时间戳转成标准时间
        last_time = DEFAULT_TIME;   //重置时间
        while (fgets(log_buf, sizeof(buf_send), f_log) != NULL)
        {
            
            usleep( count_diff(log_buf,&last_time) ); //计算差值睡眠
            //get_time(buf_send);  //把输进来字符串内的时间戳转成标准时间

            /* 字符串转hex */
            memset(buf_send, 0, 256);
            len = log_buf_to_hex(buf_send, log_buf);  //把log读取的字符串转成hex存储

            #if 0//测试
            printf("buf_send = ");
            for(int o = 0; o <=len; o++)
            {
                printf("%x ", *(buf_send+o));
            }
            printf("\n");
            #endif

            /* 写入 */
            write(fd_m, buf_send, len);  //往主端写入16进制
            std::cout << "写入主端" << std::endl;

            /* 读出 */
            if(read_flag == 1)
            {
                memset(buf_read, 0, 256);
                read(fd_s, buf_read, len);   // 读出pty从端 打印

                std::cout << "读出: ";
                for(i = 0; i < len; ++i)
                {
                    printf("0x%x ",buf_read[i]);

                }
                std::cout << std::endl;
                //printf("%s",(char *)buf_read);
                
            }
            std::cout << std::endl;
        }            
    }

    //此时read只有遇到换行符’\n’的时候才会返回，否则遇不到的话一直阻塞在那里
    //read(fd_s,buf_read,128);

    fclose(f_log);
    close(fd_m);
    close(fd_s);
   
    return 0;
}

//把输进来字符串内的时间戳转成标准时间
void get_time(char *buf)
{
    time_t t; //时间， long int
    struct tm *p;  //时间结构体616161616264
    char t_buf[128];  //存放时间戳的字符串
    bzero(t_buf,128);

    sscanf(buf,"(%[^.]",t_buf);  //提取时间戳字符串
    t=atol(t_buf);  //转成long int
    p=gmtime(&t); //使用 t 的值来填充 tm 结构
    strftime(t_buf, sizeof(t_buf), "%Y-%m-%d %H:%M:%S", p);

    strcpy(buf,strchr(buf, ')')+1);
    strcat(t_buf,buf);
    strcpy(buf,t_buf);
}

unsigned int count_diff(char *buf, long* last_time)
{

    char t_buf[32]; //存放提取到的时间戳
    char *des = t_buf; //目标偏移指针            pause();
    char *src = buf+1;  //源偏移指针
    long now_time;    //当前时间
    unsigned int diff;   //要返回的差植

    while ( *src != ')' )
    {
        if( *src !='.' )
        {
            *des = *src;
            ++des;
        }
        ++src;
    }
    
    now_time = atol(t_buf);  //得到当前微秒

    diff = now_time-*last_time;  //计算差值
    if(*last_time == DEFAULT_TIME)
    {
        diff = DEFAULT_TIME;
    }
    *last_time = now_time;
    std::cout << "时间戳的差值：" << "\033[33m"<< (float)diff/1000000 << "\033[0m"<< "秒,  阻塞睡眠中.." << std::endl;
    return diff;
}

int log_buf_to_hex(uint8_t * des_buf, char *src_buf)
{
    int len = 0;
    char * s = strchr(src_buf , ')') + 1; //获取要转化的字符串
    if( s == 0 )
    {
        return 0;
    }
//    printf("s = %s\n",s);

    while (*s != 0x0a)
    {
        //如0x61, *s = '6' *(s+1) = '1'

        //先处理高4位
        *des_buf = char_to_int(*s) << 4; //此时6应存在高4位， des_buf = 0000 0110 << 4 = 0110 0000
        ++s;
        ++len;

        //低4位
        if(*s != 0x0a)
        {
            *des_buf |= char_to_int(*s); //把1存在低4位  des_buf = 0110 0000 | 0000 0001
            ++s; 
        }
        ++des_buf;        
    }
    *des_buf = *s;
    return len+1;  //把结束符0x0a算上
}

uint8_t char_to_int(char c)
{
    //a的ascii码是97,A是65,0是48
    if(c >= 'a' && c <= 'f')
    {
        return c-87;
    }
    else if( c >= 'A' && c <= 'F' )
    {
        return c-55;
    }
    else if( c >= '0' && c <= '9' )
    {
        return c-48;
    }
    else
    {
        return 0;
    }
}