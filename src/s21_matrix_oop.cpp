#include "s21_matrix_oop.h"

using namespace std;

S21Matrix S21Matrix::getMinor(int r_minor, int c_minor) const noexcept {
  S21Matrix result(rows_ - 1, cols_ - 1);
  int r = 0, c = 0;
  for (int i = 0; i < rows_; i++) {
    if (i == r_minor) continue;
    for (int j = 0; j < cols_; j++) {
      if (j == c_minor) continue;
      result.matrix_[r][c] = matrix_[i][j];
      c++;
    }
    c = 0;
    r++;
  }
  return result;
}

S21Matrix::S21Matrix() {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) {  // parametric constructor
  if (rows <= 0 || cols <= 0) throw ERROR_MATRIX;
  rows_ = rows;
  cols_ = cols;
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
    for (int j = 0; j < cols_; j++) matrix_[i][j] = 0.0;
  }
}

S21Matrix::S21Matrix(const S21Matrix& other) {  // copy constructor
  rows_ = other.rows();
  cols_ = other.columns();
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other(i, j);
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept {  // transfer constructor
  rows_ = other.rows(), cols_ = other.columns(), matrix_ = other.matrix_;
  ;
  other.rows_ = 0, other.cols_ = 0, other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) delete[] matrix_[i];
    delete[] matrix_;  // Destructor
  }
}

int S21Matrix::rows() const noexcept { return rows_; }

int S21Matrix::columns() const noexcept { return cols_; }

void S21Matrix::setRows(int r) {
  if (r <= 0) throw ERROR_MATRIX;
  S21Matrix result(r, cols_);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < cols_; j++) {
      if (i >= rows_)
        result.matrix_[i][j] = 0.0;
      else
        result.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = result;
}

void S21Matrix::setColumns(int c) {
  if (c <= 0) throw ERROR_MATRIX;
  S21Matrix result(rows_, c);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < c; j++) {
      if (j >= cols_)
        result.matrix_[i][j] = 0.0;
      else
        result.matrix_[i][j] = matrix_[i][j];
    }
  }
  *this = result;
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  bool is_equal = SUCCESS;
  if (rows_ != other.rows() || cols_ != other.columns()) is_equal = FAILED;
  if (other.matrix_ == nullptr && matrix_ == nullptr) return SUCCESS;
  for (int i = 0; i < rows_ && is_equal; i++) {
    for (int j = 0; j < cols_ && is_equal; j++) {
      is_equal = fabs(matrix_[i][j] - other(i, j)) <= M_DIF ? SUCCESS : FAILED;
    }
  }
  return is_equal;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows() || cols_ != other.columns()) throw ERROR_CALC;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other(i, j);
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows() || cols_ != other.columns()) throw ERROR_CALC;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) matrix_[i][j] -= other(i, j);
  }
}

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows()) throw ERROR_CALC;
  S21Matrix result(rows_, other.columns());
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.columns(); j++) {
      for (int k = 0; k < cols_; k++) {
        result.matrix_[i][j] += other(k, j) * matrix_[i][k];
      }
    }
  }
  *this = result;
}

S21Matrix S21Matrix::Transpose() const noexcept {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) result(j, i) = matrix_[i][j];
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) throw ERROR_CALC;
  double tmp = 0;
  S21Matrix minor, result(rows_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < rows_; j++) {
      minor = getMinor(i, j);
      tmp = minor.Determinant();
      result.matrix_[i][j] = tmp * pow(-1.0, i + j);
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) throw ERROR_CALC;
  double result = 0.0;
  S21Matrix minor;
  if (rows_ == 1) result = matrix_[0][0];
  if (rows_ == 2)
    result = matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
  if (rows_ > 2) {
    for (int i = 0; i < rows_; i++) {
      minor = getMinor(i, 0);
      result += minor.Determinant() * matrix_[i][0] * pow(-1.0, (double)(i));
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = this->Determinant();
  if (fabs(det) < M_DIF) throw ERROR_CALC;
  S21Matrix result(rows_, cols_);
  if (rows_ == 1)
    result(0, 0) = 1.0 / matrix_[0][0];
  else {
    result = CalcComplements();
    result.MulNumber(1.0 / det);
    result = result.Transpose();
  }
  return result;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SumMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SubMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.MulMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator*(const double& num) const {
  S21Matrix tmp(*this);
  tmp.MulNumber(num);
  return tmp;
}

S21Matrix operator*(const double& num, const S21Matrix& other) {
  S21Matrix tmp(other);
  tmp.MulNumber(num);
  return tmp;
}

bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  return EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) noexcept {
  if (this == &other) return *this;
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++)
      if (matrix_[i] != nullptr) delete[] matrix_[i];
    delete[] matrix_;
  }

  cols_ = other.columns();
  rows_ = other.rows();
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) matrix_[i][j] = other(i, j);
  }
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double& num) noexcept {
  MulNumber(num);
  return *this;
}

double& S21Matrix::operator()(int r, int c) const {
  if (r >= rows_ || c >= cols_ || r < 0 || c < 0) throw ERROR_MATRIX;
  return matrix_[r][c];
}

/*void print_matrix(const S21Matrix& mat) {
    int r = mat.rows(), c = mat.columns();
    for(int i = 0; i < r; i ++) {
        for(int j = 0; j < c; j++) {
            cout << mat(i, j) << " ";
        }
        cout << endl;
    }
    cout << endl;
}*/
