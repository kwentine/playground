package main

import (
	"os"
	"fmt"
	scan "zozot/scan"
	parse "zozot/parse"
)

func main() {
	args := os.Args[1:]
	if len(args) != 1 {
		fmt.Fprintf(os.Stderr, "Usage: %s FILENAME\n", os.Args[0])
		os.Exit(1)
	}
	filename := args[0]
	bytes, err := os.ReadFile(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %s\n", err.Error())
		os.Exit(1)
	}
	// TODO: --scan
	for _, tok := range scan.ScanString(string(bytes)) {
		fmt.Printf("%d:%d:%s %s\n", tok.Line, tok.Col, scan.TokenNames[tok.Kind], tok.Literal)
	}
	// TODO: --parse
	s := scan.NewScanner(string(bytes))
	parse.Parse(s)
}
