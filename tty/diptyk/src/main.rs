// CSI escape sequences: https://man7.org/linux/man-pages/man4/console_codes.4.html
use std::io::{self, Write};

fn main() -> io::Result<()> {

    let mut buff = LineBuffer::new(3);
    buff.add("One".to_string());
    buff.add("Two".to_string());
    buff.add("Three".to_string());
    buff.add("Four".to_string());
    let mut screen = io::stdout();

    // Erase screen
    screen.write(b"\x1b\x5b2J")?;

    // Draw buffer
    draw(&buff, 1, &mut screen)?;

    Ok(())
}

// Draw the contents of a circular buffer to screen, starting from a given row.
fn draw(buff: &LineBuffer, start_row: u8, screen: &mut io::Stdout) -> io::Result<()> {
    let l = buff.len;
    let mut next = (buff.last_idx + 1) % l;
    write!(screen, "\x1b\x5b{}d", start_row)?;
    loop {
        match &buff.lines[next] {
            Some(s) => {
                screen.write(s.as_bytes())?;
                screen.write(b"\n")?;
            },
            None => {},
        }
        if next == buff.last_idx {
            break
        }
        next = (next + 1) % l;
    }
    return Ok(());
}

struct LineBuffer {
    len: usize,
    last_idx: usize,
    lines: Vec<Option<String>>,
}

impl LineBuffer {

    fn new(nrows: usize) -> Self {
        return Self {
            len: nrows,
            last_idx: nrows - 1,
            lines: vec![None; nrows]
        }
    }

    fn add(&mut self, line: String) {
        self.last_idx = (self.last_idx + 1) % self.len;
        self.lines[self.last_idx] = Some(line);
    }
}
