package scan

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

type Token struct {
	kind TokenKind
	literal string
	line int
	col int
}

type Scanner struct {
	chars []rune
	start int
	next int
	line int
	col int
}

func NewScanner(source string) *Scanner {
	chars := []rune(source)
	return &Scanner{
		chars: append(chars, -1),
		next: 0,
		line: 1,
		col: 0,
	}
}

// Must be called when pos is the last character of the token
func (s *Scanner) makeToken(kind TokenKind) Token {
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
		kind: kind,
		literal: literal,
		col: s.col,
		line: s.line,
	}
}

func (s *Scanner) peek() rune {
	return s.chars[s.next]
}

func (s *Scanner) getchar() rune {
	var c rune
	if c = s.chars[s.next]; c != -1 {
		s.next++
		s.col++
	}
	return c
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

func (s *Scanner) NextToken() Token {
	var c rune
	s.start = s.next
	for c = s.getchar(); isWhitespace(c); c = s.getchar() {
		if c == '\n' {
			s.line += 1
			s.col = 0
		}
		s.start++
	}
	switch c {
	case -1:
		return s.makeToken(EOF)
	case '(':
		return s.makeToken(OPEN_PAREN)
	case ')':
		return s.makeToken(CLOSE_PAREN)
	default:
		if isDigit(c) {
			for ; isDigit(s.peek()); s.getchar() {}
			return s.makeToken(NUMBER)
		} else if isAlpha(c) {
			for ; isDigit(s.peek()) || isAlpha(s.peek()); s.getchar() {}
			return s.makeToken(SYMBOL)
		}
	}
	return s.makeToken(ERROR)
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
