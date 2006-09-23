/* Test Octagonal_Shape::contains_integer_point().
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

namespace {

bool
test01() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  Octagonal_Shape<mpz_class> oct(2);
  oct.add_constraints(cs);

  print_constraints(oct, "*** oct ***");

  bool contains = oct.contains_integer_point();

  nout << "oct.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  Octagonal_Shape<mpq_class> oct(2);
  oct.add_constraints(cs);

  print_constraints(oct, "*** oct ***");

  bool contains = oct.contains_integer_point();

  nout << "oct.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  Octagonal_Shape<float> oct(3);
  oct.add_constraints(cs);

  print_constraints(oct, "*** oct ***");

  bool contains = oct.contains_integer_point();

  nout << "oct.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y - 3*z <= 2);
  cs.insert(8*z - 8*y >= 7);

  Octagonal_Shape<mpq_class> oct(3);
  oct.add_constraints(cs);

  print_constraints(oct, "*** oct ***");

  bool contains = oct.contains_integer_point();

  nout << "oct.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
