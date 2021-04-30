

#ifndef _WORKSHOP_HPP__
#define _WORKSHOP_HPP__

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
struct Workshop_defs{
    struct out : public out_port<int> {};
    struct CreamIn : public in_port<int> {};
    struct BiscuitIn: public in_port<int> {};
    
};

template<typename TIME> class Workshop{
    public:
    // ports definition
    using input_ports=std::tuple<typename Workshop_defs::CreamIn, typename Workshop_defs::BiscuitIn>;
    using output_ports=std::tuple<typename Workshop_defs::out>;
    // state definition
    struct state_type{
        bool transmitting;
        bool biscuitsin;
        bool creamin;
        int cream;
        int biscuitnum;
        int oreonum;
    }; 
    state_type state;    
    // default constructor
    Workshop() noexcept {
        state.transmitting = false;
        state.creamin = false;
        state.biscuitsin = false;
        state.biscuitnum    = 0;
        state.cream = 0;
        state.oreonum = 0;
    }     
    // internal transition
    void internal_transition() {
        state.transmitting = false;
        state.oreonum = 0;
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        
        for(const auto &x : get_messages<typename Workshop_defs::CreamIn>(mbs)) 
        {
            state.creamin = true;
            state.cream =x;
        }
        for(const auto &y : get_messages<typename Workshop_defs::BiscuitIn>(mbs))
        {
            state.biscuitsin = true;
            state.biscuitnum = y;
        }
        if( state.biscuitsin && state.creamin)
        {
            state.oreonum = state.biscuitnum;
            state.transmitting = true;
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
        get_messages<typename Workshop_defs::out>(bags).push_back(state.oreonum);
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.transmitting) {            
            next_internal = TIME("00:00:15:00");
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Workshop<TIME>::state_type& i) {
        os << "\n Oreo: " << i.oreonum << "\t biscuitsnum: " << i.biscuitnum<<"\t cream: "<< i.cream;
        return os;
    }
};    
#endif // _WORKSHOP_HPP__
