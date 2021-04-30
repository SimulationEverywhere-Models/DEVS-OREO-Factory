

#ifndef Formation_hpp
#define Formation_hpp

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
struct Formation_defs
{
    
    struct fOUT : public out_port<int> {};
    struct fIN : public in_port<int> { };
   
};

template<typename TIME> class Formation
{
    public:
        //Parameters to be overwriten when instantiating the atomic model
        TIME   formTime;
        // default constructor
    Formation() noexcept
        {
          formTime  = TIME("00:00:10:00");
          state.model_state   = false;
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
        using input_ports=std::tuple<typename Formation_defs::fIN>;
        using output_ports=std::tuple<typename Formation_defs::fOUT>;

        // internal transition
        void internal_transition()
       {
           state.model_state = false;
           state.NUM = 0;
               
        }

        // external transition
        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
        {
            
            for(const auto &x : get_messages<typename Formation_defs::fIN>(mbs))
            {
                if(state.model_state == false){
                    state.index++ ;
                    state.NUM = x ;
                    state.model_state = true;
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
               
                get_messages<typename Formation_defs::fOUT>(bags).push_back(state.NUM);
           
            }
           
            return bags;
        }

        // time_advance function
        TIME time_advance() const
        {
            TIME next_internal4;
            if (state.model_state)
            {
                  next_internal4 = formTime;
            }
            else
            {
                  next_internal4 = numeric_limits<TIME>::infinity();
            }
               return next_internal4;
        }

        friend std::ostringstream& operator<<(std::ostringstream& os, const typename Formation<TIME>::state_type& i)
       {
            os << "\n foramtionNum: " << i.NUM << " formationTimes:"<< i.index ;
        return os;
       }
};



#endif /* Formation_hpp */
