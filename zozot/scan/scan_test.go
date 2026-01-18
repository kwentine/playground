package scan

import "testing"

func TestScanner(t *testing.T) {
	s := newScanner("(a 1)")
	tokens := s.Scan()
	expected := []Token{
		{OPEN_PAREN, "(", 0, 1},
		{SYMBOL, "a", 1, 1},
		{NUMBER, "1", 3, 1},
		{CLOSE_PAREN, ")", 4, 1},
		{EOF, "", 5, 1},
	}
	for i := range expected {
		if tokens[i] != expected[i] {
			t.Error("Wanted: ", expected[i], "Got: ", tokens[i])
		}
	}
}


func TestAddToken(t *testing.T) {
	s := &Scanner{
		chars: []rune{'('},
		pos: 1,
		line: 1,
		tokens: make([]Token, 0, 1),
	}
	s.addToken(OPEN_PAREN, 0)
	expected := Token{
		OPEN_PAREN,
		"(",
		0,
		1,
	}
	if s.tokens[0] != expected {
		t.Error("Wanted: ", expected, "Got: ", s.tokens[0])
	}
}
