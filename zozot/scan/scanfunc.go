package scan


func scanAll(code string) []Token {
	var chars = []rune(code)
	var charCount = len(chars)
	var tokens = make([]Token, 0, charCount)
	var kind rune
	for start, next := 0, 0; start < charCount; start = next {
		start = skipWhitespace(start, chars, charCount)
		kind, next = scanToken(start, chars, charCount)
		tokens = append(
			tokens,
			Token{
				kind: kind,
				pos: start,
				literal: string(chars[start:next]),
			},
		)
	}
	return tokens
}

func charAt(pos int, buff []rune, len int) rune {
	if pos >= len {
		return EOF
	}
	return buff[pos]
}

func skipWhitespace(pos int, buff []rune, len int) int {
	return 0
}

func number(pos int, buff []rune, len int) (rune, int) {
	return 0, 0
}

func symbol(pos int, buff []rune, len int) (rune, int) {
	return 0, 0
}

func scanToken(start int, buff []rune, len int) (rune, int) {
	c := charAt(start, buff, len)
	switch c {
	case '(': return OPEN_PAREN, start + 1
	case ')': return CLOSE_PAREN, start + 1
	case '\n': return NEWLINE, start + 1
	default:
		if isDigit(c) {
			return number(start, buff, len)
		} else if isAlpha(c) {
			return symbol(start, buff, len)
		} else {
			return ERROR, start + 1
		}
	}
}
