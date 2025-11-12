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
        for (int j = 0; j < w; ++j) {
            if (columnFlags[j]) {
                result[index++] = j;
            }
        }
    }

    free(columnFlags);
    return result;
}

// Функция для удаления столбцов
void removeColumns(double** matrix, int rows, int& cols, int* columnsToRemove, int k) {
    if (k == 0) return;

    int newCols = cols - k;

    for (int i = 0; i < rows; i++) {
        int newJ = 0;
        for (int j = 0; j < cols; j++) {
            bool skip = false;
            for (int idx = 0; idx < k; idx++) {
                if (j == columnsToRemove[idx]) {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                matrix[i][newJ] = matrix[i][j];
                newJ++;
            }
        }
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

// Функция для изменения размера матрицы
void resizeMatrix(double*** matrix, int oldRows, int* oldCols, int newRows, int newCols) {
    // Изменяем количество строк
    *matrix = (double**)realloc(*matrix, newRows * sizeof(double*));

    // Для существующих строк изменяем количество столбцов
    for (int i = 0; i < oldRows; i++) {
        (*matrix)[i] = (double*)realloc((*matrix)[i], newCols * sizeof(double));
        // Инициализируем новые ячейки нулями
        for (int j = *oldCols; j < newCols; j++) {
            (*matrix)[i][j] = 0.0;
        }
    }

    // Создаем новые строки если нужно
    for (int i = oldRows; i < newRows; i++) {
        (*matrix)[i] = (double*)calloc(newCols, sizeof(double));
    }

    *oldCols = newCols;
}

// Функция для освобождения памяти
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

// Функция для ввода неотрицательного числа
double inputNonNegative(const std::string& prompt) {
    double value;
    do {
        std::cout << prompt;
        std::cin >> value;
        if (value < 0) {
            std::cout << "Error: Value cannot be negative! Please try again." << std::endl;
        }
    } while (value < 0);
    return value;
}

int main() {
    // Создаем одну матрицу 2x2
    int rows = 2;
    int cols = 2;
    double** matrix = createMatrix(rows, cols);

    // Заполняем матрицу с проверкой на отрицательные числа в первой строке
    std::cout << "Enter 4 numbers for 2x2 matrix:" << std::endl;

    matrix[0][0] = inputNonNegative("Enter element [0][0] (non-negative): ");
    matrix[0][1] = inputNonNegative("Enter element [0][1] (non-negative): ");

    std::cout << "Enter element [1][0]: ";
    std::cin >> matrix[1][0];
    std::cout << "Enter element [1][1]: ";
    std::cin >> matrix[1][1];


    std::cout << "Initial 2x2 matrix:" << std::endl;
    printMatrix(matrix, rows, cols);

    // Вычисляем новые размеры матрицы
    int newRows = 2 + static_cast<int>(matrix[0][0]);
    int newCols = 2 + static_cast<int>(matrix[0][1]);

    // Проверка на корректность размеров
    if (newRows <= 0 || newCols <= 0) {
        std::cout << "Error: Invalid matrix dimensions!" << std::endl;
        freeMatrix(matrix, rows);
        return 1;
    }

    // Расширяем существующую матрицу до новых размеров
    std::cout << "Expanding matrix to " << newRows << "x" << newCols << "..." << std::endl;
    resizeMatrix(&matrix, rows, &cols, newRows, newCols);
    rows = newRows;

    // Заполняем расширенную часть матрицы
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Пропускаем уже заполненные ячейки исходной матрицы 2x2
            if ((i == 0 && j == 0)||(i==0&&j==1)||
            (i == 1 && j == 0) || (i == 1 && j == 1)) {
                continue;
            }
            matrix[i][j] = (i - 1) * matrix[1][0] + (j - 1) * matrix[1][1];
        }
    }

    std::cout << "Expanded matrix:" << std::endl;
    printMatrix(matrix, rows, cols);

    // Находим столбцы с нулями
    int temp_k;
    int* zeroColumns = function(matrix, rows, cols, &temp_k);
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
    int originalCols = cols;

    // Удаляем столбцы с нулями
    if (k > 0 && zeroColumns != 0) {
        removeColumns(matrix, rows, cols, zeroColumns, k);

        std::cout << "Matrix after deleting columns:" << std::endl;
        printMatrix(matrix, rows, cols);

        std::cout << "Old columns: " << originalCols << ", New columns: " << cols << std::endl;

        free(zeroColumns);
    }
    else {
        std::cout << "No columns to delete" << std::endl;
    }

    // Освобождаем память
    freeMatrix(matrix, rows);
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



        







