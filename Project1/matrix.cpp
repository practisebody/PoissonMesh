#include "stdafx.h"
#include "matrix.h"
#include "matrix_inl.h"

namespace matrix {

	double inner_product (const vector& lhs, const vector& rhs) {
		double product = 0;
		assert(lhs.size() == rhs.size());
		for(size_t i = 0; i < lhs.size(); ++i) {
			product += lhs[i] * rhs[i];
		}
		return product;
	}


	const vector DOK::conjugate_gradient(const vector& b, vector& x, size_t max_iter, double eps) const {
//		vector x;
		vector r = b - *this * x;
		vector p = r;
		double rsold = inner_product(r, r);

		size_t i;
		for(i = 0; i < max_iter && rsold > eps; ++i) {
			vector Ap = *this * p;
			double alpha = rsold / inner_product(p, Ap);
			x += alpha * p;
			r -= alpha * Ap;
			double rsnew = inner_product(r, r);
			p *= rsnew / rsold;
			p += r;
			rsold = rsnew;
		}
		if (i == max_iter)
			printf("exce\n");
		return x;
	}

	const vector operator *(const DOK& A, const vector& x) {
		assert(A.col_num == x.size());
		vector ret(A.row_num);
		for(std::map<std::pair<size_t, size_t>, double>::const_iterator it = A.items.begin(); it != A.items.end(); ++it) {
			ret[it->first.first] += it->second * x[it->first.second];
		}
		return ret;
	}

	CSR::CSR(CoordinateList& coordinate_list) {
		row_num = coordinate_list.row_num;
		col_num = coordinate_list.col_num;
		std::vector<CoordinateItem >& items = coordinate_list.items;
		values.reserve(items.size());
		row_ptrs.reserve(row_num + 1);
		col_indices.reserve(items.size());
		sort(items.begin(), items.end());
		size_t row = 0;
		for(size_t i = 0; i < items.size(); ++i) {
			values.push_back(items[i].value);
			col_indices.push_back(items[i].col);
			for(; row <= items[i].row; ++row) {
				row_ptrs.push_back(i);
			}
		}

		for(; row <= row_num; ++row) {
			row_ptrs.push_back(items.size());
		}
	}

	CSR::CSR(const DOK& dok) {
		row_num = dok.row_num;
		col_num = dok.col_num;
		std::map<std::pair<size_t, size_t>, double> items = dok.items;
		values.reserve(items.size());
		row_ptrs.reserve(row_num + 1);
		col_indices.reserve(items.size());
		size_t row = 0;
		size_t i = 0;
		for(std::map<std::pair<size_t, size_t>, double>::const_iterator it = items.begin(); it != items.end(); ++i, ++it) {
			values.push_back(it->second);
			col_indices.push_back(it->first.second);
			for(; row <= it->first.first; ++row) {
				row_ptrs.push_back(i);
			}
		}

		for(; row <= row_num; ++row) {
			row_ptrs.push_back(items.size());
		}
	}

	double CSR::operator () (size_t row, size_t col) {
		std::pair<std::vector<size_t>::iterator, std::vector<size_t>::iterator> col_index
			= equal_range(col_indices.begin() + row_ptrs[row], col_indices.begin() + row_ptrs[row + 1], col);;
		if(col_index.first == col_index.second)
			return 0;
		return values[col_index.first - col_indices.begin()];
	}
	
	const vector operator *(const CSR& A, const vector& x) {
		assert(A.col_num == x.size());
		vector ret(A.row_num);
		for(size_t row = 0; row < A.row_num; ++row) {
			size_t row_begin = A.row_ptrs[row];
			size_t row_end = A.row_ptrs[row+1];
			for(size_t i = row_begin; i < row_end; ++i) {
				size_t col = A.col_indices[i];
				ret[row] += A.values[i] * x[col];
			}
		}
		return ret;
	}

	const vector CSR::conjugate_gradient(const vector& b, vector& x, size_t max_iter, double eps) const {
//		vector x;
		vector r = b - *this * x;
		vector p = r;
		double rsold = inner_product(r, r);

		for(size_t i = 0; i < max_iter && rsold > eps; ++i) {
			vector Ap = *this * p;
			double alpha = rsold / inner_product(p, Ap);
			x += alpha * p;
			r -= alpha * Ap;
			double rsnew = inner_product(r, r);
			p *= rsnew / rsold;
			p += r;
			rsold = rsnew;
		}
		return x;
	}
}
