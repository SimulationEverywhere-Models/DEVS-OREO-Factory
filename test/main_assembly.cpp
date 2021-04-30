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

#include "../atomics/BrokenCheck.hpp"
#include "../atomics/Workshop.hpp"


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

struct assembl_b : public cadmium::in_port<int>{};
struct assembl_c : public cadmium::in_port<int>{};
struct assembl_o : public cadmium::out_port<int>{};

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
    const char * i_input_data_control = "../input_data/assembly_cream_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader" , std::move(i_input_data_control));
    const char * i_input_data_control2 = "../input_data/assembly_biscuit_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader2 = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader2" , std::move(i_input_data_control2));

    /****** Brokencheck atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> brokencheck1 = dynamic::translate::make_dynamic_atomic_model<BrokenCheck, TIME>("brokencheck1");
    /****** Workshop atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> workshop1 = dynamic::translate::make_dynamic_atomic_model<Workshop, TIME>("workshop1");


 
    
    /*******Assembly  COUPLED MODEL********/
    dynamic::modeling::Ports iports_assembly = {typeid(assembl_b),typeid(assembl_c)};
    dynamic::modeling::Ports oports_assembly = {typeid(assembl_o)};
    dynamic::modeling::Models submodels_ass = { brokencheck1, workshop1};
    dynamic::modeling::EICs eics_ass = {
        dynamic::translate::make_EIC<assembl_c, Workshop_defs::CreamIn>("workshop1"),
        dynamic::translate::make_EIC<assembl_b, BrokenCheck_defs::in>("brokencheck1")
    };
    dynamic::modeling::EOCs eocs_ass = {
        dynamic::translate::make_EOC<Workshop_defs::out,assembl_o>("workshop1")
    };
    dynamic::modeling::ICs ics_ass = {
        dynamic::translate::make_IC<BrokenCheck_defs::biscuitout,Workshop_defs::BiscuitIn>("brokencheck1","workshop1")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> ass;
    ass = make_shared<dynamic::modeling::coupled<TIME>>(
        "ass", submodels_ass, iports_assembly, oports_assembly, eics_ass, eocs_ass, ics_ass
    );
    
 
    /*******TOP Coulped MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(assembl_o)};
    dynamic::modeling::Models submodels_TOP = {input_reader,input_reader2, ass};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<assembl_o,assembl_o>("ass"),
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<int>::out,assembl_c>("input_reader","ass"),
        dynamic::translate::make_IC<iestream_input_defs<int>::out,assembl_b>("input_reader2","ass")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );
    

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/assembly_test_output_messages.txt");
    struct oss_sink_messages
    {
        static ostream& sink()
        {
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/assembl_test_output_state.txt");
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
    r.run_until(NDTime("00:05:00:000"));
    return 0;
}
