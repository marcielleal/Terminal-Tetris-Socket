#include "BlackBone_GPIO.h"


BlackBone_GPIO::BlackBone_GPIO()
{

}

void BlackBone_GPIO::setup(const std::string port, const GPIO SIG )
{

    //Check if string is in correct format
 	if(SIG == GPIO::IN || SIG == GPIO::OUT )
    {
    		int n_port = pinToIO[port];
            std::string  folderr;
            folderr = GPIO_PATH + "/gpio" + std::to_string(pinToIO[port]);
            struct stat sb;
            if(!(stat(folderr.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode) ))
            {
            		std::fstream fs;
            		fs.open(GPIO_PATH + "/export" ,std::fstream::in | std::fstream::out | std::fstream::app);
                    fs << port;
            		fs.close();
            }
            std::string dir = GPIO_PATH + "/gpio" + std::to_string(pinToIO[port]);
            std::fstream fs;
            fs.open(dir + "/direction" ,std::fstream::in | std::fstream::out | std::fstream::app);
            
            if(SIG ==  GPIO::IN)
            {
                fs << "in";
            }
            else if(SIG ==  GPIO::OUT)
            {               
                fs << "out";
            }
            fs.close();
    }
}

void BlackBone_GPIO::output(const std::string port, const GPIO SIG )
{
        std::string dir = GPIO_PATH + "/gpio" + std::to_string(pinToIO[port]);
    	std::fstream fs;
        if(SIG == GPIO::HIGH || SIG == GPIO::LOW )
        {
        	fs.open(dir + "/value" ,std::fstream::in | std::fstream::out | std::fstream::app);
            if( SIG == GPIO::HIGH )
            {
        		                    
                fs << 1;

            }
            if( SIG == GPIO::LOW )
            {
                fs << 0;
           	}
            fs.close();
        }
}

unsigned BlackBone_GPIO::input(const std::string port)
{
        std::fstream fs;
        std::string dir = GPIO_PATH + "/gpio" + std::to_string(pinToIO[port]);
        fs.open(dir + "/value" ,std::fstream::in | std::fstream::out | std::fstream::app);
        std::string input;
        getline(fs, input);
        return atoi(input.c_str());

}

