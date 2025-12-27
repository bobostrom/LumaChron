#include "colorCron.hpp"

// const std::map<std::string,int8_t> colorCron::timeModeMap  {
//     {"NOCHG", -1},
//     {"DDHHMM", 0},
//     {"HHMMSS", 6},
//     {"MMSSTT", 12},
//     {"AUTO", 16}
// };

int colorCron::load(json colorcronjson){
    
    // std::vector<std::vector<std::string>> data;
    
    // std::stringstream ss;
    // std::string cell;
    
    // ss.clear();
    // ss.str(line);
    // cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    // std::cout << "colorcronjson: " << colorcronjson.dump() << std::endl;
    this->decSec_ms = colorcronjson["decSec_ms"];
    this->seconds = colorcronjson["seconds"];
    this->minutes = colorcronjson["minutes"];
    this->hours = colorcronjson["hours"];
    this->days = colorcronjson["days"];


    if(colorcronjson["symbolColor"].type() == nlohmann::json::value_t::string){
        this->symbolColor = RGB(std::string(colorcronjson["symbolColor"]));
    }else if(colorcronjson["symbolColor"].type() == nlohmann::json::value_t::number_integer ||
        colorcronjson["symbolColor"].type() == nlohmann::json::value_t::number_unsigned){        
        this->symbolColor = RGB(std::uint32_t(colorcronjson["symbolColor"]));
    }

    if(colorcronjson["colonColor"].type() == nlohmann::json::value_t::string){
        this->colonColor = RGB(std::string(colorcronjson["colonColor"]));
    }else if(colorcronjson["colonColor"].type() == nlohmann::json::value_t::number_integer ||
        colorcronjson["colonColor"].type() == nlohmann::json::value_t::number_unsigned){        
        this->colonColor = RGB(std::uint32_t(colorcronjson["colonColor"]));
    }

    if(colorcronjson["dpColor"].type() == nlohmann::json::value_t::string){
        this->dpColor = RGB(std::string(colorcronjson["dpColor"]));
    }else if(colorcronjson["dpColor"].type() == nlohmann::json::value_t::number_integer ||
        colorcronjson["dpColor"].type() == nlohmann::json::value_t::number_unsigned){        
        this->dpColor = RGB(std::uint32_t(colorcronjson["dpColor"]));
    }


    try{this->timeMode = colorCron::timeModeMap.at(colorcronjson["timeMode"]);}
    catch(const std::out_of_range& e){
        std::cerr << "Error: Key not found: " << e.what() << std::endl;
    }

    this->symbolBlink = colorcronjson["symbolBlink"];
    this->colonBlink = colorcronjson["colonBlink"];
    this->dpBlink = colorcronjson["dpBlink"];
    this->brightness = colorcronjson["brightness"];

    
    return(0);
}


int colorCron::load(std::string line){

	
	std::vector<std::vector<std::string>> data;
	
	std::stringstream ss;
	std::string cell;
    
    ss.clear();
    ss.str(line);
    // cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    // while(getline(ss,cell,',')){
    getline(ss,cell,','); this->decSec_ms = std::stol(cell); 
    getline(ss,cell,','); this->seconds = std::stoi(cell);
    getline(ss,cell,','); this->minutes = std::stol(cell); 
    getline(ss,cell,','); this->hours = std::stoi(cell); 
    getline(ss,cell,','); this->days = std::stol(cell); 

    getline(ss,cell,',');
    cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    this->symbolColor = RGB(cell);

    getline(ss,cell,',');
    cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    this->colonColor = RGB(cell);

    getline(ss,cell,',');
    cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    this->dpColor = RGB(cell);

    // getline(ss,cell,',');
    // cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    // this->secondsColor = RGB(cell);

    getline(ss,cell,','); 
    cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace),cell.end());
    try{this->timeMode = colorCron::timeModeMap.at(cell);}
    catch(const std::out_of_range& e){
        std::cerr << "Error: Key not found: " << e.what() << std::endl;
    }

    getline(ss,cell,','); this->symbolBlink = std::stol(cell);
    getline(ss,cell,','); this->colonBlink = std::stol(cell);
    getline(ss,cell,','); this->dpBlink = std::stol(cell);
    getline(ss,cell,','); this->brightness = std::stoi(cell,nullptr, 16);


	
    return(0);
}