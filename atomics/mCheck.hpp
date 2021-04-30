

#ifndef mCheck_hpp
#define mCheck_hpp


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
struct mCheck_defs
{
    struct materials : public out_port<int> {};
    struct others : public out_port<int> {};
    struct m_in : public in_port<int> {};
};

template<typename TIME> class mCheck
{
    public:
    // ports definition
    TIME   checkTime;
    using input_ports=tuple<typename mCheck_defs::m_in>;
    using output_ports=tuple<typename mCheck_defs::materials,typename mCheck_defs::others>;
    // state definition
    struct state_type
    {
        bool check_m_out;
        bool model_state;
        int materils;
        int others;
    };
    state_type state;
    // default constructor
    mCheck() noexcept
    {
        state.check_m_out = false;
        state.model_state= false;
        state.materils        = 0;
        state.others       = 0;
        checkTime=TIME("00:00:10:00");
    }
    // internal transition
    void internal_transition()
    {
        
        state.check_m_out = false;
        
    }
    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
        
        for(const auto &x : get_messages<typename mCheck_defs::m_in>(mbs))
        {
            if ((double)rand() / (double) RAND_MAX  < 0.95){
                state.materils=x;
                state.others=0;
                state.model_state=true;
                state.check_m_out = true;
            }
            else
            {
                state.others =x;
                state.materils= 0;
                state.model_state=true;
                state.check_m_out = false;
            }
        }
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs)
    {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const
    {
        typename make_message_bags<output_ports>::type bags;
        
        if (state.model_state)
            {
               if(state.check_m_out)
               {
                 get_messages<typename mCheck_defs::materials>(bags).push_back(state.materils);
               }
                else
                {
                    get_messages<typename mCheck_defs::others>(bags).push_back(state.others);
                }
           
            }
        return bags;
    }
    // time_advance function
    TIME time_advance() const
    {
        TIME next_internal;
        if (state.model_state)
        {
            next_internal = checkTime;
        }
        else
        {
            next_internal = numeric_limits<TIME>::infinity();
        }
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename mCheck<TIME>::state_type& i)
    {
        os << "\nMaterials: " << i.materils <<"\tOthers: " << i.others << " & check_materils_out: " << i.check_m_out;
        return os;
    }
};


#endif /* mCheck_hpp */
