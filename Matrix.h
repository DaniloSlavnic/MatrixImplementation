#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

template<typename T>
struct Node{
    Node(){
        nextInRow = nullptr;
        nextInCol = nullptr;
    }
    Node<T>* nextInRow;
    Node<T>* nextInCol;
    T value;
};

template<typename T>
class Matrix{
public:
    class MatrixIterator: public std::iterator<std::forward_iterator_tag, T>{
    public:
        MatrixIterator(Node<T>*, Node<T>*);

        T& operator*();
        T* operator->();
        MatrixIterator& operator++();
        MatrixIterator operator++(int);
        bool operator==(const MatrixIterator&);
        bool operator!=(const MatrixIterator&);
    private:
        Node<T>* current;
        Node<T>* row_root;
    };

public:
    //Construcors/Destructor
    Matrix();
    Matrix(const T**, const int, const int);
    Matrix(const Matrix&);
    Matrix(Matrix&&);
    ~Matrix();
    
    //Return values of subsections of the matrix
    T* row(int);
    T* column(int);
    T& valueAt(int,int);

    Matrix& transpose();
    Matrix& operator+(const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix& operator*(const Matrix&);
    Matrix& operator*=(const Matrix&);
    bool operator==(const Matrix&);

    MatrixIterator begin();
    MatrixIterator end();
    MatrixIterator row_begin(int);
    MatrixIterator row_end(int);

    
    template<typename R>
    friend std::ostream& operator<<(const std::ostream&, const Matrix<R>&);
private:
    T& dotProduct(T*,T*, int);
    Node<T>* root;
    int rows,cols;

};

#include "Matrix.cpp.h"
#endif