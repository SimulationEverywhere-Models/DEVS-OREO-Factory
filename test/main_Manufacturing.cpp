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
#include "../atomics/Formation.hpp"
#include "../atomics/Bake.hpp"
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

/***** Define input port for coupled models *****/
struct materialsIN : public cadmium::in_port<int>{};
struct materials : public cadmium::in_port<int>{};
struct fIN : public cadmium::in_port<int>{};
struct assembl_b : public cadmium::in_port<int>{};
struct assembl_c : public cadmium::in_port<int>{};
struct bakein : public cadmium::in_port<int>{};

/***** Define output ports for coupled model *****/
struct oreo : public cadmium::out_port<int>{};
struct cream : public cadmium::out_port<int>{};
struct biscuits : public cadmium::out_port<int>{};
struct fOUT : public cadmium::out_port<int>{};
struct bakeout : public cadmium::out_port<int>{};
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
    const char * i_input_data_control = "../input_data/manufact_input_test.txt";
    shared_ptr<dynamic::modeling::model> input_reader_con = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader_con" , move(i_input_data_control));

    /******   mcheck model instantiation *******************/
    shared_ptr<dynamic::modeling::model> mcheck1 = dynamic::translate::make_dynamic_atomic_model<mCheck, TIME>("mcheck1");
    /******   classification model instantiation *******************/
    shared_ptr<dynamic::modeling::model> classify1 = dynamic::translate::make_dynamic_atomic_model< Classify, TIME>("classify1");
    /******   Formation model instantiation *******************/
    shared_ptr<dynamic::modeling::model> formation1 = dynamic::translate::make_dynamic_atomic_model< Formation, TIME>("formation1");
    /****** Bake atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> bake1 = dynamic::translate::make_dynamic_atomic_model<Bake, TIME>("bake1");
    /****** Brokencheck atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> brokencheck1 = dynamic::translate::make_dynamic_atomic_model<BrokenCheck, TIME>("brokencheck1");
    /****** Workshop atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> workshop1 = dynamic::translate::make_dynamic_atomic_model<Workshop, TIME>("workshop1");


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
    
    /*******Assembly  COUPLED MODEL********/
    dynamic::modeling::Ports iports_assembly = {typeid(assembl_b),typeid(assembl_c)};
    dynamic::modeling::Ports oports_assembly = {typeid(assembl_o)};
    dynamic::modeling::Models submodels_ass = {brokencheck1, workshop1};
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
    
    /*******manufact Coulped MODEL********/
    dynamic::modeling::Ports iports_mfc = {typeid(materialsIN)};
    dynamic::modeling::Ports oports_mfc = {typeid(oreo)};
    dynamic::modeling::Models submodels_mfc = {Inspect,formation1,bake1,ass};
    dynamic::modeling::EICs eics_mfc = { dynamic::translate::make_EIC<materialsIN, materials>("Inspect")};
    dynamic::modeling::EOCs eocs_mfc = {
        dynamic::translate::make_EOC< assembl_o,oreo>("ass")};
    dynamic::modeling::ICs ics_mfc = {
        dynamic::translate::make_IC<biscuits,Formation_defs::fIN>("Inspect","formation1"),
        dynamic::translate::make_IC<cream,assembl_c>("Inspect","ass"),
        dynamic::translate::make_IC<Formation_defs::fOUT,Bake_defs::in>("formation1","bake1"),
        dynamic::translate::make_IC<Bake_defs::out,assembl_b>("bake1","ass"),
      
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> Mfc;
    Mfc = make_shared<dynamic::modeling::coupled<TIME>>(
        "Mfc", submodels_mfc, iports_mfc, oports_mfc, eics_mfc, eocs_mfc, ics_mfc
    );
    
    /*******TOP Coulped MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(oreo)};
    dynamic::modeling::Models submodels_TOP = {input_reader_con, Mfc};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC< oreo,oreo>("Mfc"),
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<int>::out,materialsIN>("input_reader_con","Mfc")
      
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/manufact_test_output_messages.txt");
    struct oss_sink_messages
    {
        static ostream& sink()
        {
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/manufact_test_output_state.txt");
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







