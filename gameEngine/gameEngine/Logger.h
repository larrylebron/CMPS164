/**
Core code from http://www.yolinux.com/TUTORIALS/C++Singleton.html

Expanded by Larry LeBron

Logging Class

USAGE:
==========================================================================================================
	Logger::Instance()->err("string error message"); //Writes to the error log and cerr
	Logger::Instance()->write("string message"); //Writes to the general log and cout
	Logger::Instance()->write("string filename", "string message"); //Writes to the designated file
		
	All files will be placed in the /logs directory. If this directory doesn't exist, the logger will fail

	A Logger* can also be used like:
		Logger* log = Logger::Instancer()
		log->write("string message");
*/

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <set>

#include "externalLibs\Timer.h"

using std::cout;
using std::cerr;
using std::string;

static const string ERROR_LOG = "ErrorLog.txt";
static const string GENERAL_LOG = "log.txt";

class Logger{
public:
   static Logger* Instance();

   //Write an error to the error log file
   bool err(string errorMsg);

   //write a message to the general log file
   bool write(string message);

   //write a message to the specified file
   bool write(string filename, string message);

 
private:
	Logger(){};
	//write message to filename and console. Will write to cerr if error is true
	bool pWrite(string filename, string message, bool error);
   /*
   Logger(Logger const&){};             // copy constructor is private
   Logger& operator=(Logger const&){};  // assignment operator is private
   */
   static Logger* m_pInstance;
   std::set<string> fileNames;				//The set of files that have been written to
   std::ofstream writer;
};