#include "parser.hpp"

int nested_layer = 0;

const char *InvalidTokenException::what() const throw()
{
    std::string msg = "Syntax error: expected " + labels[expected] + ",but got " + labels[got] + "\n";
    return msg.c_str();
}
std::string InvalidTokenException::where()
{
    return std::to_string(pos.row) + "," + std::to_string(pos.col);
}

const char *NestExceededException::what() const throw()
{
    std::string msg = "Syntax error: Nest layer exceeded max limit (" + std::to_string(MAX_NEST_LAYER) + ")\n";
    return msg.c_str();
}
std::string NestExceededException::where()
{
    return std::to_string(pos.row) + "," + std::to_string(pos.col);
}

const token_t *next()
{
    return token_add();
};

void check(const token_t *token, int count, ...)
{
    SYMBOL expected;
    va_list argptr;
    va_start(argptr, count);

    for (int i = 1; i <= count; i++)
    {
        expected = static_cast<SYMBOL>(va_arg(argptr, int));
        if (token->type == expected)
            return;
    }
    va_end(argptr);
    throw InvalidTokenException(expected, token->type, err);
}

void parse()
{
    try
    {
        next();
        if (tail->type != eof)
        {
            parse_subprogram(tail);
            check(tail, 2, period, eof);
        }
    }
    catch (InvalidTokenException &e)
    {
        std::cerr << "./" << info->infile << ":" << e.where() << ": error: " << e.what() << std::endl;
        std::cout << "./" << info->infile << " is not a valid PL/0 program." << std::endl;
        exit(1);
    }
    catch (NestExceededException &e)
    {
        std::cerr << "./" << info->infile << ":" << e.where() << ": error: " << e.what() << std::endl;
        std::cout << "./" << info->infile << " is not a valid PL/0 program." << std::endl;
        exit(1);
    }
}

void parse_subprogram(const token_t *token)
{
    if (tail->type == constsym)
    {
        parse_const(tail);
        next();
    }

    if (tail->type == varsym)
    {
        parse_var(tail);
        next();
    }

    // avoid const appears after var
    if (tail->type == constsym)
    {
        parse_const(tail);
        next();
    }

    while (tail->type == procsym)
    {
        parse_proc(tail);
    }

    parse_statement(tail);
    // tail point at next head
}

void parse_const(const token_t *token)
{
    next();
    do
    {
        if (tail->type == comma)
            next();
        parse_initialization(tail);
    } while (next()->type != semicolon);
    // tail point at current end
}

void parse_initialization(const token_t *token)
{
    check(tail, 1, ident);    // a
    check(next(), 1, eql);    // =
    check(next(), 1, number); // 1
    // tail point at current end
}

void parse_var(const token_t *token)
{
    next();
    do
    {
        if (tail->type == comma)
            next();
        check(tail, 1, ident);
    } while (next()->type != semicolon);
    // tail point at current end
}

void parse_proc(const token_t *token)
{
    check(next(), 1, ident);
    check(next(), 1, semicolon);
    parse_subprogram(next());
    check(tail, 1, semicolon);
    next();
    // tail point at next head
}

void parse_statement(const token_t *token)
{
    // TODO
    switch (token->type)
    {
    case ident:
        parse_assignment(tail);
        break;
    case ifsym:
        parse_if(tail);
        break;
    case whilesym:
        parse_while(tail);
        break;
    case callsym:
        parse_call(tail);
        break;
    case readsym:
        parse_read(tail);
        break;
    case writesym:
        parse_write(tail);
        break;
    case beginsym:
        parse_composite(tail);
        break;
    case repeatsym:
        parse_repeat(tail);
        break;
    case semicolon:
        next();
    default:
        return;
    }
    // tail point at next head;
}

void parse_assignment(const token_t *token)
{
    check(next(), 1, becomes);
    parse_expression(next());
    // tail point at next head;
}

void parse_expression(const token_t *token)
{
    if (token->type == plus || token->type == minus)
        next();
    for (;;)
    {
        parse_term(tail);
        if (tail->type == plus || tail->type == minus)
        {
            next();
            continue;
        }
        else
            break;
    }
    // tail point at next head
}

void parse_term(const token_t *token)
{
    for (;;)
    {
        parse_factor(tail);
        next();
        if (tail->type == times || tail->type == slash)
        {
            next();
            continue;
        }
        else
            break;
    }
}

void parse_factor(const token_t *token)
{
    if (token->type == lparen)
    {
        parse_expression(next());
        check(tail, 1, rparen);
        return;
    }
    check(tail, 2, ident, number);
    // tail point at current end
}

void parse_if(const token_t *token)
{
    if (++nested_layer >= MAX_NEST_LAYER)
        throw NestExceededException(err);
    parse_condition(next());
    check(tail, 1, thensym);
    parse_statement(next());
    if (tail->type == elsesym)
        parse_statement(next());
    nested_layer--;
    // tail point at next head
}

void parse_condition(const token_t *token)
{
    if (token->type == oddsym)
    {
        parse_expression(next());
        return;
    }
    parse_expression(tail);
    check(tail, 6, eql, neq, lss, leq, gtr, geq);
    parse_expression(next());
    // tail point at next head
}

void parse_while(const token_t *token)
{
    if (++nested_layer >= MAX_NEST_LAYER)
        throw NestExceededException(err);
    parse_condition(next());
    check(tail, 1, dosym);
    parse_statement(next());
    nested_layer--;
    // tail point at next head
}

void parse_call(const token_t *token)
{
    check(next(), 1, ident);
    next();
    // tail point at next head
}

void parse_read(const token_t *token)
{
    check(next(), 1, lparen);
    do
    {
        check(next(), 1, ident);
    } while (next()->type == comma);
    check(tail, 1, rparen);
    next();
    // tail point at next head
}

void parse_write(const token_t *token)
{
    check(next(), 1, lparen);
    do
    {
        check(next(), 1, ident);
    } while (next()->type == comma);
    check(tail, 1, rparen);
    next();
    // tail point at next head
}

void parse_composite(const token_t *token)
{
    next();
    do
    {
        parse_statement(tail);
        if (tail->type == semicolon)
            next();
    } while (tail->type != endsym);
    next();
    // tail point at next head
}

void parse_repeat(const token_t *token)
{
    if (++nested_layer >= MAX_NEST_LAYER)
        throw NestExceededException(err);
    next();
    do
    {
        parse_statement(tail);
        if (tail->type == semicolon)
            next();
    } while (tail->type != untilsym);
    parse_condition(next());
    nested_layer--;
    // tail point at next head
}
