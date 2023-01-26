#include "Layer.h"

namespace sol
{

Layer::Layer(const std::string name, bool enabled)
    : debug_name(name)
    , enabled(enabled)
{
}

} // namespace sol