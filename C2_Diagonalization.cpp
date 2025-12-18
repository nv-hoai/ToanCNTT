#include <iostream>
#include <cmath>
#include <iomanip>
#include <Eigen/Dense>

#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using Eigen::MatrixXd;

// Thuat toan Jacobi tu viet - Cheo hoa ma tran doi xung
void JacobiDiagonalization(const MatrixXd& A, MatrixXd& P, MatrixXd& D)
{
    int n = A.rows();
    const double EPS = 1e-9;
    
    // Khoi tao P = I (ma tran don vi)
    P = MatrixXd::Identity(n, n);
    
    // D = A (ban sao de xu ly)
    D = A;
    
    bool converged = false;
    int maxIterations = 50;
    
    for (int iter = 0; iter < maxIterations && !converged; iter++)
    {
        // Tim phan tu off-diagonal lon nhat
        double maxOffDiag = 0;
        int p = 0, q = 1;
        
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (abs(D(i, j)) > maxOffDiag)
                {
                    maxOffDiag = abs(D(i, j));
                    p = i;
                    q = j;
                }
            }
        }
        
        // Kiem tra hoi tu
        if (maxOffDiag < EPS)
        {
            converged = true;
            break;
        }
        
        // Tinh goc quay theta
        double theta;
        if (abs(D(p, p) - D(q, q)) < EPS)
        {
            theta = M_PI / 4;
        }
        else
        {
            theta = 0.5 * atan(2 * D(p, q) / (D(p, p) - D(q, q)));
        }
        
        double c = cos(theta);
        double s = sin(theta);
        
        // Luu lai cac gia tri can thiet
        MatrixXd temp = D;
        
        // Cap nhat D theo cong thuc Jacobi
        for (int i = 0; i < n; i++)
        {
            if (i != p && i != q)
            {
                D(i, p) = c * temp(i, p) + s * temp(i, q);
                D(p, i) = D(i, p);
                D(i, q) = -s * temp(i, p) + c * temp(i, q);
                D(q, i) = D(i, q);
            }
        }
        
        D(p, p) = c * c * temp(p, p) + 2 * c * s * temp(p, q) + s * s * temp(q, q);
        D(q, q) = s * s * temp(p, p) - 2 * c * s * temp(p, q) + c * c * temp(q, q);
        D(p, q) = D(q, p) = 0;
        
        // Cap nhat ma tran eigenvector P
        for (int i = 0; i < n; i++)
        {
            double temp_p = P(i, p);
            double temp_q = P(i, q);
            P(i, p) = c * temp_p + s * temp_q;
            P(i, q) = -s * temp_p + c * temp_q;
        }
    }
}

void PrintMatrix(const MatrixXd& matrix, const string& name)
{
    int n = matrix.rows();
    int m = matrix.cols();
    
    cout << "\nMa tran " << name << " (" << n << "x" << m << "):\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << setw(10) << fixed << setprecision(4) << matrix(i, j) << " ";
        }
        cout << "\n";
    }
}

int main()
{
    // Du lieu dau vao: Ma tran doi xung nxn
    int n = 4;
    MatrixXd A(n, n);
    A << 4, -1, 1, 0,
        -1, 3, -2, 1,
         1, -2, 3, -1,
         0, 1, -1, 2;

    cout << "=== CHEO HOA MA TRAN ===\n";
    cout << "Ma tran A ban dau:";
    PrintMatrix(A, "A");
    
    // Thuc hien cheo hoa
    MatrixXd P(n, n), D(n, n);
    JacobiDiagonalization(A, P, D);
    
    // Hien thi ket qua
    PrintMatrix(P, "P (Eigenvectors)");
    PrintMatrix(D, "D (Eigenvalues - duong cheo)");
    
    // Tinh P^(-1) = P^T (vi P la ma tran truc giao)
    MatrixXd P_inv = P.transpose();
    PrintMatrix(P_inv, "P^(-1)");
    
    // Kiem tra A = P * D * P^(-1)
    MatrixXd result = P * D * P_inv;
    cout << "\nKiem tra A = P * D * P^(-1):";
    PrintMatrix(result, "P*D*P^(-1)");
    
    // Kiem tra sai so
    double error = (A - result).norm();
    cout << "\nSai so ||A - P*D*P^(-1)||: " << fixed << setprecision(10) << error << "\n";

    return 0;
}
