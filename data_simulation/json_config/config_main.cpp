#include <iostream>
#include <string>

extern "C"{
#include <getopt.h>
}

#include "dev_json.h"



void show_usage(const char * err_tip = NULL)
{
    if(err_tip != NULL)
    {
        fprintf(stderr, "           **********************************\n");
	    fprintf(stderr, "           *  %s\n",err_tip);
        fprintf(stderr, "           **********************************\n");
    }

	fprintf(stderr, "使用方法\n"
    "    *** -a gpio -i 8 -v 1 -d out\n"
    "    *** -r can -i 2\n"
    "    *** -a usart -i 1 -b 115200 -p /dev/pts/2 -l 2\n\n"
    "    -h, --help             帮助\n"
    "    -a, --add <设备>        添加设置\n"
    "    -r, --rmove <设备>      移除\n"
    "    -i,                    操作索引\n"
    "    -v,                    adc或gpio的值\n"
    "    -d,                    gpio的输入输出 如-d in\n"
    "    -p,                    串口路径\n"
    "    -b,                    波特率\n"
    "    -l,                    循环次数\n"
    "    -o, --output <文件>     输出json文件(默认out.json)\n"
    );
}

#if 1
int main(int argc, char ** argv)
{
    int         c;              //getopt_long的返回标识
    gpio_data   g_data;         //gpio属性
    int         a_data;         //adc的值
    serial_data s_data;         //串口属性
    std::string dev_name;       //要操作的设备名 gpio usart can adc
    int         dev_index = -1;      //设备名下标 如gpio8 的8， can2的2
    int         add_flag = 0;   //添加标识
    int         rm_flag = 0;    //移除标识
    uint8_t     gpio_rule = 0;  //gpio传入规则
    uint8_t     serial_rule = 0;//串口传入规则
    std::string json_file;       //

    //长选项参数处理合集
    static struct option longOpts[] = 
    {
        { "--help", no_argument,       NULL, 'h' },
        { "--add",  required_argument, NULL, 'a' },
        { "--rmove", required_argument, NULL, 'r' },
        { "--output", required_argument, NULL, 'o' },
        { 0, 0, 0, 0 }
    };

    //参数处理函数，包含了短选项
    while ( (c = getopt_long(argc, argv, "a:b:d:hi:l:o:p:r:v:", longOpts, NULL)) != -1)
    {
        switch(c)
        {
            case '?':
                show_usage();
                return 0;
            case 'h':
                show_usage();
                return 0;
            case 'a': //添加
                add_flag = 1;
                dev_name = optarg;
                break;
            case 'r':  //移除
                rm_flag = 1;
                dev_name = optarg;            
                break;
            case 'i':  //下标0
                dev_index = atoi(optarg);
                break;                
            case 'v':  //gpio或adc值
                gpio_rule |= V; 
                g_data.value = atoi(optarg);
                a_data = atoi(optarg);
                break;
            case 'd':  //gpio的输入输出
                gpio_rule |= D; 
                g_data.direction = optarg;
                break;
            case 'o':
                json_file = optarg;
                break;
            case 'p':  //路径
                serial_rule |= P;
                s_data.path = optarg;
                break;
            case 'b':  //波特率
                serial_rule |= B;
                s_data.baud = optarg;
                break;
            case 'l':  //循环次数
                serial_rule |= L;
                s_data.looptime = atoi(optarg);
                break;
            default:
                show_usage("");
                return 0;        
        }
    }

    if(dev_index < 0 )
    {
        show_usage("索引值空或有误!");
        return 0;
    }
    //要操作的总合集
    device_Data dev;

    /*加载，输入json, 默认是out.json*/
    if(json_file.empty() == 0)
    {
        dev.json_file = json_file;
    }
    dev.load_from_json();

    /***********添加操作******/
    if(add_flag == 1)
    {
        //将名字,数据插入
       if(dev_name == "gpio")
       {
            dev.set_gpio_data(dev_index,g_data,gpio_rule);
       }
       else if(dev_name == "can")
       {
            dev.set_can_data(dev_index,s_data,serial_rule);
       }
       else if(dev_name == "adc")
       {
            dev.set_adc_data(dev_index,a_data);
       }
       else if(dev_name == "usart")
       {
            dev.set_usart_data(dev_index,s_data,serial_rule);
       }
       else
       {
            show_usage("指定的设备名有误!");
            return 0;
       }                 
    }
    /**********移除操作***********/    
    else if(rm_flag == 1)
    {
        int ret = dev.rm_dev(dev_name, dev_index);
        if(ret == 0)
        {
            show_usage("找不到要删除的设备!");
        }
    }
    else
    {
       show_usage("没有指定的操作!");
    }

//查找测试
/*
    std::cout << "tttol = " << dev.gpio.gpio_map["gpio8"].value << std::endl; 

    gpio_data t;
    dev.get_gpio_data(8, t);
    std::cout << "dd : " << t.value << std::endl;

    serial_data t2;
    dev.get_usart_data(2,t2);
    std::cout << "ssd : " << t2.baud << std::endl;
*/
    /*存到json*/
    dev.save_to_json();

/*******************
 * 学习测试使用
 * *****************/
#if 0
    /***************************************************************
     *  一定要加括号！
     *  注意，cereal的JSONOutputArchive在析构时才保证数据由缓存写入到文件
     *  因此，使用大括号确保output对象在离开作用域后执行析构函数，从而将数据flush
     *  尝试去掉括号后在 usart 加入 ttys3,将得到错误的JSON
    ***************************************************************/
    {
        std::ofstream file("out.json");
        cereal::JSONOutputArchive output(file);
        
        //添加usart
        serial_data ttys0;
        serial_data ttys1;
        serial_data ttys2;
        dev.usart.serial_map.insert(std::map<std::string,struct serial_data>::value_type("ttys0", ttys0));
        dev.usart.serial_map.insert(std::map<std::string,struct serial_data>::value_type("ttys1", ttys1));

        //添加can
        serial_data can0;
        dev.can.serial_map.insert(std::map<std::string,struct serial_data>::value_type("can0", can0));

        //添加gpio
        gpio_data gpio8;
        dev.gpio.gpio_map.insert(std::map<std::string,struct gpio_data>::value_type("gpio8", gpio8));
        gpio_data gpio10;
        gpio10.direction = "in";
        dev.gpio.gpio_map.insert(std::map<std::string,struct gpio_data>::value_type("gpio10", gpio10));

        //添加adc
        dev.adc.adc_map.insert(std::map<std::string,int>::value_type("in0_input", 12456));
        dev.adc.adc_map.insert(std::map<std::string,int>::value_type("in1_input", 558));

        //和 output(cereal::make_nvp("dev", dev)); 类似,但去了dev这一层级
        dev.serialize(output);
    }

    //中途加入 ttys3
    serial_data ttys3;
    ttys3.baud = "55888";
    dev.usar0
        std::ofstream file2("out.json");
        cereal::JSONOutputArchive output2(file2);
        dev.serialize(output2);
        //output2(cereal::make_nvp("dev", dev));
    }



    //读取

    {
        device_Data dev2;
        std::ifstream file("out.json");
	    cereal::JSONInputArchive archive(file);
	    dev2.serialize(archive);
        //要修改某个配置的时候

        for( std::map<std::string,struct serial_data>::iterator it = dev2.usart.serial_map.begin(); it != dev2.usart.serial_map.end(); ++it )
        {
            std::cout << it->first << " " << it->second.baud << std::endl;
        }

        for( std::map<std::string,struct serial_data>::iterator it = dev2.can.serial_map.begin(); it != dev2.can.serial_map.end(); ++it )
        {
            std::cout << it->first << " " << it->second.baud << std::endl;
        }

        for( std::map<std::string,struct gpio_data>::iterator it = dev2.gpio.gpio_map.begin(); it != dev2.gpio.gpio_map.end(); ++it )
        {
            std::cout << it->first << " " << it->second.direction << std::endl;
        }
    }
#endif
    return 0;
}
#endif
















//学习测试

#if 0
struct Data
{
	int index = 1;
	double d[3] = { 1.0,2.33e-5,1000000.0 };
	std::vector<std::string> vs;

	template <class T>
	void serialize(T & ar)
	{
		ar(index, d, vs);
	}
};

int main()
{

    std::vector<int> gpio = {8, 9, 10};
    std::vector<int> adc = {0, 1, 2, 3};
	Data mydata;
	std::string s1 = "中文字符串/*-+!@#$%^&";
	std::string s2 = "english/*-+!@#$%^&";
	mydata.vs.push_back(s1);
	mydata.vs.push_back(s2);


	std::ofstream file("out.json");
	cereal::JSONOutputArchive archive(file);
	archive(CEREAL_NVP(gpio),CEREAL_NVP(adc),CEREAL_NVP(mydata));
    return 0;

    #if 0

    std::ofstream file("out.json");
	cereal::JSONOutputArchive archive(file);
	std::string s[] = { "this is a string", " 中文string也是可以支持的" };
	std::vector<double> vec = { 1.00001, 2e3, 30.1, -4, 5 };
	archive(CEREAL_NVP(vec), CEREAL_NVP(s));
    return 0;

    #endif



    #if 0

    std::ifstream file("out.json");
	cereal::JSONInputArchive archive(file);
	std::string s[2];   //注意变量名要对应json
	archive(CEREAL_NVP(s_out));
	std::cout << s[1] << std::endl;
    return 0;

    #endif



}

#endif