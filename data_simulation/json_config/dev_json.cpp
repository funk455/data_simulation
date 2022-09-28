#include "dev_json.h"

/***device_Data*****/
void device_Data::save_to_json()
{
    std::ofstream file(json_file);
    cereal::JSONOutputArchive output(file);    
    this->serialize(output);     //逐层序列化里面的参数
}

int device_Data::load_from_json()
{
    std::ifstream file(json_file); 
    if(file.is_open() == 0 || file.peek() == EOF)
    {
        return 0;
    }
    
    cereal::JSONInputArchive input(file);    
    this->serialize(input);     //逐层序列化里面的参数
    return 1;
    
}

template<class Archive>
void device_Data::serialize(Archive &ar)
{
    ar(
        cereal::make_nvp("gpio", this->gpio),
        cereal::make_nvp("adc", this->adc),
        cereal::make_nvp("usart", this->usart),
        cereal::make_nvp("can", this->can)
    );
}

void device_Data::set_gpio_data(int dev_index, gpio_data data, uint8_t rules)
{
    //V 0000 0001
    if(rules >> 0 & 0x01)
    {
       this->gpio.gpio_map["gpio" + std::to_string(dev_index)].value = data.value; 
    }
    //D 0000 0010
    if(rules >> 1 & 0x01)
    {
        this->gpio.gpio_map["gpio" + std::to_string(dev_index)].direction = data.direction; 
    }
}

void device_Data::set_can_data(int dev_index, serial_data data, uint8_t rules)
{
    //P 0000 0001
    if(rules >> 0 & 0x01)
    {
       this->can.serial_map["can" + std::to_string(dev_index)].path = data.path; 
    }
    //L 0000 0010
    if(rules >> 1 & 0x01)
    {
        this->can.serial_map["can" + std::to_string(dev_index)].looptime = data.looptime; 
    }
    //B 0000 0100
    if(rules >> 2 & 0x01)
    {
       this->can.serial_map["can" + std::to_string(dev_index)].baud = data.baud; 
    }
}

void device_Data::set_usart_data(int dev_index, serial_data data, uint8_t rules)
{
    //P 0000 0001
    if(rules >> 0 & 0x01)
    {
       this->usart.serial_map["ttys" + std::to_string(dev_index)].path = data.path; 
    }
    //L 0000 0010
    if(rules >> 1 & 0x01)
    {
        this->usart.serial_map["ttys" + std::to_string(dev_index)].looptime = data.looptime; 
    }
    //B 0000 0100
    if(rules >> 2 & 0x01)
    {
       this->usart.serial_map["ttys" + std::to_string(dev_index)].baud = data.baud; 
    }    
}

void device_Data::set_adc_data(int dev_index, int data)
{
    this->adc.adc_map["in" + std::to_string(dev_index) + "_input"] = data; 
}

int device_Data::rm_dev(std::string dev_name, int dev_index)
{
    int ret = 0;
    //根据名字,移除,若删除成功ret = 1
    if(dev_name == "gpio")
    {
        ret = this->gpio.gpio_map.erase(dev_name+std::to_string(dev_index));
    }
    else if(dev_name == "can")
    {
        ret = this->can.serial_map.erase(dev_name+std::to_string(dev_index));
    }
    else if(dev_name == "adc")
    {
        ret = this->adc.adc_map.erase("in" + std::to_string(dev_index) + "_input");
    }
    else if(dev_name == "usart")
    {
        ret = this->usart.serial_map.erase("ttys"+std::to_string(dev_index));
    }
    else
    {
        return -1;
    }

    return ret;
}   

/****查找返回****/
int device_Data::get_gpio_data(int dev_index, gpio_data &get_data)
{
    std::map<std::string,gpio_data>::iterator it = this->gpio.gpio_map.find("gpio" + std::to_string(dev_index));
    if(it != this->gpio.gpio_map.end())
    {
        get_data = it->second;
        return 1;
    }
    else
    {
        return 0;
    }
}

int device_Data::get_usart_data(int dev_index, serial_data &get_data)
{
    std::map<std::string,serial_data>::iterator it = this->usart.serial_map.find("ttys" + std::to_string(dev_index));
    if(it != this->usart.serial_map.end())
    {
        get_data = it->second;
        return 1;
    }
    else
    {
        return 0;
    }
}

int device_Data::get_can_data(int dev_index, serial_data &get_data)
{
    std::map<std::string,serial_data>::iterator it = this->can.serial_map.find("can" + std::to_string(dev_index));
    if(it != this->can.serial_map.end())
    {
        get_data = it->second;
        return 1;
    }
    else
    {
        return 0;
    }
}

int device_Data::get_adc_data(int dev_index, int &get_data)
{
    std::map<std::string, int>::iterator it = this->adc.adc_map.find("in" + std::to_string(dev_index) + "_input");
    if(it != this->adc.adc_map.end())
    {
        get_data = it->second;
        return 1;
    }
    else
    {
        return 0;
    }    
}

/***********adc************/
template <class T>
void adc::save(T & ar) const
{
    for( std::map<std::string,int>::const_iterator it = adc_map.begin(); it != adc_map.end(); ++it )
    {
        ar(cereal::make_nvp(it->first, it->second));
    }
}

template <class T>
void adc::load(T & ar)
{
    int adc_data;
    /******循环获取名字********/
    while (1)
    {
        auto ptr = ar.getNodeName();  
        if(ptr == NULL)
        {
            break;
        }

        /****将数据读取到g_data中****/
        ar(cereal::make_nvp(ptr, adc_data));
        adc_map.insert(std::map<std::string, int >::value_type(ptr, adc_data));
    }
}

/********************serial*********************/
template <class T>
void serial::save(T & ar) const   //save 记得加const 不然编译过不了
{
    //注意const_iterator和iterator的区别
    for( std::map<std::string, serial_data>::const_iterator it = serial_map.begin(); it != serial_map.end(); ++it )
    {
        //first 是名字, 如ttys0
        ar(cereal::make_nvp(it->first, it->second));
    }
    
    //ar(CEREAL_NVP(serial_map));

}


template <class T>
void serial::load(T & ar)
{
    serial_data s_data;
    /******循环获取名字如 "ttys0", "ttys1"********/
    while (1)
    {
        auto ptr = ar.getNodeName();  //不能用string ptr 否则ar.getNodeName()越界时将直接段错误
        if(ptr == NULL)
        {
            break;
        }
        /****将数据读取到s_data中****/
        ar(cereal::make_nvp(ptr, s_data));
        serial_map.insert(std::map<std::string, serial_data >::value_type(ptr, s_data));
    }
}

/*************gpio*************/
template <class T>
void gpio::save(T & ar) const   //save 记得加const 不然编译过不了
{
    //注意const_iterator和iterator的区别
    for( std::map<std::string, gpio_data>::const_iterator it = gpio_map.begin(); it != gpio_map.end(); ++it )
    {
        //first 是名字, 如"gpio8"
        ar(cereal::make_nvp(it->first, it->second));
    }

}

//提供给cereal 序列化的模板函数  
template <class T>
void gpio::load(T & ar)
{
    gpio_data g_data;
    /******循环获取名字如 "gpio8","gpio9"********/
    while (1)
    {
        auto ptr = ar.getNodeName();  //不能用string ptr 否则ar.getNodeName()越界时将直接段错误
        if(ptr == NULL)
        {
            break;
        }
        /****将数据读取到g_data中****/
        ar(cereal::make_nvp(ptr, g_data));
        gpio_map.insert(std::map<std::string, gpio_data >::value_type(ptr, g_data));
    }
}