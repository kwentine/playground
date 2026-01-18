package main

const (
	KEYWORD rune = -(iota + 1)
	SYMBOL
	OPEN_PAREN
	CLOSE_PAREN
	NUMBER
	EOF
)

type Token struct {
	kind rune
	literal string
	pos int
	line int
}

type Scanner struct {
	chars []rune
	pos int
	line int
	tokens []Token
}

func newScanner(source string) *Scanner {
	chars := []rune(source)
	tokens := make([]Token, 0, len(chars) + 1)
	return &Scanner{
		chars: chars,
		pos: 0,
		line: 1,
		tokens: tokens,
	}
}

func (s *Scanner) addToken(kind rune, start int) {
	var literal string
	if start < len(s.chars) {
		literal = string(s.chars[start:s.pos])
	} else {
		literal = ""
	}
	if kind == SYMBOL && isKeyword(literal) {
		kind = KEYWORD
	}
	s.tokens = append(s.tokens, Token{
		kind: kind,
		literal: literal,
		pos: start,
		line: s.line,
	})
}

func (s *Scanner) current() rune {
	if s.pos >= len(s.chars) {
		return EOF
	}
	return s.chars[s.pos]
}

func (s *Scanner) step() {
	s.pos++
}

func (s *Scanner) skipSpace() {
	for c := s.current(); isWhitespace(c); c = s.current() {
		if c == '\n' {
			s.line++
		}
		s.step()
	}
}

func (s *Scanner) paren() {
	var k rune
	start := s.pos
	c := s.current()
	if c == '(' {
		k = OPEN_PAREN
	} else {
		k = CLOSE_PAREN
	}
	s.step()
	s.addToken(k, start)
}

func (s *Scanner) number() {
	start := s.pos
	for isDigit(s.current()) {
		s.step()
	}
	s.addToken(NUMBER, start)
}

func (s *Scanner) symbol() {
	start := s.pos
	for c := s.current(); isDigit(c) || isAlpha(c); c = s.current() {
		s.step()
	}
	s.addToken(SYMBOL, start)
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

func isKeyword(s string) bool {
	return s == "lambda" || s == "define"
}

func (s *Scanner) Scan() []Token {
	for c := s.current(); c != EOF; c = s.current() {
		switch c {
		case '(', ')': s.paren()
		case ' ', '\t', '\n': s.skipSpace()
		default:
			if isDigit(c) {
				s.number()
			} else if isAlpha(c) {
				s.symbol()
			}
		}
	}
	s.addToken(EOF, len(s.chars))
	return s.tokens
}

func ScanString(source string) []Token {
	s := newScanner(source)
	return s.Scan()
}
