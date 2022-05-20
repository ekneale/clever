

#include <libconstants.hpp>

// all spatial dimensions defined in cm
// all temporal dimensions defined in ns

const float libConstants::ns_to_cm = 21.8; // group speed of light in ns
const float libConstants::dPMT = 50; // minimum search distance from PMT structure 50 cm
const int libConstants::ncheck = 13; // number of 'checks'

// TODO dlim and tlim assume even spacing of PMTs. Should this be allocated for each PMT so that it works with all PMT configurations including uneven????
// TODO move calculation of dlimit and tlimit to libgeometry or put it in an optimisation step
float const libConstants::dlim = 0.1785; // PMT pair maximal distance fraction (dlimit = dlim * circumference). TODO Needs to be the maximum distance between PMTs so should find the number of PMTs in a given row and divide circumference by that.
float const libConstants::tlim = 0.1079; // PMT pair maximal time difference fraction (tlimit = tlim * circumference * cm_to_ns) TODO Needs to be the maximum time between PMTs so should find the number of PMTs in a given row and divide circumference by that.
float const libConstants::tres = 1.0; // PMT time resolution in ns TODO set this
float const libConstants::tcoinc = 1.0; // PMT coincidence time difference TODO what is this?

float const libConstants::openangle = 90; // Search open angle
