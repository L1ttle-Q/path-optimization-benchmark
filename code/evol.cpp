#include "paths.h"
#include <cstdio>
#include <set>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <queue>
#include <ctime>

using namespace std;

#define gc() getchar()
#define digit(x) (x >= '0' && x <= '9')

int read()
{
    int x = 0, f = 1;
    char c = gc();
    while (!digit(c))
    {
        if (c == '-')
            f = -1;
        if (c == EOF)
            return (1 << 30);
        c = gc();
    }
    while (digit(c))
        x = (x << 3) + (x << 1) + c - '0', c = gc();
    return x * f;
}

const int MAX_NUM = 85;
const int MAX_GEN = 25000000;
const int N = 105;
const double VARI_RATE = 0.04;
const double PI = acos(-1.0);

const int mod = 998244357;

int n;
double d[N][N];
double ans;

struct Node
{
    double x, y;
    Node() {}
    Node(const double &a, const double &b) : x(a), y(b) {}
} Point[N];
double operator-(const Node &a, const Node &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

class Single
{
public:
    double comf;
    double length;
    int arr[N];

    void Init()
    {
        length = len();
        comf = comfort();
        Srand();
    }
    Single() {}
    Single(int *ref)
    {
        for (int i = 1; i <= n; i++)
            arr[i] = ref[i];
        this->Init();
    }
    Single(const Single *ref)
    {
        for (int i = 1; i <= n; i++)
            arr[i] = ref->arr[i];
        this->Init();
    }

    int *Vari()
    {
        int *res = (int *)malloc(N * sizeof(int));
        for (int i = 1; i <= n; i++)
            res[i] = arr[i];
        for (int i = 1; i <= n; i++)
            if (Rand(1) <= VARI_RATE)
            {
                int tmp = int(Rand(0));
                if (tmp != i)
                    swap(arr[i], arr[tmp]);
            }
        return res;
    }

    int *operator+(const Single &b)
    {
        int *res = (int *)malloc(N * sizeof(int));
        int l = (int)Rand(0), r = (int)Rand(0);

        bool f[N];
        memset(f, 0, sizeof(f));
        if (r < l)
        {
            for (int i = l; i <= n; i++)
                res[i] = b.arr[i], f[res[i]] = 1;
            for (int i = 1; i <= r; i++)
                res[i] = b.arr[i], f[res[i]] = 1;
            int j = 1;
            for (int i = r + 1; i < l; i++)
            {
                while (f[arr[j]])
                    j++;
                res[i] = arr[j], j++;
            }
        }
        else
        {
            for (int i = l; i <= r; i++)
                res[i] = b.arr[i], f[res[i]] = 1;
            int j = 1;
            for (int i = 1; i < l; i++)
            {
                while (f[arr[j]])
                    j++;
                res[i] = arr[j], j++;
            }
            for (int i = r + 1; i <= n; i++)
            {
                while (f[arr[j]])
                    j++;
                res[i] = arr[j], j++;
            }
        }
        return res;
    }
    bool operator<(const Single &b) const
    {
        return comf > b.comf;
    }
    bool operator>(const Single &b) const
    {
        return comf < b.comf;
    }

private:
    int seed;
    void Srand()
    {
        for (int i = 1; i <= n; i++)
            seed += arr[i] * i;
        seed = (seed * 233 + 29393) * int(length) ^ 932545;
        seed %= mod;
        seed += mod;
        seed %= mod;
    }
    double Rand(int op)
    {
        seed = (seed * 353 + 99935) ^ 44532193;
        seed %= mod;
        seed += mod;
        seed %= mod;
        if (op)
            return seed / mod;
        else
            return seed % n + 1;
    }
    double len()
    {
        length = 0;
        for (int i = 1; i < n; i++)
            length += d[arr[i]][arr[i + 1]];
        return length;
    }
    double comfort()
    {
        return 1000.0 * (PI / 2 - atan(length)) - 1e-5 * (arr[1] - arr[n >> 1 | 1] - arr[n]);
    }
};

typedef pair<double, int> pi;
set<Single> S;

void Pre() // heuristic process
{
    priority_queue<pi, vector<pi>, greater<pi>> P[N];

    bool f[N];
    int seq[N], cnt = 0;
    for (int beg = 1; beg <= n; beg++)
    {
        for (int i = 1; i <= n; i++)
        {
            while (!P[i].empty())
                P[i].pop();
            for (int j = 1; j <= n; j++)
                if (i != j)
                    P[i].push(make_pair(d[i][j], j));
        }

        memset(f, 0, sizeof(f));
        f[beg] = 1;
        seq[cnt = 1] = beg;
        int now = beg;

        while (cnt < n)
        {
            pi tmp = P[now].top();
            P[now].pop();
            if (!f[tmp.second])
            {
                seq[++cnt] = tmp.second;
                f[tmp.second] = 1;
                now = tmp.second;
            }
        }

        Single ins = Single(seq);
        ans = min(ans, ins.length);
        S.insert(ins);
    }
}

static int sed = time(NULL);
void Srand(int sd)
{
    sed = sd;
}
int Rand_()
{
    return (sed = ((sed * 7317 + 221592) ^ 112133) % mod + mod) %= mod;
}

void Init()
{
    ans = 1e9;
    S.clear();
}

int Find_f2(int siz, int x)
{
    int res = 0;
    do
    {
        int sum = (siz * (siz + 1)) >> 1;
        sum = Rand_() % sum + 1;
        for (int i = 1; i <= siz; i++)
        {
            if (sum <= siz - i + 1)
            {
                res = i;
                break;
            }
            sum -= siz - i + 1;
        }
    } while (res == x);
    return res;
}

void Solve()
{
    for (int i = 1; i <= MAX_GEN; i++)
    {
        int siz = S.size();
        int sum = (siz * (siz + 1)) >> 1;
        sum = Rand_() % sum + 1;
        int f1, f2;
        for (int j = 1; j <= siz; j++)
        {
            if (sum <= siz - j + 1)
            {
                f1 = j, f2 = Find_f2(siz, j);
                break;
            }
            sum -= siz - j + 1;
        }

        Single fa1, fa2;
        if (f1 > f2)
            swap(f1, f2);
        int cntit = 1;
        set<Single>::iterator it = S.begin();
        while (it != S.end())
        {
            if (cntit == f1)
                fa1 = Single(&(*it));
            if (cntit == f2)
                fa2 = Single(&(*it));
            cntit++;
            it++;
        }

        int *tmp_ = fa1 + fa2;
        Single New(tmp_);
        ans = min(ans, New.length);
        free(tmp_);
        S.insert(New);

        tmp_ = New.Vari();
        New = Single(tmp_);
        ans = min(ans, New.length);
        free(tmp_);
        S.insert(New);

        while (S.size() > MAX_NUM)
        {
            it = S.end();
            S.erase(--it);
        }
    }
}

void Print_ans()
{
    printf("%.2f\n", ans);
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
        double a, b;
        fscanf(inputFile, "%lf %lf", &a, &b);
        Point[i] = Node(a, b);
    }
    fclose(inputFile);

    Srand(n + int(Point[1].x + Point[n].y));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            d[i][j] = Point[i] - Point[j];

    Pre();
    Solve();

    FILE *outputFile = fopen(outputFileName.c_str(), "w");
    if (!outputFile)
    {
        std::perror("Output");
        return 1;
    }
    fprintf(outputFile, "%.4f\n", ans);
    fclose(outputFile);

    FILE *ansFile = fopen(ansFileName.c_str(), "w");
    if (!ansFile)
    {
        std::perror("Answer");
        return 1;
    }
    for (auto it = S.begin(); it != S.end(); ++it)
    {
        for (int i = 1; i <= n; i++)
        {
            fprintf(ansFile, "%d ", it->arr[i]);
        }
        fprintf(ansFile, "\n");
    }
    fclose(ansFile);

    return 0;
}