#include "symbols.hpp"

token_t tokens[PREALLOC_TOKEN_NUM];
token_t *tail;
pos_t cur, err;
std::string id;
int token_num;

const char *InvalidSymbolException::what() const throw()
{
    std::string msg = "Invalid symbol: " + s + "\n";
    return msg.c_str();
}
std::string InvalidSymbolException::where()
{
    return std::to_string(pos.row) + "," + std::to_string(pos.col);
}

int _get()
{
    int ch = info->fin.get();
    cur.col++;
    if (ch == '\n')
    {
        cur.row++;
        cur.col = 0;
    }
    return ch;
}

int _unget(int ch)
{
    info->fin.putback(ch);
    cur.col--;
    if (ch == '\n')
        cur.row--;
    return 0;
}

void init()
{
    tail = tokens;
    cur.row = 1;
    cur.col = 0;
    token_num = 0;
    return;
}

void set_errcur()
{
    err = cur;
}

SYMBOL getSymbol()
{
    int ch;
    while ((ch = _get()) != EOF && ch <= ' ')
        ;
    set_errcur();
    switch (ch)
    {
    case EOF:
        id = "EOF";
        return eof;
    case '+':
        id = "+";
        return plus;
    case '-':
        id = "-";
        return minus;
    case '*':
        id = "*";
        return times;
    case '/':
        id = "/";
        return slash;
    case '(':
        id = "(";
        return lparen;
    case ')':
        id = ")";
        return rparen;
    case ',':
        id = ",";
        return comma;
    case ';':
        id = ";";
        return semicolon;
    case '.':
        id = ".";
        return period;
    case ':':
        ch = _get();
        if (ch == '=')
        {
            id = ":=";
            return becomes;
        }
        id = ":" + std::to_string(ch);
        throw InvalidSymbolException(id, err);
    case '=':
        id = "=";
        return eql;
    case '#':
        id = "#";
        return neq;
    case '<':
        ch = _get();
        if (ch == '=')
        {
            id = "<=";
            return leq;
        }
        _unget(ch);
        id = "<";
        return lss;
    case '>':
        ch = _get();
        if (ch == '=')
        {
            id = ">=";
            return geq;
        }
        _unget(ch);
        id = ">";
        return gtr;
    default:
        id = ch;
        bool first_digit = isdigit(ch), digit = first_digit, alpha = isalpha(ch);
        while ((ch = _get()) != EOF && isalnum(ch))
        {
            id.append(1, ch);
            if (isdigit(ch))
                digit = true;
            else if (isalpha(ch))
                alpha = true;
        }
        _unget(ch);
        // [0-9]+
        if (digit && !alpha)
        {
            if (id.length() > MAX_NUMBER_LENGTH)
                throw InvalidSymbolException(id + " (MAX_NUMBER_LENGTH_EXCEEDED)", err);
            int num = atoi(id.c_str());
            return number;
        }
        // [a-zA-Z][a-zA-Z0-9]+
        else if (!first_digit && alpha)
        {
            if (id.length() > MAX_IDENTIFIER_LENGTH)
                throw InvalidSymbolException(id + " (MAX_IDENTIFIER_LENGTH_EXCEEDED)", err);
            return getSymbolFromId(id);
        }
        // [0-9][a-zA-Z0-9]+
        else
            throw InvalidSymbolException(id, err);
    }
}

SYMBOL getSymbolFromId(std::string id)
{
    for (int i = 0; i < KEYWORD_NUM; i++)
        if (id == keywords[i])
            return static_cast<SYMBOL>(SYM_OFFSET + i);
    return ident;
}

token_t *token_create(int num, std::string ident, SYMBOL type)
{
    token_t *t = num < PREALLOC_TOKEN_NUM ? tokens + num - 1 : new token_t;
    t->seq = token_num;
    t->text = ident;
    t->type = type;
    return t;
}

const token_t *token_add()
{
    try
    {
        SYMBOL sym = getSymbol();
        token_num++;
        token_t *t = token_create(token_num, id, sym);
        tail->next = t;
        tail = t;
        return tail;
    }
    catch (InvalidSymbolException &e)
    {
        std::cerr << "./" << info->infile << ":" << e.where() << ": error: " << e.what() << std::endl;
        exit(1);
    }
}

void result_print()
{
    bool out_to_file = info->outfile.length();
    char buf[300];
    std::string buff;

    if (out_to_file)
        std::cout << "Lexeme table has been written to \"" << info->outfile << "\".\n";
    else
        std::cout << "Lexeme Table:\n";

    sprintf(buf, "+-----+--------------------+-------------+\n"
                 "|%4s |%19s |%12s |\n"
                 "+-----+--------------------+-------------+\n",
            "Seq", "Lexeme", "Token Type");
    buff = buf;

    if (out_to_file)
        info->fout << buff;
    else
        std::cout << buff;

    for (token_t *t = tokens; t != tail; t = t->next)
    {
        sprintf(buf, "|%4d |%19s |%12s |\n", t->seq, t->text.c_str(),
                labels[t->type].c_str());
        buff = buf;
        if (out_to_file)
            info->fout << buff;
        else
            std::cout << buff;
    }

    buff = "+-----+--------------------+-------------+\n\n";
    if (out_to_file)
        info->fout << buff;
    else
        std::cout << buff;

    info->fout.close();
}
