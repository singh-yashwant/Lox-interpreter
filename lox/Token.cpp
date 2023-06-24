#include <string>
#include <boost/preprocessor.hpp>

// TODO:: Move this string enum macro to seperate file
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
    case elem : return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
    enum name {                                                               \
        BOOST_PP_SEQ_ENUM(enumerators)                                        \
    };                                                                        \
                                                                              \
    inline const char* ToString(name v)                                       \
    {                                                                         \
        switch (v)                                                            \
        {                                                                     \
            BOOST_PP_SEQ_FOR_EACH(                                            \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
                name,                                                         \
                enumerators                                                   \
            )                                                                 \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                                     \
    }

// TokenType ENUM
DEFINE_ENUM_WITH_STRING_CONVERSIONS(TokenType, 
  // Single Character tokens
  (LEFT_PAREN)(RIGHT_PAREN)(LEFT_BRACE)(RIGHT_BRACE)
  (COMMA)(DOT)(MINUS)(PLUS)(SEMICOLON)(SLASH)(STAR)

  // One or two character tokens.
  (BANG)(BANG_EQUAL)
  (EQUAL)(EQUAL_EQUAL)
  (GREATER)(GREATER_EQUAL)
  (LESS)(LESS_EQUAL)

  // Literals.
  (IDENTIFIER)(STRING)(NUMBER)

  // Keywords.
  (AND)(CLASS)(ELSE)(FALSE)(FUN)(FOR)(IF)(NIL)(OR)
  (PRINT)(RETURN)(SUPER)(THIS)(TRUE)(VAR)(WHILE)

  (EOFI)
)

struct Literal {
private: 
  std::string value; 
public:
  Literal(Literal &literal){
    value = literal.value;
  }
  Literal(){
    value = "";
  }
  // TODO:: Figure out what goes here
  std::string literalToString(){
    return "";
  }
};

class Token{
  TokenType type;
  Literal* literal;
  std::string lexeme;
  int line;  

public:
  Token(TokenType type, std::string lexeme, Literal *literal, int line): 
    type(type),
    lexeme(lexeme),
    literal(literal),
    line(line) {}

  std::string toString(){
    return ToString(type) + lexeme + literal->literalToString();
  }
};

