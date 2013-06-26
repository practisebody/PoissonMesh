#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<algorithm>
#include<cassert>
#include<map>
#include<vector>

namespace matrix {

	class vector {
	public:
		explicit vector(size_t size);
		vector(const vector& v);
		const vector& operator = (const vector& v);
		double& operator [] (size_t index);
		double operator [] (size_t index) const;
		void operator -= (const vector& op2);
		void operator += (const vector& op2);
		void operator *= (double scale);
		double norm_2() const;
		size_t size() const;
		~vector();
	private:
		double *_data;
		size_t _size;
	};

	double inner_product (const vector& lhs, const vector& rhs);
	const vector operator * (const vector& v, double scale);
	const vector operator * (double scale, const vector& v);
	const vector operator - (const vector& op1, const vector& op2);
	const vector operator + (const vector& op1, const vector& op2);

	struct CoordinateItem {
		size_t row, col;
		double value;
		CoordinateItem(size_t row, size_t col, double value);
	};

	bool operator < (const CoordinateItem& lhs, const CoordinateItem& rhs);

	class CoordinateList {
	public:
		CoordinateList(size_t row_num, size_t col_num);
		void insert(size_t row, size_t col, double value);
		std::vector<CoordinateItem> items;
		size_t row_num, col_num;
	};

	class DOK {
	public:
		typedef std::pair<size_t, size_t> Pos;
		DOK(size_t row_num, size_t col_num);
		double& operator() (size_t row, size_t col);
		const vector conjugate_gradient(const vector& b, vector& x, size_t max_iter, double eps) const;
		void reset(size_t row);

		std::map<Pos, double> items;
		size_t row_num, col_num;
	};

	const vector operator *(const DOK& A, const vector& x);

	class CSR {
	public:
		explicit CSR(CoordinateList& coordinate_list);
		explicit CSR(const DOK& dok);
		double operator () (size_t row, size_t col);
		const vector conjugate_gradient(const vector& b, vector& x, size_t max_iter, double eps) const ;

		std::vector<double> values;
		std::vector<size_t> col_indices;
		std::vector<size_t> row_ptrs;
		size_t col_num, row_num;
	};
	
	const vector operator *(const CSR& A, const vector& x);

}

#endif