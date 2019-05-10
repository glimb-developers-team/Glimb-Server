/*
* Ushakov Alexsandr
* Mail: alex556.ru@gmail.com
* Github: AlexUsh
* Date: 16.04.19
*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <exception>
#include "ConfigReader.h"

using namespace std;

string ConfigReader::get(string param){
     string trans;
     char value[512];
     char filpar[512];
     ifstream file("config.txt");

     if (!file.is_open()){
          throw runtime_error("Can't open config file");
	}
     else{
          while(getline(file, trans)){
               sscanf(trans.c_str(), "%s = %s", filpar, value);
               if (strcmp(param.c_str(), filpar) == 0) {
                    file.close();
                    return value;
               }
          }
          throw runtime_error("Can't find " + param + " in config file");
     }
}
