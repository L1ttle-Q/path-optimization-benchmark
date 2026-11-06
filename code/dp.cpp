#include "paths.h"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 21;
const double INF = 100000000.0;

int n;
double d[1 << N][N];
double a[N], b[N];
double x[N][N];
int path[1 << N][N];

void Init()
{
    for (int s = 1; s < (1 << n); s++)
        for (int i = 1; i <= n; i++)
            d[s][i] = INF;
    for (int i = 1; i <= n; i++)
        d[1 << (i - 1)][i] = 0;
}

void OutputPath(int s, int i, int *route, int &routeIndex)
{
    if (s == 0)
        return;
    route[routeIndex++] = i;
    OutputPath(s ^ (1 << (i - 1)), path[s][i], route, routeIndex);
}

int main(int argc, char **argv)
{
    FILE *inputFile = nullptr;
    std::string outputFileName, ansFileName;
    if (!open_paths(argc, argv, inputFile, outputFileName, ansFileName))
        return 1;
    if (fscanf(inputFile, "%d", &n) != 1 || n < 2 || n > 20)
    {
        std::fprintf(stderr, "Invalid city count (expected 2..20).\n");
        fclose(inputFile);
        return 1;
    }
    for (int i = 1; i <= n; i++)
        fscanf(inputFile, "%lf %lf", &a[i], &b[i]);
    fclose(inputFile);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j < i; j++)
            x[i][j] = x[j][i] = sqrt(pow(a[i] - a[j], 2) + pow(b[i] - b[j], 2));

    Init();
    for (int s = 1; s < (1 << n); s++)
        for (int i = 1; i <= n; i++)
            if (s & (1 << (i - 1)))
                for (int j = 1; j <= n; j++)
                {
                    if (s & (1 << (j - 1)))
                        continue;
                    double newDist = d[s][i] + x[i][j];
                    if (newDist < d[s | (1 << (j - 1))][j])
                    {
                        d[s | (1 << (j - 1))][j] = newDist;
                        path[s | (1 << (j - 1))][j] = i;
                    }
                }

    double ans = INF;
    int last = -1;
    for (int i = 1; i <= n; i++)
    {
        if (d[(1 << n) - 1][i] < ans)
        {
            ans = d[(1 << n) - 1][i];
            last = i;
        }
    }

    FILE *outputFile = fopen(outputFileName.c_str(), "w");
    if (!outputFile)
    {
        std::perror("Output");
        return 1;
    }
    fprintf(outputFile, "%.4f\n", ans);
    fclose(outputFile);

    int route[N];
    int routeIndex = 0;
    OutputPath((1 << n) - 1, last, route, routeIndex);
    reverse(route, route + routeIndex);

    FILE *ansFile = fopen(ansFileName.c_str(), "w");
    if (!ansFile)
    {
        std::perror("Answer");
        return 1;
    }
    for (int i = 0; i < routeIndex; i++)
        fprintf(ansFile, "%d ", route[i]);
    fprintf(ansFile, "\n");
    fclose(ansFile);

    return 0;
}