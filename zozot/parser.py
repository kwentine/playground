class Stream():

    def __init__(self, f):
        self.buff = []
        self.f = f

    def get(self):
        if self.buff:
            return self.buff.pop()
        return self.f.read(1)

    def put(self, c):
        if c:
            self.buff.append(c)

    def peek(self):
        if self.buff:
            return self.buff[-1]
        c = self.f.read(1)
        self.put(c)
        return c

    def match(self, c):
        m = self.peek() == c
        if m:
            self.get()
        return m


def parse_list(stream):
    result = []
    assert stream.get() == "("
    while not (stream.match(")")):
        result.append(parse_expr(stream))
    return result

def parse_int(stream):
    n = 0
    while (c := stream.get()).isdigit():
        n = 10*n + int(c)
    stream.put(c)
    return n

# prog ::= (expr (\n)+)*
# expr ::= sym | list
# list ::= "(" expr* ")"

def parse_expr(stream):
    while (stream.match(" ")):
        pass
    c = stream.peek()
    print(f"parse_expr: {c}")
    if c == "(":
        return parse_list(stream)
    if c.isdigit():
        return parse_int(stream)

def parse(stream):
    res = []
    while stream.peek():
        res.append(parse_expr(stream))
        while stream.match("\n"):
            pass
    return res

def scan(source):
    return source.replace('(', ' ( ').replace(')', ' ) ').split()

if __name__ == "__main__":
    stream = Stream(open("test.zt"))
    print(parse(stream))
