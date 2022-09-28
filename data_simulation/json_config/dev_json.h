#ifndef DEV_JSON_H
#define DEV_JSON_H
#include <iostream>
#include <fstream>
#include <cereal/types/map.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

//为规则设置时传入参数的规则使用
#define V ((uint8_t)0x01)
#define D ((uint8_t)0x02)

#define P ((uint8_t)0x01)
#define L ((uint8_t)0x02)
#define B ((uint8_t)0x04)


//端口属性
class serial_data
{
public:    
    /****端口属性****/
    std::string path;      //路径
    std::string baud;      //波特率
    int looptime = -1;     //循环次数

    //提供给cereal 序列化的模板函数，让cereal知道哪些参数要序列化 
    template <class T>
	void serialize(T & ar)
	{
        //传递参数
		ar(cereal::make_nvp("path", path), cereal::make_nvp("baud", baud),cereal::make_nvp("looptime", looptime));
	}    

};

//GPIO属性
class gpio_data
{
public:    
    std::string direction;  //"in" "out" 
    int value = -1;

    template <class T>
	void serialize(T & ar)
	{
        //传递参数
		ar(cereal::make_nvp("direction", direction), cereal::make_nvp("value", value));
	}        
};

//GPIO合集
class gpio
{
public:
    /***端口合集***/
    std::map<std::string, gpio_data> gpio_map;  //map存了 <名字， 端口属性>

    //提供给cereal 序列化的模板函数  
	template <class T>
	void save(T & ar) const;   //save 记得加const 不然编译过不了

    //提供给cereal 序列化的模板函数  
	template <class T>
	void load(T & ar); 
};

//can,usart串口
class  serial
{   
public:
    //提供给cereal 序列化的模板函数  
	template <class T>
	void save(T & ar) const;   //save 记得加const 不然编译过不了

    //提供给cereal 序列化的模板函数  
	template <class T>
	void load(T & ar);

    /***端口合集***/
    std::map<std::string, serial_data> serial_map;  //map存了 <名字， 端口属性>    
};

//adc合集
class adc
{
public:
	template <class T>
	void save(T & ar) const;  //从adc_map写到json

	template <class T>
	void load(T & ar);   //从json读取到adc_map

    /***端口合集***/
    std::map<std::string, int> adc_map;  //map存了 <名字， 属性>
};

//设备合集
class device_Data
{
public:
    /******序列化声明*******/
    template<class Archive>
    void serialize(Archive &ar);

    //把类保存到json
    void save_to_json(); 
    //从json加载到类
    int load_from_json();

    //把gpio添进来  (下标， 数据， 写入规则)
    void set_gpio_data(int dev_index, gpio_data data, uint8_t rules = V|D);  //rules 写入规则 规则可写 V D V|D
    //把can添进来
    void set_can_data(int dev_index, serial_data data, uint8_t rules = B|P|L);  //rules 写入规则 规则可写
    //把usart添进来
    void set_usart_data(int dev_index, serial_data data, uint8_t rules = B|P|L);
    //把adc添进来
    void set_adc_data(int dev_index, int data);

    //获取gpio的值 要自己传入接收的参数  (读取下标 ， 存放接收参数)
    int get_gpio_data(int dev_index, gpio_data &get_data);
    //获取usart的值 要自己传入接收的参数
    int get_usart_data(int dev_index, serial_data &get_data);
    //获取can的值 要自己传入接收的参数
    int get_can_data(int dev_index, serial_data &get_data);
    //获取adc的值 要自己传入接收的参数
    int get_adc_data(int dev_index, int &get_data);

    //移除设备
    int rm_dev(std::string dev_name, int dev_index); 

    std::string json_file = "out.json";

    
    serial usart;
    serial can;
    class gpio gpio;
    class adc adc;

};


#endif