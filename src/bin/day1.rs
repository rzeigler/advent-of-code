use advent_of_code::input;

use anyhow::Result;
use std::io;

fn part1(mut left: Vec<u32>, mut right: Vec<u32>) -> u32 {
    left.sort();
    right.sort();

    left.iter()
        .zip(right.iter())
        .map(|(l, r)| l.abs_diff(*r))
        .sum()
}

fn part2(left: Vec<u32>, right: Vec<u32>) -> u32 {
    left.iter()
        .map(|l| {
            let occured = right.iter().filter(|r| *r == l).count();
            l * (occured as u32)
        })
        .sum()
}

pub fn main() -> Result<()> {
    let [left, right] = input::read_matrix::<_, u32, 2>(&mut io::stdin().lock())?;

    println!("{}", part1(left.clone(), right.clone()));
    println!("{}", part2(left, right));
    Ok(())
}
