#include "matrix.h"

namespace matrix {

	inline vector::vector(size_t size) {
		_size = size;
		_data = (double*)malloc(sizeof(double) * _size);
		memset(_data, 0, sizeof(double) * _size);
	}

	inline vector::vector(const vector& v) {
		_size = v._size;
		_data = (double*)malloc(sizeof(double) * _size);
		memcpy(_data, v._data, sizeof(double) * _size);
	}

	inline const vector& vector::operator = (const vector& v) {
		free(_data);
		_size = v._size;
		_data = (double*)malloc(sizeof(double) * _size);
		memcpy(_data, v._data, sizeof(double) * _size);
		return *this;
	}

	inline double& vector::operator [] (size_t index) {
		return _data[index];
	}

	inline double vector::operator [] (size_t index) const {
		return _data[index];
	}

	inline void vector::operator -= (const vector& op2) {
		assert(_size == op2._size);
		for(size_t i = 0; i < _size; ++i) {
			_data[i] -= op2._data[i];
		}
	}
		
	inline void vector::operator += (const vector& op2) {
		assert(_size == op2._size);
		for(size_t i = 0; i < _size; ++i) {
			_data[i] += op2._data[i];
		}
	}
		
	inline void vector::operator *= (double scale) {
		for(size_t i = 0; i < _size; ++i) {
			_data[i] *= scale;
		}			
	}

	inline double vector::norm_2() const {
		double product = 0;
		for(size_t i = 0; i < _size; ++i) {
			product += _data[i] * _data[i];
		}
		return sqrt(product);
	}
		
	inline size_t vector::size() const {
		return _size;
	}

	inline vector::~vector() {
		free(_data);
	}

	inline const vector operator * (const vector& v, double scale) {
		vector ret = v;
		for(size_t i = 0; i < ret.size(); ++i) {
			ret[i] *= scale;
		}
		return ret;
	}

	inline const vector operator * (double scale, const vector& v) {
		return v * scale;
	}

	inline const vector operator - (const vector& op1, const vector& op2) {
		assert(op1.size() == op2.size());
		vector ret(op1.size());
		for(size_t i = 0; i < ret.size(); ++i) {
			ret[i] = op1[i] - op2[i];
		}
		return ret;
	}

	inline const vector operator + (const vector& op1, const vector& op2) {
		assert(op1.size() == op2.size());
		vector ret(op1.size());
		for(size_t i = 0; i < ret.size(); ++i) {
			ret[i] = op1[i] + op2[i];
		}
		return ret;
	}

	inline bool operator < (const CoordinateItem& lhs, const CoordinateItem& rhs) {
		if(lhs.row != rhs.row)
			return lhs.row < rhs.row;
		return lhs.col < rhs.col;
	}

	inline CoordinateItem::CoordinateItem(size_t row, size_t col, double value)
		:row(row), col(col), value(value) {}

	inline CoordinateList::CoordinateList(size_t row_num, size_t col_num)
		:row_num(row_num), col_num(col_num) {}

	inline void CoordinateList::insert(size_t row, size_t col, double value) {
		items.push_back(CoordinateItem(row, col, value));
	}

	inline DOK::DOK(size_t row_num, size_t col_num)
		:row_num(row_num), col_num(col_num) {}

	inline double& DOK::operator() (size_t row, size_t col) {
		std::map<std::pair<size_t, size_t>, double>::iterator item = items.find(std::pair<size_t, size_t>(row, col));
		if (item != items.end()) {
			return item->second;
		} else {
			items[std::pair<size_t, size_t>(row, col)] = 0;
			item = items.find(std::pair<size_t, size_t>(row, col));
			return item->second;
		}
	}

	inline void DOK::reset(size_t row) {
		auto dummy_begin = Pos(row, 0);
		auto dummy_end = Pos(row, -1);
		items[dummy_begin] = 0;
		items[dummy_end] = 0;
		auto begin = items.find(dummy_begin);
		auto end = items.find(dummy_end);
		end++;
		items.erase(begin, end);
		items[Pos(row, row)] = 1;
	}


}