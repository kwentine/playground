use std::{io, thread, time};

const DISPLAY_SIZE: usize = 16;
const HISTORY_SIZE: usize = DISPLAY_SIZE;
const CACHE_SIZE: usize = 2 * DISPLAY_SIZE + HISTORY_SIZE;
const EMPTY: &str = "~\n";

fn main() {
    let mut line_buffer: [String; CACHE_SIZE] = std::array::from_fn(|_| String::new());
    let stdin = io::stdin();
    for line in &mut line_buffer {
        let n = stdin.read_line(line).expect("read_line");
        if n == 0 {
            break;
        }
    }
    play(&stdin, &mut line_buffer, 0, 0);
}

fn play(stdin: &io::Stdin, line_buffer: &mut [String], mut read_idx: usize, mut write_idx: usize) {
    let pause = time::Duration::from_millis(500);
    while line_buffer[read_idx].len() != 0 {
        draw(&line_buffer, &read_idx);
        thread::sleep(pause);
        write_idx = refill(&stdin, line_buffer, read_idx, write_idx);
        read_idx = step(1, &line_buffer, read_idx, write_idx);
    }
}

fn draw(line_buffer: &[String], read_idx: &usize) {
    print!("\x1b[2J\x1b[1d");
    let size = line_buffer.len();
    for i in 0..DISPLAY_SIZE {
        let line = &line_buffer[(read_idx + i) % size];
        if line.len() == 0 {
            print!("{}", EMPTY);
        } else {
            print!(" {line}");
        }
    }
}

fn step(
    count: usize,
    line_buffer: &[String],
    read_idx: usize,
    write_idx: usize,
) -> usize {
    let size = line_buffer.len();
    let mut i = 0;
    let mut next = (read_idx + 1) % size;
    while i < count && next != write_idx {
        i += 1;
        next = (next + 1) % size;
    }
    return (read_idx + i) % size;
}

fn refill(stdin: &io::Stdin, line_buffer: &mut [String], read_idx: usize, mut write_idx: usize) -> usize {
    let size = line_buffer.len();
    let w = if write_idx < read_idx {
        write_idx + size
    } else {
        write_idx
    };
    if read_idx + DISPLAY_SIZE < w {
        return write_idx;
    }
    let stop = (read_idx + size - HISTORY_SIZE) % size;
    while write_idx != stop {
        let line = &mut line_buffer[write_idx];
        line.clear();
        stdin.read_line(line).expect("read_line");
        write_idx = (write_idx + 1) % size;
    }
    return write_idx;
}
