#include "main.hpp"


// Define the class of function object
class fn_object_class {
    // Overload () operator
public:
    void operator()(uint8_t params)
    { 
		while(1){
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			std::cout << "Hello World from thread!" << static_cast<unsigned int>(params) << "\n"; 
		}
	}
};


enum command:char{
	CLOCK = 'C',
	STOPWATCH = 'S',
	DIMM = 'D',
	BRIGHT = 'B',
	LOADFILE = 'F',
	CLEAR = 'R',
	LOAD = 'L',
	RESET = 'X',
	START = 'T',
	STOP = 'P',
	PAUSE = 'H',
	QUIT = 'Q',
};



int main()
{
	
	MessageBuffer msgBuffer;
	
	fdComm fdcomm(&msgBuffer);

	colorCron colorcronbuf;
	gpiod::line::offsets pins={20,21,22,23,24,25};
	GPIOaccess gpios;

	gpios.init(pins);

	I2Cbus i2cbus[2] = {
		I2Cbus((std::string)"/dev/i2c-1"),
		I2Cbus((std::string)"/dev/i2c-0")
	};

	Display disp[1] = {
		//Display(&i2cbus[0], 0x30),
		Display(&i2cbus[1], 0x30, &gpios)
	};


	disp[0].displayMode = Display::displayMode_t::CLOCK;
	// std::this_thread::sleep_for(std::chrono::seconds(30));
	//disp[0].displayMode = Display::displayMode_t::STOPWATCH;

	std::string inCmd;

	// vector<string> row;
	std::string filename;
	std::ifstream file;
	std::string line;
	std::list<colorCron>::iterator range_begin;
	std::list<colorCron>::iterator range_end;
	json colorcronjson;
	std::string inString;
	std::ifstream f;
	Display::displayMode_t tempmode;
	uint8_t updatecnt=0;
	while(1){
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		msgBuffer.mtx.lock();
		for(MessageBuffer::Message msg : msgBuffer.messages){
			// check timestamp
			// if the message is older than 5 seconds, delete it
			// delete the message if it is older than 5 seconds
			if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - msg.timestamp).count() > 15) {
				std::cout << "Message " << msg.ID << "  is older than 5 seconds, deleting it" << std::endl;
				msgBuffer.deleteMessage(msg.ID);
				continue;
			}

			if(msg.ready == false) {
				continue;
				msgBuffer.mtx.unlock();
			}
			std::cout << "****************************************" << std::endl;
			std::cout << "Processing message: " << std::endl;
			std::cout << "ID: " << msg.ID << std::endl;
			std::cout << "Source: " << msg.source << std::endl;
			std::cout << "Command: " << msg.command << std::endl;
			std::cout << "Data: " << msg.data << std::endl;
			const std::time_t t_c = std::chrono::system_clock::to_time_t(msg.timestamp);
			std::cout << "Timestamp: " << std::put_time(std::localtime(&t_c), "%F %T") 
			// << " "  << msg.timestamp.time_since_epoch().count() 
			// << std::chrono::duration_cast<std::chrono::seconds>(
				// msg.timestamp.time_since_epoch()).count() 
			<< std::endl;
			std::cout << "File descriptor: " << msg.channel << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			long bright = 0;
			
			switch(msg.command.c_str()[0]){
				case command::CLOCK:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].displayMode = Display::displayMode_t::CLOCK;
					}
					break;
				case command::STOPWATCH:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].displayMode = Display::displayMode_t::STOPWATCH;
					}
					break;
				case command::DIMM:
					bright = -5;
					try{
						bright = std::stol(msg.data.substr(0,msg.data.length()),nullptr, 0);
						} catch (const std::invalid_argument& e) {
		
						} catch (const std::out_of_range& e) {
						std::cerr << "Out of range, setting the brightness to 5: " << e.what() << std::endl;

						} catch (...){
						std::cerr << "Unknown data size error, setting the brightness: " << std::endl;

						}
					std::cout << "Brightness: " << disp[0].dispBrightness << std::endl;
					for(uint8_t i=0;i<std::size(disp);i++){
							disp[i].dispBrightness += bright;
						}
					break;
				case command::BRIGHT:
					bright = 5;
					try{
						bright = std::stol(msg.data.substr(0,msg.data.length()),nullptr, 0);
						} catch (const std::invalid_argument& e) {

						} catch (const std::out_of_range& e) {
						std::cerr << "Out of range, setting the brightness to 5: " << e.what() << std::endl;
						} catch (...){
						std::cerr << "Unknown data size error, setting the brightness: " << std::endl;
						}
					std::cout << "Brightness: " << bright << std::endl;
					for(uint8_t i=0;i<std::size(disp);i++){
							disp[i].dispBrightness += bright;
						}
					break;
				case command::LOADFILE:
					// std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //flush the input
					// std::cout << "Enter the filename: ";
					// std::getline(std::cin, filename);
					filename = msg.data;
					if(filename.length() == 0) {
						std::cerr << "Filename is empty." << std::endl;
						break;
					}
					std::cout << "Loading file: " << filename << std::endl;
					f.open(filename);
					if(f.is_open() == false) {
						std::cerr << "Could not open the file: " << filename << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						break;
					}
					try{
					colorcronjson = json::parse(f);
					}
					catch (const std::exception& e) {
						std::cerr << "Error parsing json file: " << e.what() << std::endl;
						f.close();
						break;
						file.close();
					}
					file.close();

					//pause the display
					
					for(uint8_t i=0;i<std::size(disp);i++){
						tempmode = disp[i].displayMode;
						disp[i].displayMode = Display::displayMode_t::PAUSE;
					}
					//clear the colorList
					for(uint8_t i=0;i<std::size(disp);i++){
						// disp[i].colorList.clear();
						range_begin = disp[i].colorList.begin();
						range_end = disp[i].colorList.end();
						disp[i].colorList.erase(range_begin, range_end);
					}
					
					for (auto& Item : colorcronjson) {
						colorcronbuf.load(Item);
						for(uint8_t i=0;i<std::size(disp);i++){
							// disp[i].mode = (colorCron)inQueue.data[0];
							if(disp[i].colorList.empty()) {
								disp[i].colorList.assign({colorcronbuf});
							} else {
								disp[i].colorList.push_back(colorcronbuf);
							}
							
						}
					}
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].displayMode = tempmode;
					}
					break;
				case command::CLEAR: //cleaR the color list
					for(uint8_t i=0;i<std::size(disp);i++){
						tempmode = disp[i].displayMode;
						disp[i].displayMode = Display::displayMode_t::PAUSE;
					}
					//clear the colorList
					for(uint8_t i=0;i<std::size(disp);i++){
						// disp[i].colorList.clear();
						range_begin = disp[i].colorList.begin();
						range_end = disp[i].colorList.end();
						disp[i].colorList.erase(range_begin, range_end);
					}
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].displayMode = tempmode;
					}
					break;
				case command::LOAD: //load color list from json
					try{
						colorcronjson = json::parse(msg.data);
					}
					catch (const std::exception& e) {
						std::cerr << "Error parsing json file: " << e.what() << std::endl;
						break;
					}

					//pause the display
					for(uint8_t i=0;i<std::size(disp);i++){
						tempmode = disp[i].displayMode;
						disp[i].displayMode = Display::displayMode_t::PAUSE;
					}

					
					for (auto& Item : colorcronjson) {
						colorcronbuf.load(Item);
						for(uint8_t i=0;i<std::size(disp);i++){
							// disp[i].mode = (colorCron)inQueue.data[0];
							if(disp[i].colorList.empty()) {
								disp[i].colorList.assign({colorcronbuf});
							} else {
								disp[i].colorList.push_back(colorcronbuf);
							}
							
						}
					}

					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].displayMode = tempmode;
					}
					break;
				case command::RESET:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].stopwatch.reset();
					}
					break;
				case command::STOP:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].stopwatch.stop();
					}
					break;
				case command::PAUSE:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].stopwatch.lapPause();
					}
					break;
				case command::START:
					for(uint8_t i=0;i<std::size(disp);i++){
						disp[i].stopwatch.start();
					}
					break;
				case command::QUIT:
					std::cout << "Quitting..." << std::endl;

					exit(0);
					break;
				default:
					cout << "Unkown Command: " << inCmd << std::endl;

			}
			
			msgBuffer.deleteMessage(msg.ID);
			
			// std::cout << "----------------------------------------" << std::endl;
		}
		msgBuffer.mtx.unlock();
        
	}

	return 0;
}


	// // open gpio chip
    // struct gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip4");
    // if (chip == NULL)
    // {
    //     perror("gpiod_chip_open");

    // }
	// // configure gpio lines
    // struct gpiod_line_settings* lineSettings = gpiod_line_settings_new();
    // if (lineSettings == NULL)
    // {
    //     perror("gpiod_line_settings_new");
      
    // }
	// gpiod_line_settings_set_direction(lineSettings, GPIOD_LINE_DIRECTION_INPUT);
    // // gpiod_line_settings_set_edge_detection(lineSettings, GPIOD_LINE_EDGE_BOTH);
    // // gpiod_line_settings_set_event_clock(lineSettings, GPIOD_LINE_CLOCK_MONOTONIC);
	// struct gpiod_line_config* lineConfig = gpiod_line_config_new();
    // if (lineConfig == NULL)
    // {
    //     perror("gpiod_line_config_new");

    // }
	// const uint lines[] = { pins::STOP,pins::START };
    // gpiod_line_config_add_line_settings(lineConfig, lines, 1, lineSettings);

    // struct gpiod_request_config* requestConfig = gpiod_request_config_new();
    // if (requestConfig == NULL)
    // {
    //     perror("gpiod_request_config_new");

    // }
	// gpiod_request_config_set_consumer(requestConfig, "me");

    // printf("Line request config created\n");

	// // gpiod request object to monitor events
	// struct gpiod_line_request* request;
	// // create request (event handler thread shall start handling events)
	// request = gpiod_chip_request_lines(chip, requestConfig, lineConfig);
	// if (request == NULL)
	// {
	// 	perror("gpiod_chip_request_lines");

	// }