/* Test the construction of a polyhedron built from a system of
   generators that has a point with a negative denominator.
   Copyright (C) 2001-2004 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include "ppl_test.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

#ifndef NOISY
#define NOISY 0
#endif

int
main() TRY {
  Variable x(0);
  Variable y(1);

  GenSys gs;
  gs.insert(point(x + y, -1));

  C_Polyhedron ph(gs);

#if NOISY
  print_generators(ph, "*** ph ***");
#endif

  Constraint_System cs = ph.constraints();

  C_Polyhedron known_result(2);
  known_result.add_constraint(x == -1);
  known_result.add_constraint(y == -1);

  int retval = (ph == known_result && ph.OK()) ? 0 : 1;

#if NOISY
  print_constraints(ph, "*** ph constraints ***");
  print_generators(ph, "*** ph generators ***");
#endif

  return retval;
}
CATCH
