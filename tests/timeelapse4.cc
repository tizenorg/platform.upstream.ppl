/* Testing time_elapse_assign() with NOT_NECESSARY_CLOSED polyhedra.
   Copyright (C) 2001, 2002 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_install.hh"
#include "print.hh"
#include "ehandlers.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

#define NOISY 0

int
main() {
  set_handlers();

  Variable x(0);
  Variable y(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(x == 3);
  ph1.add_constraint(y > 2);
 
  NNC_Polyhedron ph2(2);
  ph2.add_constraint(x > 3);
  ph2.add_constraint(y > 2);

#if NOISY
  print_constraints(ph1, "**** ph1 ****");
  print_constraints(ph2, "**** ph2 ****");
#endif

  ph1.time_elapse_assign(ph2);

  GenSys gs;
  gs.insert(closure_point(3*x + 2*y));
  gs.insert(point(3*x + 3*y));
  gs.insert(ray(y));
  gs.insert(ray(x));
 
  NNC_Polyhedron known_result(gs);

  int retval = (ph1 == known_result) ? 0 : 1;

#if NOISY
  print_generators(ph1, "**** ph1_time_elapse_assign(ph2) ****");
#endif

  return retval;
}
