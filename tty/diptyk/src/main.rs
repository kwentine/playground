// CSI escape sequences: https://man7.org/linux/man-pages/man4/console_codes.4.html
use std::io::{self, Write};

fn main() -> io::Result<()> {
    let mut buff = LineBuffer::new(3);
    buff.add("One".to_string());
    buff.add("Two".to_string());
    buff.add("Three".to_string());
    buff.add("Four".to_string());
    let mut screen = io::stdout();
    clear_screen(&mut screen)?;
    draw(&buff, 1, &mut screen)?;
    Ok(())
}


fn clear_screen(screen: &mut io::Stdout) -> io::Result<()> {
    screen.write(b"\x1b\x5b2J")?;
    Ok(())
}

// Draw the contents of a circular buffer to screen, starting from a given row.
fn draw(buff: &LineBuffer, start_row: u8, screen: &mut io::Stdout) -> io::Result<()> {
    write!(screen, "\x1b\x5b{}d", start_row)?;
    for s in &buff.lines() {
        screen.write(s.as_bytes())?;
        screen.write(b"\n")?;
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

    fn lines(&self) -> Vec<&String> {
        let mut res = Vec::with_capacity(self.len);
        let mut next = (self.last_idx + 1) % self.len;
        loop {
            match &self.lines[next] {
                Some(s) => res.push(s),
                None => {},
            }
            if next == self.last_idx {
                return res;
            }
            next = (next + 1) % self.len;
        }
    }
}
