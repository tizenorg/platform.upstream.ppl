/* Test linear_partition() with two telescopic polyhedra.
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

namespace {

bool
partition_ok(const C_Polyhedron& p,
	     const C_Polyhedron& q,
	     const std::pair<C_Polyhedron,
	     Powerset<Determinate<NNC_Polyhedron> > >& partition) {
  const C_Polyhedron& r = partition.first;
  // `r' must be a subset of or equal to `q'.
  if (!q.contains(r))
    return false;
  const Powerset<Determinate<NNC_Polyhedron> >& s = partition.second;
  NNC_Polyhedron the_union(r);
  // These are the NNC versions of `p' and `q'.
  NNC_Polyhedron nnc_p(p);
  NNC_Polyhedron nnc_q(q);
  typedef Powerset<Determinate<NNC_Polyhedron> >::const_iterator iter;
  for (iter i = s.begin(), s_end = s.end(); i != s_end; ++i) {
    const NNC_Polyhedron& a = i->element();
    // All elements of `s' must be disjoint from `p'.
    if (!a.is_disjoint_from(nnc_p))
      return false;
    iter j = i;
    for (++j; j != s_end; ++j) {
      const NNC_Polyhedron& b = j->element();
      // All elements of `s' must be pairwise disjoint.
      if (!a.is_disjoint_from(b))
	return false;
    }
    the_union.poly_hull_assign(a);
  }
  // The union of all the elements in `partition' must be exactly `q'.
  return the_union == nnc_q;
}

} // namespace

int main() TRY {
  set_handlers();

  Variable x(0);
  Variable y(1);

  C_Polyhedron p(2);
  p.add_constraint(x >= -2);
  p.add_constraint(x <=  2);
  p.add_constraint(y >=  0);
  p.add_constraint(y <=  4);

  nout << "p = " << p << endl;

  C_Polyhedron q(2);
  q.add_constraint(x >= -1);
  q.add_constraint(x <=  1);
  q.add_constraint(y >=  1);
  q.add_constraint(y <=  3);

  nout << "q = " << q << endl;

  std::pair<C_Polyhedron, Powerset<Determinate<NNC_Polyhedron> > >
    result = linear_partition(p, q);

  nout << "*** q partition ***" << endl;
  nout << "  === p inters q === " << endl << "  " << result.first << endl;
  nout << "  ===    rest    === " << endl << "  " << result.second << endl;

  if (!partition_ok(p, q, result))
    return 1;

  result = linear_partition(q, p);

  nout << "*** p partition ***" << endl;
  nout << "  === q inters p === " << endl << "  " << result.first << endl;
  nout << "  ===    rest    === " << endl << "  " << result.second << endl;

  if (!partition_ok(q, p, result))
    return 1;

  return 0;
}
CATCH
