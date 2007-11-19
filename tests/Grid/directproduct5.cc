/* Test Grid::congruences().
   Copyright (C) 2001-2007 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
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

using namespace IO_Operators;

// #define PH_IS_NNC
#define PH_IS_FIRST

#ifdef PH_IS_NNC
typedef NNC_Polyhedron Poly;
#else
typedef C_Polyhedron Poly;
#endif

#ifdef PH_IS_FIRST
typedef Domain_Product<Poly, Grid>::Direct_Product Product;
#else
typedef Domain_Product<Grid, Poly>::Direct_Product Product;
#endif

namespace {

// add_constraints
bool
test01() {

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B == 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_constraints(cs);

  Product known_dp(2);
  known_dp.add_constraint(A >= 0);
  known_dp.add_constraint(B == 0);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_recycled_constraints
bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B <= 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_recycled_constraints(cs);

  Product known_dp(2);
  known_dp.add_constraint(A + B <= 0);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_constraints_and_minimize
bool
test03() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A + B == 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_constraints_and_minimize(cs);

  Product known_dp(2);
  known_dp.add_constraint(A >= 0);
  known_dp.add_constraint(A + B == 0);

  bool ok = (dp == known_dp);

  return ok;
}

// add_recycled_constraints_and_minimize
bool
test04() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B >= 0);
  cs.insert(A - B == 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_recycled_constraints_and_minimize(cs);

  Product known_dp(2);
  known_dp.add_constraint(B >= 0);
  known_dp.add_constraint(A - B == 0);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_congruences
bool
test05() {

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert((B == 0) / 2);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_congruences(cgs);

  Product known_dp(2);
  known_dp.add_congruence((A %= 0) / 2);
  known_dp.add_congruence((B == 0) / 2);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_congruences_and_minimize
bool
test06() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert(A + B == 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_congruences_and_minimize(cgs);

  Product known_dp(2);
  known_dp.add_congruence((A %= 0) / 2);
  known_dp.add_constraint(A + B == 0);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_recycled_congruences
bool
test07() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_recycled_congruences(cgs);

  Product known_dp(2);
  known_dp.add_congruence((A + B %= 0) / 2);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// add_recycled_congruences_and_minimize
bool
test08() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((B %= 0) / 2);
  cgs.insert(A - B == 0);

  Product dp(2);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  dp.add_recycled_congruences_and_minimize(cgs);

  Product known_dp(2);
  known_dp.add_congruence((B %= 0) / 2);
  known_dp.add_constraint(A - B == 0);

  bool ok = (dp == known_dp);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// relation_with a generator
bool
test09() {
  Variable A(0);
  Variable B(1);

  Generator pnt(point(A + B));

  Product dp(2);

  bool ok = Poly_Gen_Relation::subsumes() == dp.relation_with(pnt);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// relation_with a constraint
bool
test10() {
  Variable A(0);
  Variable B(1);

  Constraint c(A == 2);

  Product dp(2);

  bool ok = Poly_Con_Relation::nothing() == dp.relation_with(c);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// Empty product; relation_with a constraint.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A == 1);
  dp.add_congruence((A %= 2) / 0);

  bool ok = (dp.relation_with(B == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates())
	     && dp.relation_with(B >= 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product in 3D; relation_with a constraint.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(3*A + 3*B + C == 7);
  dp.add_constraint(3*A - C >= 2);
  dp.add_congruence(6*A + 3*B %= 0);

#ifdef PH_IS_FIRST
  bool okdp1 = (dp.domain2().relation_with(2*A + B >= 3)
	     == Poly_Con_Relation::strictly_intersects());

  bool okdp2 = (dp.domain1().relation_with(2*A + B >= 3)
	       == Poly_Con_Relation::is_included());
#else
  bool okdp1 = (dp.domain1().relation_with(2*A + B >= 3)
	     == Poly_Con_Relation::strictly_intersects());

  bool okdp2 = (dp.domain2().relation_with(2*A + B >= 3)
	       == Poly_Con_Relation::is_included());
#endif

  bool ok = (okdp1 && okdp2
             && dp.relation_with(A + B + C == 0)
	     == Poly_Con_Relation::nothing()
	     && dp.relation_with(A + B == 0)
	     == Poly_Con_Relation::nothing()
	     && dp.relation_with(A == 0)
	     == Poly_Con_Relation::nothing()
	     && dp.relation_with(Linear_Expression(0) == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates())
	     && dp.relation_with(2*A + B >= 3)
	     == Poly_Con_Relation::is_included()
	     && dp.relation_with(3*A + 3*B + C >= 7)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates()));

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product where the components strictly intersect the constraint.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(C == 0);
  dp.add_congruence(6*A + 3*B %= 0);

  bool okdp1 = (dp.domain1().relation_with(2*A + B >= 3)
		== Poly_Con_Relation::strictly_intersects());

  bool okdp2 = (dp.domain2().relation_with(2*A + B >= 3)
	       == Poly_Con_Relation::strictly_intersects());

  bool ok = (okdp1 && okdp2
	     && dp.relation_with(2*A + B >= 3)
	     == Poly_Con_Relation::nothing());

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// relation_with a congruence
bool
test14() {
  Variable A(0);
  Variable B(1);

  Congruence cg(A %= 2);

  Product dp(2);

  Poly_Con_Relation rel = dp.relation_with(cg);
  bool ok = Poly_Con_Relation::nothing() == rel;

  nout << "dp.relation_with(A %= 2) == " << rel << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// Empty product; relation_with a congruence.
bool
test15() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A == 1);
  dp.add_congruence((A %= 2) / 0);

  Poly_Con_Relation rel = dp.relation_with(B %= 0);
  bool ok = (rel == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  nout << "dp.relation_with(B %= 0) == " << rel << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product in 3D; relation_with a congruence.
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(3*A + 3*B + C == 7);
  dp.add_constraint(3*A - C >= 2);
  dp.add_congruence((2*A + B %= 0) / 2);

  Poly_Con_Relation rel1 = dp.domain1().relation_with(2*A + B %= 3);
  Poly_Con_Relation rel2 = dp.domain2().relation_with(2*A + B %= 3);
#ifdef PH_IS_FIRST
  bool okdp1 = (rel1 == Poly_Con_Relation::strictly_intersects());
  bool okdp2 = (rel2 == Poly_Con_Relation::is_included());
#else
  bool okdp1 = (rel2 == Poly_Con_Relation::strictly_intersects());
  bool okdp2 = (rel1 == Poly_Con_Relation::is_included());
#endif

  if (!okdp2 || !okdp1) {
    nout << "dp.domain1().relation_with(2*A + B %= 3) == " << rel1 << endl;
    nout << "dp.domain2().relation_with(2*A + B %= 3) == " << rel2 << endl;
    print_constraints(dp, "*** dp constraints ***");
    print_congruences(dp, "*** dp congruences ***");
    return false;
  }

  Poly_Con_Relation rel3 = dp.relation_with(A + B + C %= 0);
  Poly_Con_Relation rel4 = dp.relation_with(A + B %= 0);
  Poly_Con_Relation rel5 = dp.relation_with(A %= 0);
  Poly_Con_Relation rel6 = dp.relation_with(Linear_Expression(0) %= 0);
  Poly_Con_Relation rel7 = dp.relation_with(2*A + B %= 3);
  Poly_Con_Relation rel8 = dp.relation_with(3*A + 3*B + C %= 7);
  bool ok = (rel3 == Poly_Con_Relation::nothing()
	     && rel4 == Poly_Con_Relation::nothing()
	     && rel5 == Poly_Con_Relation::nothing()
	     && rel6 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates())
	     && rel7 == Poly_Con_Relation::is_included()
	     && rel8 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates()));

  nout << "dp.relation_with(A + B + C %= 0) == " << rel3 << endl;
  nout << "dp.relation_with(A + B %= 0) == " << rel4 << endl;
  nout << "dp.relation_with(A %= 0) == " << rel5 << endl;
  nout << "dp.relation_with(Linear_Expression(0) %= 0) == " << rel6 << endl;
  nout << "dp.relation_with(2*A + B %= 3) == " << rel7 << endl;
  nout << "dp.relation_with(3*A + 3*B + C %= 7) == " << rel8 << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product where the components strictly intersect the congruence.
bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(C == 0);
  dp.add_congruence(6*A + 3*B %= 0);

  bool okdp1 = (dp.domain1().relation_with(2*A + B %= 3)
		== Poly_Con_Relation::strictly_intersects());

  bool okdp2 = (dp.domain2().relation_with(2*A + B %= 3)
	       == Poly_Con_Relation::strictly_intersects());

  Poly_Con_Relation rel = dp.relation_with(2*A + B %= 3);
  bool ok = (okdp1 && okdp2
	     && rel == Poly_Con_Relation::nothing());

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");
  nout << "dp.relation_with(2*A + B %= 3) == " << rel << endl;

  return ok;
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
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
END_MAIN
