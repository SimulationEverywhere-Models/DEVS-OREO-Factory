

#ifndef _BAKE_HPP__
#define _BAKE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

//#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Bake_defs{
    struct out : public out_port<int> {};
    struct in : public in_port<int> {};
};

template<typename TIME> class Bake{
    public:
    TIME   bakeTime;
    // ports definition
    using input_ports=tuple<typename Bake_defs::in>;
    using output_ports=tuple<typename Bake_defs::out>;
    // state definition
    struct state_type{
        bool transmitting;
        Message_t packet;
        int index;
        int Backnum;
    }; 
    state_type state;    
    // default constructor
    Bake() noexcept{
        state.transmitting = false;
        state.index        = 0;
        state.Backnum=0;
        bakeTime=TIME("00:00:10:10");
    }     
    // internal transition
    void internal_transition() {
        state.transmitting = false;  
        state.Backnum=0;
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        for(const auto &x : get_messages<typename Bake_defs::in>(mbs))
        {if(state.transmitting == false){
            state.index ++;
            state.Backnum=x;
            state.transmitting=true;}
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
        // vector<Message_t> bag_port_out;            
        // bag_port_out.push_back(state.packet);
        // get_messages<typename Bake_defs::out>(bags) = bag_port_out; 
        if (state.transmitting)
        {
            
            get_messages<typename Bake_defs::out>(bags).push_back(state.Backnum);
        
        }
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.transmitting) {            
            next_internal = bakeTime;
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Bake<TIME>::state_type& i) {
        os << "\nBake: " << i.Backnum << " & transmitting: " << i.transmitting; 
        return os;
    }
};    
#endif // _SUBNET_HPP__
