package eval

import (
	"log"
	"reflect"
	"zozot/parse"
)

type Closure struct {
	Lambda parse.Lambda
	Env *Env
}

type Env struct {
	parent *Env
	frame map[string]any
}

func (e *Env) get(key string) (any, bool) {
	for curr := e; curr != nil; curr = curr.parent {
		if val, ok :=  e.frame[key]; ok {
			return val, true
		}
	}
	return nil, false
}

func (e *Env) bind(params []parse.Symbol, values []any) *Env {
	if len(params) != len(values) {
		log.Fatalf("ERORR: wrong number of arguments: expected %d got %d", len(params), len(values))
	}
	var child = &Env{
		parent: e,
		frame: map[string]any{},
	}
	for i, key := range params {
		child.frame[string(key)] = values[i]
	}
	return child
}

func evLambda(lambda parse.Lambda, env *Env) Closure {
	return Closure{
		Lambda: lambda,
		Env: env,
	}
}

func evSym(s parse.Symbol, env *Env) any {
	v, ok := env.get(string(s))
	if !ok {
		log.Fatal("ERROR: Unbound variable:", s)
	}
	return v
}

func evList(list parse.List, env *Env) any {
	if len(list.Elems) == 0 {
		return nil
	}
	op := eval(list.Elems[0], env)
	args := make([]any, 0)
	for _, e := range list.Elems[1:] {
		args = append(args, eval(e, env))
	}
	return apply(op, args)
}

func apply(op any, args []any) any {
	switch f := op.(type) {
	case Closure:
		return eval(f.Lambda.Body, f.Env.bind(f.Lambda.Params, args))
	case func (int, int) int:
		// TODO Sanity checks
		return f(args[0].(int), args[1].(int))
	default:
		log.Fatal("ERROR: Unknown operator: ", reflect.TypeOf(op))
		return nil
	}
}

func eval(expr any, env *Env) any {
	switch e := expr.(type) {
	case parse.Number:
		return e
	case parse.Symbol:
		return evSym(e, env)
	case parse.Lambda:
		return evLambda(e, env)
	case parse.List:
		return evList(e, env)
	default:
		log.Fatal("ERROR: Unknown expression type:", expr)
		return nil
	}
}
