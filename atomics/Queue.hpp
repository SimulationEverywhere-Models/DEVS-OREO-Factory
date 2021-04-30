#ifndef Queue_hpp
#define Queue_hpp

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

struct Queue_defs{
    
    struct qOut : public out_port<string> {};
    struct qDone : public in_port<string> { };
    struct qIn : public in_port<string> { };
};

template<typename TIME> class Queue
{
    public:
        //Parameters to be overwriten when instantiating the atomic model
        TIME   QueueTime;
    unsigned int totalNum;
        // default constructor
        Queue() noexcept
        {
          QueueTime  = TIME("00:00:05:00");
            totalNum  =  4;
            state.Num = 0;
            state.full = false;
            state.sending = false;
          state.model_active     = false;
            state.Material = "None";
         }
        
        // state definition
        struct state_type
        {
            int Num;
            string Material;
            bool full;
            bool sending;
            bool model_active;
        };
        state_type state;
        // ports definition
        using input_ports=std::tuple<typename Queue_defs::qIn, typename Queue_defs::qDone>;
        using output_ports=std::tuple<typename Queue_defs::qOut>;

        // internal transition
        void internal_transition()
        {
            state.model_active = false;
        }

        // external transition
        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
           
            for(const auto &x : get_messages<typename Queue_defs::qIn>(mbs))
            {
                if(x == "IN")
                {
                    if(state.Num < totalNum )
                    {
                  
                        state.Num++;
                        state.model_active = true;
                        state.Material = "IN";
                    }
                    
                    else
                    {
                        state.full = true;
                        state.model_active = false;
                    }
                    
                }
            }
        
            for(const auto &x : get_messages<typename Queue_defs::qDone>(mbs))
            {
                if(x == "DONE")
                {
                    state.Num--;
                }
                
            }
        }

        // confluence transition
        void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
        {
            internal_transition();
            external_transition(TIME(), std::move(mbs));
        }

        // output function
        typename make_message_bags<output_ports>::type output() const
        {
            typename make_message_bags<output_ports>::type bags;
            if (state.model_active)
            {
               
                get_messages<typename Queue_defs::qOut>(bags).push_back(state.Material);
                
            }
            
            return bags;
        }

        // time_advance function
        TIME time_advance() const
       {
          TIME next_internal;
          if (state.model_active)
          {
                next_internal = QueueTime;
          }
          else
          {
                next_internal = numeric_limits<TIME>::infinity();
          }
             return next_internal;
        }

        friend std::ostringstream& operator<<(std::ostringstream& os, const typename Queue<TIME>::state_type& i)
       {
           os << "\ntotalNum: " << i.Num ;
        return os;
        }
};
#endif /* Queue_hpp */
