//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include "../vendor/NDTime.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/mCheck.hpp"
#include "../atomics/Classification.hpp"
#include "../data_structures/message.hpp"

//C++ libraries
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct materials : public cadmium::in_port<int>{};

/***** Define output ports for coupled model *****/
struct cream : public cadmium::out_port<int>{};
struct biscuits : public cadmium::out_port<int>{};

/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Int : public iestream_input<int,T>
{
    public:
        InputReader_Int () = default;
        InputReader_Int (const char* file_path) : iestream_input<int,T>(file_path) {}
};

int main(){
    
    
    /****** Input Reader atomic models instantiation *******************/
    const char * i_input_data_control = "../input_data/inspector_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_con = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader_con" , move(i_input_data_control));

    /******   mcheck model instantiation *******************/
    shared_ptr<dynamic::modeling::model> mcheck1 = dynamic::translate::make_dynamic_atomic_model<mCheck, TIME>("mcheck1");
    /******  classification model instantiation *******************/
    shared_ptr<dynamic::modeling::model> classify1 = dynamic::translate::make_dynamic_atomic_model< Classify, TIME>("classify1");

    /*******Inspector Coulped MODEL********/
    dynamic::modeling::Ports iports_Inspect = {typeid(materials)};
    dynamic::modeling::Ports oports_Inspect = {typeid(biscuits),typeid(cream)};
    dynamic::modeling::Models submodels_Inspect = {mcheck1, classify1};
    dynamic::modeling::EICs eics_Inspect = { dynamic::translate::make_EIC<materials, mCheck_defs::m_in>("mcheck1")};
    dynamic::modeling::EOCs eocs_Inspect = {
        dynamic::translate::make_EOC< Classify_defs::Biscuits,biscuits>("classify1"),dynamic::translate::make_EOC< Classify_defs::Cream,cream>("classify1")};
    dynamic::modeling::ICs ics_Inspect = {
        dynamic::translate::make_IC<mCheck_defs::materials,Classify_defs::mIN>("mcheck1","classify1")
      
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> Inspect;
    Inspect = make_shared<dynamic::modeling::coupled<TIME>>(
        "Inspect", submodels_Inspect, iports_Inspect, oports_Inspect, eics_Inspect, eocs_Inspect, ics_Inspect
    );
    
    /*******TOP Coulped MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(biscuits),typeid(cream)};
    dynamic::modeling::Models submodels_TOP = {input_reader_con, Inspect};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC< biscuits,biscuits>("Inspect"),
        dynamic::translate::make_EOC< cream,cream>("Inspect")
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<int>::out,materials>("input_reader_con","Inspect")
      
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/inspector_test_output_messages.txt");
    struct oss_sink_messages
    {
        static ostream& sink()
        {
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/inspector_test_output_state.txt");
    struct oss_sink_state
    {
        static ostream& sink()
        {
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("05:00:00:000"));
    return 0;
}






