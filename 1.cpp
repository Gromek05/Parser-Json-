// // data := `{
// //     "Name":"cats are cute",
// //     "Age": 123423,
// //     "IsAlive": true,
// //     "ScoreInt" : [1,2,3],
// //     "ScoreBool" : [false, false, true],
// //     "Scorelist" : [[1,2,3], [23,3]],
// //     "Scorestr" : ["ma", "sco", "sec"],
// //     "address" : {
// //             "city" : "Mumbai",
// //             "state" : "Maharashtra"
// //         }
// //     }

// // Name : cats are cute
// // Age : 123423
// // IsAlive : true
// // ScoreInt : [1 2 3]
// // ScoreBool : [false false true]
// // Scorelist : [[1 2 3] [23 3]]
// // Scorestr : [ma sco sec]
// // address : map[city:Mumbai state:Maharashtra]

#include <iostream>
#include <map>
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
            case '[':
                token = {TokenType::LBRACK, "["};
                break;
            case ']':
                token = {TokenType::RBRACK, "]"};
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

class Parser {
private:
    Lexer lexer;
    Token curToken;
    Token peekToken;

    void nextToken() {
        curToken = peekToken;
        peekToken = lexer.next_token();
    }




string parseArray() {
    string result = "[";               // Inicjalizujemy wynik jako otwierający nawias kwadratowy
    bool first = true;                // czy jesteśmy przy pierwszym elemencie

    nextToken(); // omijamy '['        // Przechodzimy do pierwszego tokena po '['

    while (curToken.type != TokenType::RBRACK && curToken.type != TokenType::EOF_TOKEN) {
        // Pętla dopóki nie napotkamy końca tablicy ']' lub końca pliku

        if (curToken.type == TokenType::COMMA) {
            nextToken(); // omijamy przecinki między wartościami
            continue;    // przechodzimy do kolejnego tokena
        }

        if (!first) result += " ";     // Dodajemy spację przed kolejnym elementem jeśli to nie pierwszy

        if (curToken.type == TokenType::INT ||
            curToken.type == TokenType::BOOL ||
            curToken.type == TokenType::STRING) {
            result += curToken.literal; // Dodajemy wartość (int, bool, string) do wyniku
            nextToken();                // Przechodzimy do kolejnego tokena
        } else if (curToken.type == TokenType::LBRACK) {
            result += parseArray();     // Rekurencyjnie przetwarzamy zagnieżdżoną tablicę
        } else {
            break;                      // Nieznany token - przerywamy parsowanie tablicy
        }

        first = false;                  // Po pierwszym elemencie ustawiamy flagę
    }

    if (curToken.type == TokenType::RBRACK) {
        result += "]";       // Dodajemy zamykający nawias jeśli faktycznie znaleźliśmy ']'
        nextToken();         // Przechodzimy do kolejnego tokena po ']'
    } else {
        result += "]";       // Jeśli nawiasu nie było, i tak kończymy wynik nawiasem
    }

    return result;           // Zwracamy zbudowaną reprezentację tablicy jako string
}




void parse(map<string, string>& values) {
    while (curToken.type != TokenType::RBRACE && curToken.type != TokenType::EOF_TOKEN) {
        // Pętla dopóki nie napotkamy końca obiektu '}' lub końca pliku

        if (curToken.type != TokenType::STRING) {
            nextToken();      // Jeśli nie znaleźliśmy klucza jako string, pomijamy token
            continue;
        }

        string key = curToken.literal;  // Zapisujemy klucz (nazwa pola w obiekcie)
        nextToken();  // Przechodzimy do ':' po kluczu

        if (curToken.type != TokenType::COLON) {
            return;           // Jeśli nie ma dwukropka po kluczu - błąd formatu
        }

        nextToken();  // Przechodzimy do wartości po dwukropku

        if (curToken.type == TokenType::LBRACK) {
            values[key] = parseArray(); // Jeśli wartość to tablica - wywołujemy `parseArray`
        } else if (curToken.type == TokenType::INT ||
                   curToken.type == TokenType::BOOL ||
                   curToken.type == TokenType::STRING) {
            values[key] = curToken.literal; // Wartość to zwykła wartość (int, bool, string)
            nextToken();                    // Przechodzimy do kolejnego tokena
        } else {
            return;           // Błąd - nieznany typ wartości
        }

        if (curToken.type == TokenType::COMMA) {
            nextToken();      // Jeśli jest przecinek, to przechodzimy do kolejnego pola
        } else if (curToken.type != TokenType::RBRACE) {
            return;           // Jeśli nie ma przecinka ani zamknięcia obiektu - błąd formatu
        }
    }

    if (curToken.type == TokenType::RBRACE) {
        nextToken();          // Jeśli znaleziono '}', przechodzimy dalej
    }
}


public:
    Parser(const std::string& input) : lexer(input) {
        nextToken();
        nextToken();
    }

    map<string, string> parseJson() {
        map<string, string> values;

        if (curToken.type != TokenType::LBRACE) {
            cout << "Brak znaku { na poczatku" << endl;
            return values;
        }

        nextToken();  // ominiecie '{'
        parse(values);

        return values;
    }
};

int main(){

    string data1 = "{\"Name\":\"cats are cute\", \"Age\": 11, \"isAlive\": true, \"ScoreInt\": [ 1,2,3,[9,8,7]]}";
    // string data1 = "{\"ScoreBool\": [false, false, true]}";
    Parser parser(data1);
    map<string, string> values = parser.parseJson();

    for (const auto& pair : values) {
        cout << pair.first << "  :  " << pair.second << std::endl;
    }

}
