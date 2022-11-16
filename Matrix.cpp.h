// MI = MatrixIterator
//MI Constructor
template<typename T>
Matrix<T>::MatrixIterator::MatrixIterator(Node<T>* obj_current, Node<T>* obj_row_root){ 
    current = obj_current;
    row_root = obj_row_root;
}
//MI Dereference operators
template<typename T>
T& Matrix<T>::MatrixIterator::operator*(){
    return current->value;
}
template<typename T>
T* Matrix<T>::MatrixIterator::operator->(){
    return &(current->value);
}
//MI Post/Pre-Increment operators
template<typename T>
Matrix<T>::MatrixIterator& Matrix<T>::MatrixIterator::operator++(){
    if(current->nextInRow == nullptr && current->nextInCol == nullptr)
        current = nullptr;
    else if(current->nextInRow == nullptr){
        row_root = row_root->nextInCol;
        current = row_root;
    }
    else
        current = current->nextInRow;
    return *this;
}
template<typename T>
Matrix<T>::MatrixIterator Matrix<T>::MatrixIterator::operator++(int){
    MatrixIterator copy = *this;
    if(current->nextInRow == nullptr && current->nextInCol == nullptr)
        current = nullptr;
    else if(current->nextInRow == nullptr){
        row_root = row_root->nextInCol;
        current = row_root;
    }
    else
        current = current->nextInRow;
    return copy;
}
//MI Equality/Inequality operators
template<typename T>
bool Matrix<T>::MatrixIterator::operator==(const MatrixIterator& obj){
    return (current = obj.current && row_root = obj.row_root);
}
template<typename T>
bool Matrix<T>::MatrixIterator::operator!=(const MatrixIterator& obj){
    return !(current = obj.current && row_root = obj.row_root);
}

//Default Constructor
template<typename T>
Matrix<T>::Matrix(){
    root = nullptr;
    rows = 0;
    cols = 0;
}

template<typename T>
Matrix<T>::Matrix(const T** obj, const int obj_rows, const int obj_cols){
    root = new Node<T>;
    rows = obj_rows;
    cols = obj_cols;
    Node<T>* row_connector = root;
    Node<T>* storage = root;
    Node<T>* col_connector;
    
    for(int i = 0; i < rows; i++){
        col_connector = storage;
        storage = row_connector;
        for(int j = 0; j < cols; j++){
            row_connector.value = obj[i][j];
            if(i > 0){
                col_connector->nextInCol = row_connector;
            }
            if(j < cols - 1){
                row_connector->nextInRow = new Node<T>;
                row_connector = row_connector->nextInRow;
                col_connector = col_connector->nextInRow;
            }
        }
        storage->nextInCol = new Node<T>;
        row_connector = storage->nextInCol;
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& obj){
    root = new Node<T>;
    rows = obj.rows;
    cols = obj.cols;
    Node<T>* row_connector = root;
    Node<T>* storage = root;
    Node<T>* col_connector;

    for(int i = 0; i < rows; i++){
        col_connector = storage;
        storage = row_connector;
        for(auto j = obj.row_begin(i); j != obj.row_end(i); j++){
            row_connector->value = *j;
            if(i > 0){
                col_connector->nextInCol = row_connector;
            }
            peek = j;
            peek++;
            if(peek != obj.row_end(i)){
                row_connector->nextInRow = new Node<T>;
                row_connector = row_connector->nextInRow;
                col_connector = col_connector->nextInRow;
            }
        }
        storage->nextInCol = new Node<T>;
        row_connector = storage->nextInCol;
    }
}

template<typename T>
Matrix<T>::Matrix(Matrix&& obj){
    root = obj.root;
    rows = obj.rows;
    cols = obj.cols;
    obj.root = nullptr;
}

template<typename T>
T* Matrix<T>::row(int row_number){
    T* result = new T[cols];
    int index = 0;
    for(auto i = row_begin(row_number); i != row_end(row_number); i++){
        result[index++] = *i;
    }
    return result;
}

template<typename T>
T* Matrix<T>::column(int col_number){
    T* result = new T[rows];
    for(int i = 0; i < rows; i++)
        auto j = row_begin(i);
        for(int k = 1; k < col_number; k++)
            j++;
        result[i] = *j;
    return result
}

template<typename T>
T& Matrix<T>::valueAt(int row_number, int col_number){
    auto i = row_begin(row_number);
    for(int j = 0; j < col_number; j++)
        i++;
    return *i;
}

template<typename T>
Matrix<T>& Matrix<T>::transpose(){
    T** result = new T[cols];
    for(int i = 0; i < cols; i++){
        result[i] = column(i);
    }
    return Matrix(result);
}

template<typename T>
Matrix<T>& Matrix<T>::operator+(const Matrix<T>& obj){
    if(rows != obj.rows || cols != obj.cols){
        out_of_range err("Invalid Size");
        throw(err);
    }
    Matrix copy(*this);
    for(auto i = copy.begin(), auto j = obj.begin; i != end(); i++, j++)
        *i = *i + *j;

    return copy;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& obj){
    if(rows != obj.rows || cols != obj.cols){
        out_of_range err("Invalid Size");
        throw(err);
    }
    for(auto i = copy.begin(), auto j = obj.begin; i != end(); i++, j++)
        *i = *i + *j;

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*(const Matrix<T>& obj){
    if(cols != obj.row){
        out_of_range err("Matrices are not compatible for Matrix Multiplication.")
        throw(err);
    }
    T** result = new T[rows]
    for(int i = 0; i < rows; i++)
        result[i] = new T[obj.cols];
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < obj.cols; j++)
            result[i][j] = dotProduct(row(i), obj.column(j));
}
template<typename T>
bool Matrix<T>::operator==(const Matrix<T>& obj){
    if(rows != obj.rows || cols != obj.cols){
        return false;
    }
    for(auto i = begin(), auto j = obj.begin; i != end(); i++, j++)
        if(*i != *j)
            return false;
    return true;
}

template<typename T>
Matrix<T>::MatrixIterator Matrix<T>::begin(){
    return MatrixIterator(root,root); 
}
template<typename T>
Matrix<T>::MatrixIterator Matrix<T>::end(){
    return MatrixIterator(nullptr,nullptr);
}
template<typename T>
Matrix<T>::MatrixIterator Matrix<T>::row_begin(int row_number){
    if(row_number > rows)
        return MatrixIterator(nullptr,nullptr);
    Node<T> scout = root;
    for(int i = 1; i < row_number; i++)
        scout = scout->nextInCol;
    result = MatrixIterator(scout, scout);
    scout = nullptr;
    return result;
}
template<typename T>
Matrix<T>::MatrixIterator Matrix<T>::row_end(int row_number){
    if(row_number >= rows)
        return MatrixIterator(nullptr,nullptr);
    Node<T> scout = root;
    for(int i = 1; i <= row_number; i++)
        scout = scout->nextInCol;
    result = MatrixIterator(scout, scout);
    scout = nullptr;
    return result;
}


template<typename R>
std::ostream& operator<<(const std::ostream& out, const Matrix<R>& obj){
    for(int i = 0; i < rows; i++)
        out << "|row #" << i << ":[";
        for(auto j = row_begin(i); j != row_end(i); j++)
            out << *j << ", ";
        out << "]|" << endl;
    return out;
}

template<typename T>
T& Matrix<T>::dotProduct(T* horizontal, T* vertical, int n){
    T result = horizontal[0] * vertical[0];
    for(int i = 1; i < n; i++)
        result += horizontal[i] * vertical[0];
    return result; 
}