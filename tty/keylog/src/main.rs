use rustix::termios;
use std::io::{self, Read, Write};

fn main() -> io::Result<()> {
    println!("Logging keystrokes. To quit, press 'q'.");
    println!("{:>5} {:>5} {:>10} {:>5}", "Hex", "Oct", "Bin", "Dec");
    // write! needs a mutable borrow
    let mut stdout = io::stdout();
    let stdin = io::stdin();
    let saved_termios = termios::tcgetattr(&stdout).expect("Error!");
    make_raw(&stdin)?;
    for b in io::stdin().bytes() {
        let c = b?;
        write!(stdout, "{:5x} {:5o} {:10.08b} {:5}", c, c, c, c,)?;
        if !c.is_ascii() {
            stdout.write(b"  ??")?;
        } else if c.is_ascii_control() {
            stdout.write(b"  ^")?;
            stdout.write(&[c ^ 0x40])?;
        } else {
            stdout.write(b"   ")?;
            stdout.write(&[c])?;
        }
        stdout.write(b"\r\n")?;
        if c == b'q' {
            break;
        }
    }
    termios::tcsetattr(stdout, termios::OptionalActions::Flush, &saved_termios)?;
    println!("Restored terminal state. Bidding goodbye.");
    Ok(())
}

pub fn make_raw(stdin: &io::Stdin) -> rustix::io::Result<()> {
    let mut t = termios::tcgetattr(stdin)?;
    // https://man7.org/linux/man-pages/man3/cfmakeraw.3.html
    // Including all the details to emphasize the parallel with C code,
    // but could be achieved using:
    // t.make_raw()
    t.input_modes -= termios::InputModes::BRKINT
        | termios::InputModes::ICRNL
        | termios::InputModes::IGNBRK
        | termios::InputModes::IGNCR
        | termios::InputModes::INLCR
        | termios::InputModes::INPCK
        | termios::InputModes::ISTRIP
        | termios::InputModes::IXON
        | termios::InputModes::PARMRK;
    t.local_modes -= termios::LocalModes::ICANON
        | termios::LocalModes::ISIG
        | termios::LocalModes::IEXTEN
        | termios::LocalModes::ECHO;
    t.output_modes -= termios::OutputModes::OPOST;
    t.special_codes[termios::SpecialCodeIndex::VMIN] = 1;
    t.special_codes[termios::SpecialCodeIndex::VTIME] = 0;
    termios::tcsetattr(stdin, termios::OptionalActions::Flush, &t)?;
    return Ok(());
}
