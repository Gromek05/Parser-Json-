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
        while(isalpha(ch)){ // sprawdza czy jest litera alfabetu / nie ma cudzyslowia True/False
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
                        readPosition--;  // cofamy sie zeby jeszcze raz odczytac przecinek
                    }
                }
                else if (isalpha(ch)){ // jesli znak jest z alfabetu
                    token = {TokenType::BOOL, read_bool()};
                    if (ch == ',') {
                        readPosition--;  // cofamy sie zeby jeszcze raz odczytac przecinek
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

class Parser {  // klasa Parser będzie analizować strukturę JSON
private:
    Lexer lexer;         // Obiekt analizatora leksykalnego (dzieli tekst na tokeny)
    Token curToken;      // Bieżący token (obecnie analizowany element JSON)
    Token peekToken;     // Kolejny token (do podejrzenia, co będzie dalej)

    // Przesuwa analizę o jeden token do przodu
    void nextToken() {
        curToken = peekToken;             // Ustaw bieżący token na poprzedni peekToken
        peekToken = lexer.next_token();   // Pobierz kolejny token z leksera
    }

    // Rekurencyjna funkcja do parsowania jednej pary klucz-wartość
    void parseKeyValue(map<string, string>& values) {
        string key;  // Miejsce na zapisanie klucza

        switch (curToken.type) {  // Sprawdź typ bieżącego tokena
            case TokenType::STRING:  // Klucz musi być stringiem
                key = curToken.literal;  // Zapisz klucz

                nextToken();  // Przejdź do tokena z dwukropkiem `:`

                if (curToken.type != TokenType::COLON) {  // Sprawdź, czy po kluczu jest ':'
                    cout << "Expected COLON, got: " << (int)curToken.type << endl;
                    return;  // kończymy
                }

                nextToken();  // Przejdź do wartości przypisanej do klucza

                // Jeżeli wartość to boolean
                if (curToken.type == TokenType::BOOL) {
                    string boolStr;
                    if (curToken.literal == "true") {
                        boolStr = "true";  // Poprawna wartość logiczna
                    } else if (curToken.literal == "false") {
                        boolStr = "false";
                    } else {
                        cout << "Invalid bool value" << endl;  // Błąd: nieznana wartość boolowska
                    }
                    values[key] = boolStr;  // Zapisz do mapy jako string
                }

                // Jeżeli wartość to liczba całkowita (INT)
                else if (curToken.type == TokenType::INT) {
                    try {
                        int intVal = stoi(curToken.literal);       // Konwertuj string na int
                        values[key] = to_string(intVal);           // Zapisz z powrotem jako string do mapy
                    } catch (const invalid_argument&) {
                        cout << "Invalid integer value" << endl;   // Błąd konwersji
                    }
                }

                // Jeżeli wartość to string
                else if (curToken.type == TokenType::STRING) {
                    values[key] = curToken.literal;  // Po prostu zapisujemy stringa do mapy
                }

                nextToken();  // Przejdź do następnego tokena po wartości (może to być przecinek lub })

                // Jeśli to nie koniec obiektu (czyli nie RBRACE – '}' ), to parsujemy kolejną parę
                if (curToken.type != TokenType::RBRACE) {
                    nextToken();  // Przejdź do kolejnego klucza
                    parseKeyValue(values);  // Wywołaj rekurencyjnie analizę następnej pary
                }
                break;
        }
    }

public:
    // Konstruktor – przyjmuje string JSON jako wejście
    Parser(const std::string& input) : lexer(input) {
        nextToken();  // Pobierz pierwszy token (peekToken)
        nextToken();  // Przesuń peekToken do przodu i ustaw pierwszy curToken
    }

    // Funkcja zwraca sparsowaną mapę (klucz:wartość)
    map<string, string> parseJson() {
        map<string, string> values;  // Tu zbieramy wynik

        if (curToken.type != TokenType::LBRACE)  // Sprawdź, czy JSON zaczyna się od '{'
            return values;  // Jeśli nie, zwróć pustą mapę

        nextToken();        // Przejdź do pierwszego klucza
        parseKeyValue(values);  // Rozpocznij analizę klucz–wartość

        return values;  // Zwróć wynik
    }
};

int main(){

    string data1 = "{\"Name\":\"cats are cute\", \"Age\": 11, \"isAlive\": true}";

    Parser parser(data1);
    map<string, string> values = parser.parseJson(); // Uruchamia analizę JSON-a.

    for (const auto& pair : values) { // Iteruje po wszystkich parach klucz:wartość w mapie
        cout << pair.first << "  :  " << pair.second << std::endl; // wypisuje kazda parę
    }

}