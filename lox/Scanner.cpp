#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iterator>
#include <iostream>
#include "Token.cpp"

typedef std::istreambuf_iterator<char> buf_iter;

class Scanner{
    std::ifstream &source;
    std::vector<Token> tokens;
    std::map<std::string, TokenType>keywords;
    int start, current, line;
    void addToken(TokenType type, int line);
    void addToken(TokenType type, std::string lexeme, int line);
    bool match(char c);
    void matchString();
    void matchNumber(char c);
    void matchIdentifier(char c);

public:
    Scanner(std::ifstream &in):
        source(in),
        start(0),
        current(0),
        line(1) {
            // Initialize keywords
            // TODO: Move to a helper function
            // TODO: Separate out keywords into a data class
            keywords.insert({"and",    AND});
            keywords.insert({"class",  CLASS});
            keywords.insert({"else",   ELSE});
            keywords.insert({"false",  FALSE});
            keywords.insert({"for",    FOR});
            keywords.insert({"fun",    FUN});
            keywords.insert({"if",     IF});
            keywords.insert({"nil",    NIL});
            keywords.insert({"or",     OR});
            keywords.insert({"print",  PRINT});
            keywords.insert({"return", RETURN});
            keywords.insert({"super",  SUPER});
            keywords.insert({"this",   THIS});
            keywords.insert({"true",   TRUE});
            keywords.insert({"var",    VAR});
            keywords.insert({"while",  WHILE});
        }
    
    std::vector<Token> scanTokens();
    std::string toString(std::ifstream &in);
};

// FIXME
std::string Scanner::toString(std::ifstream &in){
    return "WIP";
}

std::vector<Token> Scanner::scanTokens(){
    char cur_char;
    while(source.get(cur_char)){
        // std::cout << cur_char << " ";
        // start = current;
        current++;
        switch(cur_char){
            // TODO: Prefix all token names with enum identifier
            case '(': addToken(LEFT_PAREN, line); break;
            case ')': addToken(RIGHT_PAREN, line); break;
            case '{': addToken(LEFT_BRACE, line); break;
            case '}': addToken(RIGHT_BRACE, line); break;
            case ',': addToken(COMMA, line); break;
            case '.': addToken(DOT, line); break;
            case '-': addToken(MINUS, line); break;
            case '+': addToken(PLUS, line); break;
            case ';': addToken(SEMICOLON, line); break;
            case '*': addToken(STAR, line); break;
            case '!':
                addToken(match('=') ? BANG_EQUAL : BANG, line);
                break;
            case '=':
                addToken(match('=') ? EQUAL_EQUAL : EQUAL, line);
                break;
            case '<':
                addToken(match('=') ? LESS_EQUAL : LESS, line);
                break;
            case '>':
                addToken(match('=') ? GREATER_EQUAL : GREATER, line);
                break;
            case '/':
                // TODO: Match C style inline comments /* ... */
                if (match('/'))
                    while(source.get() != '\n');
                else 
                    addToken(SLASH, line);
                break;
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                current++;
                break;
            case '\n':
                line++;
                current = 0;
                break;
            case '"': matchString(); break;
            default:
                if(std::isdigit(cur_char))
                    matchNumber(cur_char);
                else if(std::isalpha(cur_char))
                    matchIdentifier(cur_char);
                else {
                    // log error
                }
                break;
        }
    }
    // TODO: Add a debug log plugin to dump this kind of data
    std::cout << "Total lines read: " << line << "\n";
    std::cout << "Tokens collected: " << tokens.size() << "\n";
    tokens.push_back(Token(TokenType::EOFI, "", nullptr, 0));
    return tokens;
}

void Scanner::addToken(TokenType type, int line) {
    tokens.push_back(Token(type, ToString(type), nullptr, line));
}

void Scanner::addToken(TokenType type, std::string lexeme, int line){
    // TODO: Create appropriate literal objects below
    if (type == TokenType::STRING)
        tokens.push_back(Token(type, lexeme, nullptr, line));
    if (type == TokenType::NUMBER)
        tokens.push_back(Token(type, lexeme, nullptr, line));
}

bool Scanner::match(char c){
    if(source.peek() && source.peek() == c){
        source.get();
        current++;
        return true;
    }
    return false;
}

// bool Scanner::matchNext(char c){
//     if(source.eof())
//         return false;
//     return source.peek() == c;
// }

void Scanner::matchString(){
    std::string tok = "";
    char next_char;
    while(!source.eof() && source.peek() != '"'){
        next_char = source.peek();
        if(next_char == '\n')
            tok += source.get();
        tok += source.get();
    }
    if(source.peek() != '"'){
        // log unterminated string error
        std::cout << "Unterminated string found\n";
        return;
    }

    // add token
    addToken(TokenType::STRING, tok, line);
    std::cout << "\n Matched String :: " << tok << "\n";
    // closing "
    source.get();
}

void Scanner::matchNumber(char cur_char){
    std::string num = "";
    num += cur_char;
    char next_char = source.peek();
    
    // Consume pre decimal digits
    while(!source.eof() &&  next_char != '\n' && std::isdigit(next_char)){
        num += source.get();
        next_char = source.peek();
    }

    if(next_char == '.'){
        // Consume '.'
        num += source.get();
        next_char = source.peek();
        if(!source.eof() && std::isdigit(next_char)){

            // Consume post decimal digits
            while(!source.eof() &&  next_char != '\n' && std::isdigit(next_char)){
                num += source.get();
                next_char = source.peek();
            }
        }
        else{
            // Log error
            std::cout << line << " :: No digits followed by decimal\n";
            return;
        }
    }

    addToken(TokenType::NUMBER, num, line);
    std::cout << "\n Matched Number :: " << num << "\n";
}

void Scanner::matchIdentifier(char cur_char){
    std::string identifier = "";
    identifier += cur_char;
    char next_char = source.peek();
    while(!source.eof() && std::isalnum(next_char)){
        identifier += source.get();
        next_char = source.peek();
    }
    
    // Check if the identifier is a keyword
    if(keywords[identifier]){
        std::cout << "\n Matched keyword :: " << identifier << "\n";
        addToken(keywords[identifier], line);
        return;
    }

    std::cout << "\n Matched Identifier :: " << identifier << "\n";
    addToken(TokenType::IDENTIFIER, identifier, line);
}