package main

import (
	"fmt"
	"os"
	"strings"
)

// prog   ::= expr prog | ""
// expr   ::= list | integer | symbol
// list   ::= "(" (lambda | define | prog) ")"
// lambda ::= "lambda" "(" args ")" expr
// args   ::= symbol args | ""
// define ::= "define" symbol expr
var current Token

var s *Scanner

func prog() string {
	parts := make([]string, 0)
	for {
		switch current.kind {
		case OPEN_PAREN, NUMBER, SYMBOL:
			parts = append(parts, expr())
		default:
			return strings.Join(parts, " ")
		}
	}
}

func expr() string {
	switch current.kind {
	case OPEN_PAREN:
		return list()
	case NUMBER:
		return number()
	case SYMBOL:
		return symbol()
	default:
		panic("ERROR")
	}
}

func list() string {
	match(OPEN_PAREN)
	var inner string
	switch current.kind {
	case LAMBDA:
		inner = lambda()
	case DEFINE:
		inner = define()
	default:
		inner = prog()
	}
	match(CLOSE_PAREN)
	return "(" + inner + ")"
}

func lambda() string {
	match(LAMBDA)
	match(OPEN_PAREN)
	args := args()
	match(CLOSE_PAREN)
	body := expr()
	return "lambda (" + args + ") " + body
}

func args() string {
	parts := make([]string, 0)
	for current.kind == SYMBOL {
		parts = append(parts, symbol())
	}
	return strings.Join(parts, " ")
}

func define() string {
	match(DEFINE)
	return "define " + symbol() + " " + expr()
}

func number() string {
	val := current.literal
	match(NUMBER)
	return "\x1b[34m" + val + "\x1b[0m"
}

func symbol() string {
	val := current.literal
	match(SYMBOL)
	return "\x1b[31m" + val + "\x1b[0m"
}

func match(kind TokenKind) {
	if current.kind == kind {
		current = s.NextToken()
	} else {
		fmt.Fprintf(os.Stderr, "ERROR:%d:%d:%s expected %s\n", s.line, s.col, tokenNames[current.kind], tokenNames[kind])
		os.Exit(1)
	}
}

func parse(scanner *Scanner) {
	s = scanner
	current = s.NextToken()
	res := prog()
	match(EOF)
	fmt.Println(res)
}
