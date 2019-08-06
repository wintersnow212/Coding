int main ()
{
    vector<string> input = {"page1: this is a page in one book", "page2: second page book", "page3: test book"};
    
    
    unordered_map<string, pair<int, vector<int>>> strMap;
    
    for (int i = 0; i < input.size(); ++i)
    {
        istringstream in(input[i]);
        string word;
        int cnt = 0;
        while (in.eof() == false)
        {
            getline(in, word, ' ');
            if (cnt == 0)
            {
                cnt++;
                continue;
            }
            strMap[word].first++;
            strMap[word].second.push_back(i+1);
        }
        // while (in >> word)
        // {
        //     if (cnt == 0)
        //     {
        //         cnt++;
        //         continue;
        //     }
        //     strMap[word].first++;
        //     strMap[word].second.push_back(i+1);
        // }
    }
    
    auto Comp = [ ](myPair a, myPair b)
                {
                    return a.second.first < b.second.first;  
                };
    
    priority_queue<myPair, vector<myPair>, decltype(Comp)> pq(Comp);
    for (auto s : strMap)
    {
        pq.push(s);
    }
    
    vector<string> ret;
    while (pq.empty() == false)
    {
        string subRet;
        myPair top = pq.top();
        pq.pop();
        subRet += (top.first + " ");
        for (auto v : top.second.second)
        {
            subRet += to_string(v);
            subRet += ",";
        }
        subRet.pop_back();
        
        ret.push_back(subRet);
    }
    
    
    for (auto r : ret)
    {
        cout << r << endl;
    }
    
    return 0;
}