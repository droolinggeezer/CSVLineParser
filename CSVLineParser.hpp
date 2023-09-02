#include <vector>
#include <string>

using namespace std;


// CSV Parser turns line of csv into a vector of parsed column values
class CSVLineParser {
    private:
    char delimiter_char;
    char quote_char;
    char escape_char;
    int columns;
    int colcount;

     enum parser_state {
        start_field,
        unquoted,
        quoted
    };

public:
    CSVLineParser( int columns, char escape_char = '\\', char delimiter_char = ',', char quote_char = '"') :  
               columns(columns), escape_char(escape_char), delimiter_char(delimiter_char), quote_char(quote_char) {}


    std::vector<std::string> parseline(const std::string& line) noexcept(false){
        std::vector<std::string> fields;
        std::string field = "";
        colcount = 0;
        parser_state state = start_field;
        for (int i =0; i < line.length(); i++) {
            switch(state) {
                case start_field:
                    if(line.at(i) == delimiter_char) {
                        fields.push_back(field);
                        colcount++;
                        field = "";
                    }
                    else if (line.at(i) == quote_char) {
                        state = quoted;
                    } else {
                        field.push_back(line.at(i));
                        state = unquoted;
                    }
                    break;

                case unquoted:
                    if(line.at(i) == delimiter_char) {
                        fields.push_back(field);
                        colcount++;
                        field = "";
                        state = start_field;
                    }
                    else if(line.at(i) == quote_char) {
                        throw std::out_of_range("Quote in unquoted field: "+line);
                    }
                    else {
                           field.push_back(line.at(i));
                     }
                    break;

                case quoted:
                    // check for escaped quote
                    if((line.at(i) == escape_char) &&
                       (i+1 < line.length()) && 
                       (line.at(i+1) == quote_char)) {
                            i++;
                            // add de-escaped quote
                            field.push_back(line.at(i));
                    }
                    else if(line.at(i) == quote_char) {
                        // see if closing quote or out of context
                        if((i+1 < line.length()) &&           // if not eol 
                           (line.at(i+1) != delimiter_char)){ // and no delimiter follows
                            throw std::out_of_range("Unescaped quote in quoted field: "+line);
                        } else {
                            // closing quote of quoted field
                            state = start_field;
                        }
                        break;
                    }
                    else {
                        field.push_back(line.at(i));
                    }
                    break;

            }
        }
        if( state == quoted) { // eol before closing quote found
                throw std::out_of_range("Unterminated quoted field in line: "+line);
        }
        // add last field to list
        fields.push_back(field);
        colcount++;       
      // Pad with empty columns to expected count
        if( colcount != columns) {
            throw std::out_of_range("Missing or extra fields: "+line);       
        }
        return fields;
    }
};
