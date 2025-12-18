#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace Eigen;
using namespace std;

void computeSVD(const MatrixXd &A, MatrixXd &U, VectorXd &S, MatrixXd &V)
{
    int m = A.rows();
    int n = A.cols();

    MatrixXd AtA = A.transpose() * A;
    SelfAdjointEigenSolver<MatrixXd> eigenSolverV(AtA);
    VectorXd eigenValues = eigenSolverV.eigenvalues();
    MatrixXd eigenVectors = eigenSolverV.eigenvectors();

    vector<pair<double, int>> pairs;
    for (int i = 0; i < eigenValues.size(); i++)
    {
        pairs.push_back(make_pair(eigenValues(i), i));
    }
    sort(pairs.rbegin(), pairs.rend());

    V = MatrixXd(n, n);
    int rank = min(m, n);
    S = VectorXd(rank);

    for (int i = 0; i < n; i++)
    {
        int idx = pairs[i].second;
        V.col(i) = eigenVectors.col(idx);
    }

    for (int i = 0; i < rank; i++)
    {
        double eigenVal = pairs[i].first;
        S(i) = (eigenVal > 1e-14) ? sqrt(eigenVal) : 0.0;
    }

    U = MatrixXd::Zero(m, m);

    for (int j = 0; j < m; j++)
    {
        VectorXd v;

        if (j < rank && S(j) > 1e-14)
        {
            v = A * V.col(j) / S(j);
        }
        else
        {

            v = VectorXd::Random(m);
        }

        for (int k = 0; k < j; k++)
        {
            v = v - v.dot(U.col(k)) * U.col(k);
        }

        double norm = v.norm();
        if (norm > 1e-12)
        {
            U.col(j) = v / norm;
        }
        else
        {

            for (int attempt = 0; attempt < m; attempt++)
            {
                v = VectorXd::Zero(m);
                v((j + attempt) % m) = 1.0;

                for (int k = 0; k < j; k++)
                {
                    v = v - v.dot(U.col(k)) * U.col(k);
                }

                norm = v.norm();
                if (norm > 1e-12)
                {
                    U.col(j) = v / norm;
                    break;
                }
            }

            if (U.col(j).norm() < 1e-12)
            {
                v = VectorXd::Random(m);
                for (int k = 0; k < j; k++)
                {
                    v = v - v.dot(U.col(k)) * U.col(k);
                }
                U.col(j) = v.normalized();
            }
        }
    }
}

int main()
{
    // Du lieu dau vao: Ma tran A (4x3)
    int m = 4, n = 3;
    MatrixXd A(m, n);
    A << 1, 2, 3,
         4, 5, 6,
         7, 8, 9,
         10, 11, 12;

    cout << "=== PHAN TICH SVD (Phien ban 2) ===\n";
    cout << "\nMa tran A (" << m << "x" << n << "):" << endl
         << A << endl
         << endl;

    MatrixXd U, V;
    VectorXd S;
    computeSVD(A, U, S, V);

    cout << "Ma tran U (" << U.rows() << "x" << U.cols() << "):" << endl
         << U << endl
         << endl;
    cout << "Duong cheo ma tran S:" << endl
         << S << endl
         << endl;
    cout << "Ma tran V (" << V.rows() << "x" << V.cols() << "):" << endl
         << V << endl
         << endl;

    MatrixXd A_reconstructed = MatrixXd::Zero(m, n);
    int rank = S.size();
    for (int i = 0; i < rank; i++)
    {
        A_reconstructed += S(i) * U.col(i) * V.col(i).transpose();
    }

    cout << "Kiểm tra A = U * S * V^T:" << endl
         << A_reconstructed << endl
         << endl;
    cout << "Sai số tái tạo: " << (A - A_reconstructed).norm() << endl;
    return 0;
}