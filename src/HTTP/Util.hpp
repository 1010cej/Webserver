#include<vector>
#include<string>

namespace bobo
{
    class Util
    {
    public:
        static void cutString(const std::string& s, const std::string& sep, std::vector<std::string> *out)
        {
            size_t start = 0;
            while(start < s.size())
            {
                size_t pos = s.find(sep, start);
                if (pos == std::string::npos) break;
                std::string tmp = s.substr(start, pos - start);
                out->push_back(tmp);
                start = pos + sep.size();
            }
            if(start < s.size()) out->push_back(s.substr(start));
        }
    };
}