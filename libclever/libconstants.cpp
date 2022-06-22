

#include <libconstants.hpp>

// all spatial dimensions defined in cm
// all temporal dimensions defined in ns

const float libConstants::dPMT = 50; // minimum search distance from PMT structure 50 cm
const int libConstants::ncheck = 13; // number of 'checks'

float const libConstants::dlim = 0.1785; // PMT pair maximal distance fraction (dlimit = dlim * circumference). TODO define in geometry as a function of detector size
float const libConstants::tlim = 0.1079; // PMT pair maximal time difference fraction (tlimit = tlim * circumference * cm_to_ns) TODO define in geometry as a function of detector size


const float libConstants::cm_per_ns = 21.8; // group speed of light in cm per ns
float const libConstants::tres = 1.0; // PMT time resolution in ns TODO set this
float const libConstants::tcoinc = 1.0; // PMT coincidence time difference TODO what is this?
int const libConstants::min_selected_hits = 4; // Minimum number of selected hits required for inclusion reduced hit list
float const libConstants::openangle = 90; // Search open angle


