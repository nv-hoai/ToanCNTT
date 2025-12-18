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

bool isPositiveDefinite(const MatrixXd& A)
{
    int n = A.rows();
    for (int k = 0; k < n; k++) {
        MatrixXd subMat = A.block(0, 0, k+1, k+1);
        if (subMat.determinant() <= 0)
            return false;
    }
    return true;
}

bool isSymmetric(const MatrixXd& A)
{
    const double EPS = 1e-10;
    int n = A.rows();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fabs(A(i, j) - A(j, i)) > EPS)
                return false;
        }
    }
    return true;
}

bool canPerformLDLT(const MatrixXd& A)
{
    if (!isSymmetric(A))
        return false;

    int n = A.rows();
    MatrixXd temp = A;
    const double EPS = 1e-14;
    
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int k = 0; k < i; k++) {
            sum += temp(i, k) * temp(i, k) * temp(k, k);
        }
        double pivot = temp(i, i) - sum;

        if (fabs(pivot) < EPS)
            return false;

        for (int j = i + 1; j < n; j++) {
            double sum2 = 0;
            for (int k = 0; k < i; k++) {
                sum2 += temp(j, k) * temp(i, k) * temp(k, k);
            }
            temp(j, i) = (temp(j, i) - sum2) / pivot;
        }
        temp(i, i) = pivot;
    }

    return true;
}

// Thuat toan Jacobi tu viet cho phan tich gia tri rieng
void EigenDecomposition(const MatrixXd& A, VectorXd& eigenValues, MatrixXd& eigenVectors)
{
    int n = A.rows();
    MatrixXd B = A;
    eigenVectors = MatrixXd::Identity(n, n);

    const int MAX_ITER = 100;
    const double EPS = 1e-10;

    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Tim phan tu ngoai duong cheo lon nhat
        double maxVal = 0;
        int p = 0, q = 1;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (fabs(B(i, j)) > maxVal) {
                    maxVal = fabs(B(i, j));
                    p = i;
                    q = j;
                }
            }
        }

        if (maxVal < EPS)
            break;

        // Tinh goc xoay
        double theta = 0.5 * atan2(2 * B(p, q), B(p, p) - B(q, q));
        double c = cos(theta), s = sin(theta);

        // Luu tam thoi
        MatrixXd temp = B;

        // Cap nhat ma tran B
        for (int i = 0; i < n; i++) {
            double Bip = temp(i, p), Biq = temp(i, q);
            B(i, p) = c * Bip + s * Biq;
            B(i, q) = -s * Bip + c * Biq;
            B(p, i) = B(i, p);
            B(q, i) = B(i, q);
        }

        double Bpp = temp(p, p), Bqq = temp(q, q), Bpq = temp(p, q);
        B(p, p) = c * c * Bpp + 2 * c * s * Bpq + s * s * Bqq;
        B(q, q) = s * s * Bpp - 2 * c * s * Bpq + c * c * Bqq;
        B(p, q) = B(q, p) = 0;

        // Cap nhat vector rieng
        for (int i = 0; i < n; i++) {
            double Vip = eigenVectors(i, p), Viq = eigenVectors(i, q);
            eigenVectors(i, p) = c * Vip + s * Viq;
            eigenVectors(i, q) = -s * Vip + c * Viq;
        }
    }

    // Lay gia tri rieng tu duong cheo
    eigenValues.resize(n);
    for (int i = 0; i < n; i++) {
        eigenValues(i) = B(i, i);
    }

    // Sap xep giam dan
    for (int i = 0; i < n-1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (eigenValues(i) < eigenValues(j)) {
                std::swap(eigenValues(i), eigenValues(j));
                eigenVectors.col(i).swap(eigenVectors.col(j));
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

void PrintEigenValues(const VectorXd& eigenValues)
{
    printf("Gia tri rieng:\n");
    for (int i = 0; i < eigenValues.size(); i++) {
        printf("λ%d = %8.4f\n", i + 1, eigenValues(i));
    }
}

bool verifyClassicalCholesky(const MatrixXd& A, const MatrixXd& L)
{
    MatrixXd LLT = L * L.transpose();
    const double EPS = 1e-10;
    return (A - LLT).norm() < EPS;
}

bool verifyVariantCholesky(const MatrixXd& A, const MatrixXd& L, const MatrixXd& D)
{
    MatrixXd LDLT = L * D * L.transpose();
    const double EPS = 1e-10;
    return (A - LDLT).norm() < EPS;
}

int main()
{
    // Du lieu dau vao: Ma tran A (doi xung, positive definite)
    int n = 4;
    MatrixXd A(n, n);
    A << 11, 2, 3, 1,
         2, 17, 9, 5,
         3, 9, 11, 4,
         1, 5, 4, 8;

    printf("Ma tran A (%dx%d):\n", n, n);
    PrintMatrix(A, "A");
    printf("\n");

    printf("=== KIEM TRA DIEU KIEN ===\n");
    printf("Ma tran co doi xung: %s\n", isSymmetric(A) ? "Co" : "Khong");
    printf("Ma tran co positive definite: %s\n", isPositiveDefinite(A) ? "Co" : "Khong");
    printf("Co the thuc hien LDL^T: %s\n", canPerformLDLT(A) ? "Co" : "Khong");
    printf("\n");

    if (isPositiveDefinite(A))
    {
        printf("=== CLASSICAL CHOLESKY DECOMPOSITION ===\n");
        MatrixXd L1(n, n);
        ClassicalCholesky(A, L1);
        printf("Classical Cholesky (A = L*L^T):\n");
        PrintMatrix(L1, "L");

        bool isCorrect = verifyClassicalCholesky(A, L1);
        printf("Kiem tra A = L*L^T: %s\n", isCorrect ? "✓ Dung" : "✗ Sai");
        printf("\n");
    }
    else
    {
        printf("=== CLASSICAL CHOLESKY DECOMPOSITION ===\n");
        printf("Khong the thuc hien Classical Cholesky - Ma tran khong positive definite!\n\n");
    }

    if (canPerformLDLT(A))
    {
        printf("=== VARIANT CHOLESKY (LDL^T) DECOMPOSITION ===\n");
        MatrixXd L2(n, n), D = MatrixXd::Zero(n, n);
        VariantCholesky(A, L2, D);
        printf("Variant Cholesky (A = L*D*L^T):\n");
        printf("Ma tran L:\n");
        PrintMatrix(L2, "L");
        printf("\nMa tran D:\n");
        PrintMatrix(D, "D");

        bool isCorrect = verifyVariantCholesky(A, L2, D);
        printf("Kiem tra A = L*D*L^T: %s\n", isCorrect ? "✓ Dung" : "✗ Sai");
        printf("\n");
    }
    else
    {
        printf("=== VARIANT CHOLESKY (LDL^T) DECOMPOSITION ===\n");
        printf("Khong the thuc hien LDL^T - Ma tran khong doi xung hoac co pivot = 0!\n\n");
    }

    if (isSymmetric(A))
    {
        printf("=== EIGEN DECOMPOSITION ===\n");
        VectorXd eigenValues(n);
        MatrixXd eigenVectors(n, n);

        EigenDecomposition(A, eigenValues, eigenVectors);

        PrintEigenValues(eigenValues);
        printf("\n");
        printf("Ma tran eigenvectors:\n");
        PrintMatrix(eigenVectors, "V");
        printf("\n");

        printf("Kiem tra A*v = λ*v:\n");
        for (int i = 0; i < n; i++)
        {
            printf("Eigenvector %d: ", i + 1);
            VectorXd v = eigenVectors.col(i);
            VectorXd Av = A * v;
            VectorXd lambdaV = eigenValues(i) * v;

            bool correct = (Av - lambdaV).norm() < 1e-5;
            printf("%s\n", correct ? "✓" : "✗");
        }
    }
    else
    {
        printf("=== EIGEN DECOMPOSITION ===\n");
        printf("Khong the thuc hien Eigen Decomposition - Ma tran khong doi xung!\n");
    }

    return 0;
}