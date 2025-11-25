use std::io::{self, Read};
use std::ascii;

fn main() -> io::Result<()> {
    println!("Press 'q' to quit.");
    for b in io::stdin().bytes() {
        let c = b?;
        println!("{:08b} {:02x} {:>3}  {}", c, c, c, ascii::escape_default(c));
        if c == b'q' {
            break
        }
    }
    Ok(())
}
