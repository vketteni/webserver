
#include "CGIHandler.hpp"

// Default constructor
CGIHandler::CGIHandler() { return; }

// Copy constructor
CGIHandler::CGIHandler(const CGIHandler &other) {
  *this = other;
  return;
}

// Copy assignment overload
CGIHandler &CGIHandler::operator=(const CGIHandler &rhs) {
  (void)rhs;
  return *this;
}

// Default destructor
CGIHandler::~CGIHandler() { return; }


