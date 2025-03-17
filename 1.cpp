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

#define ILLEGAL "ILLEGAL"
// #define EOF     "EOF" 
#define COMMA  ","
#define COLON  ":"
#define LBRACE  "{"
#define RBRACE  "}"
#define LBRACK  "["
#define RBRACK  "]"
#define INT     "INT"
#define BOOL    "BOOL"
#define STRING  "STRING"


// class Lexer {
//     string input;
//     int position;
//     int readPosition;
//     char ch; // byte ch
// };



// Lexer createLexer(string input) {
//     l = &Lexer{input: input};
//     l.readChar();
//     return l;
// }


string token(char a){
    string output = "";
    switch (a) {  //sprawdzamy znak

        case '{':
            output += "{    ";
            output += a;
            output += "\n";
            // tok = token.Token{Type: token.LBRACE, Literal: string(l.ch)};
            break;
        case '}':
            output += "}    ";
            output += a;
            output += "\n";
            // tok = token.Token{Type: token.RBRACE, Literal: string(l.ch)};
            break;
        case ':':
            output += ":    ";
            output += a;
            output += "\n";
            // tok = token.Token{Type: token.COLON, Literal: string(l.ch)};
            break;
        case '"': 
            output += "STRING    ";   // jesli cudzyslów to dajemy STRING (trzeba tez dodac INT i BOOL)
            // tok.Literal = l.readString() // This reads characters until " is encountered
            // tok.Type = token.STRING;
            break;
        case ',':
            output += ",    ";
            output += a;
            output += "\n";
            // tok = token.Token{Type: token.COMMA, Literal: string(l.ch)};
            break;
        case 0:
            // tok.Literal = "";
            // tok.Type = token.EOF;
            break;
        default:
            output += a;
        }
        return output;
}

string reader_string(string input) {  // w petli przechodzimy przez string wejsciowy
    string output = "";  // string wyjsciowy
    int firstMarks = 0; // zmienna sprawdzajaca czy to pierwszy cudzysłow czy drugi
    for (int i = 0; i < input.length(); i++) {
        char a = input[i]; // bierzemy jeden znak z input
        if (a == '\"' && firstMarks == 0) { // sprawdzam czy znak jest cudzysłow
            firstMarks = 1;
        } else if (a == '\"' && firstMarks == 1) {  // jesli drugi cudzyslow
            firstMarks = 0;
            output += "\n"; // nowa linia w output
            continue; // nie wywyolujemy metody token ponizej
        }
        output += token(a);   // sprawdzamy znak, wywolujac metode token
        
    }
    return output;

}

int main(){
    string data = 
    "{\n"
    "    \"Name\": \"cats are cute\"\n"
    // "    \"Age\": 123423\n"
    "}";
    data = reader_string(data);
    cout << data;

}

//chcemy miec taki wynik:
// {    {
//     STRING    Name
//     :    :
//     STRING    cats are cute
//     }    }
