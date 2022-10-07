#ifndef CHUNGARIAN_H
#define CHUNGARIAN_H


#include <iostream>
#include <vector>
#include <limits>
#include <cmath>


class CHungarian
{
public:
    CHungarian() {};
    ~CHungarian() {};
    double calculate(std::vector <std::vector<double> > &data,      // матрица C
                     std::vector<int> &availability,                // вектор a
                     std::vector<int> &demand,                      // вектор b
                     std::vector<int> &result,                      // результат - выбранные ячейки - result[row]=col
                     std::vector<std::vector<int> > &res);          // финальная матрица

private:
    void prepareData(std::vector <std::vector<double> > &data,
                     std::vector<int> &availability,
                     std::vector<int> &demand,
                     std::vector <std::vector<double> > &newData);

    void calculate(std::vector<int> &res, double &cost, std::vector<double> &data, const int m, const int n, std::vector<double> &data0);

    void finalize(const int mOrig, const int nOrig,                 // размерность исходной матрицы
                  const std::vector<int> &resultWork,               // промежуточный результат - выбранные клетки, resultWork[row] = col
                  std::vector<int> &result,                         // финальный результат - выбранные клетки
                  std::vector<std::vector<int> > &dataOut);         // финальный результат - матрица доставки

    void buildResultVector(std::vector<int> &res, std::vector<bool> &dataStar, const int m, const int n);
    void calculateCost(std::vector<int> &res, double &cost, std::vector<double> &data, const int m, const int n);
    void step2a(std::vector<int> &res, std::vector<double> &data0,
               std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
               std::vector<bool> &dataPrime,
               std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
               const int m, const int n, const int minDimension);
    void step2b(std::vector<int> &res, std::vector<double> &data0,
               std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
               std::vector<bool> &dataPrime,
               std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
               const int m, const int n, const int minDimension);
    void step3(std::vector<int> &res, std::vector<double> &data0,
               std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
               std::vector<bool> &dataPrime,
               std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
               const int m, const int n, const int minDimension);
    void step4(std::vector<int> &res, std::vector<double> &data0,
               std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
               std::vector<bool> &dataPrime,
               std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
               const int m, const int n, const int minDimension,
               const int row, const int col);
    void step5(std::vector<int> &res, std::vector<double> &data0,
               std::vector<bool> &dataStar, std::vector<bool> &dataStarNew,
               std::vector<bool> &dataPrime,
               std::vector<bool> &usedColumns, std::vector<bool> &usedRows,
               const int m, const int n, const int minDimension);
    int getIndex(const int k, const std::vector<int> &v);

    int dummyConsumerQty, dummySupplierQty;
    std::vector<int> orig_a, orig_b;
    std::vector<int> new_a, new_b;
};

#endif // CHUNGARIAN_H
