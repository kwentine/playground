package main

import (
	"os"
	"fmt"
)

type TokenKind int

const (
	KEYWORD TokenKind = iota
	SYMBOL
	OPEN_PAREN
	CLOSE_PAREN
	NUMBER
	ERROR
	NEWLINE
	WHITESPACE
	EOF
)

var tokenNames = [...]string{
	KEYWORD: "KEYWORD",
	SYMBOL: "SYMBOL",
	OPEN_PAREN: "OPEN_PAREN",
	CLOSE_PAREN: "CLOSE_PAREN",
	NUMBER: "NUMBER",
	ERROR: "ERROR",
	NEWLINE: "NEWLINE",
	WHITESPACE: "WHITESPACE",
	EOF: "EOF",

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
	if kind == SYMBOL && isKeyword(literal) {
		kind = KEYWORD
	}
	return Token{
		kind:    kind,
		literal: literal,
		col:     s.col,
		line:    s.line,
	}
}

func (s *Scanner) peek() rune {
	return s.chars[s.next]
}

func (s *Scanner) getchar() rune {
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
	return c == ' ' || c == '\t'
}

func isDigit(c rune) bool {
	return '0' <= c && c <= '9'
}

func isAlpha(c rune) bool {
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z'
}

func isAlnum(c rune) bool {
	return isAlpha(c) || isDigit(c)
}

func isKeyword(s string) bool {
	return s == "lambda" || s == "define"
}

func (s *Scanner) NextToken() Token {
	s.start = s.next
	c := s.getchar()
	for ; isWhitespace(c); c = s.getchar() {
		if c == '\n' {
			s.line += 1
			s.col = 0
		}
		s.start++
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
			for isDigit(s.getchar()) {
			}
			s.rewind()
			return s.emitToken(NUMBER)
		} else if isAlpha(c) {
			for isAlnum(s.getchar()) {
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
		fmt.Printf("Usage: %s FILENAME\n", os.Args[0])
		os.Exit(1)
	}
	filename := args[0]
	bytes, err := os.ReadFile(filename)
	if err != nil {
		fmt.Printf("ERROR: %s\n", err.Error())
		os.Exit(1)
	}
	for _, tok := range ScanString(string(bytes)) {
		fmt.Printf("%d:%d:%s %s\n", tok.line, tok.col, tokenNames[tok.kind], tok.literal)
	}

}
