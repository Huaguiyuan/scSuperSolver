/****************************************************************************
 * Copyright (C) 2010 Tim Lovorn
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

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

#include "RootFinder.hh"

RootData::RootData(bool cvg, double rt, double fnv) :
    converged(cvg), root(rt), fnvalue(fnv) 
{ }

BracketData::BracketData(bool _success, double _left, double _right,
                         double _fnleft, double _fnright) :
    success(_success), left(_left), right(_right), 
    fnleft(_fnleft), fnright(_fnright)
{ }

RootFinder::RootFinder(double (* const helper)(double, void *), 
    void * const params, const double guess, const double min, 
    const double max, const double tolerance) :
    myHelper(helper), myParams(params), myGuess(guess), myMin(min), myMax(max), 
    myTolerance(tolerance) 
{ }

// this is so broken
// need to make sure it doesn't overshoot bounds
const BracketData& RootFinder::bracket() {
    int iteration = 1;
    double step, left, right, fnleft, fnright;
    BracketData *bdata;
    step = (myMax - myMin) / (2 * RF_BRACKET_STEPS);
    do {
        // since values aren't saved this does 2x as many evaluations as
        // it needs to
        right = myGuess + iteration * step;
        left = myGuess + (iteration - 1) * step;
        if (right > myMax) right = myMax;
        if (left > myMax) left = myMax - step;
        fnright = myHelper(right, myParams);
        fnleft = myHelper(left, myParams);
        if ((fnleft >= 0 && fnright <= 0) || (fnright >= 0 && fnleft <= 0)) {
            bdata = new BracketData(true, left, right, fnleft, fnright);
            return (const BracketData&)(*bdata);
        }
        right = myGuess - (iteration - 1) * step;
        left = myGuess - iteration * step;
        if (left < myMin) left = myMin;
        if (right < myMin) right = myMin + step;
        fnright = myHelper(right, myParams);
        fnleft = myHelper(left, myParams);
        if ((fnleft >= 0 && fnright <= 0) || (fnright >= 0 && fnleft <= 0)) {
            bdata = new BracketData(true, left, right, fnleft, fnright);
            return (const BracketData&)(*bdata);
        }
        iteration++;
    } while (iteration <= RF_BRACKET_STEPS);
    bdata = new BracketData(false, myMin, myMax, 0.0, 0.0);
    return (const BracketData&)(*bdata);
}

// Lots of code cribbed from GSL documentation
// http://www.gnu.org/software/gsl/manual/html_node/Root-Finding-Examples.html

const RootData& RootFinder::findRoot() {
    const BracketData& bdata = bracket();
    if (bdata.success == false) {
        const RootData *rdata = new RootData(false, myGuess,
                                             myHelper(myGuess, myParams));
        return (const RootData&)(*rdata);
    }
    int status;
    int iter = 0, max_iter = RF_MAX_ITER;
    bool converged = true;
    const gsl_root_fsolver_type *T;
    gsl_root_fsolver *s;
    double r = (bdata.left + bdata.right) / 2;
    double x_lo = bdata.left, x_hi = bdata.right;
    gsl_function F;
    
    F.function = myHelper;
    F.params = myParams;

    T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc(T);
    gsl_root_fsolver_set(s, &F, x_lo, x_hi);

    do {
        iter++;
        status = gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(x_lo, x_hi, myTolerance, 0.0);
    } while (status == GSL_CONTINUE && iter < max_iter);

    gsl_root_fsolver_free(s);    
    
    if (iter >= max_iter || status != GSL_SUCCESS) {
        converged = false;
    }

    const RootData *rdata = new RootData(converged, r, myHelper(r, myParams));
    return (const RootData&)(*rdata);
}
