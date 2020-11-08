#include <element/Utilities.h>


const std::string Element::Utilities::extractName(const std::string& _name)
{
    std::string name = "";

    for (const auto& c : _name)
    {
        if (c == '/')
        {
            name.clear();
            continue;
        }
        else if (c == '.')
        {
            break;
        }
        name += c;
    }

    return name;
}