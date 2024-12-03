use anyhow::Result;
use std::{error::Error, io::BufRead, str::FromStr};

pub fn read_ragged_rows<R, Elem>(reader: &mut R) -> Result<Vec<Vec<Elem>>>
where
    R: BufRead,
    Elem: FromStr,
    <Elem as FromStr>::Err: Error + Send + Sync + 'static,
{
    let mut result = Vec::<Vec<Elem>>::new();
    // Is this code golfable to chained iterators?
    for line in reader.lines() {
        let line = line?;
        if line.is_empty() {
            break;
        }
        let mut row = Vec::<Elem>::new();
        for word in line.split_whitespace() {
            row.push(Elem::from_str(word)?);
        }
        result.push(row);
    }
    Ok(result)
}

pub fn read_matrix<R, Elem, const COLS: usize>(reader: &mut R) -> Result<[Vec<Elem>; COLS]>
where
    R: BufRead,
    Elem: FromStr,
    <Elem as FromStr>::Err: Error + Send + Sync + 'static,
{
    let mut result = [const { Vec::new() }; COLS];
    for line in reader.lines() {
        let line = line?;
        if line.is_empty() {
            break;
        }

        let mut words = line.split_ascii_whitespace();
        for col in result.iter_mut() {
            if let Some(word) = words.next() {
                col.push(Elem::from_str(word)?)
            } else {
                panic!("Mismatched line input")
            }
        }
    }
    Ok(result)
}
