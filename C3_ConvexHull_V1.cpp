#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <ctime>

using namespace std;

struct Point
{
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    bool operator<(const Point &other) const
    {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point &other) const
    {
        return abs(x - other.x) < 1e-9 && abs(y - other.y) < 1e-9;
    }
};

double crossProduct(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

double distance(const Point &a, const Point &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

vector<Point> andrewConvexHull(vector<Point> points)
{
    int n = points.size();
    if (n <= 1)
        return points;

    sort(points.begin(), points.end());

    vector<Point> lower;
    for (int i = 0; i < n; i++)
    {
        while (lower.size() >= 2 &&
               crossProduct(lower[lower.size() - 2], lower[lower.size() - 1], points[i]) <= 0)
        {
            lower.pop_back();
        }
        lower.push_back(points[i]);
    }

    vector<Point> upper;
    for (int i = n - 1; i >= 0; i--)
    {
        while (upper.size() >= 2 &&
               crossProduct(upper[upper.size() - 2], upper[upper.size() - 1], points[i]) <= 0)
        {
            upper.pop_back();
        }
        upper.push_back(points[i]);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

double calculateArea(const vector<Point> &hull)
{
    int n = hull.size();
    if (n < 3)
        return 0.0;

    double area = 0.0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        area += hull[i].x * hull[j].y;
        area -= hull[j].x * hull[i].y;
    }
    return abs(area) / 2.0;
}

double findShortestEdge(const vector<Point> &hull)
{
    int n = hull.size();
    if (n < 2)
        return 0.0;

    double minEdge = 1e9;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        double edgeLength = distance(hull[i], hull[j]);
        minEdge = min(minEdge, edgeLength);
    }
    return minEdge;
}

bool pointInConvexHull(const Point &p, const vector<Point> &hull)
{
    int n = hull.size();
    if (n < 3)
        return false;

    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        if (((hull[i].y > p.y) != (hull[j].y > p.y)) &&
            (p.x < (hull[j].x - hull[i].x) * (p.y - hull[i].y) / (hull[j].y - hull[i].y) + hull[i].x))
        {
            inside = !inside;
        }
    }
    return inside;
}

vector<Point> findInteriorPoints(const vector<Point> &allPoints, const vector<Point> &hull)
{
    vector<Point> interior;
    for (const Point &p : allPoints)
    {
        bool isHullPoint = false;
        for (const Point &hp : hull)
        {
            if (p == hp)
            {
                isHullPoint = true;
                break;
            }
        }
        if (!isHullPoint && pointInConvexHull(p, hull))
        {
            interior.push_back(p);
        }
    }
    return interior;
}

double closestPairRec(vector<Point> &px, vector<Point> &py, int n)
{

    if (n <= 3)
    {
        double minDist = 1e9;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                minDist = min(minDist, distance(px[i], px[j]));
            }
        }
        return minDist;
    }

    int mid = n / 2;
    Point midPoint = px[mid];

    vector<Point> pyl, pyr;
    for (int i = 0; i < n; i++)
    {
        if (py[i].x <= midPoint.x)
        {
            pyl.push_back(py[i]);
        }
        else
        {
            pyr.push_back(py[i]);
        }
    }

    vector<Point> pxl(px.begin(), px.begin() + mid);
    vector<Point> pxr(px.begin() + mid, px.end());
    double dl = closestPairRec(pxl, pyl, mid);
    double dr = closestPairRec(pxr, pyr, n - mid);

    double d = min(dl, dr);

    vector<Point> strip;
    for (int i = 0; i < n; i++)
    {
        if (abs(py[i].x - midPoint.x) < d)
        {
            strip.push_back(py[i]);
        }
    }

    for (int i = 0; i < strip.size(); i++)
    {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; j++)
        {
            d = min(d, distance(strip[i], strip[j]));
        }
    }

    return d;
}

double findShortestDistance(const vector<Point> &points)
{
    if (points.size() < 2)
        return 0.0;

    vector<Point> px = points;
    vector<Point> py = points;

    sort(px.begin(), px.end());
    sort(py.begin(), py.end(), [](const Point &a, const Point &b)
         { return a.y < b.y; });

    return closestPairRec(px, py, points.size());
}

vector<Point> generateRandomPoints(int n)
{
    vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 20.0);

    for (int i = 0; i < n; i++)
    {
        points.push_back(Point(dis(gen), dis(gen)));
    }
    return points;
}

void printPoint(const Point &p)
{
    cout << "(" << fixed << setprecision(2) << p.x << ", " << p.y << ")";
}

int main()
{
    // Du lieu dau vao
    int n = 15;

    vector<Point> points = generateRandomPoints(n);

    cout << "\n=== BAI TOAN BAO LOI ===\n";
    cout << "\nCac diem da tao:\n";
    for (int i = 0; i < points.size(); i++)
    {
        printPoint(points[i]);
        if ((i + 1) % 5 == 0)
            cout << "\n";
        else
            cout << "  ";
    }
    if (points.size() % 5 != 0)
        cout << "\n";

    cout << "\na) BAO LOI (Andrew Algorithm):\n";
    vector<Point> hull = andrewConvexHull(points);
    cout << "Cac dinh cua bao loi:\n";
    for (const Point &p : hull)
    {
        printPoint(p);
        cout << " ";
    }
    cout << "\nSo dinh bao loi: " << hull.size() << "\n";

    cout << "\nb) DIEN TICH BAO LOI:\n";
    double area = calculateArea(hull);
    cout << "Dien tich: " << fixed << setprecision(4) << area << "\n";

    cout << "\nc) CANH NGAN NHAT CUA BAO LOI:\n";
    double shortestEdge = findShortestEdge(hull);
    cout << "Do dai canh ngan nhat: " << fixed << setprecision(4) << shortestEdge << "\n";

    vector<Point> interior = findInteriorPoints(points, hull);
    cout << "\nd) CAC DIEM NAM TRONG BAO LOI:\n";
    if (interior.empty())
    {
        cout << "Khong co diem nao nam trong bao loi.\n";
    }
    else
    {
        for (const Point &p : interior)
        {
            printPoint(p);
            cout << " ";
        }
        cout << "\nSo diem ben trong: " << interior.size() << "\n";
    }

    cout << "\nf) KHOANG CACH NGAN NHAT NAM BEN TRONG BAO LOI:\n";

    if (interior.size() >= 2)
    {
        double minDist = findShortestDistance(interior);
        cout << "Khoang cach ngan nhat giua cac diem ben trong: " << fixed << setprecision(4) << minDist << "\n";
    }
    else
    {
        cout << "Can it nhat 2 diem ben trong de tinh khoang cach.\n";
    }

    return 0;
}