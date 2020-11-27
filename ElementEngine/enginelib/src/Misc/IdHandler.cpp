#include <element/IdHandler.h>

#include <random>
#include <limits>

uint32_t Element::IdHandler::idNumber = -1;

uint32_t Element::IdHandler::GetID()
{
    
    //std::random_device rd;
    //std::uniform_int_distribution<uint32_t> dist(std::numeric_limits<uint32_t>().min(), std::numeric_limits<uint32_t>().max());
    //uint32_t val;
    //do{
    //    val = dist(rd);
    //} while ();
    //currentIds.emplace_back(val);
    return ++idNumber;
}
