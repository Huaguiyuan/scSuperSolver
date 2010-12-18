/****************************************************************************
 * Copyright (C) 2010 Timothy Lovorn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef __SCSS_STATE_H
#define __SCSS_STATE_H

#include <cmath>
#include <iostream>


#include "RootFinder.hh"
#include "Environment.hh"

class State {
public:
    // Constructor needs to examine envIn to set member variables.
    State(const Environment& envIn);
    // Drive calculations needed to make this State consistent
    // with the given Environment.  Return false if unable to converge.
    bool makeSelfConsistent();
    // Return true is the errors in all self-consistent equations are within
    // their tolerances, false otherwise.
    bool checkSelfConsistent() const;
    // Return true if the error in the associated S-C equation is within
    // tolerance, false otherwise.
    bool checkD1() const;
    bool checkMu() const;
    bool checkF0() const;
    // Return absolute error in the associated S-C equation.
    double absErrorD1() const;
    double absErrorMu() const;
    double absErrorF0() const;
    // Relative error
    double relErrorD1() const;
    double relErrorMu() const;
    double relErrorF0() const;
    // Simple getters.
    double getD1() const;
    double getMu() const;
    double getF0() const;
    double getEpsilonMin() const;
    // Output what state is now.
    void logState() const;
    // RootFinder needs to be a friend to do its dirty work.
    friend class RootFinder;
    // Our Environment, containing all the configuration info we need.
    const Environment& env;
private:
    // Self-consistent variables.
    double d1, mu, f0;
    // Minimum of Spectrum::epsilonBar() on the BZone.
    // The correct value for this depends on env and d1.
    double epsilonMin;
    // Set epsilonMin to the appropriate value.
    // -- Need to call this after changing D1! --
    double setEpsilonMin();
    // Set the variable to the value which minimizes the error in the
    // associated self-consistent equation. Return value found.
    // Note: d1 and mu equations are coupled, so they must be iterated 
    // together to find a pair of values that satisfies both equations.
    bool fixD1();
    bool fixMu();
    bool fixF0();
    // Functions to be passed into RootFinder
    static double helperD1(double x, void *params);
    static double helperMu(double x, void *params);
    static double helperF0(double x, void *params);
};

// these are #included down here because they refer to State; should have it
// declared before including them
#include "Spectrum.hh"
#include "BZone.hh"

#endif