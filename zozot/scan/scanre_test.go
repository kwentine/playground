package scan

import "testing"

func TestReScanner(t *testing.T) {
	s := "abc 123\ndef"
	tokens := reScanAll(s)
	expected := []Token{
		{SYMBOL, "abc", 0, 1},
		{NUMBER, "123", 4, 1},
		{SYMBOL, "def", 8, 2},
		{EOF, "", len(s), 2},
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
	kind, _ := reScanToken(0, s)
	if kind != KEYWORD {
		t.Errorf("Wanted KEYWORD, got type: %d", kind)
	}
	s = "defined"
	kind, _ = reScanToken(0, s)
	if kind != SYMBOL {
		t.Errorf("Wanted SYMBOL, got type: %d", kind)
	}
}
