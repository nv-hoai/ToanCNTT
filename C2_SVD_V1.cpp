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

    MatrixXd AAt = A * A.transpose();
    SelfAdjointEigenSolver<MatrixXd> eigenSolverU(AAt);
    VectorXd uEigenValues = eigenSolverU.eigenvalues();
    MatrixXd uEigenVectors = eigenSolverU.eigenvectors();

    vector<pair<double, int>> uPairs;
    for (int i = 0; i < uEigenValues.size(); i++)
    {
        uPairs.push_back(make_pair(uEigenValues(i), i));
    }
    sort(uPairs.rbegin(), uPairs.rend());

    U = MatrixXd(m, m);
    for (int i = 0; i < m; i++)
    {
        int idx = uPairs[i].second;
        U.col(i) = uEigenVectors.col(idx);
    }

    for (int i = 0; i < rank; i++)
    {
        if (S(i) > 1e-14)
        {
            VectorXd Av = A * V.col(i);
            VectorXd su = S(i) * U.col(i);
            if (Av.dot(su) < 0)
            {
                U.col(i) = -U.col(i);
            }
        }
    }
}

int main()
{
    // Du lieu dau vao: Ma tran A (3x2)
    int m = 3, n = 2;
    MatrixXd A(m, n);
    A << 1, 2,
         3, 4,
         5, 6;

    cout << "=== PHAN TICH SVD ===\n";
    cout << "\nMa tran A (" << m << "x" << n << "):" << endl
         << A << endl
         << endl;

    MatrixXd U, V;
    VectorXd S;
    computeSVD(A, U, S, V);

    cout << "Ma tran U (" << U.rows() << "x" << U.cols() << "):" << endl
         << U << endl
         << endl;
    cout << "Singular Values:" << endl
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
    cout << "Sai số: " << (A - A_reconstructed).norm() << endl;

    cout << "\n=== So sanh voi SVD co san ===" << endl;
    JacobiSVD<MatrixXd> svd(A, ComputeFullU | ComputeFullV);
    cout << "Singular values (Eigen SVD):" << endl
         << svd.singularValues() << endl;
    cout << "U (Eigen SVD):" << endl
         << svd.matrixU() << endl;
    cout << "V (Eigen SVD):" << endl
         << svd.matrixV() << endl;

    return 0;
}