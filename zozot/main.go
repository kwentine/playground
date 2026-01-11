package main

import (
	"log"
	"reflect"
)

type Env struct {
	parent *Env
	frame map[string]any
}

func (e Env) get(key string) (any, bool) {
	val, ok :=  e.frame[key]
	if !ok && e.parent != nil {
		return e.parent.get(key)
	}
	return val, ok
}

func (e Env) bind(params []string, values []any) Env {
	if len(params) != len(values) {
		log.Fatalf("ERORR: wrong number of arguments: %s %s", params, values)
	}
	for i, key := range params {
		e.frame[key] = values[i]
	}
	return e
}

type ListExpr []any

type LambdaExpr struct  {
	params  []string
	body any
}

type Lambda struct {
	env *Env
	params []string
	body any
}

func evLambda(expr LambdaExpr, parent *Env) Lambda {
	return Lambda{
		&Env{
			parent,
			map[string]any{},
		},
		expr.params,
		expr.body,
	}
}

func evSym(s string, env Env) any {
	v, ok := env.get(s)
	if !ok {
		log.Fatal("ERROR: Unbound variable:", s)
	}
	return v
}

func evList(expr []any, env Env) any {
	if len(expr) == 0 {
		return []any{}
	}
	op := eval(expr[0], env)
	args := make([]any, len(expr) - 1)
	for i, e := range expr[1:] {
		args[i] = eval(e, env)
	}
	return apply(op, args)
}

func apply(op any, args []any) any {
	switch f := op.(type) {
	case Lambda:
		return eval(f.body, f.env.bind(f.params, args))
	case func (int, int) int:
		return f(args[0].(int), args[1].(int))
	default:
		log.Fatal("ERROR: Unknown operator: ", reflect.TypeOf(op))
		return nil
	}
}

func eval(expr any, env Env) any {
	switch e := expr.(type) {
	case int:
		return e
	case string:
		return evSym(e, env)
	case LambdaExpr:
		return evLambda(e, &env)
	case ListExpr:
		return evList(e, env)
	default:
		log.Fatal("ERROR: Unknown expression type:", expr)
		return nil
	}
}

func main() {
	var expr = ListExpr{
		LambdaExpr{[]string{"x", "y"}, ListExpr{"*", "x", "y"}},
		2,
		3,
	}
	var globals = Env{
		nil,
		map[string]any{
			"+": func(x int, y int) int { return x + y },
			"*": func(x int, y int) int { return x * y },
		},
	}
	res := eval(expr, globals)
	log.Println("Result:", res)
}
