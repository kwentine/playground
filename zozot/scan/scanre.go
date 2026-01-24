package main

import (
	"regexp"
	"strings"
)

type Rule struct {
	re   *regexp.Regexp
	kind TokenKind
}

var rules = [...]Rule{
	{regexp.MustCompile(`^(lambda|define)\b`), KEYWORD},
	{regexp.MustCompile(`^[a-zA-Z]+`), SYMBOL},
	{regexp.MustCompile(`^[0-9]+`), NUMBER},
	{regexp.MustCompile(`^[ \t\n]+`), WHITESPACE},
}

func reScanToken(source string) (kind TokenKind, width int) {
	width = 0
	var loc []int
	for _, rule := range rules {
		loc = rule.re.FindStringIndex(source)
		if loc != nil {
			if loc[1] > width {
				width = loc[1]
				kind = rule.kind
			}
		}
	}
	if width == 0 {
		kind, width = ERROR, 1
	}
	return kind, width
}

func reScanAll(source string) []Token {
	var tokens []Token
	var kind TokenKind
	var line int = 1
	for start, width := 0, 0; start < len(source); start += width {
		kind, width = reScanToken(source[start:])
		if kind == WHITESPACE {
			line += strings.Count(source[start:start+width], "\n")
		} else {
			tokens = append(
				tokens,
				Token{
					kind:    kind,
					literal: source[start : start+width],
					line:    line,
				},
			)
		}
	}
	tokens = append(
		tokens,
		Token{
			kind:    EOF,
			literal: "",
			line:    line,
		},
	)
	return tokens
}
