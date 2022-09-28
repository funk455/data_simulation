/*
 * @filepath: 
 * @Descripttion: 
 * @version: 
 * @Author: ruixue.huang@yunzhou-tech.com
 * @Date: 2022-09-22 19:27:01
 * @LastEditors: huangruixue
 * @LastEditTime: 2022-09-27 15:28:11
 */
#include "filename.h"

int  GetFileNames(std::string path,std::vector<std::string>& filenames)
{
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str()))){
        std::cout<<"Folder doesn't Exist!"<<std::endl;
        return 1;
    }
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0){
            filenames.push_back( ptr->d_name);
    }
    }
    closedir(pDir);
    return 0;
}

/*
int main() {
    vector<string> file_name;
    string path = "./";
 
    GetFileNames(path, file_name);
 
    for(int i = 0; i <file_name.size(); i++)
    {
        cout<<file_name[i]<<endl;
    }
 
    return 0;
}
*/