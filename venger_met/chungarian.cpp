#include "chungarian.h"


double CHungarian::calculate(std::vector <std::vector<double> > &data,      // матрица C
                             std::vector<int> &availability,                // вектор a
                             std::vector<int> &demand,                      // вектор b
                             std::vector<int> &result,                      // результат - выбранные ячейки - result[row]=col
                             std::vector<std::vector<int> > &res)           // финальная матрица
{
    // готовим данные
    std::vector <std::vector<double> > newData(data.size());
    for (int i = 0; i < int(newData.size()); i++)
    {
        newData[i].resize(data[i].size());
    }
    prepareData(data, availability, demand, newData);

    int m = newData.size();
    int n = newData[0].size();

    std::vector<double> data_0(m * n);
    std::vector<int> result_0(m);
    double cost = 0.0;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            data_0[j + n * i] = newData[i][j];

    // вычисляем
    std::vector<double> data0;
    calculate(result_0, cost, data_0, m, n, data0);

    // завершаем обработку
    finalize(data.size(), data[0].size(), result_0, result, res);
    // Ну и ещё надо стоимость пересчитать на случай, если были фиктивные элементы
    double cost_new = 0;
    for (int r = 0; r < int(data.size()); r++)
    {
        for (int c = 0; c < int(data[0].size()); c++)
        cost_new += data[r][c] * res[r][c];
    }

    return cost_new;
}


void CHungarian::prepareData(std::vector <std::vector<double> > &data,
                 std::vector<int> &availability,
                 std::vector<int> &demand,
                 std::vector <std::vector<double> > &newData)
{
    int m = availability.size(), n = demand.size();
    orig_a.resize(m);
    orig_b.resize(n);
    int totalAvailable = 0, totalDemand = 0;

    // считаем суммарные запасы и запросы, копируем a и b
    for (int i = 0; i < m; i++)
    {
        totalAvailable += availability[i];
        orig_a[i] = availability[i];
    }
    for (int j = 0; j < n; j++)
    {
        totalDemand += demand[j];
        orig_b[j] = demand[j];
    }

    // В эти членах класса будем хранить излишки по запросам и по предложению,
    // они понадобятся в завершающей фазе расчёта
    dummyConsumerQty = dummySupplierQty = 0;

    if (totalAvailable > totalDemand)
    {
        // На базах излишки - вводим фиктивного потребителя с нулевой стоимостью перевозки и заявкой на излишек
        dummyConsumerQty = totalAvailable - totalDemand;
        totalDemand = totalAvailable;
        new_b.resize(n + 1);
        new_b[n] = dummyConsumerQty;
        new_a.resize(m);
    }
    else if (totalAvailable < totalDemand)
    {
        // На базах не хватает запасов - вводим фиктивную базу с нулевой стоимостью и необходимым количеством
        dummySupplierQty = totalDemand - totalAvailable;
        totalAvailable = totalDemand;
        new_a.resize(m + 1);
        new_a[m] = dummySupplierQty;
        new_b.resize(n);
    }
    else
    {
        new_a.resize(m);
        new_b.resize(n);
    }
    // осталось скопировать orig_a, orig_b в new_a, new_b, которые нам понадобятся в finalize()
    for (int i = 0; i < m; i++)
    {
        new_a[i] = orig_a[i];
    }
    for (int j = 0; j < n; j++)
    {
        new_b[j] = orig_b[j];
    }

    // Сведём задачу к задаче назначений
    // Для этого каждую базу с запасом a[i] будем рассматривать как a[i] баз с запасом 1,
    // а каждого потребителя с требованием b[i] - как b[i] потребителей с запасом 1
    // К этому моменту totalAvailable = totalDemand, т.е. новая матрица C - квадратная.
    newData.resize(totalAvailable);
    for (int i = 0; i < totalAvailable; i++)
    {
        newData[i].resize(totalDemand);
    }

    // Заполняем новую матрицу. Каждая ячейка (i, j) в исходной матрице соответствует
    // подматрице размером a[i] x b[j] в новой матрице, значения в клетках равны C[i, j]
    int startRow = 0;
    for (int i = 0; i < m; i++)
    {
        for (int i1 = startRow; i1 < startRow + availability[i]; i1++)
        {
            int startCol = 0;
            for (int j = 0; j < n; j++)
            {
                for (int j1 = startCol; j1 < startCol + demand[j]; j1++)
                {
                    newData[i1][j1] = data[i][j];
                }
                startCol += demand[j];
            }
        }
        startRow += availability[i];
    }
    // Если есть фиктивный потребитель, заполняем соответствующие ему последние столбцы нулями
    for (int i = 0; i < totalAvailable; i++)
    {
        for (int j = totalDemand - dummyConsumerQty; j < totalDemand; j++)
        {
            newData[i][j] = 0;
        }
    }
    // Если есть фиктивная база, заполняем соответствующие ей последние строки нулями
    for (int j = 0; j < totalDemand; j++)
    {
        for (int i = totalAvailable - dummySupplierQty; i < totalAvailable; i++)
        {
            newData[i][j] = 0;
        }
    }
    // Ну и всё. demand и availability для новой матрицы уже не нужны, там все значения будут равны 1
}


void CHungarian::finalize(const int mOrig, const int nOrig,
              const std::vector<int> &resultWork,
              std::vector<int> &result,
              std::vector<std::vector<int> > &dataOut)
{
    // Мы должны свернуть вектор resultWork и матрицу dataWork к их исходной конфигурации

    // Инициализируем выходные параметры
    result.resize(mOrig, -1);
    dataOut.resize(mOrig);
    for (int i = 0; i < mOrig; i++)
    {
        dataOut[i].resize(nOrig);
        for (int j = 0; j < nOrig; j++)
        {
            dataOut[i][j] = 0;
        }
    }

    // Результат (выбранные ячейки) у нас в векторе resultWork: resultWork[row] = col
    // Мы должны пройти по вектору resultWork, по элементу (row, resultWork[row]) вычислить
    // соответствующую ячейку исходной матрицы и прибавить к её содержимому единицу.
    // Кроме того, мы помним о возможном существовании фиктивных строки или столбца.

    for (int i = 0; i < int(resultWork.size()); i++)
    {
        const int targetRow = getIndex(i, new_a);
        // Если была добавлена фиктивная строка, и мы в неё попали, то игнорируем
        if (targetRow >= mOrig)
            continue;
        const int targetCol = getIndex(resultWork[i], new_b);
        // Если был добавлен фиктивный столбец, и мы в него попали, то игнорируем
        if (targetCol >= nOrig)
            continue;
        dataOut[targetRow][targetCol]++;
        result[targetRow] = targetCol;
    }
}


void CHungarian::calculate(std::vector<int> &res, double &cost, std::vector<double> &data, const int m, const int n, std::vector<double> &data0)
{
    int idx;
    double value, minValue;
    std::vector<bool> usedColumns, usedRows, dataStar, dataStarNew, dataPrime;
    int nItems, minDimension, row, col;

    // Инициализация результатов
    cost = 0;
    for (row = 0; row < m; row++)
        res[row] = -1;

    // Создадим рабочие копии матриц и проверим, что все элементы неотрицательны
    nItems = m * n;
    data0.resize(nItems);
    //distMatrixEnd = distMatrix + nOfElements;

    for (row = 0; row < nItems; row++)
    {
        value = data[row];
        if (value < 0)
            std::cerr << "All matrix elements have to be non-negative." << std::endl;
        data0[row] = value;
    }


    // Выделение памяти
    usedColumns.resize(n);
    usedRows.resize(m);
    dataStar.resize(nItems);
    dataPrime.resize(nItems);
    dataStarNew.resize(nItems);

    // Подготовка
    if (m <= n)
    {
        minDimension = m;

        for (row = 0; row < m; row++)
        {
            // Находим минимальный элемент в строке
            idx = row * n;
            minValue = data0[idx];
            for (col = 1; col < n; col++)
            {
                if (data0[idx + col] < minValue)
                {
                    minValue = data0[idx + col];
                }
            }
            // Вычитаем найденный минимум из строки
            for (col = 0; col < n; col++)
            {
                data0[idx + col] -= minValue;
            }
        }

        // Пометим нули
        for (row = 0; row < m; row++)
        {
            for (col = 0; col < n; col++)
            {
                idx = row * n + col;
                if (fabs(data0[idx]) < std::numeric_limits<double>::epsilon())
                {
                    if (!usedColumns[col])
                    {
                        dataStar[idx] = true;
                        usedColumns[col] = true;
                        break;
                    }
                }
            }
        }
    }
    else /* if(nOfRows > nOfColumns) */
    {
        minDimension = n;

        for (col = 0; col < n; col++)
        {
            // Находим минимальный элемент в стобце
            idx = col;
            minValue = data0[idx];
            for (row = 1; row < m; row++)
            {
                idx += n;
                if (data0[idx] < minValue)
                {
                    minValue = data0[idx];
                }
            }

            // Вычитаем найденный минимум из столбца
            for (row = 0; row < m; row++)
            {
                idx = col + row * n;
                data0[idx] -= minValue;
            }
        }

        // Пометим нули
        for (col = 0; col < n; col++)
        {
            for (row = 0; row < m; row++)
            {
                idx = col + n * row;
                if (fabs((data0[idx])) < std::numeric_limits<double>::epsilon())
                {
                    if (!usedRows[row])
                    {
                        dataStar[idx] = true;
                        usedColumns[col] = true;
                        usedRows[row] = true;
                        break;
                    }
                }
            }
        }
        for (row = 0; row < m; row++)
        {
            usedRows[row] = false;
        }
    }

    // Проверка помеченных столбцов
    step2b(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);

    /* compute cost and remove invalid assignments */
    calculateCost(res, cost, data, m, n);

    return;
}


void CHungarian::calculateCost(std::vector<int> &res, double &cost, std::vector<double> &data, const int m, const int n)
{
    int row, col;

    for (row = 0; row < m; row++)
    {
        col = res[row];
        if (col >= 0)
        {
            cost += data[row * n + col];
        }
    }
}


void CHungarian::buildResultVector(std::vector<int> &res, std::vector<bool> &dataStar, const int m, const int n)
{
    int row, col;

    for (row = 0; row < m; row++)
    {
        for (col = 0; col < n; col++)
        {
            if (dataStar[col + n * row])
            {
                res[row] = col;
            }
        }
    }
}


void CHungarian::step2a(std::vector<int> &res, std::vector<double> &data0,
           std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
           std::vector<bool> &dataPrime,
           std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
           const int m, const int n, const int minDimension)
{
    int row, col;

    // Пометим как использованные все столбцы, содержащие отмеченный звёздочкой ноль
    for (col = 0; col < n; col++)
    {
        for (row = 0; row < m; row++)
        {
            if (dataStar[col + n * row])
            {
                usedColumns[col] = true;
                break;
            }
        }
    }

    // Переходим к шагу 2b
    step2b(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);
}


void CHungarian::step2b(std::vector<int> &res, std::vector<double> &data0,
           std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
           std::vector<bool> &dataPrime,
           std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
           const int m, const int n, const int minDimension)
{
    int col, nUsedColumns;

    // Считаем использованные столбцы
    nUsedColumns = 0;
    for (col = 0; col < n; col++)
    {
        if (usedColumns[col])
        {
            nUsedColumns++;
        }
    }

    if (nUsedColumns == minDimension)
    {
        // Если использованы все, то мы закончили
        buildResultVector(res, dataStar, m, n);
    }
    else
    {
        // Переходим к шагу 3
        step3(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);
    }
}


void CHungarian::step3(std::vector<int> &res, std::vector<double> &data0,
           std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
           std::vector<bool> &dataPrime,
           std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
           const int m, const int n, const int minDimension)
{
    bool zerosFound;
    int row, col, starCol;

    zerosFound = true;
    while (zerosFound)
    {
        zerosFound = false;
        for (col = 0; col < n; col++)
        {
            if (!usedColumns[col])
            {
                for (row = 0; row < m; row++)
                {
                    const int idx = col + n * row;
                    if ((!usedRows[row]) && (fabs(data0[idx]) < std::numeric_limits<double>::epsilon()))
                    {
                        // Пометим 0
                        dataPrime[idx] = true;

                        // Найдём помеченный звёздочкой элемент в текущей строке
                        for (starCol = 0; starCol < n; starCol++)
                        {
                            if (dataStar[starCol + n * row])
                                break;
                        }

                        if (starCol == n) // Не нашли
                        {
                            // Переходим к шагу 4
                            step4(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension, row, col);
                            return;
                        }
                        else
                        {
                            usedRows[row] = true;
                            usedColumns[starCol] = false;
                            zerosFound = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    // Переходим к шагу 5
    step5(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);
}


void CHungarian::step4(std::vector<int> &res, std::vector<double> &data0,
           std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
           std::vector<bool> &dataPrime,
           std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
           const int m, const int n, const int minDimension,
           const int row, const int col)
{
    int starRow, starCol, primeRow, primeCol;
    int nItems = n * m;

    // Копия матрицы помеченных данных
    for (int k = 0; k < nItems; k++)
        dataStarNew[k] = dataStar[k];

    // Пометим текущий ноль
    dataStarNew[col + n * row] = true;

    // Найдём помеченный ноль в текущем столбце оригинальной матрицы
    starCol = col;
    for (starRow = 0; starRow < m; starRow++)
        if (dataStar[starCol + n * starRow])
            break;

    while (starRow < m)
    {
        // Снимем отметку с нуля в новой матрице
        dataStarNew[starCol + n * starRow] = false;

        // Найдём помеченный штрихом 0 в текущей строке
        primeRow = starRow;
        for (primeCol = 0; primeCol < n; primeCol++)
            if (dataPrime[primeCol + n * primeRow])
                break;

        // Пометим найденный элемент звёдочкой
        dataStarNew[primeCol + n * primeRow] = true;

        // Найдём помеченный элемент в текущем столбце
        starCol = primeCol;
        for (starRow = 0; starRow < m; starRow++)
            if (dataStar[starCol + n * starRow])
                break;
    }

    // Используем новую матрицу в качестве основной матрицы элементов, помеченных звёздочкой
    // Убираем все штрихи
    for (int k = 0; k < nItems; k++)
    {
        dataPrime[k] = false;
        dataStar[k] = dataStarNew[k];
    }
    // откроем все строки
    for (int k = 0; k < m; k++)
        usedRows[k] = false;

    // Переходим к шагу 2а
    step2a(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);
}


void CHungarian::step5(std::vector<int> &res, std::vector<double> &data0,
           std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
           std::vector<bool> &dataPrime,
           std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
           const int m, const int n, const int minDimension)
{
    double h, value;
    int row, col;

    // Находим наименьший неиспользованный элемент h
    h = std::numeric_limits<double>::max();
    for (row = 0; row < m; row++)
    {
        if (!usedRows[row])
        {
            for (col = 0; col < n; col++)
            {
                if (!usedColumns[col])
                {
                    value = data0[col + n * row];
                    if (value < h)
                        h = value;
                }
            }
        }
    }

    // Добавим h ко всем использованным строкам
    for (row = 0; row < m; row++)
    {
        if (usedRows[row])
        {
            for (col = 0; col < n; col++)
            {
                data0[col + n * row] += h;
            }
        }
    }

    // Вычтем h из всех неиспользованных столбцов
    for (col = 0; col < n; col++)
    {
        if (!usedColumns[col])
        {
            for (row = 0; row < m; row++)
            {
                data0[col + n * row] -= h;
            }
        }
    }

    // Переходим к шагу 3
    step3(res, data0, dataStar, dataStarNew, dataPrime, usedColumns, usedRows, m, n, minDimension);
}


int CHungarian::getIndex(const int k, const std::vector<int> &v)
{
    int idx = 0, sum = 0;
    while ((idx < int(v.size())) && (k >= sum))
    {
        sum += v[idx];
        idx++;
    }
    return idx - 1;
}
