/* Linear_System class implementation: inline functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2011 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Linear_System_inlines_hh
#define PPL_Linear_System_inlines_hh 1

// TODO: Remove this.
// It was added to please KDevelop4.
#include "Linear_System.defs.hh"

#include "Bit_Row.defs.hh"
#include "Coefficient.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename Row>
inline memory_size_type
Linear_System<Row>::external_memory_in_bytes() const {
  return rows.external_memory_in_bytes();
}

template <typename Row>
inline memory_size_type
Linear_System<Row>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename Row>
inline bool
Linear_System<Row>::is_sorted() const {
  // The flag `sorted' does not really reflect the sortedness status
  // of a system (if `sorted' evaluates to `false' nothing is known).
  // This assertion is used to ensure that the system
  // is actually sorted when `sorted' value is 'true'.
  PPL_ASSERT(!sorted || check_sorted());
  return sorted;
}

template <typename Row>
inline void
Linear_System<Row>::set_sorted(const bool b) {
  sorted = b;
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(Topology topol)
  : rows(),
    num_columns_(0),
    row_topology(topol),
    index_first_pending(0),
    sorted(true) {
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(Topology topol,
                                  dimension_type n_rows,
                                  dimension_type n_columns)
  : rows(n_rows, Row(n_columns, typename Row::Flags(topol))),
    num_columns_(n_columns),
    row_topology(topol),
    index_first_pending(n_rows),
    sorted(true) {
}

template <typename Row>
inline dimension_type
Linear_System<Row>::first_pending_row() const {
  return index_first_pending;
}

template <typename Row>
inline dimension_type
Linear_System<Row>::num_pending_rows() const {
  PPL_ASSERT(num_rows() >= first_pending_row());
  return num_rows() - first_pending_row();
}

template <typename Row>
inline void
Linear_System<Row>::unset_pending_rows() {
  index_first_pending = num_rows();
}

template <typename Row>
inline void
Linear_System<Row>::set_index_first_pending_row(const dimension_type i) {
  index_first_pending = i;
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y)
  : rows(y.rows),
    num_columns_(y.num_columns_),
    row_topology(y.row_topology) {
  unset_pending_rows();
  // Previously pending rows may violate sortedness.
  sorted = (y.num_pending_rows() > 0) ? false : y.sorted;
  PPL_ASSERT(num_pending_rows() == 0);
}

template <typename Row>
inline
Linear_System<Row>::Linear_System(const Linear_System& y, With_Pending)
  : rows(y.rows),
    num_columns_(y.num_columns_),
    row_topology(y.row_topology),
    index_first_pending(y.index_first_pending),
    sorted(y.sorted) {
}

template <typename Row>
inline Linear_System<Row>&
Linear_System<Row>::operator=(const Linear_System& y) {
  rows = y.rows;
  num_columns_ = y.num_columns_;
  row_topology = y.row_topology;
  unset_pending_rows();
  // Previously pending rows may violate sortedness.
  sorted = (y.num_pending_rows() > 0) ? false : y.sorted;
  PPL_ASSERT(num_pending_rows() == 0);
  return *this;
}

template <typename Row>
inline void
Linear_System<Row>::assign_with_pending(const Linear_System& y) {
  rows = y.rows;
  num_columns_ = y.num_columns_;
  row_topology = y.row_topology;
  index_first_pending = y.index_first_pending;
  sorted = y.sorted;
}

template <typename Row>
inline void
Linear_System<Row>::swap(Linear_System& y) {
  std::swap(rows, y.rows);
  std::swap(num_columns_, y.num_columns_);
  std::swap(row_topology, y.row_topology);
  std::swap(index_first_pending, y.index_first_pending);
  std::swap(sorted, y.sorted);
}

template <typename Row>
inline void
Linear_System<Row>::clear() {
  // Note: do NOT modify the value of `row_topology'.
  rows.clear();
  num_columns_ = 0;
  index_first_pending = 0;
  sorted = true;
}

template <typename Row>
inline void
Linear_System<Row>::resize_no_copy(const dimension_type new_n_rows,
                                   const dimension_type new_n_columns) {
  // TODO: Check if a rows.resize_no_copy() nethod could be more efficient.
  num_columns_ = new_n_columns;
  for (dimension_type i = std::min(rows.size(), new_n_rows); i-- > 0; )
    rows[i].resize(new_n_columns);
  rows.resize(new_n_rows,
              Row(new_n_columns, typename Row::Flags(row_topology)));
  // Even though `*this' may happen to keep its sortedness, we believe
  // that checking such a property is not worth the effort.  In fact,
  // it is very likely that the system will be overwritten as soon as
  // we return.
  set_sorted(false);
}

template <typename Row>
inline void
Linear_System<Row>::set_necessarily_closed() {
  row_topology = NECESSARILY_CLOSED;
  if (!has_no_rows())
    set_rows_topology();
}

template <typename Row>
inline void
Linear_System<Row>::set_not_necessarily_closed() {
  row_topology = NOT_NECESSARILY_CLOSED;
  if (!has_no_rows())
    set_rows_topology();
}

template <typename Row>
inline bool
Linear_System<Row>::is_necessarily_closed() const {
  return row_topology == NECESSARILY_CLOSED;
}

template <typename Row>
inline Row&
Linear_System<Row>::operator[](const dimension_type k) {
  return rows[k];
}

template <typename Row>
inline const Row&
Linear_System<Row>::operator[](const dimension_type k) const {
  return rows[k];
}

template <typename Row>
inline typename Linear_System<Row>::iterator
Linear_System<Row>::begin() {
  return rows.begin();
}

template <typename Row>
inline typename Linear_System<Row>::iterator
Linear_System<Row>::end() {
  return rows.end();
}

template <typename Row>
inline typename Linear_System<Row>::const_iterator
Linear_System<Row>::begin() const {
  return rows.begin();
}

template <typename Row>
inline typename Linear_System<Row>::const_iterator
Linear_System<Row>::end() const {
  return rows.end();
}

template <typename Row>
inline bool
Linear_System<Row>::has_no_rows() const {
  return rows.empty();
}

template <typename Row>
inline dimension_type
Linear_System<Row>::num_rows() const {
  return rows.size();
}

template <typename Row>
inline Topology
Linear_System<Row>::topology() const {
  return row_topology;
}

template <typename Row>
inline dimension_type
Linear_System<Row>::max_space_dimension() {
  // Column zero holds the inhomogeneous term or the divisor.
  // In NNC linear systems, the last column holds the coefficient
  // of the epsilon dimension.
  return Row::max_num_columns() - 2;
}

template <typename Row>
inline dimension_type
Linear_System<Row>::space_dimension() const {
  const dimension_type n_columns = num_columns();
  return (n_columns == 0)
    ? 0
    : n_columns - (is_necessarily_closed() ? 1 : 2);
}

template <typename Row>
inline dimension_type
Linear_System<Row>::num_columns() const {
  return num_columns_;
}

template <typename Row>
inline void
Linear_System<Row>::swap_rows(const dimension_type i,
                              const dimension_type j) {
  PPL_ASSERT(i < rows.size());
  PPL_ASSERT(j < rows.size());
  // i and j must be either both pending or both non-pending.
  PPL_ASSERT((i < first_pending_row() && j < first_pending_row())
             || (i >= first_pending_row() && j >= first_pending_row()));
  std::swap(rows[i], rows[j]);
  set_sorted(false);
}

template <typename Row>
inline void
Linear_System<Row>::remove_trailing_rows(const dimension_type n) {
  PPL_ASSERT(rows.size() >= n);
  rows.resize(rows.size() - n);
}

template <typename Row>
inline void
Linear_System<Row>
::remove_trailing_columns(const dimension_type n) {
  remove_trailing_columns_without_normalizing(n);
  // Have to re-normalize the rows of the system,
  // since we removed some coefficients.
  strong_normalize();
}

template <typename Row>
inline void
Linear_System<Row>
::remove_trailing_columns_without_normalizing(const dimension_type n) {
  PPL_ASSERT(num_columns_ >= n);
  num_columns_ -= n;
  for (dimension_type i = rows.size(); i-- > 0; )
    rows[i].resize(num_columns_);
}

template <typename Row>
inline void
Linear_System<Row>::permute_columns(const std::vector<dimension_type>& cycles) {
  PPL_DIRTY_TEMP_COEFFICIENT(tmp);
  const dimension_type n = cycles.size();
  PPL_ASSERT(cycles[n - 1] == 0);
  for (dimension_type k = rows.size(); k-- > 0; ) {
    Row& rows_k = rows[k];
    for (dimension_type i = 0, j = 0; i < n; i = ++j) {
      // Make `j' be the index of the next cycle terminator.
      while (cycles[j] != 0)
        ++j;
      // Cycles of length less than 2 are not allowed.
      PPL_ASSERT(j - i >= 2);
      if (j - i == 2)
        // For cycles of length 2 no temporary is needed, just a swap.
        rows_k.swap(cycles[i], cycles[i + 1]);
      else {
        // Longer cycles need a temporary.
        tmp = rows_k.get(cycles[j - 1]);
        for (dimension_type l = (j - 1); l > i; --l)
          rows_k.swap(cycles[l-1], cycles[l]);
        if (tmp == 0)
          rows_k.reset(cycles[i]);
        else
          std::swap(tmp, rows_k[cycles[i]]);
      }
    }
  }
  // The rows with permuted columns are still normalized but may
  // be not strongly normalized: sign normalization is necessary.
  sign_normalize();
}

template <typename Row>
inline void
Linear_System<Row>::swap_columns(dimension_type i, dimension_type j) {
  for (dimension_type k = num_rows(); k-- > 0; )
    rows[k].swap(i, j);
}

/*! \relates Linear_System */
template <typename Row>
inline bool
operator!=(const Linear_System<Row>& x, const Linear_System<Row>& y) {
  return !(x == y);
}

template <typename Row>
inline bool
Linear_System<Row>::Row_Less_Than::operator()(const Row& x,
                                         const Row& y) const {
  return compare(x, y) < 0;
}

} // namespace Parma_Polyhedra_Library

namespace std {

/*! \relates Parma_Polyhedra_Library::Linear_System */
template <typename Row>
inline void
swap(Parma_Polyhedra_Library::Linear_System<Row>& x,
     Parma_Polyhedra_Library::Linear_System<Row>& y) {
  x.swap(y);
}

} // namespace std


namespace Parma_Polyhedra_Library {

template <typename Row>
inline
Linear_System_With_Bit_Matrix_iterator<Row>::
Linear_System_With_Bit_Matrix_iterator(Iter1 iter1, Iter2 iter2)
  : i1(iter1), i2(iter2) {
}

template <typename Row>
inline
Linear_System_With_Bit_Matrix_iterator<Row>::
Linear_System_With_Bit_Matrix_iterator(const Linear_System_With_Bit_Matrix_iterator& y)
  : i1(y.i1), i2(y.i2) {
}

template <typename Row>
inline
Linear_System_With_Bit_Matrix_iterator<Row>::
~Linear_System_With_Bit_Matrix_iterator() {
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>&
Linear_System_With_Bit_Matrix_iterator<Row>::
operator=(const Linear_System_With_Bit_Matrix_iterator& y) {
  i1 = y.i1;
  i2 = y.i2;
  return *this;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>&
Linear_System_With_Bit_Matrix_iterator<Row>::operator++() {
  ++i1;
  ++i2;
  return *this;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>
Linear_System_With_Bit_Matrix_iterator<Row>::operator++(int) {
  Linear_System_With_Bit_Matrix_iterator tmp = *this;
  operator++();
  return tmp;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>&
Linear_System_With_Bit_Matrix_iterator<Row>::operator--() {
  --i1;
  --i2;
  return *this;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>
Linear_System_With_Bit_Matrix_iterator<Row>::operator--(int) {
  Linear_System_With_Bit_Matrix_iterator tmp = *this;
  operator--();
  return tmp;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>&
Linear_System_With_Bit_Matrix_iterator<Row>::operator+=(difference_type d) {
  i1 += d;
  i2 += d;
  return *this;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>
Linear_System_With_Bit_Matrix_iterator<Row>::
operator+(difference_type d) const {
  Linear_System_With_Bit_Matrix_iterator tmp = *this;
  tmp += d;
  return tmp;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>&
Linear_System_With_Bit_Matrix_iterator<Row>::operator-=(difference_type d) {
  i1 -= d;
  i2 -= d;
  return *this;
}

template <typename Row>
inline Linear_System_With_Bit_Matrix_iterator<Row>
Linear_System_With_Bit_Matrix_iterator<Row>::
operator-(difference_type d) const {
  Linear_System_With_Bit_Matrix_iterator tmp = *this;
  tmp -= d;
  return tmp;
}

template <typename Row>
inline typename Linear_System_With_Bit_Matrix_iterator<Row>::difference_type
Linear_System_With_Bit_Matrix_iterator<Row>::
operator-(const Linear_System_With_Bit_Matrix_iterator& y) const {
  return i1 - y.i1;
}

template <typename Row>
inline bool
Linear_System_With_Bit_Matrix_iterator<Row>::
operator==(const Linear_System_With_Bit_Matrix_iterator& y) const {
  return i1 == y.i1;
}

template <typename Row>
inline bool
Linear_System_With_Bit_Matrix_iterator<Row>::
operator!=(const Linear_System_With_Bit_Matrix_iterator& y) const {
  return i1 != y.i1;
}

template <typename Row>
inline bool
Linear_System_With_Bit_Matrix_iterator<Row>::
operator<(const Linear_System_With_Bit_Matrix_iterator& y) const {
  return i1 < y.i1;
}

template <typename Row>
inline typename Linear_System_With_Bit_Matrix_iterator<Row>::reference
Linear_System_With_Bit_Matrix_iterator<Row>::operator*() const {
  return *i1;
}

template <typename Row>
inline typename Linear_System_With_Bit_Matrix_iterator<Row>::pointer
Linear_System_With_Bit_Matrix_iterator<Row>::operator->() const {
  return &*i1;
}

template <typename Row>
inline void
Linear_System_With_Bit_Matrix_iterator<Row>::
iter_swap(const Linear_System_With_Bit_Matrix_iterator& y) const {
  std::iter_swap(i1, y.i1);
  std::iter_swap(i2, y.i2);
}

} // namespace Parma_Polyhedra_Library

namespace std {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::Linear_System_With_Bit_Matrix_iterator */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
inline void
iter_swap(Parma_Polyhedra_Library
	  ::Linear_System_With_Bit_Matrix_iterator<Row> x,
	  Parma_Polyhedra_Library
	  ::Linear_System_With_Bit_Matrix_iterator<Row> y) {
  x.iter_swap(y);
}

} // namespace std

#endif // !defined(PPL_Linear_System_inlines_hh)
