#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <limits>
#include <iostream>
using namespace std;
int maxRet = 0;

class SolutionTwo
{
public:
    void maxMinPath(vector<vector<int>>& matrix, int r, int c, int minVal)
    {
        if (r == matrix.size() || c == matrix[0].size())
        {
            return;
        }
        
        if (r == matrix.size() - 1 && c == matrix[0].size() - 1)
        {
            //cout << minVal << endl;
            maxRet = max(maxRet, minVal);
            return;
        }
        
        minVal = min(minVal, matrix[r][c]);
        // 如果只是右和下两个方向 所以应该不会有环重复？？？
        for (int d = 0; d < 2; ++d)
        {
            int x = r + direct[d][0];
            int y = c + direct[d][1];
            
            maxMinPath(matrix, x, y, minVal);
        }
    }
private:
    // down and right
    int direct[2][2] = {{1, 0}, {0, 1}};
};

class Solution 
{
public:
    // 这里的rotate是双向都可以
    bool Rotation(string s1, string s2, int k)
    {
        // 其实rotate 总共有三个reverse
        reverse(s1, 0, s1.size() - 1);
        string temp = s1;
        // Try1 k from left first
        reverse(temp, 0, k-1);
        reverse(temp, k, temp.size() - 1);
        cout << temp << endl;
        if (temp == s2)
        {
            return true;
        }

        // Try1 k from right first
        temp = s1;
        reverse(temp, temp.size() - k, temp.size() - 1);
        reverse(temp, 0, temp.size() - k - 1);
        cout << temp << endl;
        return (temp == s2) ? true : false;
    }

    void reverse(string& s, int start, int end)
    {
        while (start < end)
        {
            swap(s[start++], s[end--]);
        }
    }

    vector<double> RollingAvg(vector<int>& v, int size)
    {
        /*
        input： [1,3,5,7,8] size =2
        output: [2.0,4.0,6.0,7.5]*/
        int orgSize = v.size();
        // int newSize = origSize - (size - 1);
        // vector<double> output(newSize, 0.0);
        vector<double> output;
        int start = 0;
        int end = 0;
        double sum = 0;
        while (end < orgSize)
        {
            sum += v[end];

            if (end - start + 1 == size)
            {
                output.push_back(sum/size);
                sum -= v[start];
                start++;
            }
            end++;
        }
        return output;
    }

    vector<pair<int, int>> GetMaxPair(vector<pair<int, int>> v1, vector<pair<int, int>> v2, int limit)
    {
        auto comp = [](pair<int, int>a, pair<int, int> b)
        {
            return a.second < b.second;
        };

        sort(v1.begin(), v1.end(), comp);
        sort(v2.begin(), v2.end(), comp);

        int start = 0;
        int end = v2.size() - 1;
        int minDiff = numeric_limits<int>::max();
        vector<pair<int, int>> ret;
        while (start < v1.size() || end >= 0)
        {
            pair<int, int> s = (start < v1.size()) ? v1[start] : v1[v1.size() - 1];
            pair<int, int> e = (end >= 0) ? v2[end] : v2[0];
            int sum = s.second + e.second;
            if (sum == limit)
            {
                minDiff = 0;
                ret.push_back({s.first, e.first});
                // 这里是错误的地方
                start++;
                //end--;
            }
            else if (sum > limit)
            {
                end--;
            }
            else
            {
                if (limit - sum <= minDiff)
                {
                    if (limit < minDiff)
                    {
                        ret.clear();
                    }

                    ret.push_back({s.first, e.first});
                }
                start++;
            }
            if (start == v1.size() && end == 0)
            {
                break;
            }
        }

        return ret;
    }
};

int main()
{
    Solution s;
    string s1 = "amazon";
    string s2 = "azonam";
    bool flag = s.Rotation(s1, s2, 2);
    if (flag)
    {
        cout << "matched" << endl;
    }
    else
    {
        cout << "not matched" << endl;
    }

    SolutionTwo t;
    //vector<vector<int>> matrix = {{8,4,3,5},{6,5,9,8},{2,6,7,3}};//{{8,4,7}, {6,5,9}};
    vector<vector<int>> matrix = {{8,4,3},{6,5,9},{2,6,7}};//{{8,4,7}, {6,5,9}};
    int minVal = numeric_limits<int>::max();
    t.maxMinPath(matrix, 0, 0, minVal);
    cout << maxRet << endl;

    int max = 10000;
    vector<pair<int, int>> start = {{1, 2000}, {2, 2000}, {3, 5000}};
    vector<pair<int, int>> end = {{1, 5000}, {2, 2000}, {3, 8000}};
    vector<pair<int, int>> ret = s.GetMaxPair(start, end, max);
    cout << "----------- sepearte line ----------" << endl;
    for (auto& r : ret)
    {
        cout << "pair"<< r.first << r.second << endl;
    }

    /*
    Rolling Average
    input： [1,3,5,7,8] size =2
    output: [2.0,4.0,6.0,7.5]
    */
    // vector<int> input = {1, 3, 5, 7, 8};
    // vector<double> output = s.RollingAvg(input, 2);
    // std::copy(output.begin(), output.end(), std::ostream_iterator<double>(std::cout, " "));
    // cout << endl;
}


int a = 2;
int b = 3;

int temp = a;
a = a ^ a ^ b;
b = b ^ b ^ temp;

cout << a << endl;
cout << b << endl;