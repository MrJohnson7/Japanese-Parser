#include<iostream>
#include<fstream>
#include<string>
//#include"parser.cpp"
#include<vector>
#include<stdio.h>

using namespace std;




// --------------------------------------------------------------------------------------- //
// ----------------- Gloabl Variables ---------------------------------------------------- //
ifstream fin;  // global stream for reading from the input file
ifstream lex;  // global stream for reading in lexicon
ofstream fout; // global stream for writing into an output file

string saved_E_lexeme;
string saved_E_word;

enum tokentype {
    WORD1,        // 0
    WORD2,        // 1
    PERIOD,       // 2
    ERROR,        // 3
    VERB,         // 4
    VERBNEG,      // 5
    VERBPAST,     // 6
    VERBPASTNEG,  // 7
    IS,           // 8
    WAS,          // 9
    OBJECT,       // 10
    SUBJECT,      // 11
    DESTINATION,  // 12
    PRONOUN,      // 13
    CONNECTOR,    // 14 
    EOFM          // 15
};

//ENUMERATED TARGET VALLUES FOR gen(line_type) function
enum lineType {
  ACTR,
  CONN,
  ACTN,
  TENS,
  TO,
  DESC,
  OBJT
  };

string tokenName[30] = {
 "WORD1",          // tokenName[0]
"WORD2",           // tokenName[1]
"PERIOD",          // tokenName[2]
"ERROR",           // tokenName[3]
"VERB",            // tokenName[4]
"VERBNEG",         // tokenName[5]
"VERBPAST",        // tokenName[6]
"VERBPASTNEG",     // tokenName[7]
"IS",              // tokenName[8]
"WAS",             // tokenName[9]
"OBJECT",          // tokenName[10]
"SUBJECT",         // tokenName[11]
"DESTINATION",     // tokenName[12]
"PRONOUN",         // tokenName[13]
"CONNECTOR",       // tokenName[14]
"EOFM" };          // tokenName[15]

string reservedWords[19][2] = {         // INDEX START AT 0,0
  {"masu", tokenName[4]},               //reservedWords [0,0] && [0, 1]
  {"masen", tokenName[5]},              //reservedWords [1,0] && [1, 1]
  {"mashita", tokenName[6]},            //reservedWords [2,0] && [2, 1]
  {"masendeshita", tokenName[7]},       //reservedWords [3,0] && [3, 1]
  {"desu", tokenName[8]},               //reservedWords [4,0] && [4, 1]
  {"deshita", tokenName[9]},            //reservedWords [5,0] && [5, 1]
  {"o", tokenName[10]},                 //reservedWords [6,0] && [6, 1]
  {"wa", tokenName[11]},                //reservedWords [7,0] && [7, 1]
  {"ni", tokenName[12]},                //reservedWords [8,0] && [8, 1]
  {"watashi", tokenName[13]},           //reservedWords [9,0] && [9, 1]
  {"anata",  tokenName[13]},            //reservedWords [10,0] && [10, 1]
  {"kare",  tokenName[13]},             //reservedWords [11,0] && [11, 1]
  {"kanojo",  tokenName[13]},           //reservedWords [12,0] && [12, 1]
  {"sore",  tokenName[13]},             //reservedWords [13,0] && [13, 1]
  {"mata",  tokenName[14]},             //reservedWords [14,0] && [14, 1]
  {"soshite", tokenName[14]},           //reservedWords [15,0] && [15, 1]
  {"shikashi", tokenName[14]},          //reservedWords [16,0] && [16, 1]
  {"dakara", tokenName[14]},            //reservedWords [17,0] && [17, 1]
  {"eofm", tokenName[15]},              //reservedWords [18,0] && [18, 1]
};

tokentype saved_token; // saved token corespeonding to saved word.
string saved_lexeme;   // saved word  from input file
bool   istoken;        // flag: Is there an active token?
string filename;      // name of input file.


// ---- Function Prototypes --- //
bool word(string s);
bool period(string s);
int scanner(tokentype&, string&); // SO SCANNER CAN BE CALLED BY MAIN
void syntaxerror1(tokentype correct, string found);
void syntaxerror2(string found, string out);

void story();
void s();
void noun();
void afterSubject();
void afterNoun();
void tense();
void be();
void afterObject();



/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//=================================================
// File translator.cpp written by Group Number: 06
//=================================================

// ----- Additions to the parser.cpp ----------------------------------------------------------------------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: Kyle Monroe 

vector<pair<string,string>> lexicon;   // // Declaring a 2D vector of pairs

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result   
//                 in saved_E_word
//  Done by: Kyle Monroe
void getEword(){
  for(int i = 0 ; i < lexicon.size() ; i++){
    // check current lexeme against Japanese column in the vector
    if(lexicon.at(i).first == saved_lexeme){
      // if its found, and its corresponding english word
      cout << lexicon.at(i).first <<" ---→ " << lexicon.at(i).second << endl;
      // update saved_E_word  
      saved_E_word = lexicon.at(i).second;
        return;
    }
  }
  // saved_lexeme was not found, print out stateemnt
  cout << saved_lexeme << " ---> not found!\n";
  // keep saved_lexeme by copying it into saved_E_word
  saved_E_word = saved_lexeme;
  return;
}

//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
//  Done by: ** Noah, Kyle, Nick

void gen(lineType line_type){
  switch(line_type){
    case ACTR: {
      fout<<"ACTOR: "<<saved_E_word<<endl;
      break;
    }
    case ACTN: {
      fout << "ACTION: " << saved_E_word << endl;
      break;
    }
    case CONN: {
      fout << "CONNECTOR: " << saved_E_word << endl;
      break;
    }
    case TENS: {
      fout << "TENSE: "<<tokenName[saved_token]<<endl;

      break;
    }
    case TO: {
      fout << "TO: " << saved_E_word << endl;
      break;
    }
    case DESC: {
      fout << "DESCRIPTION: " << saved_E_word << endl;
      break;
    }
    case OBJT: {
      fout << "OBJECT: " << saved_E_word << endl;
      break;
    }
    default:{
      cout<< "how did you get here?\n";
      return;
    } 
  }
}






// ----- Changes to the parser.cpp content -----------------------------------------------------------------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.

// ---------- UTILITY FUNCTIONS FOR BUILDING DICTIONARY FROM FILE ------------------ //
void buildLexiconDictionary(string filename){
  lex.open(filename);                       // Open lexicon file
  string v1, v2;                            // Declare temp variables to store words
 
  while (lex >> v1 >> v2){                  // Loop through line in pairs of words
    lexicon.push_back(make_pair(v1,v2));    // Make and store pairs in vector.
  }  
  
  lex.close();
}

//Utility function: Prints read in lexicon to cout for testing
void printLex(){
  cout << "Here is the read in lexicon file\n" << endl;
  for(auto i = 0 ; i < lexicon.size() ; i++){
    cout << (i + 1) << " : "<< lexicon.at(i).first << " ------→ " << lexicon.at(i).second << endl;
  }
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------- Driver ---------------------------





// The final test driver to start the translator
// Done by:  ** Noah, Kyle, Nick
int main()
{
  //** opens the lexicon.txt file and reads it into lexicon
  buildLexiconDictionary("lexicon.txt");
  //** closes lexicon.txt handled by buildLexiconDictionary()
  //printLex();          // Just for Debugging

  //** opens the output file translated.txt
  fout.open("translated.txt");
  
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  story();
  //** closes the input file 
  fin.close();
  //** closes traslated.txt
 fout.close();
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions


// ====================================================================================== //
// =================== Scanner.cpp contents ============================================= //

// --------- Two DFAs ---------------------------------

// WORD DFA
// Done by: KYLE MONROE
// RE:   ALL
bool word(string s)
{

    int state = 0; //q0
    int charpos = 0;

    // replace the following todo the word dfa  **
    while (s[charpos] != '\0') {
        //state 0 is q0
        //state 1 is q0q1
        //state 2 is q0qy
        //state 3 is qc
        //state 4 is qy
        //state 5 is qsa
        //state 6 is qs
        //state 7 is qt
        //cout << s[charpos] << endl;
        //this is initial transitions from q0
        if (state == 0 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; //this is q0q1
        else if (state == 0 && s[charpos] == 'c') //c
            state = 3; //this is qc
        else if (state == 0 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'p' || s[charpos] == 'r' || s[charpos] == 'h' || s[charpos] == 'n')) // bgkmpr,h,n
            state = 4; //this is qy
        else if (state == 0 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'j' || s[charpos] == 'y'))//dwzj,y
            state = 5; // this is qsa
        else if (state == 0 && s[charpos] == 's') //s
            state = 6; //this is qs
        else if (state == 0 && s[charpos] == 't') //t
            state = 7; //this is qt

          //transitions from q0q1
        else if (state == 1 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'p' || s[charpos] == 'r' || s[charpos] == 'h')) // bgkmpr,h
            state = 4; //this is qy
        else if (state == 1 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'j' || s[charpos] == 'y'))//dwzj and y!
            state = 5; // this is qsa
        else if (state == 1 && s[charpos] == 'c')// testing c
            state = 3; // this is qc
        else if (state == 1 && s[charpos] == 's')//testing s
            state = 6; // this is qs
        else if (state == 1 && s[charpos] == 't')//testing line t
            state = 7; // this is qt
        else if (state == 1 && s[charpos] == 'n')//testing n
            state = 2; // this is q0qy
        else if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; //q0q1 loop in the diagram

          //transitions from q0qy
        else if (state == 2 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'k' || s[charpos] == 'm' || s[charpos] == 'p' || s[charpos] == 'r' || s[charpos] == 'h' || s[charpos] == 'n')) // bgkmpr,h,n
            state = 4; //this is qy
        else if (state == 2 && (s[charpos] == 'd' || s[charpos] == 'w' || s[charpos] == 'z' || s[charpos] == 'j' || s[charpos] == 'y'))//dwzj,y
            state = 5;//this is qsa
        else if (state == 2 && s[charpos] == 'c')//testing c
            state = 3; // this is qc
        else if (state == 2 && s[charpos] == 's')//testing s
            state = 6; // this is qs
        else if (state == 2 && s[charpos] == 't')//testing t
            state = 7; // this is qt
        else if (state == 2 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; //q0q1

          //transitions from qsa
        else if (state == 5 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; // this is q0q1

          //transitions from qt
        else if (state == 7 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; // this is q0q1
        else if (state == 7 && s[charpos] == 's')//testing for s
            state = 5;//this is qsa

          //transitions from qs
        else if (state == 6 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; //off to q0q1
        else if (state == 6 && s[charpos] == 'h')//testing line h
            state = 5; //this is to qsa

          //transitions from qc
        else if (state == 3 && s[charpos] == 'h')//testing line h
            state = 5; //this is to qsa

          //transitions from qy
        else if (state == 4 && (s[charpos] == 'a' || s[charpos] == 'e' || s[charpos] == 'i' || s[charpos] == 'o' || s[charpos] == 'u' || s[charpos] == 'I' || s[charpos] == 'E')) //V
            state = 1; //this is for q0q1, changed this from q0qy
        else if (state == 4 && s[charpos] == 'y') //y
            state = 5; // to qsa
        else {
            return false;
        }
        charpos++;
    }//end of while

    // where did I end up? One of the final states
    if (state == 0 || state == 1 || state == 2)
    {
        return true;  // end in a final state
    }  // End of if (state == 0 || state == 1 || state == 2)
    else
        return false;
}
// PERIOD DFA
// Done by: **//Noah DeFrench
bool period(string s)
{
    // we have to check if the character is a period and return true if so
    if (s == ".")
        return true;
    else
        return false;
}

int scanner(tokentype& tt, string& w)
{
    tt = ERROR;
    cout << endl;
    cout << "....Scanner was called... " << endl; // JUST TPO SEE THAT SCANNER HAS BEEN CALLED SUCCESSFULLY
    fin >> w; // ** Grab the next word from the file via fin

    if (w == "eofm")  // 1. If it is eofm, return right now.
    {
        tt = EOFM; // tt AUTOMATICALLY = EOFM
        return 0;
    }

    // 3. If it was a word,
    //   check against the reservedwords list.
    //   If not reserved, tokentype is WORD1 or WORD2
    //   decided based on the last character

    else  if (word(w)) // 2. Call the token functions (word and period)
    {  // one after another (if-then-else).
         // CHECKING FOR WORD1 or WORD2
        
            if (w[w.length()-1] == 'I' || w[w.length() - 1] == 'E') // If there is a capital I or E in the string WORD2
            {
                tt = WORD2; // set tt to 
                //break; //break out
            }
            else
            {
                tt = WORD1; // else
                //break;
            }
        
        for (int i = 0; i < 19; i++) // ROWS
        {
            for (int j = 0; j < 1; j++) // COLUMNS
            {
                if (w == reservedWords[i][j]) //  If it isd a reserved word tt value will change
                {
                    switch (i) //switch on the row"i"
                    {
                    case 0: // Masu
                        tt = VERB; // set tt to new value
                        break;
                    case 1: // masen
                        tt = VERBNEG;// set tt to new value
                        break;
                    case 2: //mashita
                        tt = VERBPAST; // set tt to new value
                        break;
                    case 3: //masendeshita
                        tt = VERBPASTNEG; // set tt to new value
                        break;
                    case 4: //desu
                        tt = IS; // set tt to new value
                        break;
                    case 5: // deshita
                        tt = WAS; // set tt to new value
                        break;
                    case 6: // o
                        tt = OBJECT; // set tt to new value
                        break;
                    case 7: // wa
                        tt = SUBJECT; // set tt to new value
                        break;
                    case 8: // ni
                        tt = DESTINATION; // set tt to new value
                        break;
                    case 9: // watashi
                        tt = PRONOUN; // set tt to new value
                        break;
                    case 10: // anata
                        tt = PRONOUN; // set tt to new value
                        break;
                    case 11: // kare
                        tt = PRONOUN; // // set tt to new value
                        break;
                    case 12: // kanojo
                        tt = PRONOUN; // set tt to new value
                        break;
                    case 13: // sore
                        tt = PRONOUN; // set tt to new value
                        break;
                    case 14: // mata
                        tt = CONNECTOR; // set tt to new value
                        break;
                    case 15: // soshite
                        tt = CONNECTOR; // set tt to new value
                        break;
                    case 16: // shikashi
                        tt = CONNECTOR; // set tt to new value
                        break;
                    case 17: // dakara
                        tt = CONNECTOR; // set tt to new value
                        break;

                    } // end switch
                } // end if
            } // end for


        } //end for
    } //end if

    else if (period(w))
    { //   Let the tokentype be ERROR in that case.
        tt = PERIOD; // IF TRUE TOKEN TYPE WILL = PERIOD
    }
    else // IF IT WASNT A WORD OR A PERIOD, THEN WE HAVE A LEXICAL ERROR
    {
        //cout << ">>>>>>>>>Lexical Error: The string is not in my language" << endl;  //   Generate a lexical error message if both DFAs failed.
        tt = ERROR; // TOKENTYPE WILL EQUAL ERROR
    }

    // 4. Return the token type & string  (pass by reference)

  return 0;
}//the end of scanner


// ====================================================================================== //
// ===================== Parser.cpp contents ============================================ //

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: ** Outputs formated syntax error message when system encounters an unexpected token, then exits.
// Done by: Kyle Monroe

void syntaxerror1(tokentype correct, string found ) {   //syntax error method
    cout << "Syntax Error: expected " <<  tokenName[correct] << " but found " << found << endl; // output Syntax error message to sceen 
    exit(1); //exit 1 gives problems on empress must use #include <cstdlib> 
}
// Type of error: ** Outputs formated syntax error message when system encounters an unexpected word in a grammer rule, then exits.
// Done by: Kyle Monroe

void syntaxerror2(string found, string out) {    //2nd syntax error method
    cout << "Syntax Error: unexpected " << found << " found in " << out << endl; //output Syntax error message to sceen
    exit(1); //exit 1 gives problems on empress must use #include <cstdlib> 
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: ** if there is no unused token, Uses scanner to read next word in file and get it's token value.
//returns the token_type value of the next word in input file.
// Done by: ** noah
tokentype next_token() {
    if (!istoken)  
    {
        std::string oldlex= saved_lexeme;
        scanner(saved_token, saved_lexeme); 
        cout << "Scanning: " << saved_lexeme << endl;
        istoken = true;
    }
  //THIS IS NEW
   // getEword();
  //END NEW
    return saved_token;  
}

// Purpose: ** Checks the next token in the input file against the expected next token, per grammer rules. If the next token matches the expected token; the token will be used up. If the next token does not match the expected token, a syntax error will be printed and the progeam will exit.
//returns a bool: was next_token == expected token.
// Done by: ** noah
bool match(tokentype expected) {

    if (next_token() != expected){

        syntaxerror1(expected, saved_lexeme); 
        return false;
    }else  {

        cout << "Match for: " << tokenName[expected] << endl;
        istoken = false;   
        return true;                       
    }
}


// Grammar: ** <story> ::= <s> { <s> }
// Done by: ** noah
void story() {
    cout << "Doing <story> " << endl; // output message to screen for tracing
    //next_token();
    //loop through input file calling s(); until EOFM. 
    do {
        s();
        next_token(); //gets eofm if present
        fout<<endl;
    } while (saved_token != EOFM);

}
// Grammar: ** <s> ::=  [CONNECTOR] <noun> SUBJECT <afterSubject>
// Done by: ** noah
void s() {
    cout << "Doing: <s> " << endl; // output message to screen for tracing
    if (next_token() == CONNECTOR) { 
        getEword();
        gen(CONN);
        match(CONNECTOR); //Connector possible       
    }
    noun(); //First: noun/pronoun
    getEword();    
    match(SUBJECT); //second: subject
    gen(ACTR);
    afterSubject(); //Next segment of rule begins
}
// Grammar: ** <noun> ::= WORD1 | PRONOUN 
// Done by: ** noah
void noun() { // noun function

    cout << "Doing: <noun> " << endl; // output message to screen for tracing
    //tokentype temp = ;
    //getEword();
    switch (next_token()) { // start of case stament swirching on next_token()
    case WORD1: // WORD1     
        match(WORD1); // call match function on WORD1 parameter
        break; // Leave function
    case PRONOUN: // PRONOUN
        match(PRONOUN); // call match function on PRONOUN prameter
        break; // Leave function
    default: // default case
        syntaxerror2(saved_lexeme, "noun"); // call syntaxerror function with paramters saved_lexeme, string "noun"
    }// end switch
    
    
} // end noun function

// Grammer: <after subject> ::= <verb> <tense> PERIOD | <noun> <after noun>
//Done By: Nick
void afterSubject(){

    cout << "Doing <afterSubject> " << endl; // output message to screen for tracing
    switch (next_token()) { // start of case stament swirching on next_token()
        //HANDLE NOUNDS
    case WORD1: //WORD1
        match(WORD1); //match on WORD1
        getEword();
        afterNoun(); // call afterNoun() function
        break; // Leave function
    case PRONOUN: //PRONOUN
        match(PRONOUN); //match on pronoun
        getEword();
        afterNoun(); // call afterNoun() function
        break;  // Leave function
        //HANDLE VERBS
    case WORD2: { //WORD2 IS A VERB
        match(WORD2); //WORD2 = Verb
        getEword();
        gen(ACTN);
        tense(); // call tense() function
        match(PERIOD); //match on PERIOD
        break; // Leave function
    case VERB: // VERB
        match(VERB); // match on VERB
        getEword();
        gen(ACTN);
        tense(); //call tense() function

        match(PERIOD); // match on period
        break; // Leave function

    } // end of VERB 
    default: // If word is not a possible match, call syntaxerror.
        syntaxerror2(saved_lexeme, "after subject"); // 
    } // end switch
} 

// Utility function, handles : <tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by Kyle Monroe
void tense() {
    // Output message for tracing 
    cout << "Doing: <tense>" << endl; // output message to screen for tracing
    switch (next_token()) {
    case VERBPAST:
        gen(TENS);
        match(VERBPAST); // check wether current token against VERBPAST
        break; // Leave function

    case VERBPASTNEG:
        gen(TENS);
        match(VERBPASTNEG); // check wether current token against VERBPAST
        break; // Leave function

    case VERB:
        gen(TENS);
        match(VERB);
        break; // Leave function

    case VERBNEG:
        gen(TENS);
        match(VERBNEG);
        break; // Leave function

    default: // If word is not a possible match, call syntaxerror.
        syntaxerror2(saved_lexeme, "tense");
    }
}
// Grammar: ** <after noun> ::= <be> PERIOD | DESTINATION < verb> <tense> PERIOD | OBJECT <after object>
// Done by: ** noah
void afterNoun() {
    cout << "Doing <afterNoun> " << endl; // output message to screen for tracing
    switch (next_token()) {
        //HANDLE BE
    case IS:
    case WAS:
        be();
        gen(DESC);
        gen(TENS);

        match(PERIOD);
        break; // Leave function
        //HANDLE DESTINATION
    //********************************
    case DESTINATION: {
        match(DESTINATION);
        gen(TO);
        switch (next_token()) {
          //HANDLE VERB HERE.
        case WORD2:
            match(WORD2);
            getEword();
            gen(ACTN);
            tense();

            match(PERIOD);
            break; // Leave function
        case VERB:
            match(VERB);
            getEword();
            gen(ACTN);
            tense();

            match(PERIOD);
            break; // Leave function
        default: // If word is not a possible match, call syntaxerror.
            syntaxerror2(saved_lexeme, "after noun");
        }
        break; // Leave function
      }
        //HANDLE OBJECT
    //*********************************

    case OBJECT:
        match(OBJECT);
        gen(OBJT);
        afterObject();
        break; // Leave function

    default:
        syntaxerror2(saved_lexeme, "after noun");
    }
}
// Grammar: ** after object ::= <verb> <tense> PERIOD | <noun> <after noun>
// Done by: ** noah
void afterObject() { // function afterSubject()
    cout << "Doing <afterObject> " << endl; // output message to screen for tracing
    switch (next_token()) { // switch on next token
        //HANDLE VERB HERE
    case VERB: // VERB case
        match(VERB); // match on verb
        getEword();
        gen(ACTN);
        tense(); // call tense function
        match(PERIOD); // match on PERIOD
        break; // Leave function
    case WORD2: // WORD2
        match(WORD2); // match on WORD2
        getEword();
        gen(ACTN);
        tense(); // call tense() function
        match(PERIOD); // match on period
        break; // Leave function
        //HANDLE NOUN HERE
    //*************************************************
    case WORD1: // WORD1
        match(WORD1); // match on WORD1
        getEword();
        
        match(DESTINATION); // match on DESTINATION
        gen(TO);
        match(WORD2); // match on WORD2
        getEword();
        gen(ACTN);
        tense(); // call tense() function
        match(PERIOD); // match on PERIOD
        break; // Leave function

    case PRONOUN:
        match(PRONOUN); // match on PRONOUN
        getEword();
      
        match(DESTINATION); // match on DESTINATION
        gen(TO);
        match(WORD2); // match on WORD2
        getEword();
        gen(ACTN);
        tense(); // call tense() function

        match(PERIOD); // match on PERIOD
        break; // Leave function
    default: // If word is not a possible match, call syntaxerror.
        syntaxerror2(saved_lexeme, "after object"); // call syntax function
    }
}
// Grammar: ** <be> ::=   IS | WAS
// Done by: ** noah
void be() { // start of function 
    cout << "Doing <be>" << endl; // output message to screen for tracing
    switch (next_token()) {
    case IS: 
        match(IS); // Check if current token is a "IS"
        break; // Leave function
    case WAS:
        match(WAS); // Check if current token is a "WAS"
        break; // Leave function
    default: // If word is not a possible match
        syntaxerror2(saved_lexeme, "be"); // call syntaxerror2.
    }
}

