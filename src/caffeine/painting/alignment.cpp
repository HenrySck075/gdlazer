#include "gdlazer/caffeine/painting/alignment.hpp"

namespace caffeine {

// Define static const Alignment instances
const Alignment Alignment::topLeft{-1.0f, -1.0f};
const Alignment Alignment::topCenter{0.0f, -1.0f};
const Alignment Alignment::topRight{1.0f, -1.0f};

const Alignment Alignment::centerLeft{-1.0f, 0.0f};
const Alignment Alignment::center{0.0f, 0.0f};
const Alignment Alignment::centerRight{1.0f, 0.0f};

const Alignment Alignment::bottomLeft{-1.0f, 1.0f};
const Alignment Alignment::bottomCenter{0.0f, 1.0f};
const Alignment Alignment::bottomRight{1.0f, 1.0f};

}  // namespace caffeine
