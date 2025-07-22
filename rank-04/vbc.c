#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

static char	*g_expr;
static int	g_pos;

static void error(const char *msg)
{
	if (msg)
		printf("%s\n", msg);
	exit(1);
}

static void	unexpected_token(char c)
{
	printf("Unexpected token '%c'\n", c);
	exit(1);
}

static void	skip_spaces(void)
{
	while (g_expr[g_pos] == ' ')
		g_pos++;
}

static int	parse_expression(void);

static int	parse_factor(void)
{
	int	result;

	skip_spaces();
	if (!g_expr[g_pos])
		error("Unexpected end of input");
	if (isdigit(g_expr[g_pos]))
	{
		result = g_expr[g_pos] - '0';
		g_pos++;
		return result;
	}
	else if (g_expr[g_pos] == '(')
	{
		g_pos++;
		result = parse_expression();
		skip_spaces();
		if (g_expr[g_pos] != ')')
			error("Unexpected end of input");
		g_pos++;
		return result;
	}
	else
		unexpected_token(g_expr[g_pos]);
	return 0;
}

static int	parse_term(void)
{
	int result = parse_factor();

	while (1)
	{
		skip_spaces();
		if (g_expr[g_pos] == '*')
		{
			g_pos++;
			result *= parse_factor();
		}
		else
			break;
	}
	return result;
}

static int	parse_expression(void)
{
	int result = parse_term();

	while (1)
	{
		skip_spaces();
		if (g_expr[g_pos] == '+')
		{
			g_pos++;
			result += parse_term();
		}
		else
			break;
	}
	return result;
}

int main(int argc, char **argv)
{
	int	result;

	if (argc != 2)
		exit(1);
	g_expr = argv[1];
	g_pos = 0;
	result = parse_expression();
	skip_spaces();
	if (g_expr[g_pos] == ')')
		unexpected_token(')');
	if (g_expr[g_pos])
		unexpected_token(g_expr[g_pos]);
	printf("%d\n", result);
	return 0;
}
