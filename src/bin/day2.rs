use advent_of_code::input;
use anyhow::Result;
use std::{cmp::Ordering, io};

fn is_safe(input: &[u32]) -> bool {
    let mut order = Ordering::Equal;
    for pair in input.windows(2) {
        let diff = pair[0].abs_diff(pair[1]);
        let direction = pair[0].cmp(&pair[1]);
        if diff < 1 || 3 < diff {
            return false;
        }
        if order != Ordering::Equal && direction != order {
            return false;
        }
        order = direction;
    }
    true
}

fn permute(input: &[u32]) -> Vec<Vec<u32>> {
    let mut results = Vec::new();
    for i in 0..input.len() {
        let mut dup = input.to_vec();
        dup.remove(i);
        results.push(dup);
    }
    results
}

fn part1(input: &[Vec<u32>]) -> usize {
    input.iter().filter(|row| is_safe(row.as_ref())).count()
}

fn part2(input: &[Vec<u32>]) -> usize {
    input
        .iter()
        .filter(|row| {
            is_safe(row.as_ref())
                || permute(row.as_ref())
                    .iter()
                    .find(|p| is_safe(p.as_ref()))
                    .is_some()
        })
        .count()
}

pub fn main() -> Result<()> {
    let input = input::read_ragged_rows::<_, u32>(&mut io::stdin().lock())?;

    println!("{}", part1(input.as_ref()));
    println!("{}", part2(input.as_ref()));
    Ok(())
}
