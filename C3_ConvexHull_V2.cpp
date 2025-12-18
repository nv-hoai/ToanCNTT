#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <climits>
#include <fstream>

using namespace std;

// Struct để lưu trữ điểm 2D
struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(const Point& other) const {
        return abs(x - other.x) < 1e-9 && abs(y - other.y) < 1e-9;
    }
};

// Tính tích chéo của 2 vector OA và OB từ điểm O
double crossProduct(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Tính khoảng cách giữa 2 điểm
double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 1. Andrew's Monotone Chain Algorithm để tìm bao lồi
// Độ phức tạp: O(n log n)
vector<Point> convexHull(vector<Point> points) {
    int n = points.size();
    if (n <= 1) return points;
    
    // Sắp xếp các điểm theo tọa độ x, sau đó theo y
    sort(points.begin(), points.end());
    
    // Xây dựng bao lồi dưới
    vector<Point> lower;
    for (int i = 0; i < n; i++) {
        while (lower.size() >= 2 && 
               crossProduct(lower[lower.size()-2], lower[lower.size()-1], points[i]) <= 0) {
            lower.pop_back();
        }
        lower.push_back(points[i]);
    }
    
    // Xây dựng bao lồi trên
    vector<Point> upper;
    for (int i = n - 1; i >= 0; i--) {
        while (upper.size() >= 2 && 
               crossProduct(upper[upper.size()-2], upper[upper.size()-1], points[i]) <= 0) {
            upper.pop_back();
        }
        upper.push_back(points[i]);
    }
    
    // Loại bỏ điểm cuối của mỗi phần để tránh trùng lặp
    lower.pop_back();
    upper.pop_back();
    
    // Ghép hai phần lại
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

// 2. Shoelace Formula để tính diện tích
double calculateArea(const vector<Point>& hull) {
    int n = hull.size();
    if (n < 3) return 0.0;
    
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += hull[i].x * hull[j].y;
        area -= hull[j].x * hull[i].y;
    }
    return abs(area) / 2.0;
}

// Kiểm tra điểm có nằm trong tam giác không
bool pointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
    double d1, d2, d3;
    bool has_neg, has_pos;
    
    d1 = crossProduct(a, b, p);
    d2 = crossProduct(b, c, p);
    d3 = crossProduct(c, a, p);
    
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

// 3. Kiểm tra điểm có nằm trong bao lồi không
bool pointInConvexHull(const Point& p, const vector<Point>& hull) {
    int n = hull.size();
    if (n < 3) return false;
    
    // Sử dụng ray casting algorithm cho đa giác lồi
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((hull[i].y > p.y) != (hull[j].y > p.y)) &&
            (p.x < (hull[j].x - hull[i].x) * (p.y - hull[i].y) / (hull[j].y - hull[i].y) + hull[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

// Tìm các điểm nằm trong bao lồi
vector<Point> findInteriorPoints(const vector<Point>& allPoints, const vector<Point>& hull) {
    vector<Point> interior;
    for (const Point& p : allPoints) {
        bool isHullPoint = false;
        for (const Point& hp : hull) {
            if (p == hp) {
                isHullPoint = true;
                break;
            }
        }
        if (!isHullPoint && pointInConvexHull(p, hull)) {
            interior.push_back(p);
        }
    }
    return interior;
}

// 4. Divide and Conquer để tìm khoảng cách nhỏ nhất
double closestPairRec(vector<Point>& px, vector<Point>& py, int n) {
    if (n <= 3) {
        double minDist = INFINITY;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                minDist = min(minDist, distance(px[i], px[j]));
            }
        }
        return minDist;
    }
    
    int mid = n / 2;
    Point midPoint = px[mid];
    
    vector<Point> pyl(mid), pyr(n - mid);
    int li = 0, ri = 0;
    
    for (int i = 0; i < n; i++) {
        if (py[i].x <= midPoint.x && li < mid) {
            pyl[li++] = py[i];
        } else {
            pyr[ri++] = py[i];
        }
    }
    
    double dl = closestPairRec(px, pyl, mid);
    vector<Point> pxr(px.begin() + mid, px.end());
    double dr = closestPairRec(pxr, pyr, n - mid);
    
    double d = min(dl, dr);
    
    vector<Point> strip;
    for (int i = 0; i < n; i++) {
        if (abs(py[i].x - midPoint.x) < d) {
            strip.push_back(py[i]);
        }
    }
    
    for (int i = 0; i < strip.size(); i++) {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; j++) {
            d = min(d, distance(strip[i], strip[j]));
        }
    }
    
    return d;
}

double findClosestPair(vector<Point> points) {
    int n = points.size();
    if (n < 2) return INFINITY;
    
    vector<Point> px = points;
    vector<Point> py = points;
    
    sort(px.begin(), px.end());
    sort(py.begin(), py.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });
    
    return closestPairRec(px, py, n);
}

// 5. Tìm điểm trung gian nhất (geometric median approximation)
Point findMedianPoint(const vector<Point>& points) {
    if (points.empty()) return Point(0, 0);
    if (points.size() == 1) return points[0];
    
    // Sử dụng centroid như một xấp xỉ đơn giản của geometric median
    double sumX = 0, sumY = 0;
    for (const Point& p : points) {
        sumX += p.x;
        sumY += p.y;
    }
    
    Point centroid(sumX / points.size(), sumY / points.size());
    
    // Tìm điểm gần centroid nhất
    Point medianPoint = points[0];
    double minDist = distance(centroid, points[0]);
    
    for (const Point& p : points) {
        double dist = distance(centroid, p);
        if (dist < minDist) {
            minDist = dist;
            medianPoint = p;
        }
    }
    
    return medianPoint;
}

void readFile(const string& filename, vector<Point>& points) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Khong the mo file: " << filename << endl;
        return;
    }

    Point p;
    while (file >> p.x >> p.y) {
        points.push_back(p);
    }
    file.close();
}

void printPoint(const Point& p) {
    cout << "(" << fixed << setprecision(2) << p.x << ", " << p.y << ")";
}

int main(int argc, char* argv[]) {
    vector<Point> points;

    if (argc == 2) {
        readFile(argv[1], points);
    }
    else
    {
        int n;
        cout << "Nhap so luong diem n: ";
        cin >> n;
        
        cout << "Nhap " << n << " diem (x y):\n";
        for (int i = 0; i < n; i++) {
            cin >> points[i].x >> points[i].y;
        }
    }
    
    cout << "\n=== GIAI BAI TOAN BAO LOI ===\n";
    
    // 1. Tìm bao lồi
    cout << "\n1. BAO LOI (Andrew Algorithm - O(n log n)):\n";
    vector<Point> hull = convexHull(points);
    cout << "Cac dinh cua bao loi:\n";
    for (const Point& p : hull) {
        printPoint(p);
        cout << " ";
    }
    cout << "\nSo dinh bao loi: " << hull.size() << "\n";
    
    // 2. Tính diện tích
    cout << "\n2. DIEN TICH BAO LOI (Shoelace Formula):\n";
    double area = calculateArea(hull);
    cout << "Dien tich: " << fixed << setprecision(4) << area << "\n";
    
    // 3. Tìm điểm trong bao lồi
    cout << "\n3. CAC DIEM NAM TRONG BAO LOI:\n";
    vector<Point> interior = findInteriorPoints(points, hull);
    if (interior.empty()) {
        cout << "Khong co diem nao nam trong bao loi.\n";
    } else {
        cout << "Cac diem ben trong:\n";
        for (const Point& p : interior) {
            printPoint(p);
            cout << " ";
        }
        cout << "\nSo diem ben trong: " << interior.size() << "\n";
    }
    
    // 4. Khoảng cách nhỏ nhất
    cout << "\n4. KHOANG CACH NHO NHAT (Divide and Conquer):\n";
    vector<Point> allRelevantPoints = hull;
    allRelevantPoints.insert(allRelevantPoints.end(), interior.begin(), interior.end());
    
    if (allRelevantPoints.size() >= 2) {
        double minDist = findClosestPair(allRelevantPoints);
        cout << "Khoang cach nho nhat: " << fixed << setprecision(4) << minDist << "\n";
    } else {
        cout << "Can it nhat 2 diem de tinh khoang cach.\n";
    }
    
    // 5. Điểm trung gian nhất
    cout << "\n5. DIEM TRUNG GIAN NHAT (Median Algorithm):\n";
    if (!interior.empty()) {
        Point median = findMedianPoint(interior);
        cout << "Diem trung gian nhat trong cac diem ben trong: ";
        printPoint(median);
        cout << "\n";
    } else {
        cout << "Khong co diem ben trong de tim median.\n";
    }
    
    return 0;
}