#include "paths.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
#define down 0.998
#define INF 1e12
int n, m;

const int N = 105;
const int epoch = 30000;

struct P
{
    double x, y;
} a[N];

double dis[N][N];
int Arr[N];
int Ans[N];
double ans = INF;

double calc()
{
    double res = 0;
    for (int i = 1; i < n; i++)
    {
        res += dis[Arr[i]][Arr[i + 1]];
    }
    if (res < ans)
    {
        ans = res;
        memcpy(Ans, Arr, N * sizeof(int));
    }
    return res;
}

double Rand()
{
    return rand() % 100000 / 100000.0;
}

double SA()
{
    double now = ans;
    double T = 1300;
    while (T > 1e-12)
    {
        int x = rand() % n + 1, y = rand() % n + 1;
        if (x == y)
            continue;
        swap(Arr[x], Arr[y]);
        double ew = calc();
        if (ew < now || exp(now - ew) > Rand() * T)
            now = ew;
        else
            swap(Arr[x], Arr[y]);
        T *= down;
    }
    return now;
}

void solve()
{
    memcpy(Ans, Arr, N * sizeof(int));
    for (int i = 1; i <= epoch; i++)
    {
        SA();
    }
}

void Init()
{
    ans = INF;
}

int main(int argc, char **argv)
{
    FILE *inputFile = nullptr;
    std::string outputFileName, ansFileName;
    if (!open_paths(argc, argv, inputFile, outputFileName, ansFileName))
        return 1;
    Init();
    if (fscanf(inputFile, "%d", &n) != 1 || n < 2 || n > 100)
    {
        std::fprintf(stderr, "Invalid city count (expected 2..100).\n");
        fclose(inputFile);
        return 1;
    }
    for (int i = 1; i <= n; i++)
    {
        fscanf(inputFile, "%lf %lf", &a[i].x, &a[i].y);
    }
    fclose(inputFile);

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            dis[i][j] = sqrt((a[i].x - a[j].x) * (a[i].x - a[j].x) + (a[i].y - a[j].y) * (a[i].y - a[j].y));
        }
        Arr[i] = i;
    }
    calc();
    solve();

    FILE *outputFile = fopen(outputFileName.c_str(), "w");
    if (!outputFile)
    {
        std::perror("Output");
        return 1;
    }
    fprintf(outputFile, "%.4lf\n", ans);
    fclose(outputFile);

    FILE *ansFile = fopen(ansFileName.c_str(), "w");
    if (!ansFile)
    {
        std::perror("Answer");
        return 1;
    }
    for (int i = 1; i <= n; i++)
    {
        fprintf(ansFile, "%d ", Ans[i]);
    }
    fprintf(ansFile, "\n");
    fclose(ansFile);

    return 0;
}