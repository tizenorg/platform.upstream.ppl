/* Test Polyhedron::remove_space_dimensions(): the set of the variables
   that we want to remove is empty.
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

int
main() TRY {
  set_handlers();

  Variable A(0);
  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  C_Polyhedron ph(gs);
  ph.add_constraint(A >= 2);

  print_constraints(ph, "*** ph ***");

  C_Polyhedron known_result(ph);

  // This is the set of the variables that we want to remove.
  Variables_Set to_be_removed;

  ph.remove_space_dimensions(to_be_removed);

  int retval = (ph == known_result) ? 0 : 1;

  print_constraints(ph,
		    "*** After ph.remove_space_dimensions(to_be_removed) ***");

  return retval;
}
CATCH
