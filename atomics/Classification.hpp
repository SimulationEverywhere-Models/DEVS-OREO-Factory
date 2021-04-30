

#ifndef Classification_hpp
#define Classification_hpp

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <stdlib.h>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

//Port definition
    struct Classify_defs{
        struct Biscuits : public out_port<int> { };
        struct Cream : public out_port<int> { };
        struct mIN : public in_port<int> { };
    };
   
template<typename TIME> class Classify
{
    public:
    
    TIME classifyTime;
    // state definition
    struct state_type
    {
        bool model_state;
        Message_t Bstate;
        Message_t Cstate;
        int iNum;
    };
    state_type state;
    // default constructor
    Classify() noexcept
    {
        classifyTime  = TIME("00:00:10:00");
        state.model_state = false;
        state.iNum = 0;
        state.Bstate.NUM = 0;
        state.Cstate.NUM = 0;
    }
    // ports definition
    using input_ports=std::tuple<typename Classify_defs::mIN>;
    using output_ports=std::tuple<typename Classify_defs::Biscuits, typename Classify_defs::Cream>;

    // internal transition
    void internal_transition()
    {
        state.model_state = false;
        state.Bstate.NUM = 0;
        state.Cstate.NUM = 0;
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
        for(const auto &x : get_messages<typename Classify_defs::mIN>(mbs))
        {
            if(state.model_state == false){
            state.iNum = x ;
            state.Bstate.Material = "Biscuits";
            state.Cstate.Material = "Cream";
            state.Bstate.NUM = x/2;
            state.Cstate.NUM = x-x/2;
            state.model_state = true;
                
            }
        
        }
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const
    {
        typename make_message_bags<output_ports>::type bags;
        if(state.model_state){
        get_messages<typename Classify_defs::Biscuits>(bags).push_back(state.Bstate.NUM);
            get_messages<typename Classify_defs::Cream>(bags).push_back(state.Cstate.NUM);}
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal3;
        if (state.model_state)
        {
              next_internal3 = classifyTime;
        }
        else
        {
              next_internal3 = numeric_limits<TIME>::infinity();
        }
           return next_internal3;
     }

    friend std::ostringstream& operator<<(std::ostringstream& os, const typename Classify<TIME>::state_type& i)
    {
        os <<"\n biscuit state: "<<i.Bstate << "\t cream state: " << i.Cstate ;
        return os;
    }


};

#endif /* Classification_hpp */
