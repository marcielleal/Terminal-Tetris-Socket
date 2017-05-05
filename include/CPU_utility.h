#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <vector>

/**
* \brief is_number check if string cointains a number or not
*  @param s string with number 
*  \return True: s contains a number, False: s dosen't contain a number	 
*/
bool is_number(const std::string& s);

/**
* \brief Check if the process is valid and is not from system
*  @param struct dirent* entity struct checking if is a valid folder 
*  \return True: entity is a valid process, not a configuration file, False: isin't a folder, probabily configuration file, or hidden folder	 
*/
bool ProcessEntity(struct dirent* entity);


/**
* \brief Absolute value for real numbers
*   \return |x|
*/
double abs(double x1);

/**
* \brief Process struct contains PID as a string, int info as a cpu_usage/time of process in jiff and double total, total cpu usage of process, using /proc/<PID>/stat
*  	 
*/
struct Process
{
	std::string pid;
	int info;
	double total;
};

/**
* \brief Comparation method for Process struct
*	@param Process a First Process
*	@param Process b Second Process
*  \return True: Process a.total > Process b.total	 
*/
bool comp(Process a, Process b);

/**
* \brief Return CPU_time in format user + nice + syste /,  user + nice + system + idle from /proc/stat
* \return std::pair<int,int> CPU time 
*/
std::pair<int,int> getCPUtime();

/**
* \brief Calculates total CPU usage in porcentage with getCpuTime()	 
*/
double getCPUusage();

/**
* \brief Calculates the Total CPU usage in JIFF, not in procentage 
*  \return CPU usage
*/
int diffCPUtime(std::pair<int,int>t1 ,std::pair<int,int> t2 );

/**
* \brief Calculates the Process CPU usage with /proc/<pid>/stat 
*	@param pid process ID - PID
*  \return Process usage
*/
int getProcessCPUusage(std::string pid);

/**
* \brief Iterates in /proc/ folder and gets the process with the biggest CPU_Usage using getProcessCPUusage
*	\return PID of the process with the biggest CPU usage in string format
*/
std::string biggestCPUusage();

