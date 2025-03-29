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

// #define ILLEGAL "ILLEGAL"
// // #define EOF     "EOF" 
// #define COMMA  ","
// #define COLON  ":"
// #define LBRACE  "{"
// #define RBRACE  "}"
// #define LBRACK  "["
// #define RBRACK  "]"
// #define INT     "INT"
// #define BOOL    "BOOL"
// #define STRING  "STRING"


enum class Token_type{
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

struct Token {
    Token_type type;
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

    public:

    Lexer(const string &input):input(input){ // konstruktor
        // this->input = input;
        read_char();
    }

    Token next_token(){
        Token token;

        // pominiecie bialych znakow
        while (isspace(ch)) read_char();


        switch (ch){
            case '{':
                token = {Token_type::LBRACE, "{"};
                break;
            case '}':
                token = {Token_type::RBRACE, "}"};
                break;
            case ':':
                token = {Token_type::COLON, ":"};
                break;
            case ',':
                token = {Token_type::COMMA, ","};
                break;
            case '"':
                token = {Token_type::STRING, read_string()}; // metoda read_string zwroci napis po znaku "
                break;
            case 0: // sprawdza czy koniec wyrazu
                token = {Token_type::EOF_TOKEN, ""};
                break;
        }

        read_char();
        return token;
    }

};


int main(){
    string data = 
    "{\n"
    "    \"Name\": \"cats are cute\"\n"
    // "    \"Age\": 123423\n"
    "}";
    
   Token token;
   Lexer lexer(data);
   do {
    token = lexer.next_token();
    cout << "Token Type: " << static_cast<int>(token.type) << " " << token.literal << endl; // problem z wyswietleniem token.type jako string
   } while(token.type != Token_type::EOF_TOKEN);

}

//chcemy miec taki wynik:
// {    {
//     STRING    Name
//     :    :
//     STRING    cats are cute
//     }    }