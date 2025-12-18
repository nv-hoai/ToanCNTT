#include <stdio.h>
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

// Thuat toan Cholesky co dien tu viet: A = L * L^T
void ClassicalCholesky(const MatrixXd& A, MatrixXd& L)
{
    int n = A.rows();
    L = MatrixXd::Zero(n, n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L(i, k) * L(j, k);
            
            if (i == j)
                L(i, j) = sqrt(A(i, i) - sum);
            else
                L(i, j) = (A(i, j) - sum) / L(j, j);
        }
    }
}

// Thuat toan Cholesky bien the tu viet: A = L * D * L^T
void VariantCholesky(const MatrixXd& A, MatrixXd& L, MatrixXd& D)
{
    int n = A.rows();
    L = MatrixXd::Identity(n, n);
    D = MatrixXd::Zero(n, n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L(i, k) * D(k, k) * L(j, k);
            
            if (i == j) {
                D(i, i) = A(i, i) - sum;
            } else {
                L(i, j) = (A(i, j) - sum) / D(j, j);
            }
        }
    }
}

void PrintMatrix(const MatrixXd& mat, const char* name)
{
    printf("%s:\n", name);
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = 0; j < mat.cols(); j++) {
            printf("%8.4f ", mat(i, j));
        }
        printf("\n");
    }
}

int main()
{
    // Du lieu dau vao: Ma tran A (doi xung, xac dinh duong) 4x4
    int n = 4;
    MatrixXd A(n, n);
    A << 4, 12, -16, 2,
         12, 37, -43, 5,
         -16, -43, 98, -10,
         2, 5, -10, 15;

    printf("=== PHAN TICH CHOLESKY ===\n\n");
    PrintMatrix(A, "Ma tran A");
    printf("\n");

    // Classical Cholesky
    MatrixXd L1;
    ClassicalCholesky(A, L1);
    printf("Classical Cholesky (A = L * L^T):\n");
    PrintMatrix(L1, "Ma tran L");
    printf("\n");

    // Variant Cholesky
    MatrixXd L2, D;
    VariantCholesky(A, L2, D);
    printf("Variant Cholesky (A = L * D * L^T):\n");
    PrintMatrix(L2, "Ma tran L");
    printf("\n");
    PrintMatrix(D, "Ma tran D");
    
    return 0;
}