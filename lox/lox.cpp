#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "Scanner.cpp"


class Lox {
    bool hadError = false;
    void run(std::ifstream &in){
        std::string token;
        std::vector<Token>tokens = Scanner(in).scanTokens();
    }

    void error(int line, std::string& msg){
        std::string where;
        report(line, where, msg);
    }
    void report(int line, std::string &where, std::string &msg){
        hadError = true;
        std::cout << "[line" << line << "]" << where << ": " << msg << "\n";
    }
public:
    void runPrompt(){
        std::cout << "runPrompt\n";
    }
    void runFile(char* path){
        std::ifstream in(path);
        run(in);

        // Indicate an error in the exit code
        if (hadError)
            exit(65);

        in.close();
    }
    Lox(int argc, char* argv[]){
        if(argc > 2){
            std::cout << "Usage: jlox [script]\n";
            return;
        }
        else if(argc == 2){
            runFile(argv[1]);
        }
        else{
            runPrompt();
        }
    }
};

int main(int argc, char *argv[]){
    Lox Compiler(argc, argv);
    return 0;
}