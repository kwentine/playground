package scan

import "testing"

func TestScanString(t *testing.T) {
	tokens := ScanString("(a 1)")
	expected := []Token{
		{OPEN_PAREN, "(", 1, 1},
		{SYMBOL, "a", 1, 2},
		{NUMBER, "1", 1, 4},
		{CLOSE_PAREN, ")", 1, 5},
		{EOF, "", 1, 6},
	}
	for i := range expected {
		if tokens[i] != expected[i] {
			t.Error("Wanted: ", expected[i], "Got: ", tokens[i])
		}
	}
}


func TestNextToken(t *testing.T) {
	s := NewScanner("(")
	tok := s.NextToken()
	expected := Token{
		OPEN_PAREN,
		"(",
		1,
		1,
	}
	if s.tokens[0] != expected {
		t.Error("Wanted: ", expected, "Got: ", s.tokens[0])
	}
}
