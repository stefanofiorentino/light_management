#pragma once

#include <iostream>

struct light_bulb_t final 
{
    light_bulb_t() : m_status{false}{}
    void draw(std::ostream &oss) const
    {
        oss << "<light_bulb_t>"<< std::boolalpha << m_status << "</light_bulb_t>\n";
    }
    void do_switch(bool status)
    {
        m_status = status;
    }
private:
    bool m_status;
};
