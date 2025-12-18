#include <iostream>
#include <Eigen/Dense>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using namespace Eigen;

// In ma tran chuyen trang thai
void printTransitionMatrix(const MatrixXd& P, const vector<string>& states)
{
    int n = P.rows();
    cout << "MA TRAN CHUYEN TRANG THAI P:\n";
    cout << "(Hang: trang thai hien tai, Cot: trang thai ke tiep)\n\n";
    
    cout << "     ";
    for (int j = 0; j < n; j++)
    {
        cout << setw(8) << states[j];
    }
    cout << "\n";
    cout << "   " << string(n * 8 + 2, '-') << "\n";
    
    for (int i = 0; i < n; i++)
    {
        cout << setw(3) << states[i] << " |";
        for (int j = 0; j < n; j++)
        {
            cout << fixed << setprecision(4) << setw(8) << P(i, j);
        }
        cout << "\n";
    }
    cout << "\n";
}

// Tinh xac suat sau n buoc thoi gian: p_n = p_0 * P^n
VectorXd computeStateAfterNSteps(const VectorXd& p0, const MatrixXd& P, int n)
{
    VectorXd p = p0;
    for (int i = 0; i < n; i++)
    {
        p = (p.transpose() * P).transpose();
    }
    return p;
}

// Tinh phan phoi dung (steady state distribution)
VectorXd computeSteadyState(const MatrixXd& P, double epsilon = 1e-10, int maxIter = 10000)
{
    int n = P.rows();
    VectorXd pi = VectorXd::Ones(n) / n;
    
    for (int iter = 0; iter < maxIter; iter++)
    {
        VectorXd pi_new = (pi.transpose() * P).transpose();
        
        if ((pi_new - pi).norm() < epsilon)
        {
            return pi_new;
        }
        
        pi = pi_new;
    }
    
    return pi;
}

// In vector xac suat
void printProbabilityVector(const VectorXd& p, const vector<string>& states, const string& label)
{
    cout << label << " = [";
    for (int i = 0; i < p.size(); i++)
    {
        cout << fixed << setprecision(4) << p(i);
        if (i < p.size() - 1) cout << ", ";
    }
    cout << "]\n";
    
    cout << "Chi tiet:\n";
    for (int i = 0; i < p.size(); i++)
    {
        cout << "  " << states[i] << ": " << fixed << setprecision(2) 
             << (p(i) * 100) << "%\n";
    }
}

// Kiem tra tinh chat ma tran stochastic
bool isStochasticMatrix(const MatrixXd& P, double epsilon = 1e-6)
{
    int n = P.rows();
    for (int i = 0; i < n; i++)
    {
        double rowSum = 0.0;
        for (int j = 0; j < n; j++)
        {
            if (P(i, j) < 0) return false;
            rowSum += P(i, j);
        }
        if (abs(rowSum - 1.0) > epsilon) return false;
    }
    return true;
}

int main()
{
    cout << "=== CHUOI MARKOV - PHAN TICH CHUYEN TRANG THAI ===\n\n";
    
    // Du lieu dau vao: Dinh nghia cac trang thai
    vector<string> states = {"R", "A", "P", "D"};
    int n = states.size();
    
    // Ma tran chuyen trang thai P (nxn)
    // P(i,j) = xac suat chuyen tu trang thai i sang trang thai j
    MatrixXd P(n, n);
    
    // R(giau) - hang 0
    P(0, 0) = 0.0;   P(0, 1) = 0.75;  P(0, 2) = 0.2;   P(0, 3) = 0.05;
    // A(trung binh) - hang 1
    P(1, 0) = 0.05;  P(1, 1) = 0.2;   P(1, 2) = 0.3;   P(1, 3) = 0.45;
    // P(ngheo) - hang 2
    P(2, 0) = 0.1;   P(2, 1) = 0.4;   P(2, 2) = 0.3;   P(2, 3) = 0.2;
    // D(no) - hang 3
    P(3, 0) = 0.0;   P(3, 1) = 0.15;  P(3, 2) = 0.3;   P(3, 3) = 0.55;
    
    // Trang thai ban dau: bat dau tu trang thai A
    VectorXd p0 = VectorXd::Zero(n);
    p0(1) = 1.0;
    
    // Kiem tra tinh hop le cua ma tran
    cout << "1. KIEM TRA MA TRAN CHUYEN TRANG THAI:\n";
    if (isStochasticMatrix(P))
    {
        cout << "   Ma tran P la ma tran stochastic hop le!\n";
        cout << "   (Tong moi hang = 1, tat ca phan tu >= 0)\n\n";
    }
    else
    {
        cout << "   Canh bao: Ma tran P khong hop le!\n\n";
    }
    
    // In ma tran chuyen trang thai
    printTransitionMatrix(P, states);
    
    // Trang thai ban dau
    cout << "\n2. TRANG THAI BAN DAU:\n";
    printProbabilityVector(p0, states, "p0");
    
    // Tinh xac suat sau cac buoc
    cout << "\n\n3. TRANG THAI SAU NHIEU BUOC THOI GIAN:\n";
    
    vector<int> steps = {1, 2, 3, 5, 10};
    for (int step : steps)
    {
        VectorXd p_n = computeStateAfterNSteps(p0, P, step);
        cout << "\nSau " << step << " buoc:\n";
        printProbabilityVector(p_n, states, "p" + to_string(step));
    }
    
    // Tinh phan phoi dung
    cout << "\n\n4. PHAN PHOI DUNG (STEADY STATE):\n";
    VectorXd pi = computeSteadyState(P);
    cout << "Phan phoi xac suat khi t -> vo cung:\n";
    printProbabilityVector(pi, states, "pi");
    
    // Kiem tra phan phoi dung
    VectorXd pi_check = (pi.transpose() * P).transpose();
    double error = (pi_check - pi).norm();
    cout << "\nKiem tra: ||pi * P - pi|| = " << scientific << setprecision(2) << error << "\n";
    
    cout << "\n\n=== KET LUAN ===\n";
    cout << "Bat dau tu trang thai " << states[1] << ", he thong se hoi tu ve phan phoi dung.\n";
    cout << "Trong dai han, xac suat o trang thai " << states[0] 
         << " la " << fixed << setprecision(2) << (pi(0) * 100) << "%\n";
    
    return 0;
}
