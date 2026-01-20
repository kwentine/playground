package scan

type TokenKind int

const (
	KEYWORD int = iota
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
	pos int
	line int
}

type Scanner struct {
	chars []rune
	c rune
	pos int
	next int
	line int
}

func NewScanner(source string) *Scanner {
	chars := []rune(source)
	return &Scanner{
		chars: chars,
		c: -1
		pos: -1,
		next: 0
		line: 1,
	}
}

// Must be called when pos is the last character of the token
func (s *Scanner) makeToken(kind TokenKind, start int) {
	var literal string
	if start < len(s.chars) && start < s.next {
		literal = string(s.chars[start:s.next])
	} else {
		literal = ""
	}
	if kind == SYMBOL && isKeyword(literal) {
		kind = KEYWORD
	}
	return Token{
		kind: kind,
		literal: literal,
		pos: start,
		line: s.line,
	}
}

func (s *Scanner) current() rune {
	if s.pos >= len(s.chars) {
		return -1
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

// Current character is a letter
func (s *Scanner) scanSymbol() {
	start := s.pos
	for c := s.GetChar(); isDigit(c) || isAlpha(c); c = s.GetChar() {}
	// Step back
	s.prev()
	return s.makeToken(SYMBOL. start)
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

func (s *Scanner) GetChar() rune {
	if s.next == len(s.chars) {
		return -1
	}
	s.c = s.chars[s.next]
	s.pos, s.next = s.next, s.next + 1
	return s.c
}

func (s *Scanner) NextToken() Token {
	var c rune
	for c = s.GetChar(); isWhitespace(c); c := s.GetChar() {
		if c == '\n' {
			s.line += 1
		}
	}
	switch c {
	case '(':
		return s.makeToken(OPEN_PAREN, s.pos)
	case ')':
		return s.makeToken(CLOSE_PAREN, s.pos)
	default:
		if isDigit(c) {
			return s.scanNumber()
		} else if isAlpha(c) {
			s.scanSymbol()
		}
	}
}

func ScanString(source string) []Token {
	s := newScanner(source)
	return s.Scan()
}
