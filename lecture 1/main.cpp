#include <iostream>
int* function(double* matrix, int n, int w, int& k) {
    // Создаем массив флагов для отслеживания столбцов с нулями
    bool* columnFlags = (bool*)calloc(w, sizeof(bool));
    k = 0;
    
    // Проходим по матрице и отмечаем столбцы с нулями
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < w; ++j) {
            if (matrix[i * w + j] == 0 && !columnFlags[j]) {
                columnFlags[j] = true;
                k++;
            }
        }
    }
    
    // Выделяем память под результат
    int* result = (int*)malloc(k * sizeof(int));
    int index = 0;
    // Заполняем результат индексами столбцов
    for (int j = 0; j < w; ++j) {
        if (columnFlags[j]) {
            result[index++] = j;
        }
    }
    
    // Освобождаем память флагов
    free(columnFlags);
    return result;
}

// Функция для удаления столбцов
double* removeColumns(double* matrix, int rows, int& cols, int* columnsToRemove, int k) {
    if (k == 0) return matrix;
    
    int newCols = cols - k;
    double* newMatrix = (double*)calloc(rows * newCols, sizeof(double));
    
    for (int i = 0; i < rows; i++) {
        int newJ = 0;
        for (int j = 0; j < cols; j++) {
            // Проверяем, нужно ли пропустить этот столбец
            bool skip = false;
            for (int idx = 0; idx < k; idx++) {
                if (j == columnsToRemove[idx]) {
                    skip = true;
                    break;
                }
            }
            
            if (!skip) {
                newMatrix[i * newCols + newJ] = matrix[i * cols + j];
                newJ++;
            }
        }
    }
    
    // Освобождаем старую матрицу
    free(matrix);
    cols = newCols;
    return newMatrix;
}

int main() {
    // Выделяем память под исходную матрицу 2x2
    double* arr = (double*)calloc(2 * 2, sizeof(double));
    
    // Заполняем матрицу с клавиатуры
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cin >> arr[i * 2 + j];
        }
    }

    // Вычисляем новые размеры матрицы
    int newRows = 2 + static_cast<int>(arr[0]);
    int newCols = 2 + static_cast<int>(arr[1]);
    
    // Выделяем память под новую матрицу
    double* matrix = (double*)calloc(newRows * newCols, sizeof(double));
    int a = static_cast<int>(arr[1]) + 2;
    int b = static_cast<int>(arr[1]) + 3;
    matrix[0] = arr[0];
    matrix[1] = arr[1];
    matrix[a] = arr[2];
    matrix[b] = arr[3];
    
    // Заполняем новую матрицу
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            if ((i == 0 && j == 0) || (i == 1 && j == 0) ||  
                (i == 1 && j == 1) || (i == 0 && j == 1)) {
                continue;
            }
            matrix[i * newCols + j] = (i - 1) * arr[2] + (j - 1) * arr[3];
        }
    }

    std::cout << "Begin matrix:" << std::endl;
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            std::cout << matrix[i * newCols + j] << " ";
        }
        std::cout << std::endl;
    }

    // Находим столбцы с нулями
    int k;
    int* zeroColumns = function(matrix, newRows, newCols, k);
    
    std::cout << "Zero column: ";
    for (int i = 0; i < k; i++) {
        std::cout << zeroColumns[i] << " ";
    }
    std::cout << std::endl;

    // Удаляем столбцы с нулями
    if (k > 0) {
        matrix = removeColumns(matrix, newRows, newCols, zeroColumns, k);
        
        std::cout << "Matrix after delete columns:" << std::endl;
        for (int i = 0; i < newRows; ++i) {
            for (int j = 0; j < newCols; ++j) {
                std::cout << matrix[i * newCols + j] << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No columns for delete:" << std::endl;
    }
    //Пункт 2
    double* x = new double;
    double* y = new double;
    *x = 0.0;
    *y = 0.0;
    std::cin>>*x;
    std::cin>>*y;
    std::cout<<*x<<' '<<*y<<std::endl;
    double* ptr = x;
    double* ptr1 = y;
    *ptr *= 3;
    std::swap(*ptr,*ptr1);
    std::cout<<*ptr<<" "<<*ptr1<<std::endl;
    delete x;
    delete y;

}

#include <iostream>
#include <cstdlib>

int* function(double** matrix, int n, int w, int* k) {
    // Создаем массив флагов для отслеживания столбцов с нулями
    bool* columnFlags = (bool*)calloc(w, sizeof(bool));
    *k = 0;

    // Проходим по матрице и отмечаем столбцы с нулями
    for (int j = 0; j < w; ++j) {
        for (int i = 0; i < n; ++i) {
            if (matrix[i][j] == 0) {
                columnFlags[j] = true;
                (*k)++;
                break; 
            }
        }
    }

    // Выделяем память под результат
    int* result = 0;
    if (*k > 0) {
        result = (int*)malloc((*k) * sizeof(int));
        int index = 0;
        // Заполняем результат индексами столбцов
        for (int j = 0; j < w; ++j) {
            if (columnFlags[j]) {
                result[index++] = j;
            }
        }
    }

    // Освобождаем память флагов
    free(columnFlags);
    return result;
}

// Функция для удаления столбцов 
void removeColumns(double** matrix, int rows, int& cols, int* columnsToRemove, int k) {
    if (k == 0) {
        return;
    }

    int newCols = cols - k;

    // Для каждой строки перераспределяем память с помощью realloc
    for (int i = 0; i < rows; i++) {
        int newJ = 0;

        // Сначала сдвигаем элементы влево в существующем массиве
        for (int j = 0; j < cols; j++) {
            bool skip = false;
            for (int idx = 0; idx < k; idx++) {
                if (j == columnsToRemove[idx]) {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                // Перемещаем элемент на новую позицию
                matrix[i][newJ] = matrix[i][j];
                newJ++;
            }
        }

        // Теперь изменяем размер строки с помощью realloc
        matrix[i] = (double*)realloc(matrix[i], newCols * sizeof(double));
    }

    cols = newCols;
}

// Функция для создания двумерного массива
double** createMatrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)calloc(cols, sizeof(double));
    }
    return matrix;
}

// Функция для освобождения памяти двумерного массива
void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Функция для вывода матрицы
void printMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    // Выделяем память под исходную матрицу 2x2
    double** arr = createMatrix(2, 2);

    // Заполняем матрицу с клавиатуры
    std::cout << "Enter 4 numbers for 2x2 matrix:" << std::endl;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cin >> arr[i][j];
        }
    }

    // Проверка на отрицательные числа в первой строке
    if (arr[0][0] < 0 || arr[0][1] < 0) {
        std::cout << "Error: Elements in first row cannot be negative!" << std::endl;
        freeMatrix(arr, 2);
        return 1;
    }

    // Вычисляем новые размеры матрицы
    int newRows = 2 + static_cast<int>(arr[0][0]);
    int newCols = 2 + static_cast<int>(arr[0][1]);

    // Проверка на корректность размеров
    if (newRows <= 0 || newCols <= 0) {
        std::cout << "Error: Invalid matrix dimensions!" << std::endl;
        freeMatrix(arr, 2);
        return 1;
    }

    // Выделяем память под новую матрицу

    double** matrix = createMatrix(newRows, newCols);

    // Копируем исходные значения
    matrix[0][0] = arr[0][0];
    matrix[0][1] = arr[0][1];
    matrix[1][0] = arr[1][0];
    matrix[1][1] = arr[1][1];

    // Заполняем новую матрицу
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            // Пропускаем уже заполненные ячейки
            if ((i == 0 && j == 0)||(i==0&&j==1)||
            (i == 1 && j == 0) || (i == 1 && j == 1)) {
                continue;
            }
            matrix[i][j] = (i - 1) * arr[1][0] + (j - 1) * arr[1][1];
        }
    }

    std::cout << "Begin matrix:" << std::endl;
    printMatrix(matrix, newRows, newCols);

 
    int temp_k;
    int* zeroColumns = function(matrix, newRows, newCols, &temp_k);
    int k = temp_k; 

    std::cout << "Number of zero columns: " << k << std::endl;

    if (k > 0 && zeroColumns != 0) {
        std::cout << "Zero columns indices: ";
        for (int i = 0; i < k; i++) {
            std::cout << zeroColumns[i] << " ";
        }
        std::cout << std::endl;
    }

    // Сохраняем исходное количество столбцов
    int originalCols = newCols;

    // Удаляем столбцы с нулями (изменяем существующую матрицу)
    if (k > 0 && zeroColumns != 0) {
        removeColumns(matrix, newRows, newCols, zeroColumns, k);

        std::cout << "Matrix after deleting columns:" << std::endl;
        printMatrix(matrix, newRows, newCols);

        std::cout << "Old columns: " << originalCols << ", New columns: " << newCols << std::endl;

        // Освобождаем память zeroColumns
        free(zeroColumns);
    }
    else {
        std::cout << "No columns to delete" << std::endl;
    }

    // Освобождаем память
    freeMatrix(arr, 2);
    freeMatrix(matrix, newRows);
    //Пункт 2
    double* x = new double;
    double* y = new double;
    std::cin >> *x;
    std::cin >> *y;
    std::cout << *x << ' ' << *y << std::endl;

    double* ptr = x;
    double* ptr1 = y;

    
    *ptr = *ptr * 3;

    double* temp = new double;
    *temp = *ptr;   
    *ptr = *ptr1;    
    *ptr1 = *temp;   
    delete temp;

    std::cout << *ptr << " " << *ptr1 << std::endl;

    delete x;
    delete y;

    return 0;
}



        







