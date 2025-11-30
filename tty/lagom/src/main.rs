use std::io;

fn main() {
    const DISPLAY_SIZE: usize = 16;
    const HISTORY_SIZE: usize = DISPLAY_SIZE;
    const CACHE_SIZE: usize = 2 * DISPLAY_SIZE + HISTORY_SIZE;

    let mut line_buffer: [String; CACHE_SIZE] = std::array::from_fn(|_| String::new());
    let stdin = io::stdin();
    for line in &mut line_buffer {
        let n = stdin.read_line(line).expect("read_line");
        if n == 0 {
            break;
        }
    }

    let mut read_idx = 0;
    let mut write_idx = 0;

    for s in &line_buffer {
        if s.len() == 0 {
            break;
        }
        print!("> {s}");
    }

}
