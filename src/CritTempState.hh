/*
  Copyright (c) 2011 Timothy Lovorn

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef __SCSS_CRIT_TEMP_STATE_H
#define __SCSS_CRIT_TEMP_STATE_H

#define BC_MAX_ITERS 100

#include "BaseState.hh"
#include "CritTempEnvironment.hh"
#include "RootFinder.hh"

class CritTempState : public BaseState {
public:
    // Constructor needs to examine envIn to set member variables.
    CritTempState(const CritTempEnvironment& envIn);
    // Drive calculations needed to make this State consistent
    // with the given Environment.  Return false if unable to converge.
    bool makeSelfConsistent();
    // Return true is the errors in all self-consistent equations are within
    // their tolerances, false otherwise.
    bool checkSelfConsistent() const;
    // Return true if the error in the associated S-C equation is within
    // tolerance, false otherwise.
    bool checkBc() const;
    // Return absolute error in the associated S-C equation.
    double absErrorD1() const;
    double absErrorMu() const;
    double absErrorBc() const;
    // Relative error
    double relErrorD1() const;
    double relErrorMu() const;
    double relErrorBc() const;
    // Simple getters.
    double getBc() const;
    // BZone call required to calculate these.
    double getX1() const;
    double getX2() const; // x2 = x - x1
    // Need to know accuracy of omega_k approximation.
    void logOmegaAccuracy() const;
    // Output what state is now.
    void logState() const;
     // RootFinder needs to be a friend to do its dirty work.
    friend class RootFinder;
    // Our Environment, containing all the configuration info we need.
    const CritTempEnvironment& env;
protected:
    // Self-consistent variables.
    double bc;
    // Set epsilonMin to the appropriate value.
    // -- Need to call this after changing D1! --
    double setEpsilonMin();
    // Set the variable to the value which minimizes the error in the
    // associated self-consistent equation. Return value found.
    // Note: d1 and mu equations are coupled, so they must be iterated 
    // together to find a pair of values that satisfies both equations.
    bool fixD1();
    bool fixMu();
    bool fixBc();
    // Functions to be passed into RootFinder
    static double helperD1(double x, void *params);
    static double helperMu(double x, void *params);
};

// these are #included down here because they refer to State; should have it
// declared before including them
#include "CritTempSpectrum.hh"
#include "BZone.hh" 

#endif
