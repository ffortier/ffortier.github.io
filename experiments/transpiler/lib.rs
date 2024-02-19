pub mod error;

use error::Result;
use pest::Parser;

mod ca65 {

    use pest_derive::Parser;

    #[derive(Parser)]
    #[grammar = "grammars/ca65.pest"]
    pub struct MyParser;
}

pub fn transpile() -> Result<()> {
    let program = ca65::MyParser::parse(ca65::Rule::Program, "hello")
        .expect("Parse correctly")
        .into_iter()
        .next()
        .expect("Program to be found");

    dbg!(program.into_inner().next().unwrap().as_rule());

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_ca65_bubble8() {
        let source = include_str!("examples/bubble8.s");
        let pairs = ca65::MyParser::parse(ca65::Rule::Program, source).unwrap();

        dbg!(pairs);
    }
}
