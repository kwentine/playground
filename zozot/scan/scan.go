package main

import (
	"fmt"
	"os"
)

type TokenKind int

const (
	SYMBOL TokenKind = iota
	NUMBER
	LAMBDA
	DEFINE
	OPEN_PAREN
	CLOSE_PAREN
	ERROR
	EOF
	// TODO: Remove
	WHITESPACE
	NEWLINE
	KEYWORD
)

var tokenNames = [...]string{
	LAMBDA:      "LAMBDA",
	DEFINE:      "DEFINE",
	SYMBOL:      "SYMBOL",
	OPEN_PAREN:  "OPEN_PAREN",
	CLOSE_PAREN: "CLOSE_PAREN",
	NUMBER:      "NUMBER",
	ERROR:       "ERROR",
	EOF:         "EOF",
}

type Token struct {
	kind    TokenKind
	literal string
	line    int
	col     int
}

type Scanner struct {
	chars []rune
	start int
	next  int
	line  int
	col   int
}

func NewScanner(source string) *Scanner {
	chars := []rune(source)
	return &Scanner{
		chars: append(chars, -1),
		next:  0,
		line:  1,
		col:   0,
	}
}

// Must be called when pos is the last character of the token
func (s *Scanner) emitToken(kind TokenKind) Token {
	var literal string
	if s.start < s.next && s.next < len(s.chars) {
		literal = string(s.chars[s.start:s.next])
	} else {
		literal = ""
	}
	if kind == SYMBOL {
		switch literal {
		case "lambda":
			kind = LAMBDA
		case "define":
			kind = DEFINE
		}
	}
	return Token{
		kind:    kind,
		literal: literal,
		col:     s.col,
		line:    s.line,
	}
}

func (s *Scanner) advance() rune {
	var c rune = s.chars[s.next]
	if c != -1 {
		s.next++
		s.col++
	}
	return c
}

func (s *Scanner) rewind() {
	s.next--
	s.col--
}

func isWhitespace(c rune) bool {
	switch c {
	case ' ', '\t', '\n', '\r':
		return true
	}
	return false
}

func isDigit(c rune) bool {
	return '0' <= c && c <= '9'
}

func isAlpha(c rune) bool {
	switch c {
	case '*', '/', '+', '_':
		return true
	}
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '*'
}

func isAlnum(c rune) bool {
	return isAlpha(c) || isDigit(c)
}

func isKeyword(s string) bool {
	return s == "lambda" || s == "define"
}

func (s *Scanner) NextToken() Token {
	s.start = s.next
	c := s.advance()
	for ; isWhitespace(c); c = s.advance() {
		if c == '\n' {
			s.line += 1
			s.col = 0
		}
		s.start = s.next
	}
	switch c {
	case -1:
		return s.emitToken(EOF)
	case '(':
		return s.emitToken(OPEN_PAREN)
	case ')':
		return s.emitToken(CLOSE_PAREN)
	default:
		if isDigit(c) {
			for isDigit(s.advance()) {
			}
			s.rewind()
			return s.emitToken(NUMBER)
		} else if isAlpha(c) {
			for isAlnum(s.advance()) {
			}
			s.rewind()
			return s.emitToken(SYMBOL)
		}
	}
	return s.emitToken(ERROR)
}

func ScanString(source string) []Token {
	s := NewScanner(source)
	var tokens = make([]Token, 0, len(s.chars))
	var tok Token
	for tok = s.NextToken(); tok.kind != EOF; tok = s.NextToken() {
		tokens = append(tokens, tok)
	}
	return append(tokens, tok)
}

func main() {
	args := os.Args[1:]
	if len(args) != 1 {
		fmt.Fprintf(os.Stderr, "Usage: %s FILENAME\n", os.Args[0])
		os.Exit(1)
	}
	filename := args[0]
	bytes, err := os.ReadFile(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %s\n", err.Error())
		os.Exit(1)
	}
	for _, tok := range ScanString(string(bytes)) {
		fmt.Printf("%d:%d:%s %s\n", tok.line, tok.col, tokenNames[tok.kind], tok.literal)
	}
	s := NewScanner(string(bytes))
	parse(s)
}
