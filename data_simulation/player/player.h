#ifndef _PLAYER_H_
#define _PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <thread>
#include <chrono>

extern "C"
{

#include "canplayer.h"
}



#include "canplayer.h"
#include "filename.h"
#include "pty.h"
#include "dev_json.h"
#include "fd_main.h"

void playback_data();
void *playback_all(void *arg);
void *playbcak_usart(void *arg);
void *playbcak_can(void *arg);
void usart_play();
void can_play();
void auto_usart_play(char xx3[100]);
void auto_can_play(char xx3[100]);
void loop_usart_play(char count[100]);
void loop_can_play(char count[100]);
void change_allpath(std::string newpath);
void change_canpath(std::string newpath);
void change_usartdevpath(std::string newpath);
void change_usartpath(std::string newpath);
void show_help();
void change_path();


#endif