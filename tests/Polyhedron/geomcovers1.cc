/* Test Polyhedra_Powerset<PH>::geometrically_covers().
   Copyright (C) 2001-2006 Roberto Bagnara <bagnara@cs.unipr.it>

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
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include "ppl_test.hh"

using namespace Parma_Polyhedra_Library::IO_Operators;

int
main(void) {
  Variable x(0);

  Constraint_System cs1, cs2, cs3, cs4;
  cs1.insert(x >= 0);
  cs1.insert(x <= 4);
  cs2.insert(x >= 4);
  cs2.insert(x <= 6);
  cs3.insert(x >= 1);
  cs3.insert(x <= 5);
  C_Polyhedron ph1(cs1), ph2(cs2), ph3(cs3);

  Polyhedra_Powerset<C_Polyhedron> ps12(1, EMPTY);
  ps12.add_disjunct(ph1);
  ps12.add_disjunct(ph2);

  Polyhedra_Powerset<C_Polyhedron> ps3(1, EMPTY);
  ps3.add_disjunct(ph3);

  nout << "ps12 = " << ps12 << endl
       << " ps3 = " << ps3 << endl;

  if (ps12.geometrically_covers(ps3)) {

    nout << "ps12 covers ps3." << endl;

    return 0;
  }
  else {

    nout << "ps12 does not cover ps3." << endl;

    return 0;
  }
}
