#include "linearAlgebra.h"



double round(double value, int decimalPlaces) {
    double multiplier = std::pow(10.0, decimalPlaces);
    return std::round(value * multiplier) / multiplier;
}

// OPERATORS

Matrix Matrix::operator+(const Matrix& matrix) const
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] + matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] - matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    if (at(0).size() != matrix.size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), matrix.at(0).size());
    for (int i = 0; i < size(); i++) // rows iter
    {

        for (int j = 0; j < matrix.at(0).size(); j++)  // cols iter
        {
            for (int k = 0; k < matrix.size(); k++)
            {
                result[i][j] += at(i)[k] * matrix[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator/(const Matrix& matrix) const
{
    return (*this) * matrix.inverse();
}

Matrix Matrix::operator*(double scalar) const
{
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) { return matrix * scalar; }

Matrix Matrix::operator/(double scalar) const
{
    return (*this) * (1 / scalar);
}

Matrix operator/(double scalar, const Matrix& matrix) { return matrix * 1 / scalar; }

// Text

std::string Matrix::toString() const
{
    int columns = at(0).size();
    int maxLength = 1;
    std::vector<std::string> rowsTxt(size());
    int rowSize = 0;

    for (int j = 0; j < columns; j++) // columns iter
    {
        // search for the longest number in each column
        for (int i = 0; i < size(); i++) // rows iter
        {
            rowsTxt[i] += std::to_string(round(at(i)[j], 3));
            if (std::to_string(at(i)[j]).size() > maxLength) {
                maxLength = std::to_string(at(i)[j]).size();
            }
        }
        // make all numbers in the column the same length
        for (int i = 0; i < size(); i++)
		{
            rowsTxt[i] += std::string(maxLength - (rowsTxt[i].size() - rowSize) +2, ' ');
		}
        rowSize = rowsTxt[0].size();
        maxLength = 1;
    }

    // add the rows to the final string
    std::string txt = "";
    for (int i = 0; i < size(); i++)
	{
		txt += rowsTxt[i] + "\n";
	}
    return txt;
}



// METHODS

Matrix Matrix::transpose() const
{
    Matrix result(at(0).size(), size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[j][i] = at(i)[j];
        }
    }
    return result;
}

Matrix Matrix::minor(int row, int col) const
{
    Matrix result(size() - 1, at(0).size() - 1);
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            if (i != row && j != col)
            {
                result[i < row ? i : i - 1][j < col ? j : j - 1] = at(i)[j];
            }
        }
    }
    return result;
}

double Matrix::determinant(const Matrix& matrix) const
{
    if (matrix.size() != matrix.at(0).size())
    {
        throw "Matrix must be square";
    }
    // Base case
    if (matrix.size() == 1)
    {
        return matrix[0][0];
    }
    if (matrix.size() == 2)
    {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // Method: Cofactors
    double det = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
        det += (i % 2 ? -1 : 1) * matrix[0][i] * determinant(matrix.minor(0, i));
    }
    
    return det;
}

double Matrix::det() const
{
	return determinant(*this);
}

double Matrix::determinant() const
{
    return determinant(*this);
}

Matrix Matrix::inverse() const
{
    // Method: Gauss-Jordan elimination
    if (size() != at(0).size())
    {
        throw "Matrix must be square";
    }

    int rowCount= size();
    Matrix matrix(*this);
    matrix.stack(Matrix::identity(rowCount), false);

    matrix.gauss();


    Matrix result(rowCount, rowCount);
    for (int i=0; i<rowCount; i++)
	{
		for (int j=0; j<rowCount; j++)
		{
            result[i][j] = matrix.at(i)[j + rowCount];
		}
	}
    return result;

}

Matrix Matrix::inverseAdjugate() const
{
    if (size() != at(0).size())
    {
        throw "Matrix must be square";
    }
    double det = determinant();
    if (det == 0)
    {
        throw "Matrix is singular";
    }
    Matrix result(size(), at(0).size());
    // method: Adjugate matrix
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = ((i + j) % 2 ? -1 : 1) * determinant(minor(i, j)) / det;
        }
    }
    return result.transpose();
}

Matrix Matrix::inv() const {return inverse();}



void Matrix::gauss()
{
    for (int row = 0; row < size(); row++)
    {
        // Make pivot = 1
        if (at(row)[row] == 0)
		{
			// look for a row that matrix[x][i] != 0
            for (int i = row+1; i < size(); i++)
            {
                if (at(i)[row] != 0)
                {
                    sumRow(row, i);
                    break;
                }
            }
			if (at(row)[row]==0) throw "Matrix is singular";
		}
        multiplyRow(row, 1 / at(row)[row]);

        // Make other rows = 0
        for (int i = 0; i < size(); i++)
        {
            if (i == row || at(i)[row] == 0) continue;

            std::vector<double> rowToAdd = getRow(row);
            Matrix::multiplyRow(rowToAdd, -at(i)[row]);
            sumRow(i, rowToAdd);
        }
    }
}


void Matrix::multiplyRow(int row, double scalar)
{
    for (int j = 0; j < at(0).size(); j++)
    {
        at(row)[j] *= scalar;
    }
}

void Matrix::multiplyRow(std::vector<double>& vec, double scalar) const
{
	for (int j = 0; j < vec.size(); j++)
	{
		vec[j] *= scalar;
	}
}

void Matrix::sumRow(int row1, int row2)
{
    for (int j = 0; j < at(0).size(); j++)
    {
        at(row1)[j] += at(row2)[j];
    }
}

void Matrix::sumRow(int row, const std::vector<double>& vec)
{
    for (int j = 0; j < at(0).size(); j++)
    {
        at(row)[j] += vec[j];
    }
}


Matrix Matrix::solve(const Matrix& _vector) const
{
    if (size() != at(0).size())
	{
		throw "Matrix must be square";
	}
    Matrix matrix(*this);
    matrix.stack(_vector, false);

    matrix.gauss();

    return Matrix(matrix.getColumn(matrix.at(0).size() - 1)).transpose();
}


void Matrix::identity()
{
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            at(i)[j] = (i == j ? 1 : 0);
        }
    }
}

Matrix Matrix::identity(int size)
{
	Matrix result(size, size);
	result.identity();
	return result;
}


void Matrix::stack(const Matrix& matrix, bool vertical)
{
    if (vertical)
    {
        if (at(0).size() != matrix.at(0).size()) {
            throw "Matrix dimensions must agree";
        }
        insert(end(), matrix.begin(), matrix.end());
    }
    else
    {
        if (size() != matrix.size()) {
            throw "Matrix dimensions must agree";
        }
        for (int i = 0; i < size(); i++)
        {
            at(i).insert(at(i).end(), matrix[i].begin(), matrix[i].end());
        }
    }
}

void Matrix::stack(const std::vector<double>& _vector, bool vertical)
{
	if (vertical)
	{
		if (at(0).size() != 1) {
			throw "Matrix dimensions must agree";
		}
		push_back(_vector);
	}
	else
	{
		if (size() != 1) {
			throw "Matrix dimensions must agree";
		}
        for (int i = 0; i < size(); i++)
        {
            at(i).push_back(_vector[i]);
        }
	}
}


std::vector<double> Matrix::getRow(const int row) const
{
	return at(row);
}

std::vector<double> Matrix::getColumn(const int col) const
{
    std::vector<double> column;
	for (int i = 0; i < size(); i++)
	{
		column.push_back(at(i)[col]);
	}
	return column;
}




double dot(const std::vector<double>& v1, const std::vector<double>& v2)
{
	if (v1.size() != v2.size())
	{
		throw "Vectors must have the same size";
	}
	double result = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		result += v1[i] * v2[i];
	}
	return result;
}

std::vector<double> cross(const std::vector<double>& v1, const std::vector<double>& v2)
{
	if (v1.size() != 3 || v2.size() != 3)
	{
		throw "Vectors must have size 3";
	}
	std::vector<double> result(3);
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return result;
}
