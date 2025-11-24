use std::io;
use std::io::Read;
use std::ascii;

fn main() {
    println!("Press 'q' to quit.");
    let mut c: [u8; 1] = [0];
    let mut stdin = io::stdin();
    loop {
        let n = stdin.read(&mut c).unwrap();
        if n == 0 {
            break
        }
        if c[0] == 0x71 {
            break
        }
        let b = c[0];
        let e = ascii::escape_default(c[0]);
        println!("{:>3} {:0>2x} {}", b, b, e);
    }
}
