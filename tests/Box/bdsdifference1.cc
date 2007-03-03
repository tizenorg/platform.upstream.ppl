/* Test Box::box_difference_assign().
   Copyright (C) 2001-2007 Roberto Bagnara <bagnara@cs.unipr.it>

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
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A >= 0);
  box1.add_constraint(A <= -2);
  box1.add_constraint(B == 0);

  TBox box2(2);
  box2.add_constraint(A >= 0);
  box2.add_constraint(A <= 2);
  box2.add_constraint(B >= 0);
  box2.add_constraint(B <= 2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** ph2 ***");

  box1.box_difference_assign(box2);

  Rational_Box known_result(2, EMPTY);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(ph2) ***");

  return ok;
}

bool
test02() {
  TBox box1;
  TBox box2;

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.box_difference_assign(box2);

  Constraint_System cs;
  cs.insert(Linear_Expression(-4) >= 0);
  Rational_Box known_result(cs);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 2);
  box1.add_constraint(x >= 0);
  box1.add_constraint(y <= 5);
  box1.add_constraint(y >= 2);

  TBox box2(2);
  box2.add_constraint(x <= 3);
  box2.add_constraint(x >= 1);
  box2.add_constraint(y <= 4);
  box2.add_constraint(y >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.box_difference_assign(box2);

  Rational_Box known_result(2);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y <= 5);
  known_result.add_constraint(y >= 2);
  known_result.add_constraint(y - x >= 1);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 8);
  box1.add_constraint(x >= 0);
  box1.add_constraint(y <= 7);
  box1.add_constraint(y >= 2);

  TBox box2(2);
  box2.add_constraint(x <= 3);
  box2.add_constraint(x >= 1);
  box2.add_constraint(y <= 0);
  box2.add_constraint(y >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(box1);

  box1.box_difference_assign(box2);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBox box1(2);
  box1.add_constraint(x <= 8);
  box1.add_constraint(x >= 0);
  box1.add_constraint(y <= 7);
  box1.add_constraint(y >= 2);

  TBox box2(2);
  box2.add_constraint(x <= 9);
  box2.add_constraint(x >= 0);
  box2.add_constraint(y <= 8);
  box2.add_constraint(y >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.box_difference_assign(box2);

  Rational_Box known_result(2, EMPTY);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(3);
  box1.add_constraint(x <= 8);
  box1.add_constraint(y <= 7);
  box1.add_constraint(y >= 1);
  box1.add_constraint(z <= 2);

  TBox box2(3);
  box2.add_constraint(x == 8);
  box2.add_constraint(y <= 2);
  box2.add_constraint(y >= 1);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  box1.box_difference_assign(box2);

  Rational_Box known_result(3);
  known_result.add_constraint(x <= 8);
  known_result.add_constraint(y <= 7);
  known_result.add_constraint(y >= 1);
  known_result.add_constraint(z <= 2);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBox box1(2);
  box1.add_constraint(A >= 0);
  box1.add_constraint(A <= 4);
  box1.add_constraint(B >= 0);
  box1.add_constraint(B <= 2);

  TBox box2(2);
  box2.add_constraint(A >= 2);
  box2.add_constraint(A <= 4);
  box2.add_constraint(B >= 0);
  box2.add_constraint(B <= 2);

  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  Rational_Box known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 2);

  box1.box_difference_assign(box2);

  bool ok = (Rational_Box(box1) == known_result);

  print_constraints(box1, "*** after box1.box_difference_assign(box2) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

bool
test08() {
  TBox box1(3);
  TBox box2(5);

  try {
    // This is an incorrect use of method
    // Box::box_difference_assign(box2): it is impossible to apply
    // this method to two polyhedra of different dimensions.
    box1.box_difference_assign(box2);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
END_MAIN
