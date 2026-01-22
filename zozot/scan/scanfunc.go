package scan


func scanAll(code string) []Token {
	var chars = append([]rune(code), -1)
	var tokens = make([]Token, 0, len(chars))
	var kind TokenKind
	for start, width := 0, 0; start < len(chars); start += width {
		start += skipWhitespace(chars[start:])
		kind, width = scanToken(chars[start:])
		tokens = append(
			tokens,
			Token{
				kind: kind,
				literal: string(chars[start:start + width]),
			},
		)
	}
	return tokens
}

func skipWhitespace(buff []rune) int {
	return 0
}

func number(buff []rune) (TokenKind, int) {
	return 0, 0
}

func symbol(buff []rune) (TokenKind, int) {
	return 0, 0
}

func scanToken(chars []rune) (TokenKind, int) {
	switch c := chars[0]; c {
	case '(': return OPEN_PAREN, 1
	case ')': return CLOSE_PAREN, 1
	case '\n': return NEWLINE, 1
	case -1: return EOF, 1
	default:
		if isDigit(c) {
			return number(chars)
		} else if isAlpha(c) {
			return symbol(chars)
		} else {
			return ERROR, 1
		}
	}
}
