#ifndef PARSER_HPP
#define PARSER_HPP

#include "symbols.hpp"

#include <cstdarg>

/* program only permit nested program that layer < MAX_NEST_LAYER */
const int MAX_NEST_LAYER = 3;

/* the exception throw when parser get a unexpected token */
class InvalidTokenException : public std::exception
{
    std::string msg;
    SYMBOL expected;
    SYMBOL got;
    pos_t pos;

public:
    InvalidTokenException(SYMBOL expected, SYMBOL got, pos_t pos) : expected(expected), got(got), pos(pos)
    {
        msg = "Syntax error: expected " + labels[expected] + ",but got " + labels[got] + "\n";
    }
    ~InvalidTokenException() throw(){};
    const char *what() const throw();
    std::string where();
};
class NestExceededException : public std::exception
{
    std::string msg;
    pos_t pos;

public:
    NestExceededException(pos_t pos) : pos(pos)
    {
        msg = "Syntax error: Nest layer exceeded max limit (" + std::to_string(MAX_NEST_LAYER) + ")\n";
    }
    ~NestExceededException() throw(){};
    const char *what() const throw();
    std::string where();
};

extern token_t *tail;
extern pos_t err;
extern const std::string labels[];

/* get next token */
const token_t *next();

/* check token valid or not, when invalid, throw InvalidTokenException*/
void check(const token_t *, int, ...);

/* obvious function names */
void parse();
void parse_subprogram(const token_t *token);
void parse_const(const token_t *token);
void parse_initialization(const token_t *token);
void parse_var(const token_t *token);
void parse_proc(const token_t *token);
void parse_statement(const token_t *token);
void parse_assignment(const token_t *token);
void parse_expression(const token_t *token);
void parse_term(const token_t *token);
void parse_factor(const token_t *token);
void parse_if(const token_t *token);
void parse_condition(const token_t *token);
void parse_while(const token_t *token);
void parse_call(const token_t *token);
void parse_read(const token_t *token);
void parse_write(const token_t *token);
void parse_composite(const token_t *token);
void parse_repeat(const token_t *token);

#endif