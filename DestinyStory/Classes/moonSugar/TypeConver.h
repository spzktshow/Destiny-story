#ifndef __MoonSugar__TypeConver__
#define __MoonSugar__TypeConver__

#include <string>
#include <vector>
#include "moonSugar.h"

NS_MS_BEGIN
class TypeConver
{
public:
    static char * ConstCharToChar(const char * constChar);
    
    static char * StringToChar(std::string * str);
    
    static std::string * CharToString(char * charStr);
    
    static int CharToInt(const char * s);
    
    static int charToInt(const char s);
    static std::string intToString(int intValue);

    static char * combinationChar(char * char1, char * char2);
        
    static std::string filterStr(int num, int bit);
        
    static std::vector<std::string> splitStr(std::string strValue, std::string tagValue);
};
NS_MS_END;

#endif // !__MoonSugar__TypeConver__
