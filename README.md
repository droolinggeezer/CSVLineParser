# CSVLineParser
A simple C++ line-at-a-time CSV parser

## Constructor:  
    CSVLineParser( int columns, char escape_char = '"', char delimiter_char = ',', char quote_char = '"') :   

columns => If zero, return the number of cells in the line. If != 0, throw exception if the line does no contain exactly this number of cells  
quote_char  => the character used to enclose cells that contain instances of this character (default is double quote)  
escape_char => the character that is inserted to introduce an instance of quote_char within a quoted cell (default is double quote)  
delimiter_char => the character that separates the cell values (default is comma)  

## Parser function:  
    std::vector<std::string> parseline(const std::string& line)  

    Returns a vector of strings containing the distinct cell contents  
  
    throws:  
        std::out_of_range("Quote in unquoted cell: "+line)  
        std::out_of_range("Unescaped quote in quoted cell: "+line)  
        std::out_of_range("Unterminated quoted cell in line: "+line)  
        if columns != 0   
            throw std::out_of_range("Missing or extra cells: "+line)  
