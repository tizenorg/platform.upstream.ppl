/* Test BD_Shape::limited_BHMZ05_extrapolation_assign().
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
  TBD_Shape bd1;
  TBD_Shape known_result;

    print_constraints(bd1, "*** bd1 ****");

  TBD_Shape bd2;

  print_constraints(bd2, "*** bd2 ****");

  Constraint_System cs;

  print_constraints(cs, "*** cs ****");

  bd1.limited_BHMZ05_extrapolation_assign(bd2, cs);

  print_constraints(bd1,
		    "*** bd1.limited_BHMZ05_extrapolation_assign(bd2) ***");

  return (bd1 == known_result) ? 0 : 1;
}
CATCH
