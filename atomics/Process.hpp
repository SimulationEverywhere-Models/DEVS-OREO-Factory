
#ifndef Process_hpp
#define Process_hpp


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <list>

//#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Process_defs
{
    
    struct pOUT : public out_port<int> {};
    struct pIN : public in_port<string> { };
   
};

template<typename TIME> class Process
{
    public:
        //Parameters to be overwriten when instantiating the atomic model
        TIME   ProcessTime;
        // default constructor
        Process() noexcept
        {
          ProcessTime  = TIME("00:00:05:00");
          state.model_state     = false;
            state.index = 0;
            state.NUM = 0;
        }
        
        // state definition
        struct state_type
       {
            int index;
            int NUM;
            bool model_state;
        };
        state_type state;
        // ports definition
        using input_ports=std::tuple<typename Process_defs::pIN>;
        using output_ports=std::tuple<typename Process_defs::pOUT>;

        // internal transition
        void internal_transition()
       {
           state.model_state = false;
           state.NUM = 0;
               
        }

        // external transition
        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
        {
            
            for(const auto &x : get_messages<typename Process_defs::pIN>(mbs))
            {
                if(x == "IN")
                {
                    state.index++ ;
                    state.NUM = 2 ;
                    state.model_state = true;
                    
                }
                else
                {
                    state.NUM = 0 ;
                    state.model_state = false;
                }
                    
            }
        }
                          
        

        // confluence transition
        void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
            internal_transition();
            external_transition(TIME(), std::move(mbs));
        }

        // output function
        typename make_message_bags<output_ports>::type output() const
    {
            typename make_message_bags<output_ports>::type bags;
           
           if (state.model_state)
            {
               
                get_messages<typename Process_defs::pOUT>(bags).push_back(state.NUM);
           
            }
           
            return bags;
        }

        // time_advance function
        TIME time_advance() const {
            TIME next_internal;
            if (state.model_state)
            {
                  next_internal = ProcessTime;
            }
            else
            {
                  next_internal = numeric_limits<TIME>::infinity();
            }
               return next_internal;
        }

        friend std::ostringstream& operator<<(std::ostringstream& os, const typename Process<TIME>::state_type& i)
       {
            os << "\ntotalNum: " << i.NUM << "\tsendingNum:"<< i.index ;
        return os;
       }
};
#endif // __APPLICATIONPROCESS_HPP__

