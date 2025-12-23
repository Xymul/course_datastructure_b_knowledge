#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include "./sorting.hpp"

constexpr static std::size_t MAX_TRIPLETS = 1000;

template <typename T>
struct triple
{
	int row = 0;
	int col = 0;
	T element;

	bool operator>(const triple& other)
	{
		return (row > other.row) || (row == other.row && col > other.col);
	}

	bool operator<(const triple& other)
	{
		return (row < other.row) || (row == other.row && col < other.col);
	}

	bool operator==(const triple& other)
	{
		return row == other.row && col == other.col;
	}
};

template <typename T = std::int32_t>
struct t_sparse_matrix
{
	triple<T> data[MAX_TRIPLETS + 1];
	int nrow;
	int ncol;
	int nitm;
};

using tsmatrix = t_sparse_matrix<std::int64_t>;

void init_tsmatrix(tsmatrix& matrix, int nrow, int ncol)
{
	matrix.nrow = nrow;
	matrix.ncol = ncol;
	matrix.nitm = 0;
}

void print_matrix(tsmatrix& matrix) {
	std::cout << "\n";
	int k = 1;
	for (int i = 1; i <= matrix.nrow; i++) {
		for (int j = 1; j <= matrix.ncol; j++) {
			if (matrix.data[k].row == i && matrix.data[k].col == j) {
				std::cout << matrix.data[k].element << " ";
				k++;
			} else {
				std::cout << 0 << " ";
			}
		}
		std::cout << std::endl;
	}
}

// append data to matrix
// tsmatrix::data must be sorted by row;
void append(tsmatrix& matrix, int row, int col, std::int64_t value)
{
	if (row > matrix.nrow || col > matrix.ncol) {
		throw std::out_of_range{"row or column index is out of range"};
	}

	if (matrix.nitm >= MAX_TRIPLETS) {
		throw std::runtime_error{"matrix is full, the maximum element counts is: " + std::to_string(MAX_TRIPLETS)};
	}

	int pos = 1;
	for (int i = matrix.nitm; i > 0; i--) {
		if ((row < matrix.data[i].row) ||
			(row == matrix.data[i].row && col <= matrix.data[i].col)) {
			pos = i;
			break;
		}
	}

	if (row == matrix.data[pos].row && col == matrix.data[pos].col) {
		return;
	}

	if (row > matrix.data[pos].row || (row == matrix.data[pos].row && col > matrix.data[pos].col)) {
		pos = matrix.nitm + 1;
	}

	for (int i = matrix.nitm; i >= pos; --i) {
		matrix.data[i + 1] = matrix.data[i];
	}

	matrix.data[pos].row = row;
	matrix.data[pos].col = col;
	matrix.data[pos].element = value;
	matrix.nitm++;
}

// 1 行列互换，然后排序
void transpose1(tsmatrix& matrix) {
	for (int i = 1; i <= matrix.nitm; i++) {
		std::swap(matrix.data[i].row, matrix.data[i].col);
	}

	sorting::bubble(&matrix.data[1], matrix.nitm);
}

// 2 交换行列的同时排序
void transpose2(tsmatrix& matrix, tsmatrix& dest) {
	dest.nitm = matrix.nitm;
	dest.nrow = matrix.nrow;
	dest.ncol = matrix.ncol;

	int j = 1;
	for (int k = 1; k <= matrix.nrow; k++) {
		for (int i = 1; i <= matrix.nitm; i++) {
			if (matrix.data[i].col == k) {
				dest.data[j].row = matrix.data[i].col;
				dest.data[j].col = matrix.data[i].col;
				dest.data[j].element = matrix.data[i].element;
				j++;
			}
		}
	}
}

// O(m+n), m depends on the matrix's item counts
//         n depends on the matrix's col counts
void quick_transpose(tsmatrix& matrix, tsmatrix& dest) {
	int* cnum = new int[matrix.ncol + 1];
	int* cpos = new int[matrix.ncol + 1];

	for (int i = 1; i <= matrix.ncol; i++) {
		cnum[i] = 0;
	}

	for (int i = 1; i <= matrix.nitm; i++) {
		++cnum[matrix.data[i].col];
	}

	cpos[1] = 1;
	for (int i = 2; i <= matrix.ncol; i++) {
		cpos[i] = cpos[i - 1] + cnum[i - 1];
	}

	for (int i = 1; i <= matrix.nitm; i++) {
		dest.data[cpos[matrix.data[i].col]].row = matrix.data[i].col;
		dest.data[cpos[matrix.data[i].col]].col = matrix.data[i].row;
		dest.data[cpos[matrix.data[i].col]].element = matrix.data[i].element;
		cpos[matrix.data[i].col]++;
	}

	delete[] cnum;
	delete[] cpos;
}

int main()
{
	tsmatrix m, mdest;
	init_tsmatrix(m, 10, 10);
	init_tsmatrix(mdest, 10, 10);
	append(m, 1, 1, 10);
	append(m, 1, 2, 100);
	append(m, 2, 1, 600);
	transpose1(m);
	print_matrix(m);
	quick_transpose(m, mdest);
	print_matrix(mdest);
	return 0;
}
