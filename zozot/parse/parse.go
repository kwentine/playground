package parse

import (
	"fmt"
	"os"
	"strconv"
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

type Lambda struct {
	Params []Symbol
	Body any
}

type Define struct {
	Sym Symbol
	Val any
}

type List struct {
	Elems []any
}

type Number int

type Symbol string

func prog() []any {
 	elems := make([]any, 0)
	for {
		switch current.Kind {
		case scan.OPEN_PAREN, scan.NUMBER, scan.SYMBOL:
			elems = append(elems, expr())
		default:
			return elems
		}
	}
}

func expr() any {
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

func list() any {
	match(scan.OPEN_PAREN)
	var res any
	switch current.Kind {
	case scan.LAMBDA:
		res = lambda()
	case scan.DEFINE:
		res = define()
	default:
		res = List{
			Elems: prog(),
		}
	}
	match(scan.CLOSE_PAREN)
	return res
}

func lambda() Lambda {
	match(scan.LAMBDA)
	match(scan.OPEN_PAREN)
	args := args()
	match(scan.CLOSE_PAREN)
	body := expr()
	return Lambda{
		Params: args,
		Body: body,
	}
}

func args() []Symbol {
	parts := make([]Symbol, 0)
	for current.Kind == scan.SYMBOL {
		parts = append(parts, symbol())
	}
	return parts
}

func define() Define {
	match(scan.DEFINE)
	return Define{
		Sym: symbol(),
		Val: expr(),
	}
}

func number() Number {
	val := current.Literal
	match(scan.NUMBER)
	i, err := strconv.Atoi(val)
	if err != nil {
		fmt.Fprintf(os.Stderr, "ERROR:Invalid number literal %s", val)
		os.Exit(1)
	}
	return Number(i)
}

func symbol() Symbol {
	val := current.Literal
	match(scan.SYMBOL)
	return Symbol(val)
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
