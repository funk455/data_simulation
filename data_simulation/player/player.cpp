#include "player.h"

std::string work_pathname("/home/luckyone/work/player/sim_root");//填写默认的sim_root 路径
std::string can_pathname("/home/luckyone/work/text");            //相应can的路径
std::string usart_devname("/home/luckyone/work/ttext/usart");   //相应usart 设备的路径 
std::string usart_pathname("/home/luckyone/work/ttext/usart");   //相应usart的路径 
int all_pth_flag = 0;   //回放两者的线程标志位
int can_pth_flag = 0;   //回放can的线程标志位
int usart_pth_flag = 0; //回放usart的线程标志位
char devname[100];

void change_allpath(std::string newpath)
{
    work_pathname=newpath;
    std::cout<<"现路径"<<work_pathname<<std::endl;
}
void change_canpath(std::string newpath)
{
   can_pathname=newpath;
   std::cout<<"现路径"<<can_pathname<<std::endl;
}
void change_usartdevpath(std::string newpath)
{
   usart_devname=newpath;
   std::cout<<"现路径"<<usart_devname<<std::endl;
}
void change_usartpath(std::string newpath)
{
   usart_pathname=newpath;
   std::cout<<"现路径"<<usart_pathname<<std::endl;
}
extern class find_directory m_fd;
extern int j_exsit;
void playback_data()
{

   std::string str;
   std::string strr;
   std::vector<std::string> vec;
   std::string playif = "playback";
   std::string playifusart = "usartplay";
   std::string playifcan = "canplay";
   std::string playexit = "exit";
   std::string usart_dev = "ttyS";
   std::string can_dev = "can";
   std::string stop = "playback stop all";
   pthread_t all;
   pthread_t can_pth;
   pthread_t usart_pth;
   show_help();
   int usartflag;
   int canflag;
   int helpflag=0;
   while (1)
   {  
     
      getline(std::cin, str); //获得输入的指令
     // std::cout << str << std::endl;
      if (!str.size())       //卡住，一定要有输入
      {
         continue;
      }
      int j = 0;
      int n = 0;
      
      for (int i = 0; i <= str.size(); i++)   //将命令参数分别存储
      {
         if (str[i] == 32 && str[i + 1] != 32 && str[i + 1] != 0) //省略掉空格
         {
            //printf("%d,%d\n", str[i], str[i + 1]);
            //printf("%d\n", j);
            vec.push_back(str.substr(i - n, n));//存进容器
            j++;
            n = 0;

            continue;
         }
         if (n != 0 && i == str.size())   
         {
            vec.push_back(str.substr(i - n, n));
            n = 0;
            j = j + 1;
            break;
         }
         n++;
      }
    /*  auto x = vec.begin(); //查看容器内容
      int mm = 0;
      while (x != vec.end())
      {
         std::cout << "x: " << *x << std::endl;
         std::cout << "vec: " << vec[mm] << " mm" << mm << std::endl;
         x++;
         mm++;
         
      }*/

     // printf("%d\n", j);
      // opendir();

      if (str == playif && j == 1)//回放所有的usart文件与can文件
      {
         all_pth_flag = 1;   
         pthread_create(&all, NULL, playback_all, NULL); //创建回放线程
         helpflag = 1;
         // std::thread all(playback_all);
         // all.join();
      }
      //int bugflag = vec[0].compare(playif);
     // printf("bugflag %d\n", bugflag);
      if (vec[0] == "playback" && j > 1) //单独回放
      {  
         helpflag = 1;
        // std::cout << "vec[1]:" << vec[1] << std::endl;
         int loopflag = 1;
         for (int i = 0; i < j; i++)
         {
            usartflag = vec[i].compare(0, 3, usart_dev, 0, 3);
            canflag = vec[i].compare(0, 3, can_dev, 0, 3);
            //printf("usartflag %d,usart_pth_flag %d\n",usartflag,usart_pth_flag);
            if (usartflag == 0 && usart_pth_flag == 0) //单独回放串口
            {
               if(j_exsit == 1)
               { serial_data t;
               int index;
               sscanf(vec[i].c_str(),"ttyS%d", &index);
               m_fd.get_usart_data(index,t);
               if(t.path.empty() != 1)
               {
                  change_usartpath(t.path);
               }
               }
               memset(devname,0,sizeof(devname));
               std::string devpath=usart_devname + "/" +vec[i] ;
               strcpy(devname,devpath.c_str());
               std::cout<<devname<<std::endl;
               for (int l = 0; l < j; l++)
               {
                  loopflag = vec[l].compare("-l");
                  if (loopflag == 0 )
                  {
                     std::cout << "start usart" << std::endl;
                     char buf[100] = {0};
                     std::string usartbuf=vec[l] + vec[l+1];
                     strcpy(buf, usartbuf.c_str());
                     usart_pth_flag = 1;
                     pthread_create(&usart_pth, NULL, playbcak_usart, &buf);
                     
                  }
               }
            }
            if (canflag == 0 && can_pth_flag == 0)  //单独回放can
            {
               for (int l = 0; l < j; l++)
               {
                  loopflag = vec[l].compare("-l");
                  if (loopflag == 0)
                  {
                     std::cout << "start can" << std::endl;
                     char buf[100] = {0};
                     strcpy(buf, vec[l + 1].c_str());
                     can_pth_flag = 1;
                     pthread_create(&can_pth, NULL, playbcak_can, &buf);
                     
                     // loop_can_play(vec[l + 1]);
                  }
               }
            }
         }
         
      }

      if (str == stop || str == "s")  //中途停止回放
      {
         helpflag = 1;
         printf("回放中止\n");
         if (all_pth_flag)
         {
            pthread_cancel(all);
            all_pth_flag = 0;
         }

         if (can_pth_flag)
         {
            pthread_cancel(can_pth);
            can_pth_flag = 0;
         }

         if (usart_pth_flag)
         {
            pthread_cancel(usart_pth);
            usart_pth_flag = 0;
         }
      
      }

      if (str == playexit) //退出
      {
         helpflag = 1;
         printf("回放中止\n");
         if (all_pth_flag)
         {
            pthread_cancel(all);
            all_pth_flag = 0;
         }

         if (can_pth_flag)
         {
            pthread_cancel(can_pth);
            can_pth_flag = 0;
         }

         if (usart_pth_flag)
         {
            pthread_cancel(usart_pth);
            usart_pth_flag = 0;
         }
         return;
      }      
      if ( str == "help")
      {
         show_help();
         helpflag = 1;
      }
      if ( str == "change")
      {
         change_path();
         helpflag = 1;
      }
      if( str == "vcan")
      {
          system("sudo modprobe vcan");
          system("sudo ip link add dev can0 type vcan");
          system("sudo ifconfig can0 up");
          std::cout<<"open vcan can0"<<std::endl;
          helpflag = 1;
      }
      if( str == "close vcan")
      {
          system("sudo ifconfig can0 down");
          std::cout<<"vcan can0  down"<<std::endl;
          helpflag = 1;
      }
      if( helpflag == 0  )
      {
          show_help();
      }
      if( helpflag == 1)
      {
         helpflag = 0;
      }

      vec.clear();

   }

   return ;
}
void show_help()
{
   std::cout<<"你可以使用这些指令"<<std::endl;
   std::cout<<"playback --"<<"回放所有的usart与can文件"<<std::endl;
   std::cout<<"playback ttyS1 -l 次数  --"<<"回放usart文件"<<std::endl;
   std::cout<<"playback can0 -l 次数   --"<<"回放can文件"<<std::endl; 
   std::cout<<"playback stop all 或 s 停止正在回放的线程"<<std::endl;
   std::cout<<"vcan  开启虚拟串口can0"<<std::endl;
   std::cout<<"close vcan  关闭can0的虚拟串口"<<std::endl;
   std::cout<<"change  -- 修改路径"<<std::endl;
   std::cout<<"exit --"<<"返回上一级"<<std::endl;
   std::cout<<"help --"<<"查看指令与帮助"<<std::endl;
   return;
}
void change_path()
{
    char chose;
    std::string newpath;
    while(1)
    { 
      std::cout<<"你的选择"<<std::endl;
      std::cout<<"1.查看"<<std::endl;
      std::cout<<"2.修改大目录路径"<<std::endl;
      std::cout<<"3.修改usart的路径"<<std::endl;
      std::cout<<"4.修改usart 设备的路径"<<std::endl;
      std::cout<<"5.修改can的路径"<<std::endl;
      std::cout<<"6.返回上一级"<<std::endl;

      std::cin>>chose;
      switch (chose)
      {
      case '1':
         std::cout<<"大目录："<<work_pathname<<std::endl;
         std::cout<<"usart路径:"<<usart_pathname<<std::endl;
         std::cout<<"usart 设备路径:"<<usart_devname<<std::endl;
         std::cout<<"can路径"<<can_pathname<<std::endl;
         break;
      case '2':
        std::cout<<"输入新路径"<<std::endl;
        std::cin>>newpath;
         change_allpath( newpath);          
         break;
      case '3':
        std::cout<<"输入新路径"<<std::endl;
        std::cin>>newpath;
         change_usartpath(newpath);          
         break;
      case '4':
         std::cout<<"输入新路径"<<std::endl;
         std::cin>>newpath;
         change_usartdevpath(newpath);        
         break;
      case '5':
         std::cout<<"输入新路径"<<std::endl;
         std::cin>>newpath;
         
         change_canpath(newpath);       
         break;
      case '6':
         
         return;

      default:
         std::cout<<"请重新输入"<<std::endl;
         break;
      }
    }
    return;
}

void *playback_all(void *arg)
{
   std::cout << "start" << std::endl;
   std::string path = work_pathname;
   std::vector<std::string> filename;
   std::vector<std::string> canfilename;
   std::vector<std::string> usartfilename;
   std::string canpath;
   std::string usartpath;
   GetFileNames(path, filename);
   for (int i = 0; i < filename.size(); i++)
   {
      std::cout << filename[i] << std::endl;
   }

   auto p = filename.begin();
   while (p != filename.end())
   {
      if (*p == "can")
      {
         canpath = path + "/can";
         GetFileNames(canpath, canfilename);
      }
      if (*p == "usart")
      {
         usartpath = path + "/usart";
         GetFileNames(usartpath, usartfilename);
      }

      p++;
   }
   for (int i = 0; i < canfilename.size(); i++)
   {
      std::cout << canfilename[i] << std::endl;
   }
   for (int i = 0; i < usartfilename.size(); i++)
   {
      std::cout << usartfilename[i] << std::endl;
   }
   int can_count = 0;
   auto c = canfilename.begin();
   while (c != canfilename.end())
   {
      std::string canpathfilename = canpath + "/" + canfilename[can_count];
      char cbuf[100] = {0};
      int  cfileflag=0;
      strcpy(cbuf, canpathfilename.c_str());
      std::cout << cbuf << std::endl;
      for (int i = 0; i < sizeof(cbuf); i++)
      {
         if (cbuf[i] == 46 && cbuf[i + 1] == 108 && cbuf[i + 2] == 111 && cbuf[i + 3] == 103)
         {
            auto_can_play(cbuf);
            cfileflag=1;

         }
      }
      if (cfileflag==0)
      {
         std::cout<<canfilename[can_count]<<"this no can player file"<<std::endl;
      }   
     
      can_count++;
      c++;
   }

   int usart_count = 0;
   auto u = usartfilename.begin();
   while (u != usartfilename.end())
   {
      std::string usartpathfilename = usartpath + "/" + usartfilename[usart_count];
      char ubuf[100] = {0};
      strcpy(ubuf, usartpathfilename.c_str());
      std::cout << ubuf << std::endl;
      for (int i = 0; i < sizeof(ubuf); i++)
      {
          if ( ubuf[i] == 46 && ubuf[i+1]== 108 && ubuf[i+2]== 111&& ubuf[i+3]== 103)
          {
              auto_usart_play(ubuf);
          }
          
      }    
      usart_count++;
      u++;
   }
   all_pth_flag=0;
   return NULL;
}

void *playbcak_usart(void *arg)
{
   char count[100] = {0};
   strcpy(count, (const char *)arg);
   loop_usart_play(count);
   usart_pth_flag=0;
   return NULL;
}

void *playbcak_can(void *arg)
{
   char count[100] = {0};
   strcpy(count, (const char *)arg);
   loop_can_play(count);
   can_pth_flag=0;
   return NULL;
}

void usart_play()
{
   std::string str;
   getline(std::cin, str);
   std::cout << str << std::endl;
   if (!str.size())
   {

      return;
   }
   std::vector<std::string> vec;
   int j = 0;
   int n = 0;
   for (int i = 0; i <= str.size(); i++)
   {

      if (str[i] == 32)
      {
         vec.push_back(str.substr(i - n, n));
         n = 0;
         j = j + 1;
         continue;
      }
      if (n != 0 && i == str.size())
      {
         vec.push_back(str.substr(i - n, n));
         n = 0;
         j = j + 1;
         break;
      }
      n++;
   }
   std::cout << j << std::endl;

   //   char x1[10][100];
   //   memset(x1,0,sizeof(x1));

   char xx1[100] = {0};
   char xx2[100] = {0};
   char xx3[100] = {0};
   char xx4[100] = {0};
   char xx5[100] = {0};
   char xx6[100] = {0};
   char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

   int m = 0;
   for (m; m < j; m++)
   {
      strcpy(x1[m], vec[m].c_str());
   }

   //   return;
   // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};

   main_pty(j, x1);

   vec.clear();
   // str.clear();
   memset(xx1, 0, sizeof(xx1));
   memset(xx2, 0, sizeof(xx2));
   memset(xx3, 0, sizeof(xx3));
   memset(xx4, 0, sizeof(xx4));
   memset(xx5, 0, sizeof(xx5));
   memset(xx6, 0, sizeof(xx6));
   // memset(x1,0,sizeof(x1));
   return;
}

void auto_usart_play(char xx3[100])
{

   //   char x1[10][100];
   //   memset(x1,0,sizeof(x1));

   char xx1[100] = "usart";
   char xx2[100] = "-I";

   char xx4[100] = {0};
   char xx5[100] = {0};
   char xx6[100] = {0};
   char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

   //   return;
   // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};

   main_pty(3, x1);

   // str.clear();
   memset(xx1, 0, sizeof(xx1));
   memset(xx2, 0, sizeof(xx2));
   // memset(xx3, 0, sizeof(xx3));
   memset(xx4, 0, sizeof(xx4));
   memset(xx5, 0, sizeof(xx5));
   memset(xx6, 0, sizeof(xx6));
   // memset(x1,0,sizeof(x1));
   return;
}

void loop_usart_play(char count[100])
{
   std::vector<std::string> file_name;
   int  find; 
   find=GetFileNames(usart_pathname, file_name);
    std::cout<<"usart路径:"<<usart_pathname<<std::endl;
   if ( find == 1)
   {
      char xx1[100] = "usart";
      char xx2[100] = "-I";
      char xx3[100] = {0};
      char xx4[100] = {0};
      char xx5[100] = {0};
      char xx6[100] = "-o";
      char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

      strcpy(x1[2], usart_pathname.c_str());
      strcpy(x1[3], count);
      //   return;
      // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};
     // for (int n = 0; n < 7; n++)
     // {
     //    std::cout << "x1 " << x1[n] << std::endl;
      //}
      int ufileflag=0;
      for (int i = 0; i < sizeof(xx3); i++)
      {
         if (xx3[i] == 46 && xx3[i + 1] == 108 && xx3[i + 2] == 111 && xx3[i + 3] == 103) //判断是否为log文件
         {
             main_pty(6, x1);
            ufileflag=1;

         }
      }
      if (ufileflag==0)
      {
         std::cout<<usart_pathname<<"this no can player file"<<std::endl;
      }   
      // str.clear();
      memset(xx1, 0, sizeof(xx1));
      memset(xx2, 0, sizeof(xx2));
      memset(xx3, 0, sizeof(xx3));
      memset(xx4, 0, sizeof(xx4));
      memset(xx5, 0, sizeof(xx5));
      memset(xx6, 0, sizeof(xx6));
      // memset(x1,0,sizeof(x1));
       return ;
   }
   
   std::string usartpath;
   auto x = file_name.begin();
   int i = 0;
   while (x != file_name.end())
   {
      usartpath = usart_pathname + "/" + file_name[i];
      i++;
      x++;
      std::cout << "usartpathname" << usartpath << std::endl;


      char xx1[100] = "usart";
      char xx2[100] = "-I";
      char xx3[100] = {0};
      char xx4[100] = {0};
      char xx5[100] = {0};
      char xx6[100] = "-o";
      char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

      strcpy(x1[2], usartpath.c_str());
      strcpy(x1[3], count);
      //   return;
      // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};
     // for (int n = 0; n < 7; n++)
     // {
     //    std::cout << "x1 " << x1[n] << std::endl;
      //}
      std::cout << x1[0]<<x1[1]<<x1[2]<<x1[3]<<x1[4]<<x1[5]<<std::endl;
      int ufileflag=0;
      for (int i = 0; i < sizeof(xx3); i++)
      {
         if (xx3[i] == 46 && xx3[i + 1] == 108 && xx3[i + 2] == 111 && xx3[i + 3] == 103)
         {
             main_pty(6, x1);
            ufileflag=1;

         }
      }
      if (ufileflag==0)
      {
         std::cout<<file_name[i]<<"this no can player file"<<std::endl;
      }   
      // str.clear();
      memset(xx1, 0, sizeof(xx1));
      memset(xx2, 0, sizeof(xx2));
      memset(xx3, 0, sizeof(xx3));
      memset(xx4, 0, sizeof(xx4));
      memset(xx5, 0, sizeof(xx5));
      memset(xx6, 0, sizeof(xx6));
      // memset(x1,0,sizeof(x1));
   }
   return;
}

void can_play()
{
   std::string str;
   getline(std::cin, str);
   std::cout << str << std::endl;
   if (!str.size())
   {

      return;
   }
   std::vector<std::string> vec;
   int j = 0;
   int n = 0;
   for (int i = 0; i <= str.size(); i++)
   {

      if (str[i] == 32)
      {
         vec.push_back(str.substr(i - n, n));
         n = 0;
         j = j + 1;
         continue;
      }
      if (n != 0 && i == str.size())
      {
         vec.push_back(str.substr(i - n, n));
         n = 0;
         j = j + 1;
         break;
      }
      n++;
   }
   std::cout << j << std::endl;
   char xx1[100] = {0};
   char xx2[100] = {0};
   char xx3[100] = {0};
   char xx4[100] = {0};
   char xx5[100] = {0};
   char xx6[100] = {0};
   char xx7[100] = {0};
   char xx8[100] = {0};
   char xx9[100] = {0};
   char xx10[100] = {0};
   char xx11[100] = {0};
   char xx12[100] = {0};
   char xx13[100] = {0};
   char xx14[100] = {0};
   char xx15[100] = {0};
   char *x1[16] = {xx1, xx2, xx3, xx4, xx5, xx6, xx7, xx8, xx9, xx10, xx11, xx12, xx13, xx14, xx15};

   int m = 0;
   for (m; m < j; m++)
   {
      strcpy(x1[m], vec[m].c_str());
   }

   //   return;
   // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};

   canplayer_main(j, x1);

   vec.clear();
   // str.clear();
   memset(xx1, 0, sizeof(xx1));
   memset(xx2, 0, sizeof(xx2));
   memset(xx3, 0, sizeof(xx3));
   memset(xx4, 0, sizeof(xx4));
   memset(xx5, 0, sizeof(xx5));
   memset(xx6, 0, sizeof(xx6));
   memset(xx7, 0, sizeof(xx7));
   memset(xx8, 0, sizeof(xx8));
   memset(xx9, 0, sizeof(xx9));
   memset(xx10, 0, sizeof(xx10));
   memset(xx11, 0, sizeof(xx11));
   memset(xx12, 0, sizeof(xx12));
   memset(xx13, 0, sizeof(xx13));
   memset(xx14, 0, sizeof(xx14));
   memset(xx15, 0, sizeof(xx15));

   // memset(x1,0,sizeof(x1));
   return;
}

void auto_can_play(char xx3[100])
{

   char xx1[100] = "canplayer";
   char xx2[100] = "-I";

   char xx4[100] = "-n";
   char xx5[100] = "3000";
   char xx6[100] = "-v";
   char xx7[100] = "-l";
   char xx8[100] = "1";
   char xx9[100] = {0};
   char xx10[100] = {0};
   char xx11[100] = {0};
   char xx12[100] = {0};
   char xx13[100] = {0};
   char xx14[100] = {0};
   char xx15[100] = {0};
   char *x1[16] = {xx1, xx2, xx3, xx4, xx5, xx6, xx7, xx8, xx9, xx10, xx11, xx12, xx13, xx14, xx15};

   //   return;
   // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};

   canplayer_main(8, x1);

   // str.clear();
   memset(xx1, 0, sizeof(xx1));
   memset(xx2, 0, sizeof(xx2));
   // memset(xx3, 0, sizeof(xx3));
   memset(xx4, 0, sizeof(xx4));
   memset(xx5, 0, sizeof(xx5));
   memset(xx6, 0, sizeof(xx6));
   memset(xx7, 0, sizeof(xx7));
   memset(xx8, 0, sizeof(xx8));
   memset(xx9, 0, sizeof(xx9));
   memset(xx10, 0, sizeof(xx10));
   memset(xx11, 0, sizeof(xx11));
   memset(xx12, 0, sizeof(xx12));
   memset(xx13, 0, sizeof(xx13));
   memset(xx14, 0, sizeof(xx14));
   memset(xx15, 0, sizeof(xx15));

   // memset(x1,0,sizeof(x1));
   return;
}

void loop_can_play(char count[100])
{

   std::vector<std::string> file_name;
   int  find; 
   find=GetFileNames(can_pathname, file_name);
   if ( find == 1)
   {
      char xx1[100] = "can";
      char xx2[100] = "-I";
      char xx3[100] = {0};
      char xx4[100] = "-v";
      char xx5[100] = "-l";
      char xx6[100] = {0};
      char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

      strcpy(x1[2], can_pathname.c_str());
      strcpy(x1[5], count);
      //   return;
      // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};
      int cfileflag=0;
      for (int i = 0; i < sizeof(xx3); i++)
      {
         if (xx3[i] == 46 && xx3[i + 1] == 108 && xx3[i + 2] == 111 && xx3[i + 3] == 103)
         {
           canplayer_main(6, x1);
            cfileflag=1;
         }
      }
      if (cfileflag==0)
      {
         std::cout<<can_pathname<<"this no can player file"<<std::endl;
      }   
      
      // str.clear();
      memset(xx1, 0, sizeof(xx1));
      memset(xx2, 0, sizeof(xx2));
      memset(xx3, 0, sizeof(xx3));
      memset(xx4, 0, sizeof(xx4));
      memset(xx5, 0, sizeof(xx5));
      memset(xx6, 0, sizeof(xx6));
      // memset(x1,0,sizeof(x1));
   }
   
   std::string canpath;
   auto x = file_name.begin();
   int i = 0;
   while (x != file_name.end())
   {
      canpath = can_pathname + "/" + file_name[i];
      i++;
      x++;
      std::cout << "canpath" << canpath << std::endl;

      char xx1[100] = "can";
      char xx2[100] = "-I";
      char xx3[100] = {0};
      char xx4[100] = "-v";
      char xx5[100] = "-l";
      char xx6[100] = {0};
      char *x1[7] = {xx1, xx2, xx3, xx4, xx5, xx6};

      strcpy(x1[2], canpath.c_str());
      strcpy(x1[5], count);
      //   return;
      // char * x1[5]={"usart","-I","microcom-2022-08-23_085552.log"};
      int cfileflag=0;
      for (int i = 0; i < sizeof(xx3); i++)
      {
         if (xx3[i] == 46 && xx3[i + 1] == 108 && xx3[i + 2] == 111 && xx3[i + 3] == 103)
         {
           canplayer_main(6, x1);
            cfileflag=1;
         }
      }
      if (cfileflag==0)
      {
         std::cout<<file_name[i]<<"this no can player file"<<std::endl;
      }   
      
      // str.clear();
      memset(xx1, 0, sizeof(xx1));
      memset(xx2, 0, sizeof(xx2));
      memset(xx3, 0, sizeof(xx3));
      memset(xx4, 0, sizeof(xx4));
      memset(xx5, 0, sizeof(xx5));
      memset(xx6, 0, sizeof(xx6));
      // memset(x1,0,sizeof(x1));
   }
   return;
}