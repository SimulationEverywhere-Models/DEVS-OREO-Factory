/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* BrokenCheck:
* Cadmium implementation of CD++ BrokenCheck atomic model
*/

#ifndef _BROKENCHECK_HPP__
#define _BROKENCHECK_HPP__

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
struct BrokenCheck_defs{
    struct biscuitout : public out_port<int> {};
    struct brokenout : public out_port<int> {};
    struct in : public in_port<int> {};
};

template<typename TIME> class BrokenCheck{
    public:
    // ports definition
    TIME   checkTime;
    using input_ports=tuple<typename BrokenCheck_defs::in>;
    using output_ports=tuple<typename BrokenCheck_defs::biscuitout,typename BrokenCheck_defs::brokenout>;
    // state definition
    struct state_type{
        bool check_brocken_out;
        bool model_state;
        int biscuit;
        int broken;
    }; 
    state_type state;    
    // default constructor
    BrokenCheck() noexcept{
        state.check_brocken_out = false;
        state.model_state=false;
        state.biscuit        = 0;
        state.broken        = 0;
        checkTime=TIME("00:00:05:00");
    }     
    // internal transition
    void internal_transition() {
        state.check_brocken_out = false;
        state.model_state=false;
        state.broken=0;
        state.biscuit=0;
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        // vector<Message_t> bag_port_in;
        // bag_port_in = get_messages<typename BrokenCheck_defs::in>(mbs);
        // if(bag_port_in.size()>1) assert(false && "One message at a time");                
        
        for(const auto &x : get_messages<typename BrokenCheck_defs::in>(mbs))
        {
            if(state.model_state == false){
                if ((double)rand() / (double) RAND_MAX  < 0.90){
                state.biscuit=x;             
                state.broken=0;
                state.model_state=true;
                state.check_brocken_out = true;  
            }else{
                state.broken =x;
                state.biscuit=0;
                state.model_state=true;
                state.check_brocken_out = false;
            }
                
            }
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
        // get_messages<typename BrokenCheck_defs::biscuitout>(bags) = bag_port_out; 
        if (state.model_state)
            {
               if(state.check_brocken_out)
               {
                 get_messages<typename BrokenCheck_defs::biscuitout>(bags).push_back(state.biscuit);
               }
                else
                {
                    get_messages<typename BrokenCheck_defs::brokenout>(bags).push_back(state.broken);
                }
           
            }
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.model_state) {            
            next_internal = checkTime;
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename BrokenCheck<TIME>::state_type& i) {
        os << "\n Biscuit: " << i.biscuit <<"\t Broken: " << i.broken << " & check_brocken_out: " << i.check_brocken_out; 
        return os;
    }
};    
#endif // _BROKENCHECK_HPP__
