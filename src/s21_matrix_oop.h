#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <cmath>
#include <iostream>

#define SUCCESS 1
#define FAILED 0
#define M_DIF 1e-7

#define ERROR_MATRIX 1
#define ERROR_CALC 2

using namespace std;

class S21Matrix {
 private:
  // Attributes
  int rows_;
  int cols_;
  double** matrix_;  // Pointer to the memory where the matrix is allocated
  S21Matrix getMinor(int r_minor, int c_minor) const noexcept;

 public:
  S21Matrix();
  explicit S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  int rows() const noexcept;
  int columns() const noexcept;
  void setRows(int r);
  void setColumns(int c);

  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const noexcept;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  S21Matrix operator+(const S21Matrix&) const;
  S21Matrix operator-(const S21Matrix&) const;
  S21Matrix operator*(const S21Matrix&) const;
  S21Matrix operator*(const double&) const;
  friend S21Matrix operator*(const double&, const S21Matrix&);
  bool operator==(const S21Matrix& other) const noexcept;
  S21Matrix& operator=(const S21Matrix&) noexcept;
  S21Matrix& operator+=(const S21Matrix&);
  S21Matrix& operator-=(const S21Matrix&);
  S21Matrix& operator*=(const S21Matrix&);
  S21Matrix& operator*=(const double&) noexcept;
  double& operator()(int r, int c) const;
};
S21Matrix operator*(const double&, const S21Matrix&);
// void print_matrix(const S21Matrix&);
#endif  // SRC_S21_MATRIX_H_
