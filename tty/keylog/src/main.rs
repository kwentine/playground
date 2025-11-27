use rustix::termios::{tcgetattr, tcsetattr, OptionalActions};
use std::io::{self, Write, Read};
use std::ascii;

fn main() -> io::Result<()> {
    println!("Logging keystrokes. To quit, press 'q'.");
    println!("{:>5} {:>5} {:>10} {:>5}", "Hex", "Oct", "Bin", "Dec");
    let mut stdout = io::stdout();
    let saved_termios = tcgetattr(&stdout).expect("Error!");
    make_raw(&stdout)?;
    for b in io::stdin().bytes() {
        let c = b?;
        write!(stdout, "{:5x} {:5o} {:10.08b} {:5} {:4}\r\n", c, c, c, c, ascii::escape_default(c))?;
        if c == b'q' {
            break
        }
    }
    tcsetattr(stdout, OptionalActions::Flush, &saved_termios)?;
    Ok(())
}

pub fn make_raw(stdout: &io::Stdout) -> rustix::io::Result<()> {
    let mut t = tcgetattr(stdout)?;
    t.make_raw();
    tcsetattr(stdout, OptionalActions::Flush, &t)?;
    return Ok(());
}
