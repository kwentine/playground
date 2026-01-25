package parse

import (
	"fmt"
	"os"
	"strings"
	scan "zozot/scan"
)

// prog   ::= expr prog | ""
// expr   ::= list | integer | symbol
// list   ::= "(" (lambda | define | prog) ")"
// lambda ::= "lambda" "(" args ")" expr
// args   ::= symbol args | ""
// define ::= "define" symbol expr
var current scan.Token

var s *scan.Scanner

var depth int

var indentStep string = "  "

func indent() string {
	return strings.Repeat(indentStep, depth)
}

func prog() string {
	parts := make([]string, 0)
	for {
		switch current.Kind {
		case scan.OPEN_PAREN, scan.NUMBER, scan.SYMBOL:
			parts = append(parts, expr())
		default:
			return strings.Join(parts, "\n" + indent())
		}
	}
}

func expr() string {
	switch current.Kind {
	case scan.OPEN_PAREN:
		return list()
	case scan.NUMBER:
		return number()
	case scan.SYMBOL:
		return symbol()
	default:
		panic("ERROR")
	}
}

func list() string {
	match(scan.OPEN_PAREN)
	var inner string
	depth += 1
	switch current.Kind {
	case scan.LAMBDA:
		inner = lambda()
	case scan.DEFINE:
		inner = define()
	default:
		inner = prog()
	}
	match(scan.CLOSE_PAREN)
	depth -= 1
	return "(" + inner + ")"
}

func lambda() string {
	match(scan.LAMBDA)
	match(scan.OPEN_PAREN)
	args := args()
	match(scan.CLOSE_PAREN)
	body := expr()
	return "lambda (" + args + ") \n" + indent() + body
}

func args() string {
	parts := make([]string, 0)
	for current.Kind == scan.SYMBOL {
		parts = append(parts, symbol())
	}
	return strings.Join(parts, " ")
}

func define() string {
	match(scan.DEFINE)
	return "define " + symbol() + " " + expr()
}

func number() string {
	val := current.Literal
	match(scan.NUMBER)
	return "\x1b[34m" + val + "\x1b[0m"
}

func symbol() string {
	val := current.Literal
	match(scan.SYMBOL)
	return "\x1b[31m" + val + "\x1b[0m"
}

func match(kind scan.TokenKind) {
	if current.Kind == kind {
		current = s.NextToken()
	} else {
		fmt.Fprintf(os.Stderr, "ERROR:%d:%d:%s expected %s\n", current.Line, current.Col, scan.TokenNames[current.Kind], scan.TokenNames[kind])
		os.Exit(1)
	}
}

func Parse(scanner *scan.Scanner) {
	s = scanner
	current = s.NextToken()
	res := prog()
	match(scan.EOF)
	fmt.Println(res)
}
