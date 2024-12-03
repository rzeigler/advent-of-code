use anyhow::Result;
use regex::{Captures, Match, Regex};
use std::io::{self, Read};
use std::str::FromStr;

const MUL_PAT: &'static str = "mul\\(([0-9]+),([0-9]+)\\)";
const DO_PAT: &'static str = "do()";
const DONT_PAT: &'static str = "don't()";

/* Instructions for part */
enum Inst<'a> {
    Mul(Captures<'a>),
    Do(Match<'a>),
    Dont(Match<'a>),
}

impl<'a> Inst<'a> {
    fn start(&self) -> usize {
        match self {
            Inst::Mul(c) => c.get(0).unwrap().start(),
            Inst::Do(m) => m.start(),
            Inst::Dont(m) => m.start(),
        }
    }
}

struct Solver {
    mul_re: Regex,
    do_re: Regex,
    dont_re: Regex,
}

impl Solver {
    pub fn new() -> Solver {
        Solver {
            mul_re: Regex::new(MUL_PAT).unwrap(),
            do_re: Regex::new(DO_PAT).unwrap(),
            dont_re: Regex::new(DONT_PAT).unwrap(),
        }
    }

    fn part1(&self, input: &str) -> usize {
        self.mul_re
            .captures_iter(input)
            .map(|capture| {
                let a = &capture[1];
                let b = &capture[2];
                usize::from_str(a).unwrap() * usize::from_str(b).unwrap()
            })
            .sum()
    }

    fn part2(&self, input: &str) -> usize {
        let mut commands = Vec::<Inst>::new();
        // Record all of the occurrences of multiplications and state change commands
        // They must be non-overlapping, so we can sort by starting position, walk the state,
        // and do sum
        commands.extend(self.mul_re.captures_iter(&input).map(Inst::Mul));
        commands.extend(self.do_re.find_iter(&input).map(Inst::Do));
        commands.extend(self.dont_re.find_iter(&input).map(Inst::Dont));
        commands.sort_by(|l, r| l.start().cmp(&r.start()));
        let mut tot = 0usize;
        // Start in do state
        let mut doing = true;
        for c in commands {
            if doing {
                match c {
                    Inst::Dont(_) => doing = false,
                    Inst::Mul(c) => {
                        tot += usize::from_str(&c[1]).unwrap() * usize::from_str(&c[2]).unwrap()
                    }
                    _ => {}
                }
            } else {
                match c {
                    Inst::Do(_) => doing = true,
                    _ => {}
                }
            }
        }
        tot
    }
}

fn main() -> Result<()> {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input)?;

    let solver = Solver::new();

    println!("{}", solver.part1(&input));
    println!("{}", solver.part2(&input));

    Ok(())
}
