/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* Package:
* Cadmium implementation of CD++ Package atomic model
*/

#ifndef _PACKAGE_HPP__
#define _PACKAGE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Package_defs{
    struct out : public out_port<int> {};
    struct done : public out_port<string> {};
    struct in : public in_port<int> {};
};

template<typename TIME> class Package{
    public:
    // ports definition
    using input_ports=tuple<typename Package_defs::in>;
    using output_ports=tuple<typename Package_defs::out,typename Package_defs::done>;
    
    // state definition
    struct state_type{
        bool transmitting;
        int index;
        int Oreonum;
    }; 
    state_type state;    
    // default constructor
    Package() {
        state.transmitting = false;
        state.index        = 0;
        state.Oreonum=0;
    }     
    // internal transition
    void internal_transition() {
        state.transmitting = false;
        state.Oreonum=0;
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        
        for(const auto &x : get_messages<typename Package_defs::in>(mbs))
        {
            state.index ++;
            state.Oreonum=x;
            state.transmitting=true;
        }            
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        string DONE = "DONE";
        if (state.transmitting)
        {
        
            get_messages<typename Package_defs::out>(bags).push_back(state.Oreonum);
            get_messages<typename Package_defs::done>(bags).push_back(DONE);
        
        }
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.transmitting) {            
            next_internal = TIME("00:00:10:00");
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Package<TIME>::state_type& i) {
        os << "\n productout: " << i.Oreonum << " product num: " << i.index ; 
        return os;
    }
};    
#endif // _PACKAGE_HPP__
