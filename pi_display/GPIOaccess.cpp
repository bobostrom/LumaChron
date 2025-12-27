#include "GPIOaccess.hpp"
int8_t GPIOaccess::getPinValue(uint pin){
    int8_t value=0;
   // value = gpiod_line_request_get_value(this->request, 25);
    // value = gpiod_line_request_get_value(this->request, pin);
    // value = gpiod_line_request_get_value(this->request, pin);
    if(value <0){
        perror("gpiod_line_request_get_value");

    }
    return value;
}

uint GPIOaccess::getEdgeEventCount(){
   // return gpiod_edge_event_buffer_get_num_events(this->edgeEventBuffer);
   return 0;
}
// 
const char *edge_event_type_str(const ::gpiod::edge_event &event)
{
	switch (event.type()) {
	case ::gpiod::edge_event::event_type::RISING_EDGE:
		return "Rising";
	case ::gpiod::edge_event::event_type::FALLING_EDGE:
		return "Falling";
	default:
		return "Unknown";
	}
}

void GPIOaccess::eventThreadFunc(){
   
    /* request the line*/
    try{
        gpiod::line_request request = gpiod::chip(this->chip_path)
        .prepare_request()
        .set_consumer("Display Clock")
        .add_line_settings(
            this->line_offsets,
            this->lineSettings)
        .do_request();
 


    
        for (;;) {

            // std::cout << "eventThreadRunning\n" << std::flush;
            // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //     /* Blocks until at least one event is available. */
            request.read_edge_events(edgeEventBuffer);
            for (const auto &event : edgeEventBuffer){
                this->eventVector.push_back(event);
                ::std::cout << "line: " << event.line_offset()
                        << "  type: " << ::std::setw(7) << ::std::left << edge_event_type_str(event)
                        << "  event #" << event.line_seqno()
                        << ::std::endl;
            }
        }
        
    }  
    catch (const std::exception& e) {
        std::cerr << "GPIOaccess::Error initializing GPIO: " << e.what() << std::endl;
        std::exit(1);
        
    }
    catch (...) {
        std::cerr << "GPIOaccess::Unknown GPIOd error" << std::endl;
        std::exit(1);
        
    }
}
int GPIOaccess::init(gpiod::line::offsets pins_in){
    using namespace std::chrono_literals;
    this->chip_path.assign("/dev/gpiochip0");

    
    this->lineSettings.set_direction(::gpiod::line::direction::INPUT);
    this->lineSettings.set_edge_detection(gpiod::line::edge::BOTH);
    this->lineSettings.set_bias(gpiod::line::bias::PULL_UP);
    this->lineSettings.set_drive(gpiod::line::drive::PUSH_PULL);
    this->lineSettings.set_active_low(false);
    this->lineSettings.set_debounce_period(10ms);
    this->lineSettings.set_event_clock(gpiod::line::clock::MONOTONIC);
    
    this->line_offsets = pins_in;
    
   
    /* request the line*/
    // gpiod::line_request request = gpiod::chip(this->chip_path)
    //     .prepare_request()
    //     .set_consumer("Display Clock")
    //     .add_line_settings(
    //         this->line_offsets,
    //         this->lineSettings)
    //     .do_request();


    /*
    * A larger buffer is an optimisation for reading bursts of events from
    * the kernel, but that is not necessary in this case, so 1 is fine.
    */
    // edgeEventBuffer = gpiod::edge_event_buffer(std::size_t(64));
    
    this->eventRunning = true;
    eventThread = std::thread(&GPIOaccess::eventThreadFunc, this);

    // for (;;) {
    //     /* Blocks until at least one event is available. */
    //     request.read_edge_events(edgeEventBuffer);
    //     for (const auto &event : edgeEventBuffer){
    //         this->eventVector.push_back(event);
    //         ::std::cout << "line: " << event.line_offset()
    //                 << "  type: " << ::std::setw(7) << ::std::left << edge_event_type_str(event)
    //                 << "  event #" << event.line_seqno()
    //                 << ::std::endl;
    //     }
    // }


    //     while(1){
    // /* read the current line value */
    // ::std::cout << line_offsets[0] << "="
    // << (request.get_value(line_offsets[0]) ==
    //         ::gpiod::line::value::ACTIVE ?
    //     "Active" :
    //     "Inactive")
    // << " (input)" << ::std::endl;
    //     }
    /* switch the line to an output and drive it low */
    // request.reconfigure_lines(::gpiod::line_config().add_line_settings(
    // line_offset,
    // ::gpiod::line_settings()
    //     .set_direction(::gpiod::line::direction::OUTPUT)
    //     .set_output_value(::gpiod::line::value::INACTIVE)));

    // /* report the current driven value */
    // ::std::cout << line_offset << "="
    //     << (request.get_value(line_offset) ==
    //             ::gpiod::line::value::ACTIVE ?
    //         "Active" :
    //         "Inactive")
    //     << " (output)" << ::std::endl;



return 0;
};
GPIOaccess::GPIOaccess(){
    // printf("empty gpioaccess\n");
    // this->init();
};

GPIOaccess::~GPIOaccess(){
    // gpiod_line_request_release(this->request);
    // gpiod_request_config_free(this->requestConfig);
    // gpiod_line_config_free(this->lineConfig);
    // gpiod_line_settings_free(this->lineSettings);
    // gpiod_chip_close(this->chip);
    // gpiod_edge_event_buffer_free(this->edgeEventBuffer);
}

// int GPIOaccess::init(uint* linesin, gpiod_line_direction direction_in){
//     this->direction = direction_in;
//     //build the lines array from the lines in array
//     int linessize = sizeof(linesin)/sizeof(linesin[0]);
//     this->lines = new uint[linessize];
//     for(int i = 0;i<linessize;i++){
//         this->lines[i]=linesin[i];
//     }
//     // open gpio chip
//     this->chip = gpiod_chip_open("/dev/gpiochip0");
//     if (this->chip == NULL)
//     {
//         perror("gpiod_chip_open");
//     }
// 	// configure gpio lines
//     this->lineSettings = gpiod_line_settings_new();
//     if (this->lineSettings == NULL)
//     {
//         perror("gpiod_line_settings_new");
      
//     }
// 	gpiod_line_settings_set_direction(this->lineSettings, this->direction);//GPIOD_LINE_DIRECTION_INPUT);
//     // gpiod_line_settings_set_edge_detection(lineSettings, GPIOD_LINE_EDGE_BOTH);
//     // gpiod_line_settings_set_event_clock(lineSettings, GPIOD_LINE_CLOCK_MONOTONIC);
//     gpiod_line_settings_set_bias(this->lineSettings, GPIOD_LINE_BIAS_PULL_UP);
// 	this->lineConfig = gpiod_line_config_new();
//     if (this->lineConfig == NULL)
//     {
//         perror("gpiod_line_config_new");

//     }
// 	// lines = new const uint[2]{ pins::STOP,pins::START };

//     gpiod_line_config_add_line_settings(this->lineConfig, this->lines, sizeof(this->lines)/sizeof(this->lines[0]), this->lineSettings);

//     this->requestConfig = gpiod_request_config_new();
//     if (this->requestConfig == NULL)
//     {
//         perror("gpiod_request_config_new");

//     }
// 	gpiod_request_config_set_consumer(this->requestConfig, "Display Clock");

//     printf("Line request config created\n");

// 	// gpiod request object to monitor events
// 	//request;
// 	// create request (event handler thread shall start handling events)
// 	this->request = gpiod_chip_request_lines(this->chip, this->requestConfig, this->lineConfig);
// 	if (this->request == NULL)
// 	{
// 		perror("gpiod_chip_request_lines");

// 	}

//     return 0;
// };


// GPIOaccess::GPIOaccess(uint* lines_in, gpiod_line_direction direction_in){
//     this->direction = direction_in;
//     //build the lines array from the lines in array
//     int linessize = sizeof(lines_in)/sizeof(lines_in[0]);
//     this->lines = new uint[linessize];
//     for(int i = 0;i<linessize;i++){
//         this->lines[i]=lines_in[i];
//     }
//     // this->init();  
// }


/*

// open gpio chip
    this->chip = gpiod_chip_open("/dev/gpiochip0");
    
    if (this->chip == NULL)
    {
        perror("gpiod_chip_open");
    }
	// configure gpio lines
    this->lineSettings = gpiod_line_settings_new();
    if (this->lineSettings == NULL)
    {
        perror("gpiod_line_settings_new");
      
    }
    edgeEventBuffer = gpiod_edge_event_buffer_new(64);
    if(this->edgeEventBuffer == NULL){
        perror("gpiod_edge_event_buffer_new");
    }
	gpiod_line_settings_set_direction(this->lineSettings, this->direction);//GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_edge_detection(this->lineSettings, GPIOD_LINE_EDGE_BOTH);
    gpiod_line_settings_set_event_clock(this->lineSettings, GPIOD_LINE_CLOCK_MONOTONIC);
    gpiod_line_settings_set_bias(this->lineSettings, GPIOD_LINE_BIAS_PULL_UP);
	this->lineConfig = gpiod_line_config_new();
    if (this->lineConfig == NULL)
    {
        perror("gpiod_line_config_new");

    }
	// lines = new const uint[2]{ pins::STOP,pins::START };

    gpiod_line_config_add_line_settings(this->lineConfig, this->lines, sizeof(this->lines)/sizeof(this->lines[0]), this->lineSettings);

    this->requestConfig = gpiod_request_config_new();
    if (this->requestConfig == NULL)
    {
        perror("gpiod_request_config_new");

    }
	gpiod_request_config_set_consumer(this->requestConfig, this->consumer.c_str());

    printf("Line request config created\n");

	// gpiod request object to monitor events
	//request;
	// create request (event handler thread shall start handling events)
	this->request = gpiod_chip_request_lines(this->chip, this->requestConfig, this->lineConfig);
	if (this->request == NULL)
	{
		perror("gpiod_chip_request_lines");

	}
    int64_t junk=0;
    int64_t numedges=0;
    while(numedges == 0){
        auto junk2 = gpiod_line_info_get_edge_detection(gpiod_chip_get_line_info(chip,25));
        junk++;
        numedges = gpiod_edge_event_buffer_get_num_events(this->edgeEventBuffer);
    }
    printf("%d\n",junk);
*/