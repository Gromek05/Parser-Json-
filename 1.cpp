// data := `{
//     "Name":"cats are cute",
//     "Age": 123423,
//     "IsAlive": true,
//     "ScoreInt" : [1,2,3],
//     "ScoreBool" : [false, false, true],
//     "Scorelist" : [[1,2,3], [23,3]],
//     "Scorestr" : ["ma", "sco", "sec"],
//     "address" : {
//             "city" : "Mumbai",
//             "state" : "Maharashtra"
//         }
//     }

// Name : cats are cute
// Age : 123423
// IsAlive : true
// ScoreInt : [1 2 3]
// ScoreBool : [false false true]
// Scorelist : [[1 2 3] [23 3]]
// Scorestr : [ma sco sec]
// address : map[city:Mumbai state:Maharashtra]


#include <iostream>

using namespace std;




enum class TokenType{
    ILLEGAL,
    EOF_TOKEN,    
    COMMA,  
    COLON,  
    LBRACE,
    RBRACE,
    LBRACK,
    RBRACK,
    INT,
    BOOL,
    STRING
};


string token_type_to_string(TokenType type){ // dla tokentype zwrocenie odpowiedniego napisu
    switch(type)
    {
        case TokenType::ILLEGAL:
        return "ILLEGAL";
        break;

        case TokenType::EOF_TOKEN:
        return "";
        break;

        case TokenType::COMMA:
        return ",";
        break;

        case TokenType::COLON:
        return ":";
        break;


        case TokenType::LBRACE:
        return "{";
        break;


        case TokenType::RBRACE:
        return "}";
        break;

        case TokenType::LBRACK:
        return "[";
        break;


        case TokenType::RBRACK:
        return "]";
        break;


        case TokenType::INT:
        return "INT";
        break;


        case TokenType::BOOL:
        return "BOOL";
        break;


        case TokenType::STRING:
        return "STRING";
        break;
        
        default: 
        return "";
        break;
    }
} 

struct Token {
    TokenType type;
    string literal;
};


class Lexer {
    private:
    string input;
    int position = 0;
    int readPosition = 0;
    char ch; // byte ch (tablica ASCI )

    void read_char(){
        if (readPosition >= input.length()){ // koniec napisu
            ch = 0; // ustawiamy ch
        }

        else {
            ch = input[readPosition]; // wczytujemy znak
        }

        position = readPosition;
        readPosition++;
    }

    string read_string(){
        string result;
        read_char();
        while (ch!='"' && ch != 0){ // przechodzimy przez wyraz az dojde do " lub do konca wyrazu
            result += ch;
            read_char();
        }
        return result;
    }

    string read_number(){
        string result;
        while(isdigit(ch)){ // sprawdza czy jest liczba
            result += ch;
            read_char();
        }
        return result;
        
    }


    string read_bool(){
        string result;
        while(isalpha(ch)){ // sprawdza czy jest literą alfabetu / nie ma cudzysłowia True/False
            result += ch;
            read_char();
        }
        return result;
    }

    public:

    Lexer(const string &input):input(input){ // konstruktor
        read_char();
    }

    Token next_token(){
        Token token;

        // pominiecie bialych znakow
        while (isspace(ch)) read_char();

        switch (ch){
            case '{':
                token = {TokenType::LBRACE, "{"};
                break;
            case '}':
                token = {TokenType::RBRACE, "}"};
                break;
            case ':':
                token = {TokenType::COLON, ":"};
                break;
            case ',':
                token = {TokenType::COMMA, ","};
                break;
            case '"':
                token = {TokenType::STRING, read_string()}; // metoda read_string zwroci napis po znaku "
                break;
            case 0: // sprawdza czy koniec wyrazu
                token = {TokenType::EOF_TOKEN, ""};
                break;
            default:
                if (isdigit(ch)){ // jesli jest cyfra
                    token = {TokenType::INT, read_number()};
                    if (ch == ',') {
                        readPosition--;  // cofamy się żeby jeszcze raz odczytac przecinek
                    }
                }
                else if (isalpha(ch)){ // jesli znak jest z alfabetu
                    token = {TokenType::BOOL, read_bool()};
                    if (ch == ',') {
                        readPosition--;  // cofamy się żeby jeszcze raz odczytac przecinek
                    }
                }
                else { // jesli inny znak
                    token = {TokenType::ILLEGAL, "ILLEGAL"};
                    read_char();
                }
                break;
                
        }

        read_char();
        return token;
    }

};


int main(){
    string data = 
    "{\n"
    "    \"Name\": \"cats are cute\",\n"
    "    \"Age\": 123423,\n"
    "    \"Isallive\": true\n"
    "}";
    
   Token token;
   Lexer lexer(data);
   do {
    token = lexer.next_token();
    cout << token_type_to_string(token.type) << "   " << token.literal << endl;
   } while(token.type != TokenType::EOF_TOKEN);

}
