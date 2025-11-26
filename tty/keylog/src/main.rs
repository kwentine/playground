//use termion::raw::IntoRawMode;

use std::io::{self, Read, Write, stdout};
use std::ascii;

pub mod keylog;

fn main() -> io::Result<()> {
    println!("Logging keystrokes. To quit, press 'q'.");
    println!("{:>5} {:>5} {:>10} {:>5}", "Hex", "Oct", "Bin", "Dec");
//    let mut stdout = stdout().into_raw_mode()?;
    keylog::make_raw();
    for b in io::stdin().bytes() {
        let c = b?;
        write!(stdout(), "{:5x} {:5o} {:10.08b} {:5} {:4}\r\n", c, c, c, c, ascii::escape_default(c))?;
        if c == b'q' {
            break
        }
    }
    Ok(())
}
