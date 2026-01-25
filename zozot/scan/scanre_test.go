package scan

import "testing"

func TestReScanner(t *testing.T) {
	s := "abc 123\ndef"
	tokens := reScanAll(s)
	expected := []Token{
		{kind: SYMBOL, literal: "abc", line: 1},
		{kind: NUMBER, literal: "123", line: 1},
		{kind: SYMBOL, literal: "def", line: 2},
		{kind: EOF, literal: "", line: 2},
	}
	if len(tokens) != len(expected) {
		t.Error("Length mismatch")
	}
	for i := range expected {
		if i < len(tokens) && tokens[i] != expected[i] {
			t.Error("Wanted: ", expected[i], "Got: ", tokens[i])
		}
	}
}

func TestLongestMatch(t *testing.T) {
	s := "define"
	kind, _ := reScanToken(s)
	if kind != KEYWORD {
		t.Errorf("Wanted KEYWORD, got type: %d", kind)
	}
	s = "defined"
	kind, _ = reScanToken(s)
	if kind != SYMBOL {
		t.Errorf("Wanted SYMBOL, got type: %d", kind)
	}
}
