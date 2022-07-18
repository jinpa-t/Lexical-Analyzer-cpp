/**
 * Author: github@ jinpa-t
 * Description: Lexical analyzer for C++ programs.
 * Date: 07/18/2022
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

using namespace std;

/*
    Identifier,
    Keyword,
    Operator,
    Punctuator,
    Character Literal,
    String Literal,
    Number Literal,
    Float Literal,
    Boolean Literal
    
*/

// punctuators are 
const std::vector<const char*>OPERATORS{
    /*"[]",   "()",   "{}",	",",    ":",    
    ";",    "*",	"=",    "...",  "#",
    ".",	"->",	"++",	"--"	"##",	 
    "&",	"+",	"-",	"~",	"!",
    "⁄",	"%",	"<<",	">>",	"!=",	 
    "<",	">",	"<=",	">=",	"==",	 
    "^",	"|",	"&&",	"||",	"?",
    "*=",	"⁄="	"%="	"+="	"-=", 
    "<<=",  ">>=",  "&=",   "^=",   "|=",
    ":",    */
    
    "!", "%", "^", "&", "*", "()", "-", "+", "=", "{}", "|", "~",
    "[]", "\\", ";", "'", ":", "\"", "<", ">", "?", ",", ".", "/", "#"
};

const std::vector<const char*>PUNCTUATORS{
    "[]",   "()",   "{}",	",",    ":",    
    ";",    "*",	"=",    "...",  "#",
    ".",	"->",	"++",	"--"	"##",	 
    "&",	"+",	"-",	"~",	"!",
    "⁄",	"%",	"<<",	">>",	"!=",	 
    "<",	">",	"<=",	">=",	"==",	 
    "^",	"|",	"&&",	"||",	"?",
    "*=",	"⁄="	"%="	"+="	"-=", 
    "<<=",  ">>=",  "&=",   "^=",   "|=",
    "::",    ".*",   "->*", "\"",   "'"
};

const std::vector<const char*>KEYWORDS{
"alignas","alignof","and","and_eq","asm","atomic_cancel","atomic_commit",
"atomic_noexcept","auto","bitand","bitor","bool","break","case","catch",
"char","char8_t","char16_t","char32_t","class","compl","concept","const",
"consteval","constexpr","constinit","const_cast","continue","co_await",
"co_return","co_yield","decltype","default","delete","do","double",
"dynamic_cast","else","enum","explicit","export","extern","false","float",
"for","friend","goto","if","inline","int","long","mutable","namespace",
"new","noexcept","not","not_eq","nullptr","operator","or","or_eq","private",
"protected","public","reflexpr","register","reinterpret_cast","requires",
"return","short","signed","sizeof","static","static_assert","static_cast",
"struct","switch","synchronized","template","this","thread_local","throw",
"true","try","typedef","typeid","typename","union","unsigned","using",
"virtual","void","volatile","wchar_t","while","xor","xor_eq"};

//check if the given character is a punctuator or not
bool isPunctuator(char ch)
{
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == '&' || ch == '|' || ch == '#' || ch == '"'||
        ch == '\'')
        {
            return true;
        }
    // if (std::find(OPERATORS.begin(), OPERATORS.end() ,to_string(ch)) != OPERATORS.end())
    // {
    //     return true;
    // }
    return false;
}

//check if the given identifier is valid or not
bool validIdentifier(char* str)
{
    //if first character of string is a digit or a special character, identifier is not valid
    if ((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z') || (str[0] == '_'))
    {
        int i,len = strlen(str);
        //if length is one, validation is already completed, hence return true
        if (len == 1)
        {
            return true;
        }
        else
        {
            for (i = 1 ; i < len ; i++)						//identifier cannot contain special characters
            {
                if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') ||
                    (str[i] >= 'a' && str[i] <= 'z') || (str[i] == '_')))
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

//check if the given character is an operator or not
bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '=' ||
        ch == '/' || ch == '>' || ch == '<' || ch == '%' ||
        ch == '=' || ch == '|' || ch == '&' || ch == '~')
    {
       return true;
    }
    return false;
}

//check if the given substring is a keyword or not
bool isKeyword(char *str)
{
    /*if (!strcmp(str, "if") */
    if (std::find(KEYWORDS.begin(), KEYWORDS.end() ,string(str)) != KEYWORDS.end())
    {
        return true;
    }
    
    return false;
}

//check if the given substring is a number or not
bool isNumber(char* str)
{
    // base case
    int i, len = strlen(str),numOfDecimal = 0;
    if (len == 0)
    {
        return false;
    }
    
    // is the first digit a number if not return false constraint: str size >= 1.
    if (!isdigit(str[0]))
    {
        return false;
    }
    for (i = 0 ; i < len ; i++)
    {
        if (numOfDecimal > 1 && str[i] == '.')
        {
            return false;
        }
        else if (numOfDecimal <= 1)
        {
            numOfDecimal++;
        }
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
        {
            return false;
        }
    }
    return true;
}

// is integerLiteral?
bool isIntegerLiteral(char* str)
{
    // base case
    int i, len = strlen(str), numOfDecimal = 0;;
    if (len == 0)
    {
        return false;
    }
    
    // is the first digit a number if not return false constraint: str size >= 1.
    if (!isdigit(str[0]))
    {
        return false;
    }
    
    //check suffix, valid suffixes: upper, lower, and mix case of the following
    // [ u, l, ll, lu, ul, llu, ull ]
    i = 0;
    string suffix;
    while(tolower(str[len-1-i]) == 'l' || tolower(str[len-1-i]) == 'u')
    {
        if(str[len-1-i] == 'u' || str[len-1-i] == 'U' ||
            str[len-1-i] == 'l' || str[len-1-i] == 'L')
        {
            if(isupper(str[len-1-i]))
            {
                char temp = tolower(str[len-1-i]);
                suffix = temp+suffix;
            }
            else 
            {
                suffix = str[len-1-i] + suffix;
            }
        }
        i++;
    }
    // is the suffix valid or not?
    if(suffix.length() != 0 && !(suffix == "u" || suffix == "l" ||suffix == "ll" 
        ||  suffix == "ul" || suffix == "lu" || suffix == "llu" ||  suffix == "ull"))
    {
       return false; 
    }
    
    bool firstCharIsZero = false;
    bool isBinaryNumber = false;
    bool isOctNumber = false;
    bool isHexNumber = false;
    bool exponenetFound = false;
    for (i = 0 ; i < len - suffix.length() ; i++)
    {
        if(i == 0 && str[i] == '0')
        {
            firstCharIsZero = true;
            continue;
        }
        else if(i == 1 && firstCharIsZero && (str[i] == 'b' || str[i] == 'B'))
        {
            isBinaryNumber = true;
            continue;
        } 
        else if(i == 1 && firstCharIsZero && (str[i] == 'x' || str[i] == 'X'))
        {
            isHexNumber = true;
            continue;
        }
        else if(i == 1 && firstCharIsZero && (str[i] >= '0' && str[i] <= '7'))
        {
            isOctNumber = true;
            continue;
        }
        
        if (numOfDecimal > 1 && str[i] == '.')
        {
            return false;
        }
        else if (numOfDecimal < 1 && str[i] == '.')
        {
            // first encountering dot
            numOfDecimal++;
        }
        else if(isBinaryNumber && i != 1)
        {
            if (str[i] != '0' && str[i] != '1'){
                return false;
            }
        } 
        else if(isHexNumber && i != 1)
        {
            if(!(str[i]>= 'a' && str[i]<= 'f') && !(str[i]>= 'A' && str[i]<= 'F') &&
            !(str[i]>= '0' && str[i] <= '9')){
                return false;
            }
        } 
        else if (!isHexNumber && (str[i] == 'e' || str[i] == 'E'))
        {
            if (!exponenetFound)
            {
                exponenetFound = true;
            } 
            else
            {
                return false;
            }
        }
        else if (isOctNumber && i!= 1)
        {
            if (!(str[i] >= '0' && str[i] <= '7'))
            {
                return false;
            }
        }
        else if (!(str[i]>= '0' && str[i] <= '9') || (str[i] == '-' && i > 0))
        {
            return false;
        }
    }
    
    
    return true;
}

// is float literal?
bool isFloatLiteral(char* str)
{
    // base case
    int i, len = strlen(str), numOfDecimal = 0;;
    if (len == 0)
    {
        return false;
    }
    
    // is the first digit a number if not return false constraint: str size >= 1.
    if (!isdigit(str[0]))
    {
        return false;
    }
    
    //check suffix, valid suffixes: upper, lower, and mix case of the following
    // [ u, l, ll, lu, ul, llu, ull ]
    i = 0;
    string suffix;
    while(tolower(str[len-1-i]) == 'l' || tolower(str[len-1-i]) == 'u')
    {
        if(str[len-1-i] == 'u' || str[len-1-i] == 'U' ||
            str[len-1-i] == 'l' || str[len-1-i] == 'L')
        {
            if(isupper(str[len-1-i]))
            {
                char temp = tolower(str[len-1-i]);
                suffix = temp+suffix;
            }
            else 
            {
                suffix = str[len-1-i] + suffix;
            }
        }
        i++;
    }
    // is the suffix valid or not?
    if(suffix.length() != 0 && !(suffix == "u" || suffix == "l" ||suffix == "ll" 
        ||  suffix == "ul" || suffix == "lu" || suffix == "llu" ||  suffix == "ull" || suffix == "f"))
    {
       return false; 
    }
    
    bool firstCharIsZero = false;
    bool isBinaryNumber = false;
    bool isOctNumber = false;
    bool isHexNumber = false;
    for (i = 0 ; i < len - suffix.length() ; i++)
    {
        if(i == 0 && str[i] == '0')
        {
            firstCharIsZero = true;
            continue;
        }
        else if(i == 1 && firstCharIsZero && (str[i] == 'b' || str[i] == 'B'))
        {
            isBinaryNumber = true;
            continue;
        } 
        else if(i == 1 && firstCharIsZero && (str[i] == 'x' || str[i] == 'X'))
        {
            isHexNumber = true;
            continue;
        }
        else if(i == 1 && firstCharIsZero && (str[i] >= '0' && str[i] <= '7'))
        {
            isOctNumber = true;
            continue;
        }
        
        if (numOfDecimal > 1 && str[i] == '.')
        {
            return false;
        }
        else if (numOfDecimal < 1 && str[i] == '.')
        {
            // first encountering dot
            if(suffix != "f")
            {
                return false;
            }
            numOfDecimal++;
        }
        else if(isBinaryNumber && i != 1)
        {
            if (str[i] != '0' && str[i] != '1'){
                return false;
            }
        } 
        else if(isHexNumber && i != 1)
        {
            if(!(str[i]>= 'a' && str[i]<= 'f') && !(str[i]>= 'A' && str[i]<= 'F') &&
            !(str[i]>= '0' && str[i] <= '9')){
                return false;
            }
        } 
        else if (isOctNumber && i!= 1)
        {
            if (!(str[i] >= '0' && str[i] <= '7'))
            {
                return false;
            }
        }
        else if (!(str[i]>= '0' && str[i] <= '9') || (str[i] == '-' && i > 0))
        {
            return false;
        }
    }
    
    
    return true;
}

//extract the required substring from the main string
char* subString(const char* realStr, int l, int r)
{
    int i;

    char* str = (char*) malloc(sizeof(char) * (r - l + 2));

    for (i = l; i <= r; i++)
    {
        str[i - l] = realStr[i];
        str[r - l + 1] = '\0';
    }
    return str;
}


//parse the expression
void parse(const char* str)
{
    int left = 0, right = 0;
    int len = strlen(str);
    while (right <= len && left <= right) 
    {
        //if character is a digit or an alphabet
        if (isPunctuator(str[right]) == false) 
        {
            right++;
        }

        //if character is a punctuator
        if (isPunctuator(str[right]) == true && left == right)
        {
            if (isOperator(str[right]) == true)
            {
                std::cout << str[right] <<" IS AN OPERATOR\n";
            }
            else if(str[right] != ' ')
            {
                std::cout << str[right] <<" IS A PUNCTUATOR\n";
                
            }
            else if(str[right] == '#')
            {
                std::cout << str[right] <<" IS A PUNCTUATOR\n";
                
            }
            right++;
            left = right;
        } 
        else if (isPunctuator(str[right]) == true && left != right 
                   || (right == len && left != right)) 			
        {
            //check if parsed substring is a keyword or identifier or number
            char* sub = subString(str, left, right - 1);   //extract substring
            if (isKeyword(sub) == true)
            {
                std::cout << sub <<" IS A KEYWORD\n";
            }
            else if (isIntegerLiteral(sub) == true)
            {
                std::cout << sub <<" IS A NUMBER\n";
            }
            else if (validIdentifier(sub) == true
                     && isPunctuator(str[right - 1]) == false)
            {
                std::cout << sub <<" IS A VALID IDENTIFIER\n";
            }
            else if (validIdentifier(sub) == false
                     && isPunctuator(str[right - 1]) == false)
            {
                if(strlen(sub) != 0)
                {
                    std::cout << sub<<" [IS NOT A VALID IDENTIFIER]\n";
                    
                }
            }
    
            left = right;
        }
    }
}

int main() 
{
    ifstream file("file_to_parse.txt");
    if (file.is_open()) 
    {
        string line;
        while (getline(file, line)) 
        {
            // very funky way of casting string to char *
            // since the functino is taking as const char *, we don't
            // need to worry about parse function modifying the line variable.
            parse(&*line.begin()); 
        }
        file.close();
    }
    return 0;
}
