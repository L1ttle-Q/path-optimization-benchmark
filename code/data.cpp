#include <cstdio>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <string>

using namespace std;

const int N = 100;

mt19937 engine(time(NULL));

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <output-prefix>\n", argv[0]);
        return 1;
    }
    namespace fs = std::filesystem;
    fs::path prefix = fs::u8path(argv[1]);
    std::string input = prefix.string() + ".in";
    std::string output = prefix.string() + ".out";
    std::string answer = prefix.string() + ".ans";
    if (fs::exists(input) || fs::exists(output) || fs::exists(answer))
    {
        fprintf(stderr, "Output already exists; choose a new prefix.\n");
        return 1;
    }
    std::error_code error;
    if (prefix.has_parent_path())
        fs::create_directories(prefix.parent_path(), error);
    if (error)
    {
        fprintf(stderr, "Cannot create output directory: %s\n", error.message().c_str());
        return 1;
    }

    // for (int i = 1; i <= 20; i++)
    // {
    //     char fileName[20];
    //     sprintf(fileName, "%d/%d.in", N, i);
    //     FILE* fp = fopen(fileName, "w");
    //     fprintf(fp, "%d\n", N);
    //     for (int j = 1; j <= N; j++)
    //     {
    //         double a = (engine() % 100000) / 100.0,
    //                b = (engine() % 100000) / 100.0;
    //         fprintf(fp, "%.2f %.2f\n", a, b);
    //     }
    //     fclose(fp);
    // }

    pair<pair<double, double>, int> P[N + 1];
    int cntP = 0;

    FILE *fp = fopen(input.c_str(), "w");
    if (!fp)
    {
        perror("Input output");
        return 1;
    }
    fprintf(fp, "%d\n", N);
    for (int i = 1; i <= 47; i++)
    {
        P[++cntP].first = {1, i};
        P[cntP].second = cntP;
    }
    P[++cntP].first = {0, 50};
    P[cntP].second = cntP;
    P[++cntP].first = {0.5, 50};
    P[cntP].second = cntP;
    P[++cntP].first = {1, 50};
    P[cntP].second = cntP;
    for (int i = 1; i <= 50; i++)
    {
        P[++cntP].first = {1 + i, 50 + i};
        P[cntP].second = cntP;
    }

    shuffle(P + 1, P + N + 1, engine);
    for (int i = 1; i <= N; i++)
    {
        auto p = P[i].first;
        fprintf(fp, "%.4f %.4f\n", p.first, p.second);
    }
    fclose(fp);

    fp = fopen(output.c_str(), "w");
    if (!fp)
    {
        perror("Reference output");
        return 1;
    }
    fprintf(fp, "%.4f\n", 46 + sqrt(10) + 1 + 50 * sqrt(2));
    fclose(fp);

    fp = fopen(answer.c_str(), "w");
    if (!fp)
    {
        perror("Reference answer");
        return 1;
    }
    for (int i = 1; i <= N; i++)
        fprintf(fp, "%d ", P[i].second);
    fclose(fp);
    return 0;
}