#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <fstream>
#include <string>
#include "BlackBone_GPIO.h"
#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close

#define PORTNUM 5000


std::atomic<bool> master;
std::atomic<bool> slave1;
std::atomic<char> move;

int readAnalog(int pnumber)
{
	std::fstream f;
	f.open("/sys/bus/iio/devices/iio:device0/in_voltage" + std::to_string(pnumber)+"_raw", std::fstream::in );
	std::string line;
	getline(f,line);
	return std::stoi(line);
}


void control(int n)
{
	while(1)
	{
		move = '';
		if(n == 1)
		{
			int value =  readAnalog(1);
			if( value < 2000)
			{	
				move = 'a';
				master = true;

			}else if (value > 3800)
			{
				move = 'd';
				master = true;
			}
			else
				master = false;
		}else if(n == 2 && !master)
		{
			int value =  readAnalog(3);
			if(value < 500)
			{
				move = 'e';
				slave1 = true;
			}
			else if(value > 3500)
			{
				std::cout<< "P" << std::endl;
			}
			else
				slave1 = false;
		}else if(n == 3 && !master && !slave1)
		{
			BlackBone_GPIO _GPIO;
			_GPIO.setup("P9_27", GPIO::IN);
			if(_GPIO.input("P9_27"))
			{
				move = 'w';
			}
		}
	}
}

void communication()
{
	struct sockaddr_in endereco;
    int socketId;
 
    std::string msg = "Cliente envia um Olá para o servidor";
    int bytesenviados;
 
    /*
     * Configurações do endereço
    */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    endereco.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    /*
     * Criando o Socket
     *
     * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
     * PARAM2: SOCK_STREAM ou SOCK_DGRAM
     * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
    */
    socketId = socket(AF_INET, SOCK_STREAM, 0);
 
    //Verificar erros
    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }
 
    //Conectando o socket cliente ao socket servidor
    if ( connect (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
    {
        printf("Falha ao executar connect()\n");
        exit(EXIT_FAILURE);
    }
    //printf ("Cliente conectado ao servidor\n");
 
	while (true){
		msg = move;
		//std::cout<<"Cliente vai enviar a mensagem "<<msg<<std::endl;
		bytesenviados = send(socketId,msg.c_str(),strlen(msg.c_str()),0);
		if (bytesenviados == -1)
    		{
        		printf("Falha ao executar send()");
        		exit(EXIT_FAILURE);
    		}
    		//printf("Cliente enviou a seguinte msg (%d bytes) para o servidor: %s \n",bytesenviados,msg.c_str());
 	}
    close(socketId);
}


int main()
{
	master = false;
	slave1 = false;
	std::thread tmaster(control, 1);
	std::thread tslave(control, 2);
	std::thread tslave2(control, 3);
	std::thread com(communication);

	tmaster.join();
	tslave.join();
	tslave2.join();
	com.join();
}

