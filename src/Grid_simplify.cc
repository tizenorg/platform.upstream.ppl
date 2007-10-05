/* Grid class implementation: simplify().
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

#include <ppl-config.h>
#include <cassert>

#include "Grid.defs.hh"

namespace Parma_Polyhedra_Library {

#ifdef STRONG_REDUCTION
template <typename M, typename R>
void
Grid::reduce_reduced(M& sys,
		     const dimension_type dim,
		     const dimension_type pivot_index,
		     const dimension_type start,
		     const dimension_type end,
		     const Dimension_Kinds& dim_kinds,
		     const bool generators) {
  R& pivot = sys[pivot_index];

  const Coefficient& pivot_dim = pivot[dim];

  if (pivot_dim == 0)
    return;

  TEMP_INTEGER(pivot_dim_half);
  pivot_dim_half = (pivot_dim + 1) / 2;
  Dimension_Kind row_kind = dim_kinds[dim];
  Dimension_Kind line_or_equality, virtual_kind;
  int jump;
  if (generators) {
    line_or_equality = LINE;
    virtual_kind = GEN_VIRTUAL;
    jump = -1;
  }
  else {
    line_or_equality = EQUALITY;
    virtual_kind = CON_VIRTUAL;
    jump = 1;
  }

  TEMP_INTEGER(num_rows_to_subtract);
  TEMP_INTEGER(row_dim_remainder);
  for (dimension_type row_index = pivot_index, kinds_index = dim + jump;
       row_index-- > 0;
       kinds_index += jump) {
    // Move over any virtual rows.
    while (dim_kinds[kinds_index] == virtual_kind)
      kinds_index += jump;

    // row_kind CONGRUENCE is included as PARAMETER
    if (row_kind == line_or_equality
	|| (row_kind == PARAMETER
	    && dim_kinds[kinds_index] == PARAMETER)) {
      R& row = sys[row_index];

      const Coefficient& row_dim = row[dim];
      // num_rows_to_subtract may be positive or negative.
      num_rows_to_subtract = row_dim / pivot_dim;

      // Ensure that after subtracting num_rows_to_subtract * r_dim
      // from row_dim, -pivot_dim_half < row_dim <= pivot_dim_half.
      // E.g., if pivot[dim] = 9, then after strong reduction
      // -5 < row_dim <= 5.
      row_dim_remainder = row_dim % pivot_dim;
      if (row_dim_remainder < 0) {
	if (row_dim_remainder <= -pivot_dim_half)
	  --num_rows_to_subtract;
      }
      else if (row_dim_remainder > 0 && row_dim_remainder > pivot_dim_half)
	++num_rows_to_subtract;

      // Subtract num_rows_to_subtract copies of pivot from row i.  Only the
      // entries from dim need to be subtracted, as the preceding
      // entries are all zero.
      // If num_rows_to_subtract is negative, these copies of pivot are
      // added to row i.
      if (num_rows_to_subtract != 0)
	for (dimension_type col = start; col <= end; ++col)
	  sub_mul_assign(row[col], num_rows_to_subtract, pivot[col]);
    }
  }
}
#endif // STRONG_REDUCTION

void
Grid::reduce_line_with_line(Grid_Generator& row, Grid_Generator& pivot,
			    dimension_type column) {
  const Coefficient& pivot_column = pivot[column];
  Coefficient& row_column = row[column];
  TEMP_INTEGER(reduced_row_col);
  // Use reduced_row_col temporarily to hold the gcd.
  gcd_assign(reduced_row_col, pivot_column, row_column);
  // Store the reduced ratio between pivot[column] and row[column].
  TEMP_INTEGER(reduced_pivot_col);
  exact_div_assign(reduced_pivot_col, pivot_column, reduced_row_col);
  exact_div_assign(reduced_row_col, row_column, reduced_row_col);
  // Multiply row, then subtract from it a multiple of pivot such that
  // the result in row[column] is zero.
  row_column = 0;
  // pivot.size() - 1 is the index for the parameter divisor so we
  // start reducing the line at index pivot.size() - 2.
  for (dimension_type col = pivot.size() - 2;
       col > column;
       --col) {
    Coefficient& row_col = row[col];
    row_col *= reduced_pivot_col;
    sub_mul_assign(row_col, reduced_row_col, pivot[col]);
  }
}

void
Grid::reduce_equality_with_equality(Congruence& row,
				    const Congruence& pivot,
				    const dimension_type column) {
  // Assume two equalities.
  assert(row.modulus() == 0 && pivot.modulus() == 0);

  const Coefficient& pivot_column = pivot[column];
  Coefficient& row_column = row[column];
  TEMP_INTEGER(reduced_row_col);
  // Use reduced_row_col temporarily to hold the gcd.
  gcd_assign(reduced_row_col, pivot_column, row_column);
  // Store the reduced ratio between pivot[column] and row[column].
  TEMP_INTEGER(reduced_pivot_col);
  exact_div_assign(reduced_pivot_col, pivot_column, reduced_row_col);
  exact_div_assign(reduced_row_col, row_column, reduced_row_col);
  // Multiply row, then subtract from it a multiple of pivot such that
  // the result in row[column] is zero.
  row_column = 0;
  for (dimension_type col = column; col-- > 0; ) {
    Coefficient& row_col = row[col];
    row_col *= reduced_pivot_col;
    sub_mul_assign(row_col, reduced_row_col, pivot[col]);
  }
}

template <typename R>
void
Grid::reduce_pc_with_pc(R& row, R& pivot,
			const dimension_type column,
			const dimension_type start,
			const dimension_type end) {
  Coefficient& pivot_column = pivot[column];
  Coefficient& row_column = row[column];

  TEMP_INTEGER(s);
  TEMP_INTEGER(t);
  TEMP_INTEGER(reduced_row_col);
  // Use reduced_row_col temporarily to hold the gcd.
  gcdext_assign(reduced_row_col, s, t, pivot_column, row_column);
  // Now pivot[column] * s + row[column] * t == gcd.

  // Store the reduced ratio between pivot[column] and row[column].
  TEMP_INTEGER(reduced_pivot_col);
  exact_div_assign(reduced_pivot_col, pivot_column, reduced_row_col);
  pivot_column = reduced_row_col /* gcd */;
  exact_div_assign(reduced_row_col, row_column, reduced_row_col);

  // Multiply row, then subtract from it a multiple of pivot such that
  // the result in row[column] is zero.  Afterward, multiply pivot,
  // then add to it a (possibly negative) multiple of row such that
  // the result in pivot[column] is the smallest possible positive
  // integer.
  assert(pivot.size() > 0);
  assert(row.size() > 0);
  row_column = 0;
  TEMP_INTEGER(old_pivot_col);
  for (dimension_type col = start; col < end; ++col) {
    Coefficient& pivot_col = pivot[col];
    old_pivot_col = pivot_col;
    pivot_col *= s;
    Coefficient& row_col = row[col];
    add_mul_assign(pivot_col, t, row_col);
    row_col *= reduced_pivot_col;
    sub_mul_assign(row_col, reduced_row_col, old_pivot_col);
  }
}

void
Grid::reduce_parameter_with_line(Grid_Generator& row,
				 const Grid_Generator& pivot,
				 const dimension_type column,
				 Grid_Generator_System& sys) {
  // Very similar to reduce_congruence_with_equality below.  Any
  // change here may be needed there too.

  const Coefficient& pivot_column = pivot[column];
  Coefficient& row_column = row[column];

  // Subtract one to allow for the parameter divisor column
  const dimension_type num_columns = sys.num_columns() - 1;

  // If the elements at column in row and pivot are the same, then
  // just subtract pivot from row.
  if (row_column == pivot_column) {
    for (dimension_type col = num_columns; col-- > 0; )
      row[col] -= pivot[col];
    return;
  }

  TEMP_INTEGER(reduced_row_col);
  // Use reduced_row_col temporarily to hold the gcd.
  gcd_assign(reduced_row_col, pivot_column, row_column);
  // Store the reduced ratio between pivot[column] and row[column].
  TEMP_INTEGER(reduced_pivot_col);
  exact_div_assign(reduced_pivot_col, pivot_column, reduced_row_col);
  exact_div_assign(reduced_row_col, row_column, reduced_row_col);

  // Multiply row such that a multiple of pivot can be subtracted from
  // it below to render row[column] zero.  This requires multiplying
  // all other parameters to match.
#ifdef STRONG_REDUCTION
  // Ensure that the multiplier is positive, so that the preceding
  // diagonals (including the divisor) remain positive.  It's safe to
  // swap the signs as row[column] will still come out 0.
  if (reduced_pivot_col < 0) {
    neg_assign(reduced_pivot_col);
    neg_assign(reduced_row_col);
  }
#endif
  for (dimension_type index = sys.num_rows(); index-- > 0; ) {
    Grid_Generator& gen = sys[index];
    if (gen.is_parameter_or_point())
      for (dimension_type col = num_columns; col-- > 0; )
        gen[col] *= reduced_pivot_col;
  }
  // Subtract from row a multiple of pivot such that the result in
  // row[column] is zero.
  row_column = 0;
  for (dimension_type col = num_columns - 1; col > column; --col)
    sub_mul_assign(row[col], reduced_row_col, pivot[col]);
}

void
Grid::reduce_congruence_with_equality(Congruence& row,
				      const Congruence& pivot,
				      const dimension_type column,
				      Congruence_System& sys) {
  // Very similar to reduce_parameter_with_line above.  Any change
  // here may be needed there too.
  assert(row.modulus() > 0 && pivot.modulus() == 0);

  const Coefficient& pivot_column = pivot[column];
  Coefficient& row_column = row[column];

  dimension_type num_columns = sys.num_columns();

  // If the elements at `column' in row and pivot are the same, then
  // just subtract `pivot' from `row'.
  if (row_column == pivot_column) {
    for (dimension_type col = num_columns; col-- > 0; )
      row[col] -= pivot[col];
    return;
  }

  TEMP_INTEGER(reduced_row_col);
  // Use reduced_row_col temporarily to hold the gcd.
  gcd_assign(reduced_row_col, pivot_column, row_column);
  TEMP_INTEGER(reduced_pivot_col);
  exact_div_assign(reduced_pivot_col, pivot_column, reduced_row_col);
  exact_div_assign(reduced_row_col, row_column, reduced_row_col);
  // Ensure that `reduced_pivot_col' is positive, so that the modulus
  // remains positive when multiplying the proper congruences below.
  // It's safe to swap the signs as row[column] will still come out 0.
  if (reduced_pivot_col < 0) {
    neg_assign(reduced_pivot_col);
    neg_assign(reduced_row_col);
  }
  // Multiply `row', including the modulus, by reduced_pivot_col.  To
  // keep all the moduli the same this requires multiplying all the
  // other proper congruences in the same way.
  for (dimension_type index = sys.num_rows(); index-- > 0; ) {
    Congruence& cg = sys[index];
    if (cg.is_proper_congruence())
      for (dimension_type col = num_columns; col-- > 0; )
        cg[col] *= reduced_pivot_col;
  }
  // Column num_columns contains the modulus, so start at the next
  // column.
  --num_columns;
  row_column = 0;
  // Subtract from row a multiple of pivot such that the result in
  // row[column] is zero.
  for (dimension_type col = column; col-- > 0; )
    sub_mul_assign(row[col], reduced_row_col, pivot[col]);
}

#ifndef NDEBUG
template <typename M, typename R>
bool
Grid::rows_are_zero(M& system, dimension_type first,
		    dimension_type last, dimension_type row_size) {
  while (first <= last) {
    const R& row = system[first++];
    for (dimension_type col = 0; col < row_size; ++col)
      if (row[col] != 0)
	return false;
  }
  return true;
}
#endif

void
Grid::simplify(Grid_Generator_System& sys, Dimension_Kinds& dim_kinds) {
  assert(!sys.empty());
  // For reduce_pc_with_pc.
  assert(sys.num_columns() > 0);

  // Changes here may also be required in the congruence version
  // below.

  // Subtract one to allow for the parameter divisor column
  const dimension_type num_columns = sys.num_columns() - 1;

  if (dim_kinds.size() != num_columns)
    dim_kinds.resize(num_columns);

  const dimension_type num_rows = sys.num_rows();

  // For each dimension `dim' move or construct a row into position
  // `pivot_index' such that the row has zero in all elements
  // following column `dim' and a value other than zero in column
  // `dim'.
  dimension_type pivot_index = 0;
  for (dimension_type dim = 0; dim < num_columns; ++dim) {
    // Consider the pivot and following rows.
    dimension_type row_index = pivot_index;

    // Move down over rows which have zero in column `dim'.
    while (row_index < num_rows && sys[row_index][dim] == 0)
      ++row_index;

    if (row_index == num_rows)
      // Element in column `dim' is zero in all rows from the pivot.
      dim_kinds[dim] = GEN_VIRTUAL;
    else {
      if (row_index != pivot_index)
	std::swap(sys[row_index], sys[pivot_index]);
      Grid_Generator& pivot = sys[pivot_index];
      bool pivot_is_line = pivot.is_line();

      // Change the matrix so that the value at `dim' in every row
      // following `pivot_index' is 0, leaving an equivalent grid.
      while (row_index < num_rows - 1) {
	++row_index;

	Grid_Generator& row = sys[row_index];

	if (row[dim] == 0)
	  continue;

	if (row.is_line())
	  if (pivot_is_line)
	    reduce_line_with_line(row, pivot, dim);
	  else {
	    assert(pivot.is_parameter_or_point());
	    std::swap(row, pivot);
	    pivot_is_line = true;
	    reduce_parameter_with_line(row, pivot, dim, sys);
	  }
	else {
	  assert(row.is_parameter_or_point());
	  if (pivot_is_line)
	    reduce_parameter_with_line(row, pivot, dim, sys);
	  else {
	    assert(pivot.is_parameter_or_point());
	    reduce_pc_with_pc(row, pivot, dim, dim + 1, num_columns);
	  }
	}
      }

      if (pivot_is_line)
	dim_kinds[dim] = LINE;
      else {
	assert(pivot.is_parameter_or_point());
	dim_kinds[dim] = PARAMETER;
      }

#ifdef STRONG_REDUCTION
      // Ensure a positive follows the leading zeros.
      if (pivot[dim] < 0)
	pivot.negate(dim, num_columns - 1);

      // Factor this row out of the preceding rows.
      reduce_reduced<Grid_Generator_System, Grid_Generator>
	(sys, dim, pivot_index, dim, num_columns - 1, dim_kinds);
#endif

      ++pivot_index;
    }
  }

  // Clip any zero rows from the end of the matrix.
  if (num_rows > pivot_index) {
#ifndef NDEBUG
    const bool ret = rows_are_zero<Grid_Generator_System, Grid_Generator>
      (sys,
       // index of first
       pivot_index,
       // index of last
       sys.num_rows() - 1,
       // row size
       sys.num_columns() - 1);
    assert(ret == true);
#endif
    sys.erase_to_end(pivot_index);
  }

  sys.unset_pending_rows();

  // Ensure that the parameter divisors are the same as the system
  // divisor.
  const Coefficient& system_divisor = sys[0][0];
  for (dimension_type row = sys.num_rows() - 1,
	 dim = sys.num_columns() - 2;
       dim > 0; --dim)
    switch (dim_kinds[dim]) {
    case PARAMETER:
      sys[row].set_divisor(system_divisor);
    case LINE:
      --row;
    case GEN_VIRTUAL:
      break;
    }

  assert(sys.OK());
}

bool
Grid::simplify(Congruence_System& sys, Dimension_Kinds& dim_kinds) {
  assert(sys.num_columns() > 2);

  // Changes here may also be required in the generator version above.

  // TODO: Consider normalizing the moduli only when congruences are
  //       added to con_sys.
  sys.normalize_moduli();

  const dimension_type num_columns = sys.num_columns() - 1 /* modulus */;

  if (dim_kinds.size() != num_columns)
    dim_kinds.resize(num_columns);

  const dimension_type num_rows = sys.num_rows();

  // For each dimension `dim' move or construct a row into position
  // `pivot_index' such that the row has a value of zero in all
  // elements preceding column `dim' and some other value in column
  // `dim'.
  dimension_type pivot_index = 0;
  for (dimension_type dim = num_columns; dim-- > 0; ) {
    // Consider the pivot and following rows.
    dimension_type row_index = pivot_index;

    // Move down over rows which have zero in column `dim'.
    while (row_index < num_rows && sys[row_index][dim] == 0)
      ++row_index;

    if (row_index == num_rows)
      // Element in column `dim' is zero in all rows from the pivot,
      // or `sys' is empty of rows.
      dim_kinds[dim] = CON_VIRTUAL;
    else {
      // row_index != num_rows
      if (row_index != pivot_index)
	std::swap(sys[row_index], sys[pivot_index]);
      Congruence& pivot = sys[pivot_index];
      bool pivot_is_equality = pivot.is_equality();

      // Change the matrix so that the value at `dim' in every row
      // following `pivot_index' is 0, leaving an equivalent grid.
      while (row_index < num_rows - 1) {
	++row_index;

	Congruence& row = sys[row_index];

	if (row[dim] == 0)
	  continue;

	if (row.is_equality())
	  if (pivot_is_equality)
	    reduce_equality_with_equality(row, pivot, dim);
	  else {
	    assert(pivot.is_proper_congruence());
	    std::swap(row, pivot);
	    pivot_is_equality = true;
	    reduce_congruence_with_equality(row, pivot, dim, sys);
	  }
	else {
	  assert(row.is_proper_congruence());
	  if (pivot_is_equality)
	    reduce_congruence_with_equality(row, pivot, dim, sys);
	  else {
	    assert(pivot.is_proper_congruence());
	    reduce_pc_with_pc(row, pivot, dim, 0, dim);
	  }
	}
      }

      if (pivot_is_equality)
	dim_kinds[dim] = EQUALITY;
      else {
	assert(pivot.is_proper_congruence());
	dim_kinds[dim] = PROPER_CONGRUENCE;
      }

#ifdef STRONG_REDUCTION
      // Ensure a positive follows the leading zeros.
      if (pivot[dim] < 0)
	pivot.negate(0, dim);
      // Factor this row out of the preceding ones.
      reduce_reduced<Congruence_System, Congruence>
	(sys, dim, pivot_index, 0, dim, dim_kinds, false);
#endif
      ++pivot_index;
    }
  } // end for (dimension_type dim = num_columns; dim-- > 0; )

  // For clearer naming.
  dimension_type& reduced_num_rows = pivot_index;

  if (reduced_num_rows > 0) {
    // If the last row is false then make it the equality 1 = 0, and
    // make it the only row.
    Congruence& last_row = sys[reduced_num_rows - 1];
    if (dim_kinds[0] == PROPER_CONGRUENCE) {
      if (last_row.inhomogeneous_term() % last_row.modulus() != 0) {
	// The last row is a false proper congruence.
	last_row.set_is_equality();
	dim_kinds[0] = EQUALITY;
	goto return_empty;
      }
    }
    else if (dim_kinds[0] == EQUALITY) {
      // The last row is a false equality, as all the coefficient terms
      // are zero while the inhomogeneous term (as a result of the
      // reduced form) is some other value.
    return_empty:
      last_row[0] = 1;
      dim_kinds.resize(1);
      std::swap(sys.rows[0], sys.rows.back());
      sys.erase_to_end(1);

      assert(sys.OK());
      return true;
    }
  }
  else {
    // Either sys is empty (it defines the universe) or every column
    // before the modulus column contains only zeroes.

    // Set up the integrality congruence.
    dim_kinds[0] = PROPER_CONGRUENCE;
    if (num_rows == 0) {
      sys.add_zero_rows(1,
			Linear_Row::Flags(NECESSARILY_CLOSED,
					  Linear_Row::RAY_OR_POINT_OR_INEQUALITY));
      Congruence& cg = sys[0];
      cg[num_columns] = 1;
      cg[0] = 1;

      assert(sys.OK());
      return false;
    }
    sys[0][num_columns] = 1;
    // Ensure that, after any zero row clipping below, a single row
    // will remain for the integrality congruence.
    reduced_num_rows = 1;
  }

  // Clip any zero rows from the end of the matrix.
  if (num_rows > 1 && num_rows > reduced_num_rows) {
#ifndef NDEBUG
    const bool ret = rows_are_zero<Congruence_System, Congruence>
      (sys,
       // index of first
       reduced_num_rows,
       // index of last
       num_rows - 1,
       // row size
       num_columns);
    assert(ret == true);
#endif
    sys.erase_to_end(reduced_num_rows);
  }

  assert(sys.num_rows() == reduced_num_rows);

  // Ensure that the last row is the integrality congruence.
  const dimension_type mod_index = num_columns;
  if (dim_kinds[0] == CON_VIRTUAL) {
    // The last row is virtual, append the integrality congruence.
    dim_kinds[0] = PROPER_CONGRUENCE;
    sys.add_zero_rows(1,
		      Linear_Row::Flags(NECESSARILY_CLOSED,
					Linear_Row::RAY_OR_POINT_OR_INEQUALITY));
    Congruence& new_last_row = sys[reduced_num_rows];
    new_last_row[mod_index] = 1;
    // Try use an existing modulus.
    dimension_type row_index = reduced_num_rows;
    while (row_index-- > 0) {
      Congruence& row = sys[row_index];
      if (row[mod_index] > 0) {
	new_last_row[mod_index] = row[mod_index];
	break;
      }
    }
    new_last_row[0] = new_last_row[mod_index];
#ifdef STRONG_REDUCTION
    ++reduced_num_rows;
#endif
  }
  else {
    Congruence& last_row = sys[reduced_num_rows - 1];
    last_row[0] = last_row[mod_index];
  }

#ifdef STRONG_REDUCTION
  // Factor the modified integrality congruence out of the other rows.
  reduce_reduced<Congruence_System, Congruence>
    (sys, 0, reduced_num_rows - 1, 0, 0, dim_kinds, false);
#endif

  assert(sys.OK());
  return false;
}

} // namespace Parma_Polyhedra_Library
